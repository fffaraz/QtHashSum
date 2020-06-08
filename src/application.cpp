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

#include <QCoreApplication>
#include <QDebug>
#include <QDirIterator>
#include <QThreadPool>

#include "application.h"

Application::Application()
{
    m_maxThreadCount = QThreadPool::globalInstance()->maxThreadCount();
}

QVector<FileHasher *> Application::parseDir(QString dir, QCryptographicHash::Algorithm method)
{
    FileHasherSettings settings(method, dir.size());

    QDirIterator itr(dir, QDir::AllEntries | QDir::Hidden | QDir::System, QDirIterator::Subdirectories);
    int items = 0;
    quint64 totalSize = 0;
    QVector<FileHasher *> jobs;
    while (itr.hasNext())
    {
        items++;

        if (items % 1000 == 0)
        {
            qDebug() << "items, totalsize" << items << 1.0 * totalSize / (1024 * 1024 * 1024);
            // TODO: file listing progress -> main windows status bar
            QCoreApplication::processEvents();
        }

        QString file = itr.next();
        if (itr.fileInfo().isFile())
        {
            // TODO: skip ignored files (/proc, /dev, ...)
            totalSize += static_cast<quint64>(itr.fileInfo().size());
            jobs.append(new FileHasher(file, settings));
        }
    }
    qDebug() << "Application::parseDir [items, files, totalsize]" << items << jobs.size()
             << 1.0 * totalSize / (1024 * 1024 * 1024);
    return jobs;
}

QProcessEnvironment Application::getResticEnv(QString b2id, QString b2key, QString repo, QString pass)
{
    QProcessEnvironment sysEnv = QProcessEnvironment::systemEnvironment();
    QProcessEnvironment result;
    result.insert("TMP", sysEnv.value("TMP")); // https://golang.org/pkg/os/#TempDir
    result.insert("LOCALAPPDATA", sysEnv.value("LOCALAPPDATA"));
    result.insert("B2_ACCOUNT_ID", b2id);
    result.insert("B2_ACCOUNT_KEY", b2key);
    result.insert("RESTIC_REPOSITORY", repo);
    result.insert("RESTIC_PASSWORD", pass);
    // AWS_ACCESS_KEY_ID
    // AWS_SECRET_ACCESS_KEY
    // s3:s3.wasabisys.com/my-backup-bucket
    // b2:bucket:folder
    return result;
}

void Application::setMaxThreadCount(int threads)
{
    qDebug() << "Application::setMaxThreadCount [num, max]" << threads << m_maxThreadCount;
    if (threads < 1)
        QThreadPool::globalInstance()->setMaxThreadCount(m_maxThreadCount);
    else
        QThreadPool::globalInstance()->setMaxThreadCount(threads);
}

int Application::maxThreadCount() const
{
    return m_maxThreadCount;
}
