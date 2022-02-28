#ifndef IMAGEREQUESTCACHEENTRY_H
#define IMAGEREQUESTCACHEENTRY_H

#include <QString>

class ImageRequestCacheEntry
{
public:
    ImageRequestCacheEntry();
    ImageRequestCacheEntry(QString channel, int width, int height);

    QString getChannel() const;
    void setChannel(const QString &value);

    int getWidth() const;
    void setWidth(int value);

    int getHeight() const;
    void setHeight(int value);

    QString getBoxArtUrl() const;
    void setBoxArtUrl(const QString &value);

private:
    QString channelName;
    int imageWidth;
    int imageHeight;
    QString boxArtUrl;
};

#endif // IMAGEREQUESTCACHEENTRY_H
