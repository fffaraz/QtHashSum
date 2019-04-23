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
#include <QElapsedTimer>

#include "filehasher.h"
#include "mainwindow.h"

void cli(QString dir)
{
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

void benchmark(QString file)
{
    for(int i = QCryptographicHash::Md4; i != QCryptographicHash::Sha3_512 + 1; ++i)
    {
        FileHasher fh(file, static_cast<QCryptographicHash::Algorithm>(i), 0);
        QElapsedTimer timer;
        timer.start();
        fh.run();
        qint64 elapsed = timer.elapsed();
        std::cout << fh.methodStr().toStdString() << " " << fh.hash.toStdString() << " " << elapsed << std::endl;
    }
}

/*
MD4        741
MD5       1084
SHA1      1117
SHA3-224  1671
SHA3-256  1765
SHA3-384  2275
SHA2-512  2732
SHA2-384  2802
SHA3-512  3247
SHA2-224  3622
SHA2-256  3673
*/

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
        if(dir == "-b") benchmark(app.arguments()[2]);
        else cli(dir);
        return 0;
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
