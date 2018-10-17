#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    for (int i = 1; i <= QThreadPool::globalInstance()->maxThreadCount() * 2; ++i)
    {
        ui->cmbThreads->addItem(QString::number(i));
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
