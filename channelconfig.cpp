#include "channelconfig.h"

/**
 * @brief Конструктор по умолчанию
 *
 * Создает недействительный объект с пустым именем канала на Twitch и
 * пустым списком аргументов
 */
ChannelConfig::ChannelConfig():
    twitchName(),
    moonlightArguments(),
    controlIpAndPort()
{
}

/**
 * @brief Конструктор с указанием всех параметров
 * @param twitchname Имя канала на Twitch
 * @param args Аргументы командной строки для запуска клиента Moonlight
 */
ChannelConfig::ChannelConfig(const QString & twitchname, const QStringList & args, const QString & ctl):
    twitchName(twitchname),
    moonlightArguments(args),
    controlIpAndPort(ctl)
{

}

/**
 * @brief Конструктор на основании объекта JSON
 * @param obj Объект JSON хранящий параметры канала
 */
ChannelConfig::ChannelConfig(const QJsonObject & obj):
    twitchName(),
    moonlightArguments()
{
    twitchName = obj.value("twitchName").toString();
    controlIpAndPort = obj.value("controlServer").toString();

    if (obj.contains("moonlightArguments")) {
        for (QJsonValue val: obj.value("moonlightArguments").toArray()) {
            moonlightArguments.append(val.toString());
        }
    }
}

/**
 * @brief Записывает параметры игрового канала в объект JSON
 * @return Объект JSON хранящий параметры канала
 */
QJsonObject ChannelConfig::toJson() const
{
    QJsonObject obj;
    QJsonArray args;

    for (const QString & arg: moonlightArguments) {
        args.append(QJsonValue(arg));
    }

    obj["twitchName"] = QJsonValue(twitchName);
    obj["moonlightArguments"] = args;
    obj["controlServer"] = QJsonValue(controlIpAndPort);

    return obj;
}

/**
 * @brief Возвращает имя игрового канала в Twitch
 * @return Имя игрового канала
 */
const QString & ChannelConfig::getTwitchName() const
{
    return twitchName;
}

/**
 * @brief Возвращает список аргументов для запуска Moonlight
 * @return Список аргументов для запуска Moonlight
 *
 * Возвращаемый список не должен включать в себя имя самого исполнимого
 * файла Moonlight
 */
const QStringList & ChannelConfig::getArguments() const
{
    return moonlightArguments;
}

QString ChannelConfig::getControlIpAndPort() const
{
    return controlIpAndPort;
}
