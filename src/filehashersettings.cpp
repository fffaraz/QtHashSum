#include <QVariant>

#include "filehashersettings.h"

FileHasherSettings::FileHasherSettings(QCryptographicHash::Algorithm method, int prefixLen)
    : m_method{method}, m_prefixLen(prefixLen)
{
}

QString FileHasherSettings::methodStr() const
{
    return QVariant::fromValue(m_method).value<QString>().toUpper().replace('_', '-').replace("REALSHA", "SHA");
}
