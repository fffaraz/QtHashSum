// QtHashSum: File Checksum Integrity Verifier & Duplicate File Finder
// Copyright (C) 2018-2020  Faraz Fallahi <fffaraz@gmail.com>

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

DuplicateDialog::DuplicateDialog(const QStringList &pathList, QString parentDir, QWidget *parent)
    : QDialog(parent), ui(new Ui::DuplicateDialog)
{
    ui->setupUi(this);
    QGridLayout *layout = new QGridLayout(this);

    bool matchNames = true;
    qint64 fileSize = 0;
    for (int i = 0; i < pathList.size(); ++i)
    {
        if (i != 0)
        {
            QFileInfo fileinfo1(parentDir + pathList[i]);
            QFileInfo fileinfo2(parentDir + pathList[i - 1]);
            if (fileinfo1.fileName() != fileinfo2.fileName())
                matchNames = false;
        }
        else
        {
            fileSize = QFileInfo(parentDir + pathList[i]).size();
        }

        QLabel *label = new QLabel(pathList[i], this);
        layout->addWidget(label, i, 0);

        QPushButton *button = new QPushButton("Remove", this);
        button->setStatusTip(parentDir + pathList[i]);
        QObject::connect(button, &QPushButton::clicked, this, &DuplicateDialog::btnRemove_clicked);
        layout->addWidget(button, i, 1);

        // qDebug() << "DuplicateDialog" << i << dir + pathlist[i];
    }
    layout->addWidget(new QLabel("Size: " + QString::number(fileSize), this));
    if (matchNames)
    {
        layout->addWidget(new QLabel("All files have the same name", this));
    }
    setLayout(layout);
}

DuplicateDialog::~DuplicateDialog()
{
    delete ui;
}

QStringList DuplicateDialog::selectedList()
{
    return m_selectedList;
}

void DuplicateDialog::btnRemove_clicked()
{
    QPushButton *button = qobject_cast<QPushButton *>(sender());
    if (button != nullptr)
    {
        // TODO: select multiple duplicate files if pathList.size() > 2
        m_selectedList.append(button->statusTip());
    }
    this->accept();
}
