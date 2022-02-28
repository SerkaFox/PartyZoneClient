#ifndef TWITCHINFO_H
#define TWITCHINFO_H

#include <QString>

class TwitchInfo
{
public:
    TwitchInfo(const QString & channel);

    QString getChannel() const;

    QString getGameId() const;
    void setGameId(const QString &value);

    bool getIsLive() const;
    void setIsLive(bool value);

private:
    QString channelName;
    QString gameId;
    bool isLive;
};

#endif // TWITCHINFO_H
