#include "duplicatedialog.h"
#include "ui_duplicatedialog.h"

#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QGridLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>

DuplicateDialog::DuplicateDialog(const QList<QString> &pathlist, QString dir, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DuplicateDialog)
{
    ui->setupUi(this);
    QGridLayout *layout = new QGridLayout(this);
    bool matchNames = true;
    for(int i = 0; i < pathlist.size(); ++i)
    {
        if(i != 0)
        {
            QFileInfo fileinfo1(pathlist[i]);
            QFileInfo fileinfo2(pathlist[i - 1]);
            if(fileinfo1.fileName() != fileinfo2.fileName()) matchNames = false;
        }
        QLabel *label = new QLabel(pathlist[i], this);
        layout->addWidget(label, i, 0);
        QPushButton *button = new QPushButton("Remove", this);
        button->setStatusTip(dir + pathlist[i]);
        //qDebug() << "DuplicateDialog" << i << dir + pathlist[i];
        connect(button, SIGNAL(clicked()), this, SLOT(remove_clicked()));
        layout->addWidget(button, i, 1);
    }
    QFileInfo fileinfo(pathlist[0]);
    layout->addWidget(new QLabel("Size: " + QString::number(fileinfo.size()), this));
    if(matchNames) layout->addWidget(new QLabel("All files have the same name", this));
    setLayout(layout);
}

DuplicateDialog::~DuplicateDialog()
{
    delete ui;
}

void DuplicateDialog::remove_clicked()
{
    QPushButton* button = qobject_cast<QPushButton*>(sender());
    if(button != nullptr)
    {
        QFile file(button->statusTip());
        qDebug() << "Removed" << file.fileName() << file.size();
        if(file.exists())
        {
            bool ok = file.remove();
            if(!ok) QMessageBox::warning(this, "", "Remove failed!");
        }
        else
        {
            QMessageBox::warning(this, "", "File not found!");
        }
    }
    this->accept();
}
