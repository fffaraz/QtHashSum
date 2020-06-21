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

#include <QDebug>
#include <QFileDialog>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "filehasher.h"
#include "progressdialog.h"
#include "resultdialog.h"

MainWindow::MainWindow(Application *application, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), application(application)
{
    ui->setupUi(this);
    this->setWindowTitle("QtHashSum " APPVERSION);

    for (int i = 1; i <= application->maxThreadCount(); ++i)
        ui->cmbThreads->addItem(QString::number(i));
    ui->cmbThreads->setCurrentIndex(2);

    for (int i = QCryptographicHash::Md4; i != QCryptographicHash::Sha3_512 + 1; ++i)
        ui->cmbMethods->addItem(FileHasherSettings(static_cast<QCryptographicHash::Algorithm>(i)).methodStr());
    ui->cmbMethods->setCurrentIndex(QCryptographicHash::Sha3_256);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    Q_UNUSED(event)
    QCoreApplication::quit();
}

void MainWindow::on_btnBrowseFile_clicked()
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

void MainWindow::on_btnStartFile_clicked()
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

    QVector<FileHasher *> jobs;
    for (QCryptographicHash::Algorithm method : methods)
    {
        FileHasherSettings settings(method, file.absolutePath().size());
        jobs.append(new FileHasher(file.absoluteFilePath(), settings));
    }

    ProgressDialog *pd = new ProgressDialog(jobs, this);
    pd->exec();

    QString result = Application::getResult(jobs, "", true, false);

    ResultDialog *rd = new ResultDialog(result);
    rd->show();
}

void MainWindow::on_btnStartDir_clicked()
{
    QString dir = ui->txtDir->text();
    if (dir.size() < 1)
        return;

    auto hashAlgo = static_cast<QCryptographicHash::Algorithm>(ui->cmbMethods->currentIndex());
    QVector<FileHasher *> jobs = application->parseDir(dir, hashAlgo);

    ProgressDialog *pd = new ProgressDialog(jobs, this);
    pd->exec();

    // TODO: ui->cmbFormats->currentText()
    QString result = Application::getResult(jobs, dir, false, ui->chkDuplicates->isChecked());

    ResultDialog *rd = new ResultDialog(result);
    rd->show();
}

void MainWindow::on_btnBrowseOrig_clicked()
{
    ui->txtDirOrig->setText(QFileDialog::getExistingDirectory(this));
}

void MainWindow::on_btnBrowseDup_clicked()
{
    ui->txtDirDup->setText(QFileDialog::getExistingDirectory(this));
}

void MainWindow::on_btnStartDup_clicked()
{
    QString dirOrig = ui->txtDirOrig->text();
    if (dirOrig.size() < 1)
        return;

    QString dirDup = ui->txtDirDup->text();
    if (dirDup.size() < 1)
        return;

    auto hashAlgo = QCryptographicHash::Algorithm::Md4;

    qDebug() << "Listing Original Directory";
    QVector<FileHasher *> jobsOrig = application->parseDir(dirOrig, hashAlgo);
    ProgressDialog *pd1 = new ProgressDialog(jobsOrig, this);
    pd1->exec();

    qDebug() << "Listing Duplicate Directory";
    QVector<FileHasher *> jobsDup = application->parseDir(dirDup, hashAlgo);
    ProgressDialog *pd2 = new ProgressDialog(jobsDup, this);
    pd2->exec();

    QString result = Application::removeDups(jobsOrig, jobsDup, dirDup);

    ResultDialog *rd = new ResultDialog(result);
    rd->show();
}
