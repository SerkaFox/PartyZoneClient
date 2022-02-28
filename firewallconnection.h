#ifndef FIREWALLCONNECTION_H
#define FIREWALLCONNECTION_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include <QDebug>

class FirewallConnection : public QObject
{
    Q_OBJECT
public:
    explicit FirewallConnection(QObject *parent = nullptr);

    bool isConnected() const;

public slots:
    void startConnection(const QString & peer, const QString & username, const QString & password, const QString & channel);
    void breakConnection();

signals:
    void connectionEastablished(const QString & peer);
    void connectionRefused(const QString & peer);
    void connectionTerminated(const QString & peer);

private slots:
    void tryAuthenticate();
    void reportConnectionError(QAbstractSocket::SocketError error);
    void getAnswer();

private:
    QTcpSocket * controlSock;

    QString peerName;

    QString userName;
    QString channelName;
};

#endif // FIREWALLCONNECTION_H
