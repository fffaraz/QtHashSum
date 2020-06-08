// QtHashSum: File Checksum Integrity Verifier & Duplicate File Finder
// Copyright (C) 2019  Faraz Fallahi <fffaraz@gmail.com>

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
#include <QFileDialog>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "filehasher.h"
#include "progressdialog.h"
#include "resticdialog.h"

MainWindow::MainWindow(Application *application, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), application(application)
{
    ui->setupUi(this);
    this->setWindowTitle("QtHashSum " APPVERSION);

    for (int i = 1; i <= application->maxThreadCount; ++i)
        ui->cmbThreads->addItem(QString::number(i));
    ui->cmbThreads->setCurrentIndex(2);

    for (int i = QCryptographicHash::Md4; i != QCryptographicHash::Sha3_512 + 1; ++i)
        ui->cmbMethods->addItem(Settings(static_cast<QCryptographicHash::Algorithm>(i)).methodStr());
    ui->cmbMethods->setCurrentIndex(QCryptographicHash::Sha3_256);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnBrowse_clicked()
{
    ui->txtFile->setText(QFileDialog::getOpenFileName(this));
}

void MainWindow::on_btnBrowseDir_clicked()
{
    ui->txtDir->setText(QFileDialog::getExistingDirectory(this));
}

void MainWindow::on_cmbThreads_currentIndexChanged(const QString &arg1)
{
    application->setMaxThreadCount(arg1.toInt());
}

void MainWindow::on_btnStart_clicked()
{
    QVector<QCryptographicHash::Algorithm> methods;
    if (ui->chkMD5->isChecked())
        methods.append(QCryptographicHash::Md5);
    if (ui->chkSHA1->isChecked())
        methods.append(QCryptographicHash::Sha1);
    if (ui->chkSHA2_256->isChecked())
        methods.append(QCryptographicHash::Sha256);
    if (ui->chkSHA2_512->isChecked())
        methods.append(QCryptographicHash::Sha512);
    if (ui->chkSHA3_256->isChecked())
        methods.append(QCryptographicHash::Sha3_256);
    if (ui->chkSHA3_512->isChecked())
        methods.append(QCryptographicHash::Sha3_512);
    if (methods.size() < 1)
        return;

    QFileInfo file(ui->txtFile->text());
    if (!file.exists())
        return;

    Settings settings;
    settings.prefix_len = file.absolutePath().size();

    QVector<FileHasher *> jobs;
    foreach (QCryptographicHash::Algorithm method, methods)
    {
        settings.method = method;
        jobs.append(new FileHasher(file.absoluteFilePath(), settings));
    }

    ProgressDialog *pd = new ProgressDialog(jobs, "", true, false, "", this);
    pd->show();
}

void MainWindow::on_btnStartDir_clicked()
{
    QString dir = ui->txtDir->text();
    if (dir.size() < 1)
        return;

    QVector<FileHasher *> jobs =
        application->parseDir(dir, static_cast<QCryptographicHash::Algorithm>(ui->cmbMethods->currentIndex()));

    ProgressDialog *pd =
        new ProgressDialog(jobs, dir, false, ui->chkDuplicates->isChecked(), ui->cmbFormats->currentText(), this);
    pd->show();
}

QProcessEnvironment MainWindow::getResticEnv()
{
    QProcessEnvironment sysenv = QProcessEnvironment::systemEnvironment();
    QProcessEnvironment env;
    env.insert("TMP", sysenv.value("TMP")); // https://golang.org/pkg/os/#TempDir
    env.insert("LOCALAPPDATA", sysenv.value("LOCALAPPDATA"));
    env.insert("B2_ACCOUNT_ID", ui->txtResticB2ID->text());
    env.insert("B2_ACCOUNT_KEY", ui->txtResticB2Key->text());
    env.insert("RESTIC_REPOSITORY", ui->txtResticRepo->text());
    env.insert("RESTIC_PASSWORD", ui->txtResticPassword->text());
    // AWS_ACCESS_KEY_ID
    // AWS_SECRET_ACCESS_KEY
    // s3:s3.wasabisys.com/my-backup-bucket
    // b2:bucket:folder
    return env;
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
    // database_dump.sql mysqldump -u database_user -p --all-databases | restic backup --stdin --stdin-filename
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
