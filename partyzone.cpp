#include "partyzone.h"
#include "version.h"
#include "chatwidget.h"

/**
 * @brief Инициализирует движок PartyZone launcher
 * @param parent Родительский объект Qt - например, основное окно
 */
#if QT_VERSION_MAJOR >= 6
PartyZone::PartyZone(QObject * parent):
    QQuickImageProvider(QQuickImageProvider::Pixmap),
    pzConfig(),
    currentPixmaps(),
    availableControls(),
    availableChannels(),
    clientConfig(),
    userName()
{
    QDir::home().mkpath(".partyzone");

    setParent(parent);

    networkManager = new QNetworkAccessManager(this);

    twitchClient = new TwitchClient(pzConfig.getClientId(), pzConfig.getSecretKey(), this);

    connect(networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(handleWPReply(QNetworkReply*)));

    connect(twitchClient, SIGNAL(channelOnline(const QString &)), this, SLOT(reportChannelOnline(const QString &)));
    connect(twitchClient, SIGNAL(channelOffline(const QString &)), this, SLOT(reportChannelOffline(const QString &)));
    connect(twitchClient, SIGNAL(channelImageGot(const QString &, const QPixmap &)), this, SLOT(reportImage(const QString &, const QPixmap &)));

    clientConfig.load(getConfigFilename());

    if (!clientConfig.getSavedUsername().isEmpty() && !clientConfig.getSavedPassword().isEmpty()) {
        tryLogin(clientConfig.getSavedUsername(), clientConfig.getSavedPassword());
    }

    gameProcess = nullptr;
    joystickProcess = nullptr;
    gameProcessNumber = -1;

#if WITHOUT_CHAT
    chatWidget = nullptr;
#else
    chatWidget = new ChatWidget();
#endif

    firewallConnection = new FirewallConnection(this);
}
#else
PartyZone::PartyZone(QObject * parent):
    QObject(parent),
    QQuickImageProvider(QQuickImageProvider::Pixmap),
    pzConfig(),
    currentPixmaps(),
    availableControls(),
    availableChannels(),
    clientConfig(),
    userName()
{
    QDir::home().mkpath(".partyzone");

    networkManager = new QNetworkAccessManager(this);

    twitchClient = new TwitchClient(pzConfig.getClientId(), pzConfig.getSecretKey(), this);

    connect(networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(handleWPReply(QNetworkReply*)));

    connect(twitchClient, SIGNAL(channelOnline(const QString &)), this, SLOT(reportChannelOnline(const QString &)));
    connect(twitchClient, SIGNAL(channelOffline(const QString &)), this, SLOT(reportChannelOffline(const QString &)));
    connect(twitchClient, SIGNAL(channelImageGot(const QString &, const QPixmap &)), this, SLOT(reportImage(const QString &, const QPixmap &)));

    clientConfig.load(getConfigFilename());

    if (!clientConfig.getSavedUsername().isEmpty() && !clientConfig.getSavedPassword().isEmpty()) {
        tryLogin(clientConfig.getSavedUsername(), clientConfig.getSavedPassword());
    }

    gameProcess = nullptr;
    joystickProcess = nullptr;
    gameProcessNumber = -1;

#if WITHOUT_CHAT
    chatWidget = nullptr;
#else
    chatWidget = new ChatWidget();
#endif

    firewallConnection = new FirewallConnection(this);

    connect(firewallConnection, &FirewallConnection::connectionRefused, this, &PartyZone::terminateGameProcess);
    connect(firewallConnection, &FirewallConnection::connectionTerminated, this, &PartyZone::terminateGameProcess);
}
#endif

QString PartyZone::getVersionString() const
{
    return Version::versionString();
}

QString PartyZone::getConfigFilename() const
{
    return QDir::homePath() +
            QDir::separator() +
            QString(".partyzone") +
            QDir::separator() +
            QString("pz-launcher-config.json");
}

/**
 * @brief Вход в систему с указанным именем пользователя и паролем
 * @param username Имя пользователя
 * @param password Пароль
 *
 * Об успеншости входа в систему нужно судить по сигналам loginSuccessful
 * и loginFailed. Эта функция не возвращает никакого значения
 */
void PartyZone::tryLogin(const QString & username, const QString & password)
{
    qDebug() << "Trying to login as" << username;

    if (username.isEmpty() || password.isEmpty()) {
        emit loginFailed(username, password);
        return;
    }

    userName = username;

    tryLoginViaWordpress(username, password);
}

/**
 * @brief Создание новой учетной записи с указанным именем пользователя и паролем
 * @param username Имя пользователя
 * @param password Пароль
 */
