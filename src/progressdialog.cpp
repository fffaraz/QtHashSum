// QtHashSum: File Checksum Integrity Verifier & Duplicate File Finder
// Copyright (C) 2018  Faraz Fallahi <fffaraz@gmail.com>

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

ProgressDialog::ProgressDialog(QVector<FileHasher *> jobs, bool methodName, bool removeDups, QWidget *parent = nullptr) :
    QDialog(parent),
    ui(new Ui::ProgressDialog),
    jobs(jobs),
    methodName(methodName),
    removeDups(removeDups)
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
        pds.append(pd);
    }
    setLayout(layout);
    QCoreApplication::processEvents();
    elapsedtimer.start();
    for(int i = 0; i < jobs.size(); ++i) QThreadPool::globalInstance()->start(jobs[i]);
    qDebug() << "All jobs started" << elapsedtimer.elapsed();
    connect(&timer, &QTimer::timeout, this, &ProgressDialog::timer_timeout);
    timer.start(500);
}

ProgressDialog::~ProgressDialog()
{
    delete ui;
}

void ProgressDialog::timer_timeout()
{
    updateProgress();
    int done = 0;
    for(int i = 0; i < jobs.size(); ++i) if(jobs[i]->done) done++;
    setWindowTitle("Progress Status (" + QString::number(done) + " / " + QString::number(jobs.size()) + " files done, " + QString::number(100 * done / jobs.size()) + "%)");
    if(done == jobs.size()) allDone();
}

void ProgressDialog::updateProgress()
{
    int used = 0;
    for(int i = 0; i < jobs.size() && used < pds.size(); ++i) if(jobs[i]->started && (!jobs[i]->done || jobs.size() - i <= pds.size() - used))
    {
        pds[used].pb->setValue(jobs[i]->percent());
        pds[used].label->setText(QString::number(i + 1) + "  " + jobs[i]->info());
        used++;
    }
    QCoreApplication::processEvents();
}

void ProgressDialog::allDone()
{
    qDebug() << "All jobs done" << elapsedtimer.elapsed();
    timer.stop();

    qint64 totalsize = 0;
    QMap<QString, int>           hash2count;
    QMultiMap<QString, QString>  path2info;
    QHash<QString, qint64>       hash2size;
    QMultiHash<QString, QString> hash2path;
    for(int i = 0; i < jobs.size(); ++i)
    {
        qint64 size = jobs[i]->size;
        QString hash = jobs[i]->hash;
        QString name = jobs[i]->name();
        QString method = (methodName ? jobs[i]->methodStr() + " " : "");
        delete jobs[i];

        if(methodName == false && name.endsWith("/desktop.ini"))
        {
            if(hash == "418c4c275b3c0d70d93f046cd8c2a632121b6072c725bf4355b319fe6dc7b9cd") continue;
            if(hash == "7ba990a8886cdf4823cba7579d5e1f550d593e01aef15ebbd8d2b216e1c7d36d") continue;
            if(hash == "dfc6fa51dd38a197f4294d87357b2fe218377535d294d744924122761c03ca8f") continue;
        }

        totalsize += size;

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
    for(int flag = 0; flag < 2; ++flag)
    {
        for(auto hash2count_itr = hash2count.constBegin(); hash2count_itr != hash2count.constEnd(); ++hash2count_itr)
        {
            qint64 size_mb = hash2size[hash2count_itr.key()] / 1048576;
            if(hash2count_itr.value() > 1 && ((size_mb >= 1 && flag == 0) || (size_mb < 1 && flag == 1)))
            {
                num_duplicates++;
                wasted += (hash2count_itr.value() - 1) * hash2size[hash2count_itr.key()];
                duplicates.append(QString::number(hash2count_itr.value()) + " " + QString::number(size_mb) + " " + hash2count_itr.key() + "\n");
                QList<QString> pathlist = hash2path.values(hash2count_itr.key());
                pathlist.sort();
                foreach(QString path, pathlist) duplicates.append("\t" + path + "\n");
                if(removeDups)
                {
                    DuplicateDialog dd(pathlist, this);
                    dd.exec();
                }
            }
        }
    }

    QString result;
    result.append("Checksums by QtHashSum " APPVERSION "\n");
    result.append("https://github.com/fffaraz/QtHashSum\n");
    result.append("Generated at " + QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss") + "\n");
    result.append(QString::number(jobs.size()) + " files hashed, " + QString::number(totalsize / 1048576) + " MB total\n");
    if(num_duplicates > 0) result.append(QString::number(num_duplicates) + " duplicates found, " + QString::number(wasted / 1048576) + " MB wasted\n\n" + duplicates);
    result.append("\n");
    for(auto itr = path2info.constBegin(); itr != path2info.constEnd(); ++itr) result.append(itr.value() + " " + itr.key() + "\n");

    qDebug() << "Result ready" << elapsedtimer.elapsed();

    ResultDialog *rd = new ResultDialog(result);
    rd->show();

    this->deleteLater();
}
