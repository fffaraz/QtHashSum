# QtHashSum: File Checksum Integrity Verifier & Duplicate File Finder
# Copyright (C) 2018  Faraz Fallahi <fffaraz@gmail.com>

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

DEFINES += APPVERSION=\\\"v1.6.0\\\"

CONFIG += c++11 release optimize_full

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    filehasher.cpp \
    progressdialog.cpp \
    resultdialog.cpp \
    resticdialog.cpp \
    duplicatedialog.cpp

HEADERS += \
    mainwindow.h \
    filehasher.h \
    progressdialog.h \
    resultdialog.h \
    resticdialog.h \
    duplicatedialog.h

FORMS += \
    mainwindow.ui \
    progressdialog.ui \
    resultdialog.ui \
    resticdialog.ui \
    duplicatedialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
