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

#pragma once

#include <QRunnable>

#include "filehashersettings.h"

class FileHasher : public QRunnable
{
  public:
    explicit FileHasher(const QString &path, const FileHasherSettings &settings);
    virtual ~FileHasher() = default;
    virtual void run();

    int percent() const;       //< (m_read / m_size) * 100
    qint64 size() const;       //< File size
    QString info() const;      //< "METHOD SIZE NAME"
    QString name() const;      //< File path without root dir
    QString hash() const;      //< Final result
    QString methodStr() const; //< Hash method name
    bool started() const;
    bool done() const;

  private:
    QString m_path;
    QString m_hash; //< Final hash result
    FileHasherSettings m_settings;

    volatile bool m_started = false;
    volatile bool m_done = false;
    volatile qint64 m_size = 0;
    volatile qint64 m_read = 0;
};
