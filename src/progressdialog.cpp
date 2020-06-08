// QtHashSum: File Checksum Integrity Verifier & Duplicate File Finder
// Copyright (C) 2019-2020  Faraz Fallahi <fffaraz@gmail.com>

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include <QGridLayout>
#include <QDateTime>
#include <QThreadPool>
#include <QDebug>

#include "progressdialog.h"
#include "ui_progressdialog.h"

#include "resultdialog.h"
#include "duplicatedialog.h"

ProgressDialog::ProgressDialog(QVector<FileHasher *> jobs, QString parentDir, bool methodName, bool removeDups, QString format, QWidget *parent = nullptr)
    : QDialog(parent),
    ui(new Ui::ProgressDialog),
    m_jobs(jobs),
    m_parentDir(parentDir),
    m_methodName(methodName),
    m_removeDups(removeDups),
    m_format(format)
{
    ui->setupUi(this);
    setWindowTitle("Progress Status (" + QString::number(jobs.size()) + " files)");

    QGridLayout *layout = new QGridLayout(this);
    for(int i = 0; i < qMin(QThreadPool::globalInstance()->maxThreadCount(), jobs.size()); ++i)
    {
        ProgressData pd;
        pd.pb = new QProgressBar(this);
        pd.pb->setMinimum(0);
        pd.pb->setMaximum(100);
        pd.pb->setValue(jobs[i]->percent());
        pd.label = new QLabel(this);
        pd.label->setText(QString::number(i + 1) + " " + jobs[i]->info());
        layout->addWidget(pd.label);
        layout->addWidget(pd.pb);
        m_pds.append(pd);
    }
    setLayout(layout);

    QCoreApplication::processEvents();
    m_elapsedTimer.start();

    for(int i = 0; i < jobs.size(); ++i) QThreadPool::globalInstance()->start(jobs[i]);
    qDebug() << "All jobs started" << m_elapsedTimer.elapsed();

    connect(&m_timer, &QTimer::timeout, this, &ProgressDialog::timer_timeout);
    m_timer.start(500);
}

ProgressDialog::~ProgressDialog()
{
    delete ui;
}

void ProgressDialog::timer_timeout()
{
    updateProgress();

    int numDone = 0;
    for(int i = 0; i < m_jobs.size(); ++i)
        numDone += m_jobs[i]->done();

    int percentDone = 100 * numDone / m_jobs.size();
    setWindowTitle("Progress Status (" + QString::number(numDone) + " / " + QString::number(m_jobs.size()) + " files done, " + QString::number(percentDone) + "%)");

    if(numDone == m_jobs.size())
        allDone();
}

void ProgressDialog::updateProgress()
{
    int usedPds = 0;
    for(int i = 0; i < m_jobs.size() && usedPds < m_pds.size(); ++i)
    {
        bool showThis = m_jobs[i]->started();
        if(m_jobs[i]->done() && m_jobs.size() - i > m_pds.size() - usedPds)
            showThis = false;
        if(showThis)
        {
            m_pds[usedPds].pb->setValue(m_jobs[i]->percent());
            m_pds[usedPds].label->setText(QString::number(i + 1) + "  " + m_jobs[i]->info());
            usedPds++;
        }
    }
    QCoreApplication::processEvents();
}

void ProgressDialog::allDone()
{
    qDebug() << "All jobs done" << m_elapsedTimer.elapsed();
    m_timer.stop();

    qint64 totalSize = 0;
    QMap<QString, int>           hash2count;
    QMultiMap<QString, QString>  path2info;
    QHash<QString, qint64>       hash2size;
    QMultiHash<QString, QString> hash2path;
    for(int i = 0; i < m_jobs.size(); ++i)
    {
        qint64 size = m_jobs[i]->size();
        QString hash = m_jobs[i]->hash();
        QString name = m_jobs[i]->name();
        QString method = (m_methodName ? m_jobs[i]->methodStr() + " " : "");
        delete m_jobs[i];

        if(m_methodName == false && name.endsWith("/desktop.ini")) // FIXME
        {
            if(hash == "418c4c275b3c0d70d93f046cd8c2a632121b6072c725bf4355b319fe6dc7b9cd") continue;
            if(hash == "7ba990a8886cdf4823cba7579d5e1f550d593e01aef15ebbd8d2b216e1c7d36d") continue;
            if(hash == "dfc6fa51dd38a197f4294d87357b2fe218377535d294d744924122761c03ca8f") continue;
        }
        if(m_methodName == false && name.contains(".git/objects/")) continue; // TODO: add option

        totalSize += size;

        QString info = method + hash + " " + QString::number(size);
        path2info.insertMulti(name, info);

        hash2count[hash] = hash2count[hash] + 1;
        hash2path.insertMulti(hash, name);

        if(!hash2size.contains(hash)) hash2size.insert(hash, size);
        else if(hash2size.value(hash) != size) qDebug() << "ERROR: same hash different size" << hash;
    }

    QString duplicates;
    int num_duplicates = 0;
    qint64 wasted = 0;
    static const int min_size[] = {1000, 100, 10, 1, 0};
    for(int i = 0; i < 5; ++i)
    {
        for(auto hash2count_itr = hash2count.constBegin(); hash2count_itr != hash2count.constEnd(); ++hash2count_itr)
        {
            qint64 size_mb = hash2size[hash2count_itr.key()] / 1048576;
            bool size_ok = (size_mb >= min_size[i] && (i == 0 || size_mb < min_size[i - 1]));
            if(hash2count_itr.value() > 1 && size_ok)
            {
                num_duplicates++;
                wasted += (hash2count_itr.value() - 1) * hash2size[hash2count_itr.key()];
                duplicates.append(QString::number(hash2count_itr.value()) + " " + QString::number(size_mb) + " " + hash2count_itr.key() + "\n");
                QList<QString> pathlist = hash2path.values(hash2count_itr.key());
                pathlist.sort();
                foreach(QString path, pathlist) duplicates.append("\t" + path + "\n");
                if(m_removeDups)
                {
                    DuplicateDialog dd(pathlist, m_parentDir, this);
                    dd.exec();
                }
            }
        }
    }

    QString result;
    result.append("Checksums by QtHashSum " APPVERSION "\n");
    result.append("https://github.com/fffaraz/QtHashSum\n");
    result.append("Generated at " + QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss") + "\n");
    result.append(QString::number(m_jobs.size()) + " files hashed, " + QString::number(totalSize / 1048576) + " MB total\n");
    if(num_duplicates > 0) result.append(QString::number(num_duplicates) + " duplicates found, " + QString::number(wasted / 1048576) + " MB wasted\n\n" + duplicates);
    result.append("\n");
    for(auto itr = path2info.constBegin(); itr != path2info.constEnd(); ++itr) result.append(itr.value() + " " + itr.key() + "\n");

    qDebug() << "Result ready" << m_elapsedTimer.elapsed();

    ResultDialog *rd = new ResultDialog(result);
    rd->show();

    this->deleteLater();
}
