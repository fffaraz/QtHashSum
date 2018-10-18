#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

// TODO: cancel operation -> void QThreadPool::cancel(QRunnable * runnable)
// TODO: add none to the list of hashing methods (list only)
