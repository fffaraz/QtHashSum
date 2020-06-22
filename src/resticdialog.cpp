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

#include "resticdialog.h"
#include "ui_resticdialog.h"

#include <QDebug>

#ifdef Q_OS_WIN
#include <windows.h>
#endif

ResticDialog::ResticDialog(QString cmd, QString args, QProcessEnvironment env, QWidget *parent)
    : QDialog(parent), ui(new Ui::ResticDialog)
{
    m_cmd = cmd;
    m_env = env;
    ui->setupUi(this);
    ui->txtArgs->setText(args);
    for (const QString &key : env.keys())
        ui->txtStatus->appendPlainText(key + " = " + env.value(key));
    this->setAttribute(Qt::WA_DeleteOnClose);
}

ResticDialog::~ResticDialog()
{
    qDebug() << "ResticDialog::~ResticDialog";
    if (m_process != nullptr)
    {
        delete m_process;
    }
    delete ui;
}

void ResticDialog::on_btnRun_clicked()
{
    qDebug() << "ResticDialog::on_btnRun_clicked";
    ui->btnRun->setEnabled(false);
    m_process = new QProcess();

#ifdef Q_OS_WIN
    m_process->setCreateProcessArgumentsModifier([](QProcess::CreateProcessArguments *args) {
        args->flags |= CREATE_NEW_CONSOLE;
        args->startupInfo->dwFlags &= ~STARTF_USESTDHANDLES;
        args->startupInfo->dwFlags |= STARTF_USEFILLATTRIBUTE;
        args->startupInfo->dwFillAttribute = BACKGROUND_BLUE | FOREGROUND_RED | FOREGROUND_INTENSITY;
    });
#endif

    m_process->setProcessEnvironment(m_env);
    m_process->setInputChannelMode(QProcess::ManagedInputChannel);
    m_process->setProcessChannelMode(QProcess::SeparateChannels);

    connect(m_process, &QProcess::errorOccurred, this, &ResticDialog::process_errorOccurred);
    connect(m_process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this,
            &ResticDialog::process_finished);
    connect(m_process, &QProcess::readyReadStandardError, this, &ResticDialog::process_readyReadStandardError);
    connect(m_process, &QProcess::readyReadStandardOutput, this, &ResticDialog::process_readyReadStandardOutput);
    connect(m_process, &QProcess::started, this, &ResticDialog::process_started);
    connect(m_process, &QProcess::stateChanged, this, &ResticDialog::process_stateChanged);

    m_process->start("cmd.exe /K " + m_cmd + " " + ui->txtArgs->text());
    // m_process->closeWriteChannel();
}

void ResticDialog::process_errorOccurred(QProcess::ProcessError error)
{
    qDebug() << "ResticDialog::process_errorOccurred" << error;
    ui->txtStatus->appendPlainText("errorOccurred: " + m_process->errorString());
}

void ResticDialog::process_finished(int exitCode, QProcess::ExitStatus exitStatus)
{
    qDebug() << "ResticDialog::process_finished" << exitCode << exitStatus;
    ui->txtStatus->appendPlainText("finished: " + QString::number(exitCode) + " " +
                                   QVariant::fromValue(exitStatus).value<QString>());
}

void ResticDialog::process_readyReadStandardError()
{
    QByteArray data = m_process->readAllStandardError();
    qDebug() << "ResticDialog::process_readyReadStandardError" << data.size() << data;
    ui->txtStatus->appendPlainText("readyReadStandardError: " + QString::number(data.size()));
    ui->txtStatus->appendPlainText(data);
}

void ResticDialog::process_readyReadStandardOutput()
{
    QByteArray data = m_process->readAllStandardOutput();
    qDebug() << "ResticDialog::process_readyReadStandardOutput" << data.size() << data;
    ui->txtStatus->appendPlainText("readyReadStandardOutput: " + QString::number(data.size()));
    ui->txtStatus->appendPlainText(data);
}

void ResticDialog::process_started()
{
    qDebug() << "ResticDialog::process_started";
    ui->txtStatus->appendPlainText("started");
}

void ResticDialog::process_stateChanged(QProcess::ProcessState newState)
{
    qDebug() << "ResticDialog::process_stateChanged" << newState;
    ui->txtStatus->appendPlainText("stateChanged: " + QVariant::fromValue(newState).value<QString>());
}
