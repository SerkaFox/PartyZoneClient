#include "partyzoneconfig.h"
#include "version.h"

PartyZoneConfig::PartyZoneConfig():
    clientId(),
    secretKey(),
    moonlightPath(),
    lastVersion(),
    downloadUrl(),
    channelOptions()
{
    initDefault();
}

QString PartyZoneConfig::getClientId() const
{
    return clientId;
}

QString PartyZoneConfig::getSecretKey() const
{
    return secretKey;
}

QString PartyZoneConfig::getChannelName(int no) const
{
    return channelOptions.at(no).getTwitchName();
}

int PartyZoneConfig::getChannelNumber(const QString &channel) const
{
    for (int no = 0; no < channelOptions.length(); no++) {
        if (channelOptions.at(no).getTwitchName() == channel) {
            return no;
        }
    }

    return -1;
}

QString PartyZoneConfig::getCommandProgram(int no) const
{
    return moonlightPath;
}

QStringList PartyZoneConfig::getCommandArguments(int no) const
{
    return channelOptions.at(no).getArguments();
}

void PartyZoneConfig::setMoonlightClient(const QString &path)
{
    moonlightPath = path;
}

void PartyZoneConfig::update(const QJsonObject &new_config)
{
    if (new_config.contains("twitch")) {
        QJsonObject twitch = new_config.value("twitch").toObject();
        clientId = twitch["clientId"].toString();
        secretKey = twitch["secretKey"].toString();
    }
    if (new_config.contains("upgradeVersion")) {
        QJsonObject upgrade = new_config.value("upgradeVersion").toObject();
        lastVersion = upgrade.value("latestVersion").toString();
        downloadUrl = upgrade.value("downloadUrl").toString();
    }
    if (new_config.contains("channels")) {
        channelOptions.clear();

        QJsonArray channels = new_config["channels"].toArray();

        for (const QJsonValue & ch: channels) {
            channelOptions.append(ChannelConfig(ch.toObject()));
        }

        if (channelOptions.length() != 6) {
            qWarning() << "Warning! Got" << channelOptions.length() << "channels, expected 6";
        }
    }
}

void PartyZoneConfig::update(const QJsonDocument &new_config)
{
    QJsonObject obj = new_config.object();
    update(obj);
}

void PartyZoneConfig::updateBase64(const QString &encrypted)
{
    QByteArray ba = QByteArray::fromBase64(encrypted.toUtf8());

    QJsonDocument doc = QJsonDocument::fromJson(ba);

    update(doc);
}

/**
 * @brief Проверяет, что текущая версия устарела и нужно обновление
 * @return True, если текущая версия устарела и нужно обновление
 *
 * Текущая версия считается устаревшей, если от сервера получен номер
 * последней версии и он больше номера текущей, и при этом получена
 * ссылка на скачивание новой версии
 */
bool PartyZoneConfig::upgradeAvailable() const
{
    qDebug() << "last version" << lastVersion;

    return !lastVersion.isEmpty() && !downloadUrl.isEmpty() && Version::isTooOld(lastVersion);
}

QString PartyZoneConfig::upgradeVersion() const
{
    return lastVersion;
}

QString PartyZoneConfig::upgradeUrl() const
{
    return downloadUrl;
}

QString PartyZoneConfig::getFirewallAddress(int game_no) const
{
    return channelOptions.at(game_no).getControlIpAndPort();
}

void PartyZoneConfig::initDefault()
{
    moonlightPath = QString("moonlight");

    channelOptions.clear();
    channelOptions.append(ChannelConfig("Party_Zone"));
    channelOptions.append(ChannelConfig("Partyzone_live"));
    channelOptions.append(ChannelConfig("Partyzone_chat"));
    channelOptions.append(ChannelConfig("Partyzone_ps3"));
    channelOptions.append(ChannelConfig("Partyzone_ps4"));
    channelOptions.append(ChannelConfig("Partyzone_ns"));
}
