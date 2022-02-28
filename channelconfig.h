#ifndef CHANNELCONFIG_H
#define CHANNELCONFIG_H

#include <QStringList>
#include <QJsonObject>
#include <QJsonArray>

/**
 * @brief Настройки для определенного игрового канала
 *
 * Включает имя канала на Twitch и аргументы командной строки для запуска
 * клиента Moonlight
 */
class ChannelConfig
{
public:
    /* Конструктор по умолчанию */
    ChannelConfig();
    /* Конструктор с указанием всех параметров */
    ChannelConfig(const QString & twitchname, const QStringList & args = QStringList(), const QString & ctl = QString());
    /* Конструктор с указанием параметров в JSON объекте */
    ChannelConfig(const QJsonObject &obj);

    /* Преобразует настройки в объект JSON для записи в файл */
    QJsonObject toJson() const;

    /* Возвращает имя канала наT witch */
    const QString & getTwitchName() const;
    /* Возвращает аргументы командной строки для клиента Moonlight */
    const QStringList & getArguments() const;
    /* IP адрес и порт управляющего соединения с Partyzone Firewall */
    QString getControlIpAndPort() const;

private:
    /* Имя канала на Twitch */
    QString twitchName;
    /* Аргументы командной строки для запуска Moonlight */
    QStringList moonlightArguments;
    /* IP адрес и порт управляющего соединения с Partyzone Firewall */
    QString controlIpAndPort;
};

#endif // CHANNELCONFIG_H
