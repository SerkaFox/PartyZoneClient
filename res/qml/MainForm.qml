import QtQuick 2.0
import QtQuick.Controls 2.2

Rectangle {
    width: 1280
    height: 720

    Item {
        id: keysInput;

        Keys.onPressed: {
            if (event.key === Qt.Key_C && (event.modifiers & Qt.AltModifier)) {
                partyZone.toggleChat();
            }
            if (event.key === Qt.Key_1 && (event.modifiers & Qt.AltModifier)) {
                var comp = Qt.createComponent("qrc:/res/qml/KeysDialog.qml");
                var obj = comp.createObject(null, {});
                obj.visible = true;
            }
            if (event.key === Qt.Key_3 && (event.modifiers & Qt.AltModifier)) {
                partyZone.startJoystickEmulator();
            }
            if (event.key === Qt.Key_4 && (event.modifiers & Qt.AltModifier)) {
                partyZone.stopJoystickEmulator();
            }
        }
    }

    Image {
        anchors.fill: parent
        source: "qrc:/res/img/MainMenu/background.jpg"
    }

    PictureButton {
        pressedImage: "qrc:/res/img/MainMenu/order_button_pressed.png"
        enabledImage: "qrc:/res/img/MainMenu/order_button.png"
        id: orderButton
        x: 67
        y: 63
        width: 162
        height: 117

        onClicked: {
            partyZone.tryOrder();
        }
    }

    GameWindow {
        id: gameWindowOne
        gameNo: 0
        x: 69
        y: 280
    }

    GameWindow {
        id: gameWindowTwo
        gameNo: 1
        x: 264
        y: 280
    }

    GameWindow {
        id: gameWindowThree
        gameNo: 2
        x: 459
        y: 280
    }

    GameWindow {
        id: gameWindowFour
        gameNo: 3
        x: 654
        y: 280
    }

    GameWindow {
        id: gameWindowFive
        gameNo: 4
        x: 849
        y: 280
    }

    GameWindow {
        id: gameWindowSix
        gameNo: 5
        x: 1044
        y: 280
    }

    GameButton {
        id: gameButtonOne
        x: 69
        y: 522
        width: 152
        height: 150
        enabled: gameWindowOne.online
        gameNo: 0
    }

    GameButton {
        id: gameButtonTwo
        x: 264
        y: 522
        width: 152
        height: 150
        enabled: gameWindowTwo.online
        gameNo: 1
    }

    GameButton {
        id: gameButtonThree
        x: 459
        y: 522
        width: 152
        height: 150
        enabled: gameWindowThree.online
        gameNo: 2
    }

    GameButton {
        id: gameButtonFour
        x: 654
        y: 522
        width: 152
        height: 150
        enabled: gameWindowFour.online
        gameNo: 3
    }

    GameButton {
        id: gameButtonFive
        x: 849
        y: 522
        width: 152
        height: 150
        enabled: gameWindowFive.online
        gameNo: 4
    }

    GameButton {
        id: gameButtonSix
        x: 1044
        y: 522
        width: 152
        height: 150
        enabled: gameWindowSix.online
        gameNo: 5
    }

    Timer {
        id: refreshTimer
        interval: 60000
        running: false
        repeat: true

        onTriggered: {
            partyZone.runFirewallChecks();
            for (var i = 0; i < 6; i++) {
                partyZone.checkChannelOnline(i);
            }
        }
    }

    function setChannelOnline(game_no) {
        if (game_no === 0) {
            gameWindowOne.online = true;
        } else if (game_no === 1) {
            gameWindowTwo.online = true;
        } else if (game_no === 2) {
            gameWindowThree.online = true;
        } else if (game_no === 3) {
            gameWindowFour.online = true;
        } else if (game_no === 4) {
            gameWindowFive.online = true;
        } else if (game_no === 5) {
            gameWindowSix.online = true;
        }
    }

    function setChannelHasImage(game_no) {
        partyZone.requestChannelImage(game_no, Constants.channelImageWidth, Constants.channelImageHeight);
    }

    function setChannelOffline(game_no) {
        if (game_no === 0) {
            gameWindowOne.online = false;
        } else if (game_no === 1) {
            gameWindowTwo.online = false;
        } else if (game_no === 2) {
            gameWindowThree.online = false;
        } else if (game_no === 3) {
            gameWindowFour.online = false;
        } else if (game_no === 4) {
            gameWindowFive.online = false;
        } else if (game_no === 5) {
            gameWindowSix.online = false;
        }
    }

    onVisibleChanged: {
        if (visible) {
            partyZone.runFirewallChecks();
            for (var i = 0; i < 6; i++) {
                partyZone.checkChannelOnline(i);
            }
            refreshTimer.start();
            keysInput.focus = true;
        } else {
            refreshTimer.stop();
        }
    }

    function allowKeysInput() {
        keysInput.focus = true;
    }
}
