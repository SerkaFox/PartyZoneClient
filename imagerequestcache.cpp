#include "imagerequestcache.h"

ImageRequestCache::ImageRequestCache():
    entries()
{
}

void ImageRequestCache::addRequest(const QString &gameid, const QString &channel, int width, int height)
{
    if (entries.contains(gameid)) {
        qCritical() << "Error! Channel image requested while previous request in progress!";
    }
    entries[gameid] = ImageRequestCacheEntry(channel, width, height);
}

void ImageRequestCache::setBoxartUrl(const QString &gameid, const QString &url)
{
    entries[gameid].setBoxArtUrl(url);
}

void ImageRequestCache::deleteRequest(const QString &gameid)
{
    entries.remove(gameid);
}

QString ImageRequestCache::getChannel(const QString &gameid) const
{
    return entries[gameid].getChannel();
}

int ImageRequestCache::getWidth(const QString &gameid) const
{
    return entries[gameid].getWidth();
}

int ImageRequestCache::getHeight(const QString &gameid) const
{
    return entries[gameid].getHeight();
}

QString ImageRequestCache::findByUrl(const QString &url) const
{
    for (const QString & gameid: entries.keys()) {
        if (entries[gameid].getBoxArtUrl() == url) {
            return gameid;
        }
    }

    return QString();
}
