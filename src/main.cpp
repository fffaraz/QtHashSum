// QtHashSum: File Checksum Integrity Verifier & Duplicate File Finder
// Copyright (C) 2019-2020  Faraz Fallahi <fffaraz@gmail.com>

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

#include "application.h"
#include "filehasher.h"
#include "mainwindow.h"

void cli(QString dir)
{
    QStringList files;
    qint64 totalSize = 0;

    QDirIterator itr(dir, QDir::AllEntries | QDir::Hidden | QDir::System, QDirIterator::Subdirectories);
    while(itr.hasNext())
    {
        QString file = itr.next();
        if(itr.fileInfo().isFile())
        {
            files.append(file);
            totalSize += itr.fileInfo().size();
        }
    }
    std::cout << files.size() << " files, " << totalSize / (1024 * 1024) << " MB total" << std::endl;
    files.sort(Qt::CaseSensitivity::CaseInsensitive);

    FileHasherSettings settings(QCryptographicHash::Algorithm::Sha3_256, dir.size());
    for(int i = 0; i < files.size(); ++i)
    {
        FileHasher fh(files[i], settings);
        fh.run();
        std::cout << fh.hash().toStdString() << " " << fh.size() << " " << fh.name().toStdString() << std::endl;
    }
}

void benchmark(QString file)
{
    for(int i = QCryptographicHash::Md4; i != QCryptographicHash::Sha3_512 + 1; ++i)
    {
        FileHasherSettings settings(static_cast<QCryptographicHash::Algorithm>(i));
        FileHasher fh(file, settings);
        QElapsedTimer timer;
        timer.start();
        fh.run();
        qint64 elapsed = timer.elapsed();
        std::cout << fh.methodStr().toStdString() << "\t" << fh.hash().toStdString() << "\t" << elapsed << std::endl;
    }
}

int main(int argc, char *argv[])
{
    Application application;
    if(argc < 2)
    {
        qDebug() << "QtHashSum " APPVERSION " compiled with Qt version " QT_VERSION_STR " and run-time version" << qVersion() << "CPP" << __cplusplus;
        QApplication app(argc, argv);
        MainWindow w(&application);
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
