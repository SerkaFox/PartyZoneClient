#include "clientconfig.h"
#include "version.h"

ClientConfig::ClientConfig():
    savedUsername(),
    savedPassword()
{
    savedJoystickState = false;
}

QString ClientConfig::getSavedUsername() const
{
    return savedUsername;
}

void ClientConfig::saveUsername(const QString &value)
{
    savedUsername = value;
}

QString ClientConfig::getSavedPassword() const
{
    return savedPassword;
}

void ClientConfig::savePassword(const QString &value)
{
    savedPassword = value;
}

bool ClientConfig::getSavedJoystickState() const
{
    return savedJoystickState;
}

void ClientConfig::saveJoystickState(bool value)
{
    savedJoystickState = value;
}

bool ClientConfig::save(const QString &filename) const
{
    QJsonObject obj;

    obj["launcherVersion"] = Version::versionString();
    obj["savedUsername"] = savedUsername;
    obj["savedPassword"] = savedPassword;
    obj["savedJoystickState"] = savedJoystickState;

    QJsonDocument doc(obj);

    QFile file(filename);

    if (file.open(QIODevice::WriteOnly)) {
        file.write(doc.toJson());
        file.close();

        return true;
    } else {
        return false;
    }
}

bool ClientConfig::load(const QString &filename)
{
    QFile file(filename);

    if (file.open(QIODevice::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        file.close();

        QJsonObject obj = doc.object();

        if (obj.contains("savedUsername")) {
            savedUsername = obj.value("savedUsername").toString();
        }

        if (obj.contains("savedPassword")) {
            savedPassword = obj.value("savedPassword").toString();
        }

        if (obj.contains("savedJoystickState")) {
            savedJoystickState = obj.value("savedJoystickState").toBool();
        }

        return true;
    } else {
        return false;
    }
}
