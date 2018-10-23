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
// TODO: sort duplicates by product of count x size
// TODO: add none (or very fast: hash of size + first 1mb) to the list of hashing methods (list only)
// TODO: add icon
// TODO: add QElapsedTimer
// TODO: https://github.com/Cyan4973/xxHash
// TODO: https://github.com/cmuratori/meow_hash
