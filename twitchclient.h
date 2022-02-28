#ifndef TWITCHCLIENT_H
#define TWITCHCLIENT_H

#include <QString>
#include <QMap>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QEventLoop>
#include <QPixmap>
#include <QImageReader>

#include "twitchinfo.h"
#include "imagerequestcache.h"

/**
 * @brief Клиент для подключения к Twitch
 */
class TwitchClient: public QObject
{
    Q_OBJECT
public:
    TwitchClient(const QString & client_id, const QString & secret_key, QObject * parent = nullptr);

    void setAuthParameters(const QString & client_id, const QString & secret_key);
    void checkChannelOnline(const QString & channel);
    void requestChannelImage(const QString & channel, int width, int height);

signals:
    void channelOnline(const QString & channel);
    void channelOffline(const QString & channel);
    void channelImageGot(const QString & channel, const QPixmap & pixmap);

private slots:
    void handleReply(QNetworkReply * reply);

private:
    TwitchInfo parseChannelReply(const QString & channel, QNetworkReply * reply);
    QString getImageUrl(const QString & channel, QNetworkReply * reply, int width, int height);
    bool sendRequestAsync(const QString & url);

private:
    QString clientId;
    QString secretKey;

    QMap<QString, QString> channelGames;

    QNetworkAccessManager * networkManager;

    /* Информация о запросах изображений каналов, выполняемых в данный момент */
    ImageRequestCache imageRequests;
};

#endif // TWITCHCLIENT_H
