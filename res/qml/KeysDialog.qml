import QtQuick 2.5
import QtQuick.Window 2.12
import QtQuick.Dialogs 1.2
import QtQuick.Controls 2.12

Window {
    id: keysDialog
    width: 880
    height: 360
    title: "Controls"
    flags: Qt.WindowStaysOnTopHint

    Item {
        anchors.fill: parent
        Image {
            id: keysHintImage
            source: "qrc:/res/img/keys.jpeg"
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 10
            width: 856
            height: 266
        }

        Button {
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            text: "Close"
            width: 120
            x: 750

            onClicked: keysDialog.close();
        }
    }
}
