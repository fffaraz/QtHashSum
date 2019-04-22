// QtHashSum: File Checksum Integrity Verifier & Duplicate File Finder
// Copyright (C) 2018  Faraz Fallahi <fffaraz@gmail.com>

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

#ifndef PROGRESSDIALOG_H
#define PROGRESSDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QProgressBar>
#include <QTimer>
#include <QElapsedTimer>

#include "filehasher.h"

struct ProgressData
{
    QProgressBar *pb = nullptr;
    QLabel *label = nullptr;
};

namespace Ui {
class ProgressDialog;
}

class ProgressDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ProgressDialog(QVector<FileHasher*> jobs, bool methodName, QWidget *parent);
    ~ProgressDialog();

private:
    void timer_timeout();
    void updateProgress();
    void allDone();
    Ui::ProgressDialog *ui;
    QVector<FileHasher*> jobs;
    bool methodName;
    QVector<ProgressData> pds;
    QTimer timer;
    QElapsedTimer elapsedtimer;
};

#endif // PROGRESSDIALOG_H
