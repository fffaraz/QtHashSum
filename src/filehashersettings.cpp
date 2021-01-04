#include <QVariant>

#include "filehashersettings.h"

FileHasherSettings::FileHasherSettings(QCryptographicHash::Algorithm method, int prefixLen, qint64 maxRead)
    : m_method{method}, m_prefixLen{prefixLen}, m_maxRead{maxRead}
{
}

QString FileHasherSettings::methodStr() const
{
    return QVariant::fromValue(m_method).value<QString>().toUpper().replace('_', '-').replace("REALSHA", "SHA");
}
