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

#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QThread>

#include "filehasher.h"

FileHasher::FileHasher(const QString &path, const FileHasherSettings &settings) : m_path(path), m_settings(settings)
{
    setAutoDelete(false);
}

void FileHasher::run()
{
    if (m_settings.maxRead() < 1)
    {
        m_hash = QCryptographicHash::hash(m_path.toUtf8(), m_settings.method()).toHex();
        QFileInfo info(m_path);
        m_size = info.size();
        m_started = m_done = true;
        return;
    }

    QFile file(m_path);
    if (!file.exists() || !file.open(QFile::ReadOnly) || !file.isOpen() || !file.isReadable())
    {
        m_started = m_done = true;
        m_hash = "FILE_ERROR: " + file.errorString();
        return;
    }

    m_size = file.size();
    if (m_size < 0)
    {
        m_started = m_done = true;
        m_hash = "FILE_ERROR: size < 0";
        return;
    }

    QThread::currentThread()->setPriority(QThread::LowPriority);
    m_started = true;

    const int bufferSize = m_settings.bufferSize();
    char *buffer = new char[static_cast<size_t>(bufferSize)];

    QCryptographicHash qch(m_settings.method());
    qint64 len = 0;
    while ((len = file.read(buffer, bufferSize)) > 0)
    {
        m_read += len;
        qch.addData(buffer, static_cast<int>(len));
        if (m_settings.maxRead() >= 0 && m_read >= m_settings.maxRead())
            break;
    }

    delete[] buffer;
    m_hash = qch.result().toHex();
    m_done = true;
    // qDebug() << "FileHasher::run" << path << method << size << read << hash;
}

int FileHasher::percent() const
{
    if (m_size < 1)
    {
        return 0;
    }
    return static_cast<int>(100 * m_read / m_size);
}

qint64 FileHasher::size() const
{
    return m_size;
}

QString FileHasher::info() const
{
    return m_settings.methodStr() + "  " + QString::number(m_size / 1048576) + " MB  " + name();
}

QString FileHasher::name() const
{
    return m_path.mid(m_settings.prefixLen());
}

QString FileHasher::hash() const
{
    return m_hash;
}

QString FileHasher::methodStr() const
{
    return m_settings.methodStr();
}

bool FileHasher::started() const
{
    return m_started;
}

bool FileHasher::done() const
{
    return m_done;
}
