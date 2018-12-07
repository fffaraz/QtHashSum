#ifndef FILEHASHER_H
#define FILEHASHER_H

#include <QRunnable>
#include <QString>
#include <QCryptographicHash>
#include <QFile>

struct FileHasher : public QRunnable
{
    explicit FileHasher(QString path, QCryptographicHash::Algorithm method = QCryptographicHash::Sha3_256, int prefix = 0);
    virtual ~FileHasher();
    virtual void run();
    int percent() const;
    QString info() const;
    QString name() const;
    QString methodStr() const;
    static QString methodStr(QCryptographicHash::Algorithm method);

    QString path;
    QCryptographicHash::Algorithm method;
    int prefix = 0; // file path prefix length
    QString hash; // final hash result
    volatile bool started = false;
    volatile bool done = false;
    volatile qint64 size = 0;
    volatile qint64 read = 0;
};

#endif // FILEHASHER_H
