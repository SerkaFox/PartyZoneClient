import QtQuick 2.12
import QtQuick.Controls 2.0
import QtQuick.Window 2.12

Window {
    id: mainWindow
    maximumHeight: loginForm.height
    minimumHeight: loginForm.height
    maximumWidth: loginForm.width
    minimumWidth: loginForm.width
    height: loginForm.height
    width: loginForm.width
    visible: true

    title: qsTr("PartyZone Launcher v") + partyZone.versionString

    Item {
        anchors.fill: parent
        LoginForm {
            id: loginForm
            anchors.centerIn: parent
            visible: true
        }

        MainForm {
            id: mainForm
            anchors.centerIn: parent
            visible: false
        }

        Popup {
            id: loginFailed
            anchors.centerIn: parent
            width: 400
            height: 240
            modal: true
            closePolicy: Popup.CloseOnEscape
            background: Rectangle {
                color: "#444444"
            }

            Text {
                anchors.top: parent.top
                anchors.topMargin: 20
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("Invalid username or password")
                color: "#dddddd"
                font.pixelSize: 20
            }

            Button {
                id: loginFailedOkButton
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 20
                width: 120
                height: 40
                anchors.horizontalCenter: parent.horizontalCenter
                hoverEnabled: true

                Text {
                    anchors.centerIn: parent
                    text: "Ok"
                    color: "#dddddd"
                    font.pixelSize: 20
                }

                onClicked: {
                    loginFailed.close();
                }

                background: Rectangle {
                    color: loginFailedOkButton.pressed ? "#666666" : loginFailedOkButton.hovered ? "#555555" : "#444444"
                }
            }

            onOpened: {
                loginFailedOkButton.forceActiveFocus()
            }
        }

        Popup {
            id: startGameFailed
            anchors.centerIn: parent
            width: 600
            height: 240
            modal: true
            closePolicy: Popup.CloseOnEscape
            background: Rectangle {
                color: "#444444"
            }

            property string moonlightPath: "/bin/moonlight"

            Text {
                anchors.top: parent.top
                anchors.topMargin: 20
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("Failed to start Moonlight client at path") + startGameFailed.moonlightPath
                color: "#dddddd"
                font.pixelSize: 16
                wrapMode: Text.WordWrap
            }

            Text {
                anchors.top: parent.top
                anchors.topMargin: 80
                anchors.horizontalCenter: parent.horizontalCenter
                width: 560
                text: qsTr("Please check whether path to the moonlight command is added to your PATH, or specify the full moonlight path manually using --moonlight-client option from command line")
                color: "#dddddd"
                font.pixelSize: 16
                wrapMode: Text.WordWrap
            }

            Button {
                id: startGameFailedOkButton
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 20
                width: 120
                height: 40
                anchors.horizontalCenter: parent.horizontalCenter
                hoverEnabled: true

                Text {
                    anchors.centerIn: parent
                    text: "Ok"
                    color: "#dddddd"
                    font.pixelSize: 20
                }

                onClicked: {
                    startGameFailed.close();
                }

                background: Rectangle {
                    color: startGameFailedOkButton.pressed ? "#666666" : startGameFailedOkButton.hovered ? "#555555" : "#444444"
                }
            }

            onOpened: {
                startGameFailedOkButton.forceActiveFocus()
            }
        }

        Popup {
            id: updateRequired
            anchors.centerIn: parent
            width: 600
            height: 300
            modal: true
            closePolicy: ~Popup.CloseOnEscape
            background: Rectangle {
                color: "#444444"
            }

            property string currentVersion: partyZone.versionString
            property string lastVersion: ""
            property string downloadUrl: ""

            Text {
                id: firstUpdateText
                anchors.top: parent.top
                anchors.topMargin: 40
                anchors.horizontalCenter: parent.horizontalCenter
                width: 560

                text: qsTr("Warning! You are using PartyZone launcher version ") + updateRequired.currentVersion + qsTr(" while version ") + updateRequired.lastVersion + qsTr(" is available. You should upgrade to continue.")
                color: "#dddddd"
                font.pixelSize: 16
                wrapMode: Text.WordWrap
            }

            Text {
                id: secondUpdateText
                anchors.top: firstUpdateText.bottom
                anchors.topMargin: 40
                anchors.horizontalCenter: parent.horizontalCenter
                width: 560

                text: qsTr("You can download the last version at\n") + updateRequired.downloadUrl;
                color: "#dddddd"
                font.pixelSize: 16
                wrapMode: Text.WordWrap
            }

            Button {
                id: upgradeRequiredOkButton
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 20
                width: 120
                height: 40
                anchors.horizontalCenter: parent.horizontalCenter
                hoverEnabled: true

                Text {
                    anchors.centerIn: parent
                    text: "Upgrade"
                    color: "#dddddd"
                    font.pixelSize: 20
                }

                onClicked: {
                    updateRequired.close();
                    partyZone.quitUpdate();
                }

                background: Rectangle {
                    color: upgradeRequiredOkButton.pressed ? "#666666" : upgradeRequiredOkButton.hovered ? "#555555" : "#444444"
                }
            }
        }

        Popup {
            id: controlConnectionFailed
            anchors.centerIn: parent
            width: 560
            height: 240
            modal: true
            closePolicy: Popup.CloseOnEscape
            background: Rectangle {
                color: "#444444"
            }

            Text {
                anchors.top: parent.top
                anchors.topMargin: 20
                anchors.horizontalCenter: parent.horizontalCenter
                text: qsTr("Control connection not eastablished or closed by peer")
                color: "#dddddd"
                font.pixelSize: 20
            }

            Button {
                id: controlConnectionFailedOkButton
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 20
                width: 120
                height: 40
                anchors.horizontalCenter: parent.horizontalCenter
                hoverEnabled: true

                Text {
                    anchors.centerIn: parent
                    text: "Ok"
                    color: "#dddddd"
                    font.pixelSize: 20
                }

                onClicked: {
                    controlConnectionFailed.close();
                }

                background: Rectangle {
                    color: controlConnectionFailedOkButton.pressed ? "#666666" : controlConnectionFailedOkButton.hovered ? "#555555" : "#444444"
                }
            }

            onOpened: {
                controlConnectionFailedOkButton.forceActiveFocus()
            }
        }
    }

    function showKeysHint() {
        keysHint.open();
    }

    Connections {
        target: partyZone

        onLoginSuccessful: {
            loginForm.visible = false;
            mainForm.visible = true;
            mainWindow.maximumWidth = mainForm.width
            mainWindow.maximumHeight = mainForm.height
            mainWindow.width = mainForm.width
            mainWindow.height = mainForm.height
            mainWindow.minimumWidth = mainForm.width
            mainWindow.minimumHeight = mainForm.height
        }

        onLoginFailed: {
            loginFailed.open();
        }

        onControlConnectionAvailable: {
            console.log("MainWindow: enabling channel " + game_no);
            mainForm.setChannelOnline(game_no);
        }

        onControlConnectionUnavailable: {
            console.log("MainWindow: disabling channel " + game_no);
            mainForm.setChannelOffline(game_no);
        }

        onChannelOnline: {
            console.log("MainWindow: setting image for channel " + game_no);
            mainForm.setChannelHasImage(game_no);
        }

        onChannelOffline: {
            console.log("MainWindow: setting image for channel " + game_no);
            mainForm.setChannelHasImage(game_no);
        }

        onMoonlightClientFailed: {
            console.log(moonlight_path)
            startGameFailed.moonlightPath = moonlight_path
            startGameFailed.open();
        }

        onUpdateRequired: {
            updateRequired.lastVersion = last_version
            updateRequired.downloadUrl = download_url
            updateRequired.open();
        }

        onControlConnectionFailed: {
            controlConnectionFailed.open();
        }
    }
}
