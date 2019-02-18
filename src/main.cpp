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

#include "mainwindow.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qDebug() << "QtHashSum " APPVERSION " compiled with Qt version" << QT_VERSION_STR << "run-time" << qVersion();
    MainWindow w;
    w.show();
    return a.exec();
}

// TODO: cancel operation -> void QThreadPool::cancel(QRunnable * runnable)
// TODO: sort duplicates by product of count x size
// TODO: add none (or very fast: hash of size + first 1mb) to the list of hashing methods (list only)
// TODO: add icon
// TODO: add QElapsedTimer
// TODO: https://github.com/Cyan4973/xxHash
// TODO: https://github.com/cmuratori/meow_hash
// TODO: ignore */desktop.ini, /.git/* in Create Digest tab
// TODO: empty file/folder find & delete
// TODO: restic save settings
// TODO: restic combobox
