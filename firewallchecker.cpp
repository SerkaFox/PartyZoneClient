#include "firewallchecker.h"

FirewallChecker::FirewallChecker(QObject *parent):
    QObject(parent),
    target_list()
{
}

bool FirewallChecker::inProgress() const
{
    return !target_list.isEmpty();
}

void FirewallChecker::checkTargets(const QStringList & targets)
{
    target_list = targets;

    for (const QString & target: target_list) {
        QTcpSocket * sock = new QTcpSocket(this);
        sock->setProperty("target", target);

        connect(sock, &QTcpSocket::readyRead, this, &FirewallChecker::checkAnswer);
        connect(sock, &QTcpSocket::connected, this, &FirewallChecker::checkConnection);

        connect(sock, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(checkError(QAbstractSocket::SocketError)));

        qDebug() << "FirewallChecker: Checking availability of target" << target;

        sock->connectToHost(QHostAddress(target.split(":").first()), target.split(":").last().toInt());
    }
}

void FirewallChecker::checkConnection()
{
    QTcpSocket * sock = qobject_cast<QTcpSocket *> (sender());

    qDebug() << "FirewallChecker: Connection to" << sock->property("target").toString() << "eastablished, waiting for status";

    sock->write(QString("CHECK\n").toUtf8());
    sock->waitForBytesWritten();
}

void FirewallChecker::checkAnswer()
{
    QTcpSocket * sock = qobject_cast<QTcpSocket *> (sender());

    if (sock->canReadLine()) {
        QString answer = QString::fromUtf8(sock->readLine()).trimmed();
        QString target = sock->property("target").toString();

        if (answer == QString("UP")) {
            qDebug() << "FirewallChecker: Target" << target << "reported to be UP";

            emit targetAvailable(target);
        } else {
            qDebug() << "FirewallChecker: Target" << target << "reported to be DOWN";

            emit targetUnavailable(target);
        }

        sock->disconnectFromHost();
        sock->deleteLater();

        target_list.removeAll(target);

        if (target.isEmpty()) {
            emit allTargetsChecked();
        }
    }
}

void FirewallChecker::checkError(QAbstractSocket::SocketError error)
{
    QTcpSocket * sock = qobject_cast<QTcpSocket *> (sender());
    QString target = sock->property("target").toString();

    qDebug() << "FirewallChecker: An error" << error << "occured while connecting to" << target;

    emit targetUnavailable(target);

    sock->deleteLater();

    target_list.removeAll(target);

    if (target.isEmpty()) {
        emit allTargetsChecked();
    }
}
