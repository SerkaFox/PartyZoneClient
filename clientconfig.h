#ifndef CLIENTCONFIG_H
#define CLIENTCONFIG_H

#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>

/**
 * @brief Настройки клиента Partyzone Launcher
 *
 * Содержит два строковых поля с сохраненными именами пользователя и
 * паролем, введенными пользователем ранее. При последующих запусках
 * автоматически пробуем логиниться с ними.
 */
class ClientConfig
{
public:
    ClientConfig();

    QString getSavedUsername() const;
    void saveUsername(const QString &value);

    QString getSavedPassword() const;
    void savePassword(const QString &value);

    bool getSavedJoystickState() const;
    void saveJoystickState(bool value);

    bool save(const QString & filename) const;
    bool load(const QString & filename);

private:
    QString savedUsername;
    QString savedPassword;

    bool savedJoystickState;
};

#endif // CLIENTCONFIG_H
