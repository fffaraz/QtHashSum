#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connect(&timer, SIGNAL(timeout()), this, SLOT(timer_timeout()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_btnBrowse_clicked()
{
    ui->txtFile->setText(QFileDialog::getOpenFileName(this));
}

void MainWindow::on_btnStart_clicked()
{
    ui->txtResult->clear();
    file = new QFile(ui->txtFile->text());
    if(!file->exists()) return;
    file->open(QFile::ReadOnly);

    ui->btnStart->setEnabled(false);

    chash = new MyCryptographicHash(MyCryptographicHash::Md5);
    connect(chash, SIGNAL(finished()), this, SLOT(chash_finished()));

    timer.start(50);
    chash->addData(file);
}

void MainWindow::timer_timeout()
{
    ui->progressBar->setValue(chash->finishedPercent() * 100);
}

void MainWindow::chash_finished()
{
    timer.stop();
    ui->progressBar->setValue(100);
    ui->txtResult->setText(chash->result().toHex());
    chash->deleteLater();
    file->deleteLater();
    ui->btnStart->setEnabled(true);
}
