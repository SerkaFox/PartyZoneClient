import QtQuick 2.7
import QtQuick.Controls 2.2


Rectangle {
    width: 800
    height: 600

    Image {
        anchors.fill: parent
        source: "qrc:/res/img/StartMenu/login_background.jpg"
    }

    TextField {
        id: usernameField
        text: ""
        x: 316
        y: 232
        width: 338
        height: 56
        font.pixelSize: 32
        focus: true

        onAccepted: {
            passwordField.forceActiveFocus();
        }
    }

    TextField {
        id: passwordField
        text: ""
        x: 316
        y: 330
        width: 338
        height: 56
        font.pixelSize: 32
        echoMode: TextInput.Password

        onAccepted: {
            loginButton.clicked();
        }
    }

    PictureButton {
        pressedImage: "qrc:/res/img/StartMenu/login_button_pressed.png"
        enabledImage: "qrc:/res/img/StartMenu/login_button.png"
        id: loginButton
        x: 438
        y: 414
        width: 102
        height: 41

        onClicked: {
            partyZone.tryLogin(usernameField.text, passwordField.text);
        }
    }

    PictureButton {
        pressedImage: "qrc:/res/img/StartMenu/signup_button_pressed.png"
        enabledImage: "qrc:/res/img/StartMenu/signup_button.png"
        id: signupButton
        x: 552
        y: 414
        width: 102
        height: 41

        onClicked: {
            partyZone.trySignup();
        }
    }

    PictureButton {
        pressedImage: "qrc:/res/img/StartMenu/donate_button_pressed.png"
        enabledImage: "qrc:/res/img/StartMenu/donate_button.png"
        id: donateButton
        x: 61
        y: 57
        width: 110
        height: 92

        onClicked: {
            partyZone.tryDonate();
        }
    }

    PictureButton {
        pressedImage: "qrc:/res/img/StartMenu/twitch_button_pressed.png"
        enabledImage: "qrc:/res/img/StartMenu/twitch_button.png"
        id: twitchButton
        x: 58
        y: 496
        width: 54
        height: 54

        onClicked: {
            partyZone.tryTwitch();
        }

        ToolTip.visible: hovered
        ToolTip.delay: 500
        ToolTip.text: qsTr("Twitch")
    }

    PictureButton {
        pressedImage: "qrc:/res/img/StartMenu/discord_button_pressed.png"
        enabledImage: "qrc:/res/img/StartMenu/discord_button.png"
        id: discordButton
        x: 148
        y: 496
        width: 54
        height: 54

        onClicked: {
            partyZone.tryDiscord();
        }

        ToolTip.visible: hovered
        ToolTip.delay: 500
        ToolTip.text: qsTr("Discord")
    }

    PictureButton {
        pressedImage: "qrc:/res/img/StartMenu/youtube_button_pressed.png"
        enabledImage: "qrc:/res/img/StartMenu/youtube_button.png"
        id: youtubeButton
        x: 238
        y: 496
        width: 54
        height: 54

        onClicked: {
            partyZone.tryYoutube();
        }

        ToolTip.visible: hovered
        ToolTip.delay: 500
        ToolTip.text: qsTr("Youtube")
    }
}
