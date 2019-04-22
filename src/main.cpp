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

#include <iostream>

#include <QApplication>
#include <QDebug>
#include <QDirIterator>

#include "filehasher.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    qDebug() << "QtHashSum " APPVERSION " compiled with Qt version " QT_VERSION_STR " and run-time version" << qVersion();
    if(app.arguments().size() < 2)
    {
        MainWindow w;
        w.show();
        return app.exec();
    }
    else
    {
        QString dir = app.arguments()[1];
        QDirIterator itr(dir, QDir::AllEntries | QDir::Hidden | QDir::System, QDirIterator::Subdirectories);
        while(itr.hasNext())
        {
            QString file = itr.next();
            if(itr.fileInfo().isFile())
            {
                FileHasher fh(file, QCryptographicHash::Algorithm::Sha3_256, dir.size());
                fh.run();
                std::cout << fh.hash.toStdString() << " " << fh.size << " " << fh.name().toStdString() << std::endl;
            }
        }
    }
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
