#ifndef FIREWALLCHECKER_H
#define FIREWALLCHECKER_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>
#include <QDebug>

/**
 * @brief Проверка доступности сервера PartyZone
 */
class FirewallChecker : public QObject
{
    Q_OBJECT
public:
    explicit FirewallChecker(QObject *parent = nullptr);

    bool inProgress() const;

public slots:
    void checkTargets(const QStringList & targets);

signals:
    void targetAvailable(const QString & target);
    void targetUnavailable(const QString & target);
    void allTargetsChecked();

private slots:
    void checkConnection();
    void checkAnswer();
    void checkError(QAbstractSocket::SocketError error);

private:
    QStringList target_list;
};

#endif // FIREWALLCHECKER_H
