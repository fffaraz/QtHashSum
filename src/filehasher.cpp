#include "filehasher.h"
#include <QDebug>
#include <QThread>

FileHasher::FileHasher(QString path, QCryptographicHash::Algorithm method, int prefix) :
    path(path), method(method), prefix(prefix)
{
}

FileHasher::~FileHasher()
{
}

void FileHasher::run()
{
    QThread::currentThread()->setPriority(QThread::LowPriority);
    QFile file(path);
    if(!file.exists() || !file.open(QFile::ReadOnly))
    {
        started = done = true;
        hash = "FILE_ERROR";
        return;
    }
    size = file.size();
    started = true;
    QCryptographicHash qch(method);
    int buffer_size = 1 * 1024 * 1024; // TODO: settings
    char *buffer = new char[static_cast<size_t>(buffer_size)];
    qint64 len = 0;
    while((len = file.read(buffer, buffer_size)) > 0)
    {
        read += len;
        qch.addData(buffer, static_cast<int>(len));
    }
    hash = qch.result().toHex();
    delete[] buffer;
    done = true;
    //qDebug() << path << method << size << read << hash;
}

int FileHasher::percent() const
{
    if(size < 1) return 0;
    return static_cast<int>(100 * read / size);
}

QString FileHasher::info() const
{
    return methodStr() + "  " + QString::number(size / 1048576) + " MB  " + name();
}

QString FileHasher::name() const
{
    return path.mid(prefix);
}

QString FileHasher::methodStr() const
{
    return methodStr(method);
}

QString FileHasher::methodStr(QCryptographicHash::Algorithm method)
{
    return QVariant::fromValue(method).value<QString>().toUpper().replace('_', '-').replace("REALSHA", "SHA");
}
