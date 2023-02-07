import QtQuick
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import SinViewItem
import BoxView

Window {
    id: window
    width: 1920
    height: 1080
    visible: true
    title: qsTr("Hello World")
    color: "#000000"

    Material.theme: Material.Dark
    Material.accent: Material.Purple

    Rectangle {
        id: rectangle
        y: 957
        width: 351
        height: 123
        color: "#000000"
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.leftMargin: 177
        anchors.bottomMargin: 0

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


    SinWaveItem {
        id: waveView
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.rightMargin: 177
        anchors.bottomMargin: 10
        width: 500
        height: 100
    }

    BoxView {
        id: boxes
        anchors.left: parent.left
        anchors.leftMargin: 50
        anchors.top: parent.top
        anchors.topMargin: 50
        width: 900
        height: 800
    }

}
