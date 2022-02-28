import QtQuick 2.0

/* Кликабельная кнопка с изображением, которое может изменяться
 * в зависимости от того, нажата кнопка или отпущена. Можно задать
 * и третье изображение для деактивированной кнопки
 */
Item {
    property string enabledImage: ""
    property string disabledImage: ""
    property string pressedImage: ""

    /* Нужно, чтобы при наведении мыши появлялась всплывающая подсказка.
     * Это свойство нельзя изменять явно.
     */
    property alias hovered: mouseArea.containsMouse

    signal clicked()

    clip: false

    MouseArea {
        id: mouseArea
        hoverEnabled: true

        anchors.fill: parent

        onPressedChanged: {
            if (containsMouse && !pressed) {
                parent.clicked()
            }
        }
    }

    Image {
        anchors.centerIn: parent

        source: (parent.enabled || parent.disabledImage == "") ? (mouseArea.pressed ? parent.pressedImage : parent.enabledImage) : parent.disabledImage
    }
}
