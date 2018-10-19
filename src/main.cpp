#include "mainwindow.h"
#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    qDebug() << "Compiled with Qt Version" << QT_VERSION_STR << "run-time" << qVersion();
    MainWindow w;
    w.show();
    return a.exec();
}

// TODO: cancel operation -> void QThreadPool::cancel(QRunnable * runnable)
// TODO: add none to the list of hashing methods (list only)
// TODO: add icon
