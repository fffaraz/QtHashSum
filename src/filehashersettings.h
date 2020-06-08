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

#include <QCryptographicHash>
#include <QString>

class FileHasherSettings final
{
public:
    explicit FileHasherSettings(QCryptographicHash::Algorithm method = QCryptographicHash::Sha3_256, int prefixLen = 0);
    QString methodStr() const;
    inline QCryptographicHash::Algorithm method() const noexcept { return m_method; }
    inline int prefixLen() const noexcept { return m_prefixLen; }
    inline qint64 maxRead() const noexcept { return m_maxRead; }

private:
    QCryptographicHash::Algorithm m_method;
    int m_prefixLen; //< File path root directory length
    qint64 m_maxRead = -1; //< File size read limit
};
