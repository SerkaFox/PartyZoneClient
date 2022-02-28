#ifndef PARTYZONE_H
#define PARTYZONE_H

#include <QObject>
#include <QDebug>
#include <QQuickImageProvider>
#include <QDesktopServices>
#include <QProcess>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QRegularExpression>

#include "partyzoneconfig.h"
#include "twitchclient.h"
#include "clientconfig.h"
#include "chatwidget.h"
#include "firewallconnection.h"
#include "firewallchecker.h"

/**
 * @brief Объект движка PartyZone, экспортируемый в интерфейс QML
 */
#if QT_VERSION_MAJOR >= 6
class PartyZone: public QQuickImageProvider
#else
class PartyZone: public QObject, public QQuickImageProvider
#endif
{
    Q_OBJECT

public:
    /* Конструктор */
    PartyZone(QObject * parent = nullptr);

    /* Возвращает номер версии программы */
    QString getVersionString() const;
    /* Возвращает имя файла настроек программы */
    QString getConfigFilename() const;

    Q_PROPERTY(QString versionString READ getVersionString NOTIFY versionChanged)

public slots:
    /* Нажатие кнопки Log in на интерфейсе QML */
    void tryLogin(const QString & username, const QString & password);
    /* Нажатие кнопки Sign up на интерфейсе QML */
    void trySignup();
    /* Нажатие кнопки Donate на интерфейсе QML */
    void tryDonate();
    /* Нажатие кнопки Twitch на интерфейсе QML */
    void tryTwitch();
    /* Нажатие кнопки Discord на интерфейсе QML */
    void tryDiscord();
    /* Нажатие кнопки Youtube на интерфейсе QML */
    void tryYoutube();
    /* Нажатие кнопки Order game на интерфейсе QML */
    void tryOrder();

    /* Нажата кнопка начала игры с заданным номером от 0 до о 5 */
    void startGame(int game_no);
    /* Нажата кнопка остановки игры с заданным номером от 0 до о 5 */
    void stopGame(int game_no);
    /* Открыть Twitch - канал игры в браузере */
    void browseGameChannel(int game_no);

    /* Проверяет доступность канала с указанным номером */
    void checkChannelOnline(int game_no);
    /* Запрашивает изображение для канала */
    void requestChannelImage(int game_no, int width=200, int height=300);

    /* Задать путь к клиенту Moonlight */
    void setMoonligthClient(const QString & path);

    /* Выход из программы */
    void quit();
    /* Выход из программы с одновременным запуском скачивания новой версии */
    void quitUpdate();
    /* Принудительно показать окно чата */
    void showChat();
    /* Принудительно скрыть окно чата */
    void hideChat();
    /* Переключить видимость окна чата */
    void toggleChat();
    /* Запустить эмулятор джойстика */
    void startJoystickEmulator();
    /* Остановить эмулятор джойстика */
    void stopJoystickEmulator();
    void pauseJoystickEmulator();
    void resumeJoystickEmulator();
    /* Выполняется при запуске программы, может выполнить некоторые команды */
    void runStartingScripts();
    /* Выполняется при завершении программы, может выполнить некоторые команды */
    void runEndingScripts();
    /* Проверить доступность серверов Partyzone firewall */
    void runFirewallChecks();

signals:
    /* Введенное имя и пароль корректны */
    void loginSuccessful(const QString & username, const QString & password);
    /* Введенное имя и пароль некорректны */
    void loginFailed(const QString & username, const QString & password);
    /* Канал с указанным номером подключен */
    void channelOnline(int game_no, const QString & channel);
    /* Канал с указанным номером отключен */
    void channelOffline(int game_no, const QString & channel);
    /* Получена картинка для данного канала */
    void channelImageGot(int game_no, const QString & channel, const QPixmap & pixmap);
    /* Клиент Moonlight запущен */
    void moonlightClientStarted(const QString & moonlight_path, const QStringList & moonlight_args);
    /* Не удалось запустить клиент Moonlight */
    void moonlightClientFailed(const QString & moonlight_path, const QStringList & moonlight_args);
    /* Клиент Moonlight остановлен */
    void moonlightClientStopped();
    /* Никогда не посылается, нужен для спецификации QML */
    void versionChanged(const QString & version);
    /* Получена информация о наличии важного обновления */
    void updateRequired(const QString & last_version, const QString & download_url);
    /* Не удалось установить соединение с Partyzone Firewall */
    void controlConnectionFailed();
    /* На соответствующем игровом канале запущен PartyZone firewall */
    void controlConnectionAvailable(int game_no);
    /* На соответствующем игровом канале PartyZone firewall не работает */
    void controlConnectionUnavailable(int game_no);

private slots:
    /* Сообщить интерфейсному модулю, что данный канал работает */
    void reportChannelOnline(const QString & channel);
    /* Сообщить интерфейсному модулю, что данный канал не работает */
    void reportChannelOffline(const QString & channel);
    /* Передать интерфейсному модулю изображение канала */
    void reportImage(const QString & channel, const QPixmap & pixmap);

    /* Ответ на запрос об авторизации пользователя от сервера PartyZOne */
    void handleWPReply(QNetworkReply * reply);

    /* Реакция на запрос изображения канала от интерфейсного модуля */
    virtual QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize);

    /* Авторизация пользователя через сервер PartyZone */
    void tryLoginViaWordpress(const QString & username, const QString & password);

    /* По завершении процесса Мунлайт освободить ресурсы и отключиться от файрвола */
    void clearGameProcess(int code, QProcess::ExitStatus status);

    /* Отключить игровой процесс при отказе управляющего соединения */
    void terminateGameProcess();

    void reportFirewallAvailable(const QString & target);
    void reportFirewallUnavailable(const QString & target);

private:
    /* Подключение к сети */
    QNetworkAccessManager * networkManager;

    /* Информация о настройках программы */
    PartyZoneConfig pzConfig;

    /* Клиент для запроса данных с Twitch */
    TwitchClient * twitchClient;

    /* Текущие изображения для разных каналов */
    QMap<int, QPixmap> currentPixmaps;

    /* Запоминает, какие из контрольных соединений были доступны */
    QMap<int, bool> availableControls;

    /* Запоминает, какие из каналов сейчас подключены */
    QMap<int, bool> availableChannels;

    /* Объект с сохраненными ранее логином и паролем */
    ClientConfig clientConfig;

    /* Запущенный процесс Moonlight */
    QProcess * gameProcess;
    /* Запущенный процесс эмулятора джойстика */
    QProcess * joystickProcess;

    /* Номер канала, для которого запущен процесс Moonlight */
    int gameProcessNumber;

    /* Окно для игрового чата */
    ChatWidget * chatWidget;

    /* Управляющее соединение с Partyzone Firewall */
    FirewallConnection * firewallConnection;

    /* Имя пользователя, под которым мы авторизовались */
    QString userName;
};

#endif // PARTYZONE_H
