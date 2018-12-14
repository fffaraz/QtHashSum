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

#include "filehasher.h"
#include <QDebug>
#include <QThread>
#include <QFile>

FileHasher::FileHasher(QString path, QCryptographicHash::Algorithm method, int prefix) :
    path(path), method(method), prefix(prefix)
{
}

FileHasher::~FileHasher()
{
}

void FileHasher::run()
{
    QThread::currentThread()->setPriority(QThread::LowPriority);
    QFile file(path);
    if(!file.exists() || !file.open(QFile::ReadOnly))
    {
        started = done = true;
        hash = "FILE_ERROR";
        return;
    }
    size = file.size();
    started = true;
    QCryptographicHash qch(method);
    int buffer_size = 1 * 1024 * 1024; // TODO: settings
    char *buffer = new char[static_cast<size_t>(buffer_size)];
    qint64 len = 0;
    while((len = file.read(buffer, buffer_size)) > 0)
    {
        read += len;
        qch.addData(buffer, static_cast<int>(len));
        // TODO: break if read > max_read
    }
    hash = qch.result().toHex();
    delete[] buffer;
    done = true;
    //qDebug() << path << method << size << read << hash;
}

int FileHasher::percent() const
{
    if(size < 1) return 0;
    return static_cast<int>(100 * read / size);
}

QString FileHasher::info() const
{
    return methodStr() + "  " + QString::number(size / 1048576) + " MB  " + name();
}

QString FileHasher::name() const
{
    return path.mid(prefix);
}

QString FileHasher::methodStr() const
{
    return methodStr(method);
}

QString FileHasher::methodStr(QCryptographicHash::Algorithm method)
{
    return QVariant::fromValue(method).value<QString>().toUpper().replace('_', '-').replace("REALSHA", "SHA");
}
