QT += core gui widgets

TARGET = QtHashSum
TEMPLATE = app
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000

DEFINES += APPVERSION=\\\"v1.2.1\\\"

CONFIG += c++11 release optimize_full

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    filehasher.cpp \
    progressdialog.cpp \
    resultdialog.cpp

HEADERS += \
    mainwindow.h \
    filehasher.h \
    progressdialog.h \
    resultdialog.h

FORMS += \
    mainwindow.ui \
    progressdialog.ui \
    resultdialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
