// QtHashSum: File Checksum Integrity Verifier & Duplicate File Finder
// Copyright (C) 2018-2020  Faraz Fallahi <fffaraz@gmail.com>

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

#include <QDebug>
#include <QGridLayout>
#include <QThreadPool>

#include "progressdialog.h"
#include "ui_progressdialog.h"

ProgressDialog::ProgressDialog(const QVector<FileHasher *> &jobs, QWidget *parent = nullptr)
    : QDialog(parent), ui(new Ui::ProgressDialog), m_jobs(jobs)
{
    ui->setupUi(this);
    setWindowTitle("Progress Status (" + QString::number(jobs.size()) + " files)");

    QGridLayout *layout = new QGridLayout(this);
    for (int i = 0; i < qMin(QThreadPool::globalInstance()->maxThreadCount(), jobs.size()); ++i)
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

    for (int i = 0; i < jobs.size(); ++i)
        QThreadPool::globalInstance()->start(jobs[i]);
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
    for (int i = 0; i < m_jobs.size(); ++i)
        numDone += m_jobs[i]->done();

    int percentDone = 100 * numDone / m_jobs.size();
    setWindowTitle("Progress Status (" + QString::number(numDone) + " / " + QString::number(m_jobs.size()) +
                   " files done, " + QString::number(percentDone) + "%)");

    if (numDone == m_jobs.size())
    {
        qDebug() << "All jobs done" << m_elapsedTimer.elapsed();
        m_timer.stop();
        this->deleteLater();
    }
}

void ProgressDialog::updateProgress()
{
    int usedPds = 0;
    for (int i = 0; i < m_jobs.size() && usedPds < m_pds.size(); ++i)
    {
        bool showThis = m_jobs[i]->started();
        if (m_jobs[i]->done() && m_jobs.size() - i > m_pds.size() - usedPds)
            showThis = false;
        if (showThis)
        {
            m_pds[usedPds].pb->setValue(m_jobs[i]->percent());
            m_pds[usedPds].label->setText(QString::number(i + 1) + "  " + m_jobs[i]->info());
            usedPds++;
        }
    }
    QCoreApplication::processEvents();
}
