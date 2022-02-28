#ifndef IMAGEREQUESTCACHE_H
#define IMAGEREQUESTCACHE_H

#include <QString>
#include <QMap>
#include <QDebug>

#include "imagerequestcacheentry.h"

/**
 * @brief Кэш запросов изображений
 */
class ImageRequestCache
{
public:
    /* Инициализация кэша запросов изображений */
    ImageRequestCache();

    /* Начинаем запрос изображения заданного размера для заданной игры */
    void addRequest(const QString & gameid, const QString & channel, int width, int height);
    /* Запоминаем URL изображения для заданного идентификатора игры */
    void setBoxartUrl(const QString & gameid, const QString & url);
    /* После обработки запроса удаляем его из кэша */
    void deleteRequest(const QString & gameid);

    QString getChannel(const QString & gameid) const;
    int getWidth(const QString & gameid) const;
    int getHeight(const QString & gameid) const;

    /* Найти идентификатор игры по ранее запомненному URL изображения */
    QString findByUrl(const QString & url) const;

private:
    QMap<QString, ImageRequestCacheEntry> entries;
};

#endif // IMAGEREQUESTCACHE_H
