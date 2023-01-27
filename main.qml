import QtQuick
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import fbitem

Window {
    id: window
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")
    color: "#000000"

    Material.theme: Material.Dark
    Material.accent: Material.Purple

    Rectangle {
        id: rectangle
        y: 357
        width: 351
        height: 123
        color: "#000000"
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 0
        anchors.horizontalCenter: parent.horizontalCenter

        Label {
            id: label
            x: 26
            y: 86
            text: qsTr("Attack")
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 16
        }

        Label {
            id: label1
            x: 112
            y: 86
            text: qsTr("Decay")
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 16
        }

        Label {
            id: label2
            x: 190
            y: 86
            text: qsTr("Sustain")
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 16
        }

        Label {
            id: label3
            x: 275
            y: 86
            text: qsTr("Release")
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 16
        }

        Dial {
            id: dial
            x: 10
            y: 9
            width: 72
            height: 71
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 38
            from: 5.0
            to: 25.0
            value: 15.0
            onMoved: waveView.setFrequency(dial.value)
        }

        Dial {
            id: dial1
            x: 95
            y: 9
            width: 72
            height: 71
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 38
            from: 0.2
            to: 0.95
            value: 0.9
            onMoved: waveView.setHeight(dial1.value)
        }

        Dial {
            id: dial2
            x: 176
            y: 9
            width: 72
            height: 71
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 38
        }

        Dial {
            id: dial3
            x: 262
            y: 9
            width: 72
            height: 71
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 38
        }
    }

    Button {
        id: button
        text: qsTr("Starting points")
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.leftMargin: 90
        anchors.topMargin: 15
    }

    Button {
        id: button1
        x: 399
        text: qsTr("Drum mode")
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.rightMargin: 90
        anchors.topMargin: 15
    }

    Button {
        id: button2
        width: 57
        height: 57
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.leftMargin: 200
        anchors.topMargin: 69
        background: Rectangle {
            color: "#ff0000"
        }
    }

    Button {
        id: button3
        x: 338
        width: 57
        height: 57
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.rightMargin: 200
        anchors.topMargin: 69
        background: Rectangle {
            color: "#ff80ff"
        }
    }

    Button {
        id: button4
        width: 57
        height: 57
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 127
        background: Rectangle {
            color: "#fff200"
        }
    }


    FbItem {
        id: waveView
        anchors.centerIn: parent
        width: 500
        height: 100
    }

}
