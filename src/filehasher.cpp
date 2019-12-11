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

#include "filehasher.h"
#include <QDebug>
#include <QThread>
#include <QFile>

FileHasher::FileHasher(const QString &path, const Settings &settings) :
    path(path), settings(settings)
{
    setAutoDelete(false);
}

FileHasher::~FileHasher()
{
}

void FileHasher::run()
{
    QThread::currentThread()->setPriority(QThread::LowPriority);

    QFile file(path);
    if(!file.exists() || !file.open(QFile::ReadOnly) || !file.isOpen() || !file.isReadable())
    {
        started = done = true;
        hash = "FILE_ERROR: " + file.errorString();
        return;
    }

    size = file.size();
    if(size < 0)
    {
        started = done = true;
        hash = "FILE_ERROR: size < 0";
        return;
    }

    started = true;

    int buffer_size = 1 * 1024 * 1024; // TODO: settings
    char *buffer = new char[static_cast<size_t>(buffer_size)];
    qint64 len = 0;

    QCryptographicHash qch(settings.method);
    while((len = file.read(buffer, buffer_size)) > 0)
    {
        read += len;
        qch.addData(buffer, static_cast<int>(len));
        if(settings.max_read >= 0 && read >= settings.max_read) break;
    }

    hash = qch.result().toHex();

    delete[] buffer;
    done = true;
    //qDebug() << "FileHasher::run" << path << method << size << read << hash;
}

int FileHasher::percent() const
{
    if(size < 1) return 0;
    return static_cast<int>(100 * read / size);
}

QString FileHasher::info() const
{
    return settings.methodStr() + "  " + QString::number(size / 1048576) + " MB  " + name();
}

QString FileHasher::name() const
{
    return path.mid(settings.prefix_len);
}

QString FileHasher::methodStr() const
{
    return settings.methodStr();
}

QString Settings::methodStr() const
{
    return QVariant::fromValue(method).value<QString>().toUpper().replace('_', '-').replace("REALSHA", "SHA");
}
