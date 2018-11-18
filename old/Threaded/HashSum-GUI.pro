#-------------------------------------------------
#
# Project created by QtCreator 2014-03-23T00:20:43
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HashSum-GUI
TEMPLATE = app


SOURCES += gui/main.cpp\
        gui/mainwindow.cpp \
    mycryptographichash.cpp

HEADERS  += gui/mainwindow.h \
    mycryptographichash.h

FORMS    += gui/mainwindow.ui
