#include "twitchinfo.h"

TwitchInfo::TwitchInfo(const QString &channel):
    channelName(channel)
{
    isLive = false;
}

QString TwitchInfo::getChannel() const
{
    return channelName;
}

QString TwitchInfo::getGameId() const
{
    return gameId;
}

void TwitchInfo::setGameId(const QString &value)
{
    gameId = value;
}

bool TwitchInfo::getIsLive() const
{
    return isLive;
}

void TwitchInfo::setIsLive(bool value)
{
    isLive = value;
}
