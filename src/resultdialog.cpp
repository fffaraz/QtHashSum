#include "resultdialog.h"
#include "ui_resultdialog.h"

ResultDialog::ResultDialog(QString result, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ResultDialog)
{
    ui->setupUi(this);
    ui->plainTextEdit->setPlainText(result);
}

ResultDialog::~ResultDialog()
{
    delete ui;
}
