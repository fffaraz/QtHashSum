#ifndef MYCRYPTOGRAPHICHASH_H
#define MYCRYPTOGRAPHICHASH_H

#include <QObject>
#include <QThread>
#include <QCryptographicHash>
#include <QIODevice>
#include <QFile>

class MyCryptographicHash : public QObject, public QCryptographicHash
{
    Q_OBJECT
public:
    explicit MyCryptographicHash(Algorithm method, QObject *parent = 0);
    ~MyCryptographicHash();
    void reset();
    double finishedPercent();

signals:
    void addData(QIODevice* device);
    void finished();

private:
    QThread thread;
    volatile qint64 l1;
    volatile qint64 l2;
    volatile bool l3;

private slots:
    void addDataThread(QIODevice* device);

};

#endif // MYCRYPTOGRAPHICHASH_H
