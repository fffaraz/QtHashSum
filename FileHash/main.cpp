#include <QCoreApplication>
#include <QCryptographicHash>
#include <QDebug>
#include <QDirIterator>
#include <QThreadPool>
#include <QFileInfo>
#include <QDateTime>

struct FileHasher : public QRunnable
{
    QString path;
    QString hash;
    FileHasher(QString path) : path(path) {}
    void run()
    {
        QFile file(path);
        file.open(QFile::ReadOnly);
        QCryptographicHash ch(QCryptographicHash::Sha3_256);
        char buffer[1024 * 1024];
        qint64 len;
        while((len = file.read(buffer, sizeof(buffer))) > 0) ch.addData(buffer, len);
        hash = ch.result().toHex();
        qDebug() << path << hash;
    }
};

void writeFile(QString name, const QMap<QString, QString> &files)
{
    QFile file(name + ".txt");
    if(file.exists()) QFile::rename(name + ".txt", name + QString::number(QDateTime::currentSecsSinceEpoch()) + ".txt");
    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);
        out << QDateTime::currentDateTime().toString() << '\n';
        QMapIterator<QString, QString> itr(files);
        while(itr.hasNext())
        {
            itr.next();
            out << itr.key() << '\t' << QFileInfo(itr.key()).size() << '\t' << itr.value() << '\n';
        }
        file.close();
    }
    else qDebug() << name << file.errorString();
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QMap<QString, QString> newFiles;
    {
        QString path;
        do
        {
            qDebug() << QDir::currentPath() << " ?";
            QTextStream cin(stdin);
            path = cin.readLine().trimmed();
            if(path.size() > 0) QDir::setCurrent(path);
        } while (path.size() > 0);
        QDirIterator it(".", QDirIterator::Subdirectories);
        while(it.hasNext())
        {
            QString file = it.next();
            if(it.fileInfo().isFile() && !file.startsWith("./checksum")) newFiles.insert(file, "");
        }
    }
    QVector<FileHasher*> jobs;
    {
        QMapIterator<QString, QString> itr(newFiles);
        while(itr.hasNext())
        {
            itr.next();
            FileHasher* job = new FileHasher(itr.key());
            job->setAutoDelete(false);
            jobs.append(job);
            QThreadPool::globalInstance()->start(job);
        }
    }
    QThreadPool::globalInstance()->waitForDone();
    qDebug() << "\nDONE.\n";
    for(int i = 0; i < jobs.size(); ++i)
    {
        newFiles[jobs[i]->path] = jobs[i]->hash;
        delete jobs[i];
    }
    QMap<QString, QString> oldFiles;
    {
        QFile checksum("checksum.txt");
        if(checksum.open(QFile::ReadOnly | QFile::Text))
        {
            QTextStream in(&checksum);
            in.readLine();
            while (!in.atEnd())
            {
                QStringList line = in.readLine().split('\t');
                if(line.size() > 2) oldFiles.insert(line[0], line[2]);
            }
            checksum.close();
        }
    }
    QMap<QString, QString> addedFiles;
    QMap<QString, QString> changedFiles;
    {
        QMapIterator<QString, QString> itr(newFiles);
        while(itr.hasNext())
        {
            itr.next();
            if(!oldFiles.contains(itr.key())) addedFiles.insert(itr.key(), itr.value());
            else if(itr.value() != oldFiles[itr.key()]) changedFiles.insert(itr.key(), itr.value());
        }
    }
    QMap<QString, QString> deletedFiles;
    {
        QMapIterator<QString, QString> itr(oldFiles);
        while(itr.hasNext())
        {
            itr.next();
            if(!newFiles.contains(itr.key())) deletedFiles.insert(itr.key(), itr.value());
        }
    }
    writeFile("checksum", newFiles);
    writeFile("checksum-add", addedFiles);
    writeFile("checksum-chg", changedFiles);
    writeFile("checksum-del", deletedFiles);
    qDebug() << "New Files" << newFiles.size();
    qDebug() << "Old Files" << oldFiles.size();
    qDebug() << "Added Files" << addedFiles.size();
    qDebug() << "Changed Files" << changedFiles.size();
    qDebug() << "Deleted Files" << deletedFiles.size();
    return a.exec();
}
