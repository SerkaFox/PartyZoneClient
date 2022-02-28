#include "imagerequestcacheentry.h"

ImageRequestCacheEntry::ImageRequestCacheEntry():
    channelName(),
    boxArtUrl()
{
    imageWidth = 0;
    imageHeight = 0;
}

ImageRequestCacheEntry::ImageRequestCacheEntry(QString channel, int width, int height):
    channelName(channel),
    boxArtUrl()
{
    imageWidth = width;
    imageHeight = height;
}

QString ImageRequestCacheEntry::getChannel() const
{
    return channelName;
}

void ImageRequestCacheEntry::setChannel(const QString &value)
{
    channelName = value;
}

int ImageRequestCacheEntry::getWidth() const
{
    return imageWidth;
}

void ImageRequestCacheEntry::setWidth(int value)
{
    imageWidth = value;
}

int ImageRequestCacheEntry::getHeight() const
{
    return imageHeight;
}

void ImageRequestCacheEntry::setHeight(int value)
{
    imageHeight = value;
}

QString ImageRequestCacheEntry::getBoxArtUrl() const
{
    return boxArtUrl;
}

void ImageRequestCacheEntry::setBoxArtUrl(const QString &value)
{
    boxArtUrl = value;
}
