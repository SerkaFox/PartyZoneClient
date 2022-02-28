#ifndef VERSION_H
#define VERSION_H

#define VERSION_MAJOR   0
#define VERSION_MINOR   5
#define VERSION_EXTRA   6

#include <QString>
#include <QStringList>

/**
 * @brief Номер текущей версии программы
 *
 * Все методы этого класса статические
 */
class Version
{
public:
    /* Возвращает строку с номером текущей версии */
    static QString versionString();

    /* Проверяет, что текущая версия устарела */
    static bool isTooOld(const QString & last);
};

#endif // VERSION_H
