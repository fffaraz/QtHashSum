#include "mycryptographichash.h"

MyCryptographicHash::MyCryptographicHash(Algorithm method, QObject *parent) :
    QObject(parent),
    QCryptographicHash(method)
{
    l1 = l2 = l3 = 0;
    this->moveToThread(&thread);
    connect(this, SIGNAL(addData(QIODevice*)), this, SLOT(addDataThread(QIODevice*)), Qt::QueuedConnection);
    thread.start();
}

MyCryptographicHash::~MyCryptographicHash()
{
    thread.terminate();
}

void MyCryptographicHash::reset()
{
    l3 = false;
    QCryptographicHash::reset();
}

void MyCryptographicHash::addDataThread(QIODevice* device)
{
    if (!device->isReadable())
        //return false;
        return;

    if (!device->isOpen())
        //return false;
        return;

    l1 = device->size();
    l2 = 0;
    l3 = true;

    char buffer[8 * 1024];
    qint64 length;

    while ((length = device->read(buffer,sizeof(buffer))) > 0)
    {
        l2 += length;
        QCryptographicHash::addData(buffer, length);
    }

    //return device->atEnd();
    emit finished();
}

double MyCryptographicHash::finishedPercent()
{
    if(l3 == false) return 0;
    return (double)l2/(double)l1;
}
