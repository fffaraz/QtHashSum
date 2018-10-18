#include "filehasher.h"
#include <QDebug>

FileHasher::FileHasher(QString path, QCryptographicHash::Algorithm method, int prefix) : path(path), method(method), prefix(prefix)
{
}

void FileHasher::run()
{
    QFile file(path);
    if(!file.exists()) return;
    file.open(QFile::ReadOnly);
    size = file.size();
    started = true;
    QCryptographicHash qch(method);
    int buffer_size = 1 * 1024 * 1024;
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
    qDebug() << path << method << size << read << hash;
}

int FileHasher::percent() const
{
    if(size < 1) return 0;
    return static_cast<int>(100 * read / size);
}

QString FileHasher::info() const
{
    return methodStr() + " : " + name();
}

QString FileHasher::name() const
{
    return path.mid(prefix + 1);
}

QString FileHasher::methodStr() const
{
    return methodStr(method);
}

QString FileHasher::methodStr(QCryptographicHash::Algorithm method)
{
    return QVariant::fromValue(method).value<QString>().toUpper().replace('_', '-');
}
