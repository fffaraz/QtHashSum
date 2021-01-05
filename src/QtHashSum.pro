# QtHashSum: File Checksum Integrity Verifier & Duplicate File Finder
# Copyright (C) 2018-2020  Faraz Fallahi <fffaraz@gmail.com>

# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

QT += core gui widgets

TARGET = QtHashSum
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

DEFINES += APPVERSION=\\\"v3.1.0\\\"

CONFIG += c++14 release optimize_full
msvc: QMAKE_CXXFLAGS += /std:c++latest /Zc:__cplusplus

SOURCES += \
    application.cpp \
    duplicatedialog.cpp \
    filehasher.cpp \
    filehashersettings.cpp \
    main.cpp \
    mainwindow.cpp \
    mainwindow_restic.cpp \
    progressdialog.cpp \
    resticdialog.cpp \
    resultdialog.cpp

HEADERS += \
    application.h \
    duplicatedialog.h \
    filehasher.h \
    filehashersettings.h \
    mainwindow.h \
    progressdialog.h \
    resticdialog.h \
    resultdialog.h

FORMS += \
    duplicatedialog.ui \
    mainwindow.ui \
    progressdialog.ui \
    resticdialog.ui \
    resultdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
