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
