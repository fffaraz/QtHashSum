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

#ifndef FILEHASHER_H
#define FILEHASHER_H

#include <QRunnable>
#include <QString>
#include <QCryptographicHash>

struct FileHasher : public QRunnable
{
    explicit FileHasher(QString path, QCryptographicHash::Algorithm method = QCryptographicHash::Sha3_256, int prefix = 0);
    virtual ~FileHasher();
    virtual void run();
    int percent() const;
    QString info() const;
    QString name() const;
    QString methodStr() const;
    static QString methodStr(QCryptographicHash::Algorithm method);

    QString path;
    QCryptographicHash::Algorithm method;
    int prefix = 0; // file path prefix length
    QString hash; // final hash result
    volatile bool started = false;
    volatile bool done = false;
    volatile qint64 size = 0;
    volatile qint64 read = 0;
};

#endif // FILEHASHER_H
