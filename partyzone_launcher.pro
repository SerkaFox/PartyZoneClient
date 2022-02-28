QT += quick network widgets
DEFINES += WITHOUT_CHAT=1 USE_UCR=1 USE_VIRTUALCONTROLLER=1

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        channelconfig.cpp \
        chatwidget.cpp \
        clientconfig.cpp \
        controlwidget.cpp \
        firewallchecker.cpp \
        firewallconnection.cpp \
        imagerequestcache.cpp \
        imagerequestcacheentry.cpp \
        main.cpp \
        partyzone.cpp \
        partyzoneconfig.cpp \
        twitchclient.cpp \
        twitchinfo.cpp \
        version.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    channelconfig.h \
    chatwidget.h \
    clientconfig.h \
    controlwidget.h \
    firewallchecker.h \
    firewallconnection.h \
    imagerequestcache.h \
    imagerequestcacheentry.h \
    partyzone.h \
    partyzoneconfig.h \
    twitchclient.h \
    twitchinfo.h \
    version.h

RC_ICONS = res/img/party.ico

DISTFILES += \
    deploy-windows-mingw.bat \
    readme.txt
