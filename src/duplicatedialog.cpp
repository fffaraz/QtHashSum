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

#include "duplicatedialog.h"
#include "ui_duplicatedialog.h"

#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QGridLayout>
#include <QLabel>
#include <QMessageBox>
#include <QPushButton>

DuplicateDialog::DuplicateDialog(const QList<QString> &pathlist, QString dir, QWidget *parent)
    : QDialog(parent), ui(new Ui::DuplicateDialog)
{
    ui->setupUi(this);
    QGridLayout *layout = new QGridLayout(this);
    bool matchNames = true;
    qint64 filesize = 0;
    for (int i = 0; i < pathlist.size(); ++i)
    {
        if (i != 0)
        {
            QFileInfo fileinfo1(dir + pathlist[i]);
            QFileInfo fileinfo2(dir + pathlist[i - 1]);
            if (fileinfo1.fileName() != fileinfo2.fileName())
                matchNames = false;
        }
        else
            filesize = QFileInfo(dir + pathlist[i]).size();
        QLabel *label = new QLabel(pathlist[i], this);
        layout->addWidget(label, i, 0);
        QPushButton *button = new QPushButton("Remove", this);
        button->setStatusTip(dir + pathlist[i]);
        // qDebug() << "DuplicateDialog" << i << dir + pathlist[i];
        connect(button, SIGNAL(clicked()), this, SLOT(remove_clicked()));
        layout->addWidget(button, i, 1);
    }
    layout->addWidget(new QLabel("Size: " + QString::number(filesize), this));
    if (matchNames)
        layout->addWidget(new QLabel("All files have the same name", this));
    setLayout(layout);
}

DuplicateDialog::~DuplicateDialog()
{
    delete ui;
}

void DuplicateDialog::remove_clicked()
{
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (button != nullptr)
    {
        QFile file(button->statusTip());
        qDebug() << "Removed" << file.fileName() << file.size();
        if (file.exists())
        {
            bool ok = file.remove();
            if (!ok)
                QMessageBox::warning(this, "", "Remove failed!");
        }
        else
        {
            QMessageBox::warning(this, "", "File not found!");
        }
    }
    this->accept();
}