void PartyZone::trySignup()
{
    qDebug() << "Trying to sign up";
    QDesktopServices::openUrl(QUrl("https://partyzone.su/profile-2/register"));
}

/**
 * @brief Переход по ссылке Donate
 */
void PartyZone::tryDonate()
{
    qDebug() << "Donate button pressed";
    QDesktopServices::openUrl(QUrl("https://donate.stream/partyzone_live"));
}

/**
 * @brief Переход на страницу в Twitch
 */
void PartyZone::tryTwitch()
{
    qDebug() << "Twitch button pressed";

    QDesktopServices::openUrl(QUrl("https://partyzone.su/twitch/"));
}

/**
 * @brief Переход на страницу в Discord
 */
void PartyZone::tryDiscord()
{
    qDebug() << "Discord button pressed";

    QDesktopServices::openUrl(QUrl("https://discord.com/invite/Ze2c46n"));
}

/**
 * @brief Переход на страницу в YouTube
 */
void PartyZone::tryYoutube()
{
    qDebug() << "Youtube button pressed";

    QDesktopServices::openUrl(QUrl("https://www.youtube.com/c/partyzone20"));
}

/**
 * @brief Переход по ссылке Order Game
 */
void PartyZone::tryOrder()
{
    qDebug() << "Order game button pressed";
}

/**
 * @brief Переход по ссылке на определенный игровой канал
 * @param game_no Номер игрового канала
 */
void PartyZone::startGame(int game_no)
{
    if (availableControls.value(game_no, false) && !gameProcess) {
        qDebug() << "Start game button" << game_no << "pressed, starting Moonlight";

        firewallConnection->startConnection(
                    pzConfig.getFirewallAddress(game_no), userName, QString(), pzConfig.getChannelName(game_no));

        gameProcess = new QProcess(this);
        gameProcess->setProgram(pzConfig.getCommandProgram(game_no));
        gameProcess->setArguments(pzConfig.getCommandArguments(game_no));

        connect(gameProcess, SIGNAL(finished(int, QProcess::ExitStatus)), this, SLOT(clearGameProcess(int, QProcess::ExitStatus)));

        gameProcess->start();

        if (!gameProcess->waitForStarted()) {
            qCritical() << "Failed to start moonlight client" << gameProcess->error();

            emit moonlightClientFailed(gameProcess->program(), gameProcess->arguments());
        } else {
            qDebug() << "Successfully started Moonlight client";

            gameProcessNumber = game_no;

            emit moonlightClientStarted(gameProcess->program(), gameProcess->arguments());

            browseGameChannel(game_no);
        }
    } else {
        qDebug() << "Start game button" << game_no << "pressed, but is disabled";
    }
}

void PartyZone::stopGame(int game_no)
{
    if (gameProcess && gameProcessNumber == game_no) {
        firewallConnection->breakConnection();

        qDebug() << "Stop game button" << game_no << "pressed";

        gameProcess->terminate();

        if (!gameProcess->waitForFinished()) {
            gameProcess->kill();
        }
    } else {
        qDebug() << "Stop game button" << game_no << "pressed, but is disabled";
    }
}

void PartyZone::browseGameChannel(int game_no)
{
    QString channel = pzConfig.getChannelName(game_no);

    QUrl url(QString("https://www.twitch.tv/popout/%1/chat?popout=").arg(channel));

    qDebug() << "Opening url" << url.toString();

    if (chatWidget) {
        chatWidget->openUrl(url);
        chatWidget->showDelayed();
    } else {
        QDesktopServices::openUrl(url);
    }
}

/**
 * @brief Проверить, работает ли сейчас игровой канал
 * @param game_no Номер игрового канала
 */
void PartyZone::checkChannelOnline(int game_no)
{
    QString channel = pzConfig.getChannelName(game_no);

    twitchClient->checkChannelOnline(channel);
}

/**
 * @brief Запросить номер указанного игрового канала
 * @param game_no Номер игрового канала от 0 до 5.
 * @param width Предпочтительная ширина запрашиваемого изображения
 * @param height Предпочтительная высота запрашиваемого изображения
 */
void PartyZone::requestChannelImage(int game_no, int width, int height)
{
    QString channel = pzConfig.getChannelName(game_no);

    twitchClient->requestChannelImage(channel, width, height);
}

/**
 * @brief Указать путь к клиентской программе Moonlight-Qt.
 * @param path Путь к клиентской программе Moonlight-Qt
 */
void PartyZone::setMoonligthClient(const QString & path)
{
    pzConfig.setMoonlightClient(path);

    qDebug() << "Moonlight client path is set to" << path;
}

