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
#include <QDateTime>
#include <QDebug>
#include <QDirIterator>
#include <QThreadPool>

#include "application.h"
#include "duplicatedialog.h"

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

QString Application::getResult(const QVector<FileHasher *> &jobs, QString parentDir, bool methodName, bool removeDups)
{
    qint64 totalSize = 0;
    QMap<QString, int> hash2count;
    QMultiMap<QString, QString> path2info;
    QHash<QString, qint64> hash2size;
    QMultiHash<QString, QString> hash2path;
    for (int i = 0; i < jobs.size(); ++i)
    {
        qint64 size = jobs[i]->size();
        QString hash = jobs[i]->hash();
        QString name = jobs[i]->name();
        QString method = (methodName ? jobs[i]->methodStr() + " " : "");
        delete jobs[i];

        if (methodName == false && name.endsWith("/desktop.ini")) // TODO: wtf?!
        {
            if (hash == "418c4c275b3c0d70d93f046cd8c2a632121b6072c725bf4355b319fe6dc7b9cd")
                continue;
            if (hash == "7ba990a8886cdf4823cba7579d5e1f550d593e01aef15ebbd8d2b216e1c7d36d")
                continue;
            if (hash == "dfc6fa51dd38a197f4294d87357b2fe218377535d294d744924122761c03ca8f")
                continue;
        }
        if (methodName == false && name.contains(".git/objects/"))
            continue; // TODO: add option

        totalSize += size;

        QString info = method + hash + " " + QString::number(size);
        path2info.insertMulti(name, info);

        hash2count[hash] = hash2count[hash] + 1;
        hash2path.insertMulti(hash, name);

        if (!hash2size.contains(hash))
            hash2size.insert(hash, size);
        else if (hash2size.value(hash) != size)
            qDebug() << "ERROR: same hash different size" << hash;
    }

    QString duplicates;
    int num_duplicates = 0;
    qint64 wasted = 0;
    const int min_size[] = {1000, 100, 10, 1, 0};
    for (int i = 0; i < 5; ++i)
    {
        for (auto hash2count_itr = hash2count.constBegin(); hash2count_itr != hash2count.constEnd(); ++hash2count_itr)
        {
            qint64 size_mb = hash2size[hash2count_itr.key()] / (1024 * 1024);
            bool size_ok = (size_mb >= min_size[i] && (i == 0 || size_mb < min_size[i - 1]));
            if (hash2count_itr.value() > 1 && size_ok)
            {
                num_duplicates++;
                wasted += (hash2count_itr.value() - 1) * hash2size[hash2count_itr.key()];
                duplicates.append(QString::number(hash2count_itr.value()) + " " + QString::number(size_mb) + " " +
                                  hash2count_itr.key() + "\n");
                QList<QString> pathlist = hash2path.values(hash2count_itr.key());
                pathlist.sort();
                foreach (QString path, pathlist)
                    duplicates.append("\t" + path + "\n");
                if (removeDups)
                {
                    DuplicateDialog dd(pathlist, parentDir, nullptr);
                    dd.exec();
                }
            }
        }
    }

    QString result;
    result.reserve(jobs.size() * 4096);
    result.append("Checksums by QtHashSum " APPVERSION "\n");
    result.append("https://github.com/fffaraz/QtHashSum\n");
    result.append("Generated at " + QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss") + "\n");
    result.append(QString::number(jobs.size()) + " files hashed, " + QString::number(totalSize / 1048576) +
                  " MB total\n");
    if (num_duplicates > 0)
        result.append(QString::number(num_duplicates) + " duplicates found, " + QString::number(wasted / 1048576) +
                      " MB wasted\n\n" + duplicates);
    result.append("\n");
    for (auto itr = path2info.constBegin(); itr != path2info.constEnd(); ++itr)
        result.append(itr.value() + " " + itr.key() + "\n");

    return result;
}

QString Application::removeDups(const QVector<FileHasher *> &jobsOrig, const QVector<FileHasher *> &jobsDup, QString parentDir)
{
    QMultiHash<QString, QString> hash2path;
    for (int i = 0; i < jobsOrig.size(); ++i)
    {
        hash2path.insertMulti(jobsOrig[i]->hash(), jobsOrig[i]->name());
    }
    QString result;
    result.reserve(jobsDup.size() * 4096);
    for (int i = 0; i < jobsDup.size(); ++i)
    {
        if(hash2path.contains(jobsDup[i]->hash()))
        {
            result.append("del /f \""); // TODO: rm -f
            QString path = parentDir + jobsDup[i]->name();
            result.append(path.replace('/', '\\')); // TODO: win only
            result.append("\"\n");
        }
    }
    return result;
}
