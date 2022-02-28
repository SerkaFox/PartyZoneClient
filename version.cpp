#include "version.h"

/**
 * @brief Возвращает строку с номером версии PartyZone Launcher
 * @return Строка с номером версии
 */
QString Version::versionString()
{
#if VERSION_EXTRA > 0
    return QString("%1.%2.%3").arg(VERSION_MAJOR).arg(VERSION_MINOR).arg(VERSION_EXTRA);
#else
    return QString("%1.%2").arg(VERSION_MAJOR).arg(VERSION_MINOR);
#endif
}

/**
 * @brief Проверяет, устарела ли текущая версия
 * @param last Номер последней версии, полученной от сервера
 * @return True, если текущая версия устарела, иначе false
 */
bool Version::isTooOld(const QString &last)
{
    /* Должно получиться 2 или 3 числовых подстроки */
    QStringList parts = last.split(".");

    if (parts.length() < 2 || parts.length() > 3) {
        return false;
    }

    int major = parts.at(0).toInt();
    int minor = parts.at(1).toInt();
    int extra = 0;

    if (parts.length() > 2) {
        extra = parts.at(2).toInt();
    }

    if (major > VERSION_MAJOR) {
        return true;
    } else if (major < VERSION_MAJOR) {
        return false;
    } else if (minor > VERSION_MINOR) {
        return true;
    } else if (minor < VERSION_MINOR) {
        return false;
    } else if (extra > VERSION_EXTRA) {
        return true;
    } else {
        return false;
    }
}
