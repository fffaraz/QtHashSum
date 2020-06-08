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

#pragma once

#include <QDialog>
#include <QProcess>

namespace Ui
{
class ResticDialog;
}

class ResticDialog : public QDialog
{
    Q_OBJECT

  public:
    explicit ResticDialog(QString cmd, QString args, QProcessEnvironment env, QWidget *parent = nullptr);
    ~ResticDialog();

  private slots:
    void on_btnRun_clicked();
    void process_errorOccurred(QProcess::ProcessError error);
    void process_finished(int exitCode, QProcess::ExitStatus exitStatus);
    void process_readyReadStandardError();
    void process_readyReadStandardOutput();
    void process_started();
    void process_stateChanged(QProcess::ProcessState newState);

  private:
    Ui::ResticDialog *ui;
    QString m_cmd;
    QProcessEnvironment m_env;
    QProcess *m_process = nullptr;
};
