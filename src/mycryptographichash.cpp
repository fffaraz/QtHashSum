#include "mycryptographichash.h"

MyCryptographicHash::MyCryptographicHash(Algorithm method, QObject *parent) :
    QObject(parent),
    QCryptographicHash(method)
{
    totalSize = readSize = isStarted = isFinished = 0;
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
    if(!isFinished) return;
    isStarted = false;
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

    totalSize = device->size();
    readSize = 0;
    isStarted = true;

    char buffer[8 * 1024];
    qint64 length;

    while ((length = device->read(buffer,sizeof(buffer))) > 0)
    {
        readSize += length;
        QCryptographicHash::addData(buffer, length);
    }

    //return device->atEnd();
    isFinished = true;
    emit finished();
}

double MyCryptographicHash::finishedPercent()
{
    if(isStarted == false) return 0;
    return (double)readSize/(double)totalSize;
}