/**
 * @brief Выход из программы
 */
void PartyZone::quit()
{
    clientConfig.save(getConfigFilename());
    qApp->quit();
}

/**
 * @brief Выход из программы с одновременным запуском скачивания новой версии
 */
void PartyZone::quitUpdate()
{
    if (!pzConfig.upgradeUrl().isEmpty()) {
        QDesktopServices::openUrl(pzConfig.upgradeUrl());
    }
    quit();
}

void PartyZone::showChat()
{
    if (chatWidget) {
        chatWidget->show();
    }
}

void PartyZone::hideChat()
{
    if (chatWidget) {
        chatWidget->hide();
    }
}

/**
 * @brief Показать или скрыть чат
 *
 * Вызывается из интерфейса по нажатию Alt+C. При использовании встроенного
 * окна чата показывает, либо скрывает его. Если же встроенного окна чата нет,
 * то открывает чат в браузере
 */
void PartyZone::toggleChat()
{
    if (chatWidget) {
        if (chatWidget->isVisible()) {
            chatWidget->hide();
        } else {
            chatWidget->show();
        }
    } else if (gameProcessNumber >= 0) {
        QString channel = pzConfig.getChannelName(gameProcessNumber);

        QUrl url(QString("https://www.twitch.tv/popout/%1/chat?popout=").arg(channel));

        QDesktopServices::openUrl(url);
    }
}

void PartyZone::startJoystickEmulator()
{
#if USE_VIRTUALCONTROLLER
    if (!joystickProcess) {
        joystickProcess = new QProcess(this);
        joystickProcess->setProgram(QString("VirtualController.exe"));
        joystickProcess->setArguments(
                    QStringList() << "/run" << "/minimize");

        joystickProcess->start();
        joystickProcess->waitForStarted();
    }
#endif
}

void PartyZone::stopJoystickEmulator()
{
    if (joystickProcess) {
        joystickProcess->terminate();

        if (!joystickProcess->waitForFinished(1500)) {
            joystickProcess->kill();
        }

        joystickProcess->deleteLater();
        joystickProcess = nullptr;
    }
}

void PartyZone::pauseJoystickEmulator()
{
    if (joystickProcess) {
        joystickProcess->terminate();

        if (!joystickProcess->waitForFinished(1500)) {
            joystickProcess->kill();
        }

        joystickProcess->deleteLater();
        joystickProcess = nullptr;

        clientConfig.saveJoystickState(true);
    } else {
        clientConfig.saveJoystickState(false);
    }

    clientConfig.save(getConfigFilename());
}

void PartyZone::resumeJoystickEmulator()
{
    if (clientConfig.getSavedJoystickState()) {
        startJoystickEmulator();
    }
}

void PartyZone::runStartingScripts()
{
#if USE_UCR
    QProcess::startDetached("UCRgo.exe");
#endif
}

void PartyZone::runEndingScripts()
{
#if USE_UCR
    QProcess::startDetached("UCRoff.exe");
#endif
}

void PartyZone::runFirewallChecks()
{
    FirewallChecker * checker = new FirewallChecker(this);

    QStringList targets;

    for (int i = 0; i < 6; i++) {
        targets.append(pzConfig.getFirewallAddress(i));
    }

    connect(checker, &FirewallChecker::allTargetsChecked, checker, &QObject::deleteLater);
    connect(checker, &FirewallChecker::targetAvailable, this, &PartyZone::reportFirewallAvailable);
    connect(checker, &FirewallChecker::targetUnavailable, this, &PartyZone::reportFirewallUnavailable);

    checker->checkTargets(targets);
}

/**
 * @brief Уведомить все компоненты программы, что данный канал сейчас подключен
 * @param channel Название канала
 */
void PartyZone::reportChannelOnline(const QString & channel)
{
    int game_no = pzConfig.getChannelNumber(channel);

    availableChannels[game_no] = true;

    emit channelOnline(game_no, channel);
}

/**
 * @brief Уведомить все компоненты программы, что данный канал сейчас отключен
 * @param channel Название канала
 */
void PartyZone::reportChannelOffline(const QString & channel)
{
    int game_no = pzConfig.getChannelNumber(channel);

    availableChannels[game_no] = false;

    emit channelOffline(game_no, channel);
}

void PartyZone::reportImage(const QString & channel, const QPixmap & pixmap)
{
    int game_no = pzConfig.getChannelNumber(channel);

    currentPixmaps[game_no] = pixmap;

    emit channelImageGot(game_no, channel, pixmap);
}

