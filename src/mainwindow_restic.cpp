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

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "resticdialog.h"

QProcessEnvironment MainWindow::getResticEnv() const
{
    return application->getResticEnv(ui->txtResticB2ID->text(), ui->txtResticB2Key->text(), ui->txtResticRepo->text(),
                                     ui->txtResticPassword->text());
}

void MainWindow::on_btnResticInit_clicked()
{
    ResticDialog *rd = new ResticDialog(ui->txtRestic->text(), "--verbose=2 init", getResticEnv(), this);
    rd->show();
}

void MainWindow::on_btnResticBackup_clicked()
{
    // --exclude .cache --exclude .local
    // mysqldump database | restic backup --stdin --stdin-filename database.sql
    // mysqldump --databases database_name -u database_user -p | restic backup --stdin --stdin-filename
    // database_dump.sql
    // mysqldump -u database_user -p --all-databases | restic backup --stdin --stdin-filename
    // all_databases.sql

    QString backup = ui->txtResticBackup->text();
    if (backup.size() < 1)
        return;

    ResticDialog *rd = new ResticDialog(ui->txtRestic->text(), "--verbose=2 backup " + backup, getResticEnv(), this);
    rd->show();
}

void MainWindow::on_btnResticCheck_clicked()
{
    // --read-data
    // --read-data-subset=1/5
    ResticDialog *rd = new ResticDialog(ui->txtRestic->text(), "--verbose=2 check", getResticEnv(), this);
    rd->show();
}

void MainWindow::on_btnResticSnapshots_clicked()
{
    // restic diff $SnapshotID1 $SnapshotID2
    ResticDialog *rd = new ResticDialog(ui->txtRestic->text(), "--verbose=2 snapshots", getResticEnv(), this);
    rd->show();
}

void MainWindow::on_btnResticRestore_clicked()
{
    // restic restore $SnapshotID --target $LocationToRestoreTo --path $PathBeingRestored
    // restic restore $SnapshotID --target $LocationToRestoreTo --include $PathtoFileBeingRestored

    QString restore = ui->txtResticRestore->text();
    if (restore.size() < 1)
        return;

    QString snapshot = ui->txtResticSnapshot->text();
    if (snapshot.size() < 1)
        return;

    ResticDialog *rd = new ResticDialog(
        ui->txtRestic->text(), "--verbose=2 restore " + snapshot + " --target " + restore, getResticEnv(), this);
    rd->show();
}

void MainWindow::on_btnResticForget_clicked()
{
    QString forget = ui->txtResticForget->text();
    if (forget.size() < 1)
        return;

    // --keep-last 1
    ResticDialog *rd = new ResticDialog(ui->txtRestic->text(), "--verbose=2 " + forget, getResticEnv(), this);
    rd->show();
}

void MainWindow::on_btnResticPrune_clicked()
{
    ResticDialog *rd = new ResticDialog(ui->txtRestic->text(), "--verbose=2 prune", getResticEnv(), this);
    rd->show();
}
