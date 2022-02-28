#include "twitchclient.h"

TwitchClient::TwitchClient(const QString & client_id, const QString & secret_key, QObject * parent):
    QObject(parent),
    clientId(client_id),
    secretKey(secret_key),
    channelGames(),
    imageRequests()
{
    networkManager = new QNetworkAccessManager(this);
    connect(networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(handleReply(QNetworkReply*)));
}

void TwitchClient::setAuthParameters(const QString &client_id, const QString &secret_key)
{
    clientId = client_id;
    secretKey = secret_key;
}

/**
 * @brief Запускает проверку доступности канала на Twitch
 * @param channel Идентификатор канала на Twitch
 *
 * Посылает запрос к Twitch о проверке доступности канала. После этого управление
 * возвращается немедленно, ответ нужно получать через сигналы channelOnline
 * и channelOffline
 */
void TwitchClient::checkChannelOnline(const QString &channel)
{
    if (!sendRequestAsync(QString("https://api.twitch.tv:443/helix/search/channels?query=%1").arg(channel))) {
        emit channelOffline(channel);
    }
}

void TwitchClient::requestChannelImage(const QString &channel, int width, int height)
{
    if (!channelGames.contains(channel)) {
        /* Тут бы запросить для этого канала номер игры, а потом запросить картинку.
         * А пока просто возвращаем пустую картинку
         */
        emit channelImageGot(channel, QPixmap());
    }

    QString gameid = channelGames.value(channel);

    imageRequests.addRequest(gameid, channel, width, height);

    if (!sendRequestAsync(QString("https://api.twitch.tv:443/helix/games?id=%1").arg(gameid))) {
        /* Отправить запрос не удалось, сеть не работает */
        emit channelOffline(channel);
        emit channelImageGot(channel, QPixmap());

        /* Не ждем ответа на этот запрос */
        imageRequests.deleteRequest(gameid);
    }
}

void TwitchClient::handleReply(QNetworkReply * reply)
{
    QString url = reply->request().url().toString();

    qDebug() << "Got answer from url: " << url;

    if (url.startsWith("https://api.twitch.tv:443/helix/search/channels?query=")) {
        QString channel = url.split("query=").last();

        TwitchInfo info = parseChannelReply(channel, reply);

        qDebug() << "Remembering channel" << channel << "to have game id" << info.getGameId();
        channelGames[channel] = info.getGameId();

        if (info.getIsLive()) {
            qDebug() << "Reporting channel" << channel << "to be online";
            emit channelOnline(channel);
        } else {
            qDebug() << "Reporting channel" << channel << "to be offline";
            emit channelOffline(channel);
        }
    } else if (url.startsWith("https://api.twitch.tv:443/helix/games?id=")) {
        QString gameid = url.split("id=").last();
        QString channel = channelGames.key(gameid);

        int width = imageRequests.getWidth(gameid);
        int height = imageRequests.getHeight(gameid);

        QString boxart_url = getImageUrl(channel, reply, width, height);

        boxart_url.replace("%20", " ");

        qDebug() << "Image URL for channel" << channel << "is" << boxart_url;

        imageRequests.setBoxartUrl(gameid, boxart_url);

        /* Запрашиваем с сервера изображение */
        sendRequestAsync(boxart_url);
    } else {
        QString gameid = imageRequests.findByUrl(url);

        if (gameid.isEmpty()) {
            qWarning() << "Got reply for unknown url" << url;
        } else {
            QImageReader reader(reply);
            QImage pic = reader.read();

            qDebug() << "Returning image for channel" << imageRequests.getChannel(gameid);

            emit channelImageGot(imageRequests.getChannel(gameid), QPixmap::fromImage(pic));

            imageRequests.deleteRequest(gameid);
        }
    }
}

TwitchInfo TwitchClient::parseChannelReply(const QString &channel, QNetworkReply *reply)
{
    QByteArray ba = reply->readAll();

    TwitchInfo info(channel);

    QJsonDocument doc = QJsonDocument::fromJson(ba);

    QJsonArray data = doc.object().value("data").toArray();

    for (int i = 0; i < data.count(); i++) {
        QJsonObject item = data.at(i).toObject();

        if (item.value("display_name").toString().toLower() != channel.toLower()) {
            continue;
        }

        if (item.contains("is_live")) {
            if (item.contains("game_id")) {
                info.setGameId(item.value("game_id").toString());
            }

            info.setIsLive(item.value("is_live").toBool());
            return info;
        }
    }

    return info;
}

/**
 * @brief Возвращает URL рисунка для канала в Twitch
 * @param reply Ответ на HTTP-запрос вида https://api.twitch.tv/helix/games?id=<ID>
 * @param width Желаемая ширина изображения
 * @param height Желаемая высота изображения
 * @return URL изображения заданной игры, заданной ширины и высоты
 */
QString TwitchClient::getImageUrl(const QString & channel, QNetworkReply *reply, int width, int height)
{
    QByteArray ba = reply->readAll();

    QJsonDocument doc = QJsonDocument::fromJson(ba);

    QJsonArray data = doc.object().value("data").toArray();

    for (int i = 0; i < data.count(); i++) {
        QJsonObject item = data.at(i).toObject();

        if (item.contains("box_art_url")) {
            QString s = item.value("box_art_url").toString();

            s = s.replace("{width}", QString::number(width));
            s = s.replace("{height}", QString::number(height));

            return s;
        }
    }

    return QString();
}

bool TwitchClient::sendRequestAsync(const QString &url)
{
    qDebug() << "Sending network request: " << url;

    QNetworkRequest req(url);
    QSslConfiguration cfg = QSslConfiguration::defaultConfiguration();

    cfg.setPeerVerifyMode(QSslSocket::VerifyNone);
    cfg.setProtocol(QSsl::AnyProtocol);

    req.setSslConfiguration(cfg);
    req.setRawHeader("client-id", clientId.toUtf8());
    req.setRawHeader("Authorization", QString("Bearer %1").arg(secretKey).toUtf8());

    networkManager->get(req);

    return true;
}
