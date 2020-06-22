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

#pragma once

#include <QMainWindow>
#include <QProcess>

#include "application.h"

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    explicit MainWindow(Application *application, QWidget *parent = nullptr);
    ~MainWindow();

  protected:
    virtual void closeEvent(QCloseEvent *event);

  private slots:
    void on_btnBrowseFile_clicked();
    void on_btnStartFile_clicked();
    void on_btnBrowseDir_clicked();
    void on_btnStartDir_clicked();
    void on_btnBrowseOrig_clicked();
    void on_btnBrowseDup_clicked();
    void on_btnStartDup_clicked();

    void on_cmbThreads_currentIndexChanged(const QString &arg1);

    void on_btnResticInit_clicked();
    void on_btnResticBackup_clicked();
    void on_btnResticCheck_clicked();
    void on_btnResticSnapshots_clicked();
    void on_btnResticRestore_clicked();
    void on_btnResticForget_clicked();
    void on_btnResticPrune_clicked();

  private:
    Ui::MainWindow *ui = nullptr;
    Application *application = nullptr;
    QProcessEnvironment getResticEnv() const;
};
