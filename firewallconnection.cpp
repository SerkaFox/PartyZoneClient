#include "firewallconnection.h"

FirewallConnection::FirewallConnection(QObject *parent):
    QObject(parent),
    peerName(),
    userName(),
    channelName()
{
    controlSock = nullptr;
}

bool FirewallConnection::isConnected() const
{
    return (controlSock && controlSock->state() == QTcpSocket::ConnectedState);
}

void FirewallConnection::startConnection(const QString & peer, const QString & username, const QString & password, const QString & channel)
{
    if (controlSock) {
        qWarning() << "Could not connect to Partyzone Firewall at" << peer << ": already connected";

        return;
    }

    controlSock = new QTcpSocket(this);

    qDebug() << "Connecting to Partyzone firewall at" << peer << "with username" << username;

    peerName = peer;
    userName = username;
    channelName = channel;

    connect(controlSock, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(reportConnectionError(QAbstractSocket::SocketError)));
    connect(controlSock, SIGNAL(connected()), this, SLOT(tryAuthenticate()));
    connect(controlSock, SIGNAL(readyRead()), this, SLOT(getAnswer()));

    controlSock->connectToHost(QHostAddress(peer.split(":").first()), peer.split(":").last().toInt());
}

void FirewallConnection::breakConnection()
{
    qDebug() << "Disconnecting from Partyzone firewall";

    if (controlSock) {
        QTcpSocket * cs = controlSock;

        controlSock = nullptr;

        if (cs->state() == QTcpSocket::ConnectedState) {
            cs->write(QString("DISCONNECT\n").toUtf8());
            cs->waitForBytesWritten();
        }
        cs->close();
        cs->deleteLater();
    }

    qDebug() << "Successfully disconnected from Partyzone Firewall";

    peerName.clear();
    userName.clear();
    channelName.clear();
}

void FirewallConnection::tryAuthenticate()
{
    qDebug() << "Authenticating at Partyzone Firewall at" << controlSock->peerName();

    controlSock->write(QString("CONNECT username=%1 channel=%2\n").arg(userName).arg(channelName).toUtf8());
}

void FirewallConnection::reportConnectionError(QAbstractSocket::SocketError error)
{
    qWarning() << "An error occured on connection to Firewall at" << peerName;

    if (controlSock) {
        controlSock->close();
        controlSock->deleteLater();

        controlSock = nullptr;

        emit connectionTerminated(peerName);
    }

    peerName.clear();
    userName.clear();
    channelName.clear();
}

void FirewallConnection::getAnswer()
{
    while (controlSock && controlSock->canReadLine()) {
        QString line = QString::fromUtf8(controlSock->readLine()).trimmed();

        qDebug() << "Got string" << line << "from Partyzone firewall";

        if (line == QString("CONNECTED")) {
            emit connectionEastablished(peerName);
        } else if (line == QString("DISCONNECT") || line == QString("DISCONNECTED")) {
            breakConnection();
        } else if (line == QString("KEEPALIVE")) {
            controlSock->write(QString("KEEPALIVE\n").toUtf8());
        }
    }
}
