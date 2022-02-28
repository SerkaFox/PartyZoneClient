import QtQuick 2.0

Item {
    width: 152
    height: 204
    clip: false

    property int gameNo: 0
    property bool online: false

    Image {
        anchors.centerIn: parent
        source: parent.online ? "qrc:/res/img/MainMenu/window_online.png" : "qrc:/res/img/MainMenu/window_offline.png"
    }

    Image {
        id: channelImage
        anchors.centerIn: parent
        width: Constants.channelImageWidth
        height: Constants.channelImageHeight
        cache: false
        source: "image://partyzone/" + gameNo + "/0"
    }

    Connections
    {
        target: partyZone
        property int frameCounter: 0

        onChannelImageGot:
        {
            frameCounter ^= 1;
            channelImage.source = "image://partyzone/" + gameNo + "/" + frameCounter;
        }
    }
}
