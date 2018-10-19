#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>
#include <QDebug>
#include <QDirIterator>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    maxThreadCount = QThreadPool::globalInstance()->maxThreadCount();
    for(int i = 1; i <= maxThreadCount * 2; ++i) ui->cmbThreads->addItem(QString::number(i));
    for(int i = QCryptographicHash::Md4; i != QCryptographicHash::Sha3_512; ++i) ui->cmbMethods->addItem(FileHasher::methodStr(static_cast<QCryptographicHash::Algorithm>(i)));
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
    int threads = arg1.toInt();
    qDebug() << "on_cmbThreads_currentIndexChanged" << threads << maxThreadCount;
    if(threads < 1) QThreadPool::globalInstance()->setMaxThreadCount(maxThreadCount);
    else QThreadPool::globalInstance()->setMaxThreadCount(threads);
}

void MainWindow::on_btnStart_clicked()
{
    QVector<QCryptographicHash::Algorithm> methods;
    if(ui->chkMD5->isChecked()) methods.append(QCryptographicHash::Md5);
    if(ui->chkSHA1->isChecked()) methods.append(QCryptographicHash::Sha1);
    if(ui->chkSHA2_256->isChecked()) methods.append(QCryptographicHash::Sha256);
    if(ui->chkSHA2_512->isChecked()) methods.append(QCryptographicHash::Sha512);
    if(ui->chkSHA3_256->isChecked()) methods.append(QCryptographicHash::Sha3_256);
    if(ui->chkSHA3_512->isChecked()) methods.append(QCryptographicHash::Sha3_512);
    QVector<FileHasher*> jobs;
    foreach(QCryptographicHash::Algorithm method, methods)
    {
        QFileInfo file(ui->txtFile->text());
        FileHasher* fh = new FileHasher(file.absoluteFilePath(), method, file.absolutePath().size());
        fh->setAutoDelete(false);
        jobs.append(fh);
    }
    ProgressDialog *pd = new ProgressDialog(jobs, this);
    pd->show();
}

void MainWindow::on_btnStartDir_clicked()
{
    QCryptographicHash::Algorithm method = static_cast<QCryptographicHash::Algorithm>(ui->cmbMethods->currentIndex());
    QVector<FileHasher*> jobs;
    QString dir = ui->txtDir->text();
    QDirIterator it(dir, QDirIterator::Subdirectories);
    int files = 0;
    while(it.hasNext())
    {
        files++;
        if(files % 1000 == 0)
        {
            qDebug() << "files" << files;
            QCoreApplication::processEvents();
        }
        QString file = it.next();
        if(it.fileInfo().isFile())
        {
            FileHasher* fh = new FileHasher(file, method, dir.size());
            fh->setAutoDelete(false);
            jobs.append(fh);
        }
    }
    ProgressDialog *pd = new ProgressDialog(jobs, this);
    pd->show();
}
