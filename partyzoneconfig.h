#ifndef PARTYZONECONFIG_H
#define PARTYZONECONFIG_H

#include <QStringList>
#include <QDir>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>

#include "channelconfig.h"

/**
 * @brief Настройки программы
 */
class PartyZoneConfig
{
public:
    PartyZoneConfig();

    /* Возвращает параметр client-id для подключения к Twitch */
    QString getClientId() const;
    /* Возвращает параметр secret-key для подключения к Twitch */
    QString getSecretKey() const;

    /* Возвращает имя Twitch-канала с заданным номером */
    QString getChannelName(int no) const;

    /* Возвращает номер Twitch-канала с заданным именем */
    int getChannelNumber(const QString & channel) const;

    /* Возвращает путь к файлу клиента Moonlight */
    QString getCommandProgram(int no) const;
    /* Возвращает аргументы командной строки клиента Moonlight */
    QStringList getCommandArguments(int no) const;

    void setMoonlightClient(const QString & path);

    void update(const QJsonObject & new_config);
    void update(const QJsonDocument & new_config);
    void updateBase64(const QString & encrypted);

    bool upgradeAvailable() const;
    QString upgradeVersion() const;
    QString upgradeUrl() const;

    QString getFirewallAddress(int game_no) const;

private:
    void initDefault();

private:
    QString clientId;
    QString secretKey;

    QString moonlightPath;

    QString lastVersion;
    QString downloadUrl;

    QList<ChannelConfig> channelOptions;
};

#endif // PARTYZONECONFIG_H
