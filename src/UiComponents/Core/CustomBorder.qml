import QtQuick

Item {
    property var sides: []

    height: parent.height
    width: parent.width

    Rectangle {
        visible: sides.includes("top")
        anchors.top: parent.top
        color: borderColor
        height: borderWidth
        width: parent.width
    }
    Rectangle {
        visible: sides.includes("right")
        anchors.right: parent.right
        color: borderColor
        height: parent.height
        width: borderWidth
    }
    Rectangle {
        visible: sides.includes("left")
        anchors.left: parent.left
        color: borderColor
        height: parent.height
        width: borderWidth
    }
    Rectangle {
        visible: sides.includes("bottom")
        anchors.bottom: parent.bottom
        color: borderColor
        height: borderWidth
        width: parent.width
    }
}
