import QtQuick 2.0

PictureButton {
    property int gameNo: 0

    enabledImage: "qrc:/res/img/MainMenu/cloud_button.png"
    disabledImage: "qrc:/res/img/MainMenu/cloud_button_disabled.png"
    pressedImage: "qrc:/res/img/MainMenu/cloud_button_pressed.png"

    onClicked: {
        partyZone.startGame(gameNo);
    }
}
