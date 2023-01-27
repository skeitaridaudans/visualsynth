import QtQuick
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import fbitem

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Hello World")
    color: "#000000"

    Material.theme: Material.Dark
    Material.accent: Material.Purple

    Label {
        id: label
        x: 163
        y: 448
        text: qsTr("Attack")
    }

    Label {
        id: label1
        x: 249
        y: 448
        text: qsTr("Decay")
    }

    Label {
        id: label2
        x: 327
        y: 448
        text: qsTr("Sustain")
    }

    Label {
        id: label3
        x: 412
        y: 448
        text: qsTr("Release")
    }

    Dial {
        id: dial
        x: 147
        y: 371
        width: 72
        height: 71
        from: 5.0
        to: 25.0
        value: 15.0
        onMoved: waveView.setFrequency(dial.value)
    }

    Dial {
        id: dial1
        x: 232
        y: 371
        width: 72
        height: 71
        from: 0.2
        to: 0.95
        value: 0.9
        onMoved: waveView.setHeight(dial1.value)
    }

    Dial {
        id: dial2
        x: 313
        y: 371
        width: 72
        height: 71
    }

    Dial {
        id: dial3
        x: 399
        y: 371
        width: 72
        height: 71
    }

    Button {
        id: button
        x: 90
        y: 15
        text: qsTr("Starting points")
    }

    Button {
        id: button1
        x: 399
        y: 15
        text: qsTr("Drum mode")
    }

    Button {
        id: button2
        x: 195
        y: 69
        width: 57
        height: 57
        background: Rectangle {
            color: "#ff0000"
        }
    }

    Button {
        id: button3
        x: 338
        y: 69
        width: 57
        height: 57
        background: Rectangle {
            color: "#ff80ff"
        }
    }

    Button {
        id: button4
        x: 270
        y: 127
        width: 57
        height: 57
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