QPixmap PartyZone::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    int game_no = id.split("/").first().toInt();

    if (currentPixmaps.contains(game_no)) {
        QPixmap pixmap = currentPixmaps.value(game_no);

        if (size) {
            *size = pixmap.size();
        }

        return pixmap;
    } else {
        /* Если изображения нет, то возвращаем полностью прозрачное изображение нужного размера */
        QPixmap pixmap(150, 200);
        pixmap.fill(qRgba(0, 0, 0, 0));
        return pixmap;
    }
}

/**
 * @brief Обработка ответа Wordpress API на запрос авторизации пользователя
 * @param reply Ответ от Wordpress API
 *
 * В случае успешного входа в систему обновляет конфигурацию клиента в соответствии
 * с данными, полученными с сервера
 */
void PartyZone::handleWPReply(QNetworkReply * reply)
{
    QString auth = QString::fromUtf8(reply->request().rawHeader("Authorization")).mid(6).trimmed();

    auth = QString::fromUtf8(QByteArray::fromBase64(auth.toUtf8()));

    QString username = auth.split(":").first();
    QString password = auth.split(":").last();

    if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 200) {
        qDebug() << "Login successful";

        QString content = QString::fromUtf8(reply->readAll());
        QRegularExpression rex("<p>Client-config:([^<]+)<");
        QRegularExpressionMatch match = rex.match(content);

        if (match.hasMatch()) {
            QString found = match.capturedTexts().last();

            qDebug() << "Configuration got from Wordpress";

            pzConfig.updateBase64(found);

            if (pzConfig.upgradeAvailable()) {
                emit updateRequired(pzConfig.upgradeVersion(), pzConfig.upgradeUrl());
            }
        } else {
            qWarning() << "No valid configuration got from Wordpress";
        }

        twitchClient->setAuthParameters(pzConfig.getClientId(), pzConfig.getSecretKey());

        clientConfig.saveUsername(username);
        clientConfig.savePassword(password);
        clientConfig.save(getConfigFilename());

        emit loginSuccessful(username, password);
    } else {
        qWarning() << "Login failed, Wordpress status code was"
                   << reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

        clientConfig.saveUsername(QString());
        clientConfig.savePassword(QString());
        clientConfig.save(getConfigFilename());

        emit loginFailed(username, password);
    }
}

/**
 * @brief Проверка имени пользователя и пароля через Wordpress Authentication API
 * @param username Имя пользователя
 * @param password Пароль пользователя
 */
void PartyZone::tryLoginViaWordpress(const QString & username, const QString & password)
{
    QString auth = QString("%1:%2").arg(username, password);
    auth = QString::fromUtf8(auth.toUtf8().toBase64()).trimmed();

    QNetworkRequest request(QUrl("https://partyzone.su/wp-json/wp/v2/pages/9240"));

    request.setRawHeader("Authorization", QString("Basic %1").arg(auth).toUtf8());

    networkManager->get(request);
}

void PartyZone::clearGameProcess(int code, QProcess::ExitStatus status)
{
    if (gameProcess) {
        gameProcess->deleteLater();
        gameProcess = nullptr;

        qDebug() << "Moonlight client at channel"
                 << gameProcessNumber
                 << "finished with status"
                 << status
                 << "and code"
                 << code;

        gameProcessNumber = -1;

        firewallConnection->breakConnection();
    }

    hideChat();
}

void PartyZone::terminateGameProcess()
{
    qDebug() << "Moonlight client at channel" << gameProcessNumber << "terminated due to broken control connection";

    if (gameProcess) {
        QProcess * gp = gameProcess;

        gameProcess = nullptr;
        gameProcessNumber = -1;

        gp->terminate();
        gp->waitForFinished();
        gp->kill();

        gp->deleteLater();
        gp = nullptr;
    }

    gameProcessNumber = -1;

    hideChat();

    emit controlConnectionFailed();
}

/**
 * @brief Отметить контрольный канал связи с Partyzone Firewall как доступный
 * @param target IP адрес и порт Partyzone Firewall.
 *
 * Убедившись, что управляющий канал связи доступен, можно начинать запрос
 * доступности канала в Twitch.
 */
void PartyZone::reportFirewallAvailable(const QString & target)
{
    for (int i = 0; i < 6; i++) {
        if (pzConfig.getFirewallAddress(i) == target) {
            emit controlConnectionAvailable(i);

            availableControls[i] = true;

            checkChannelOnline(i);
        }
    }
}

void PartyZone::reportFirewallUnavailable(const QString & target)
{
    for (int i = 0; i < 6; i++) {
        if (pzConfig.getFirewallAddress(i) == target) {
            emit controlConnectionUnavailable(i);

            availableControls[i] = false;
        }
    }
}
