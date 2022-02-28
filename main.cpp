#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QDebug>
#include <QIcon>
#include <QSharedMemory>
#include <QMessageBox>

#include "controlwidget.h"
#include "partyzone.h"
#include "version.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QApplication app(argc, argv);
    QCommandLineParser parser;

    QSharedMemory * shmem = new QSharedMemory("shm-partyzone-launcher", &app);

    shmem->attach() || shmem->create(256);

    shmem->lock();
    if (((char *) shmem->data())[0] > 0) {
        QMessageBox::critical(
                    nullptr,
                    app.tr("Could not start Partyzone Launcher"),
                    app.tr("Only one Partyzone Launcher process may run at same time"));

        shmem->unlock();
        shmem->detach();
        return 1;
    }
    ((char *) shmem->data())[0] = 1;
    shmem->unlock();

    app.setWindowIcon(QIcon("qrc:/res/img/party.ico"));

    ControlWidget cw;

    cw.show();

    qDebug() << app.tr("PartyZone launcher v%1").arg(Version::versionString());
    qDebug() << "Running with Qt version" << QT_VERSION_STR;

    parser.addOption(
                QCommandLineOption(
                    "moonlight-client",
                    "Path to Moonlight-qt client program",
                    "PATH",
                    "PartyZone")
                );
    parser.addHelpOption();
    parser.process(app);

    PartyZone * pz = new PartyZone();

    if (parser.isSet("moonlight-client")) {
        pz->setMoonligthClient(parser.value("moonlight-client"));
    }

    pz->connect(&app, &QApplication::aboutToQuit, pz, &PartyZone::pauseJoystickEmulator);
    pz->connect(&app, &QApplication::aboutToQuit, pz, &PartyZone::runEndingScripts);

    pz->resumeJoystickEmulator();
    pz->runStartingScripts();

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty(QString("partyZone"), pz);
    engine.addImageProvider("partyZone", pz);
    const QUrl url(QStringLiteral("qrc:/res/qml/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &cw, &ControlWidget::reportObjectCreation,
                     Qt::QueuedConnection);
    engine.load(url);

    int ret = app.exec();

    shmem->lock();
    ((char *) shmem->data())[0] = 0;
    shmem->unlock();
    shmem->detach();

    return ret;
}
