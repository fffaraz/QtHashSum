#include <QCoreApplication>
#include <QDebug>
#include "mycryptographichash.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    MyCryptographicHash ch1(MyCryptographicHash::Md5);
    MyCryptographicHash ch2(MyCryptographicHash::Md5);

    QFile file1("F:\\OS\\Ubuntu\\ubuntu-13.10-server-amd64.iso");
    file1.open(QFile::ReadOnly);

    QFile file2("F:\\OS\\Ubuntu\\ubuntu-13.10-desktop-amd64.iso");
    file2.open(QFile::ReadOnly);

    ch1.addData(&file1);
    ch2.addData(&file2);

    while(true)
    {
        double p1 = ch1.finishedPercent();
        double p2 = ch2.finishedPercent();

        qDebug() << "1" << p1;
        qDebug() << "2" << p2;

        if(p1 == 1 && p2 == 1) break;
        QThread::msleep(50);
    }

    qDebug() << "1" << ch1.result().toHex();
    qDebug() << "2" << ch2.result().toHex();

    //return a.exec();
    return 0;
}
