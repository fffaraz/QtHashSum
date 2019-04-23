#include "duplicatedialog.h"
#include "ui_duplicatedialog.h"

#include <QGridLayout>
#include <QLabel>
#include <QPushButton>

DuplicateDialog::DuplicateDialog(const QList<QString> &pathlist, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DuplicateDialog)
{
    ui->setupUi(this);
    QGridLayout *layout = new QGridLayout(this);
    for(int i = 0; i < pathlist.size(); ++i)
    {
       QLabel *label = new QLabel(pathlist[i], this);
       layout->addWidget(label, i, 0);
       QPushButton *button = new QPushButton("Remove", this);
       layout->addWidget(button, i, 1);
    }
    setLayout(layout);
}

DuplicateDialog::~DuplicateDialog()
{
    delete ui;
}
