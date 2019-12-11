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

#pragma once

#include <QProcess>

#include "filehasher.h"

class Application
{
public:
    Application();
    QVector<FileHasher*> parseDir(QString dir, QCryptographicHash::Algorithm method);
    QProcessEnvironment getResticEnv(QString b2id, QString b2key, QString repo, QString pass);
    void setMaxThreadCount(int threads);
    int maxThreadCount = 0;
};
