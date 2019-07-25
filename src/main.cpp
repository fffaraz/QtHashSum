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

#include <iostream>

#include <QApplication>
#include <QDebug>
#include <QDirIterator>
#include <QElapsedTimer>

#include "filehasher.h"
#include "mainwindow.h"

void cli(QString dir)
{
    QStringList files;
    qint64 size = 0;
    QDirIterator itr(dir, QDir::AllEntries | QDir::Hidden | QDir::System, QDirIterator::Subdirectories);
    while(itr.hasNext())
    {
        QString file = itr.next();
        if(itr.fileInfo().isFile())
        {
            files.append(file);
            size += itr.fileInfo().size();
        }
    }
    std::cout << files.size() << " files hashed, " << size / 1048576 << " MB total" << std::endl;
    files.sort(Qt::CaseSensitivity::CaseInsensitive);
    for(int i = 0; i < files.size(); ++i)
    {
        FileHasher fh(files[i], QCryptographicHash::Algorithm::Sha3_256, dir.size());
        fh.run();
        std::cout << fh.hash.toStdString() << " " << fh.size << " " << fh.name().toStdString() << std::endl;
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
MD4       3123  3163
MD5       4315  4415
SHA1      4608  4660
SHA3-224  6742  6898
SHA3-256  7098  7306
SHA3-384  9091  9287
SHA3-512 12677 13065
SHA512   12928 13217
SHA384   13043 13183
SHA256   17487 17458
SHA224   17330 17501
*/

int main(int argc, char *argv[])
{
    if(argc < 2)
    {
        qDebug() << "QtHashSum " APPVERSION " compiled with Qt version " QT_VERSION_STR " and run-time version" << qVersion() << "CPP" << __cplusplus;
        QApplication app(argc, argv);
        MainWindow w;
        w.show();
        return app.exec();
    }
    else
    {
        QString dir = argv[1];
        if(dir == "-b") benchmark(argv[2]);
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
