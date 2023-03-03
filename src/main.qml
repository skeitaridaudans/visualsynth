import QtQuick
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import SinViewItem
import BoxView
// This does not work
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

    Button {
        id: button
        x: 717
        width: 224
        height: 96
        text: qsTr("Send note")
        anchors.right: parent.right
        anchors.top: parent.top
        font.pointSize: 16
        anchors.rightMargin: 979
        anchors.topMargin: 957
        onPressed: {
            controller.noteOn(60)
        }
        onReleased: {
            controller.noteOff(60)
        }
    }

    Rectangle {
        // Operator info box
        id: operatorInfo
        x: 1163
        y: 45
        width: 712
        height: 361
        color: "#000000"
        // Temporary border boundsof box
        border.color: "#aa3232"
        border.width: 5
        // Box title
        Text {
            id: operatorName
            x: 27
            y: 14
            //TODO: get ID of operator and add to it!
            text: qsTr("Operator: ")
            color: "#f0f0f0"
            font.pixelSize: 32
        }


        // Frequency text
        Text {
            id: freqText
            x: 12
            y: 121
            width: 80
            height: 49
            color: "#f0f0f0"
            text: qsTr("Freq: ")
            font.family: "Noto Sans"
            font.pixelSize: 32

        }
        // Frequency amount
        Rectangle {
            id: freqValueBox
            x: 98
            y: 105
            width: 219
            height: 65
            color: "#222222"
            border.color: "#f0f0f0"
            border.width: 3

            Text {
                //TODO: get value from operator and update text
                id: freqValueText
                x: 8
                y: 8
                width: 203
                height: 49
                text: qsTr("1337 " + "Hz")
                font.pixelSize: 32
                color: "#f0f0f0"
            }
        }

        // Amplitude text
        Text {
            id: ampText
            x: 12
            y: 214
            width: 80
            height: 43
            text: qsTr("Amp:")
            font.pixelSize: 32
            color: "#f0f0f0"
        }

        Rectangle {
            id: ampValueBox
            x: 98
            y: 208
            width: 219
            height: 65
            color: "#222222"
            border.color: "#f0f0f0"
            border.width: 3

            Text {
                id: ampValueText
                x: 8
                y: 8
                width: 203
                height: 49
                text: qsTr("420 " + "Db")
                font.pixelSize: 32
                color: "#f0f0f0"
            }
        }

        Rectangle {
            id: opDrag
            x: 438
            y: 38
            width: 200
            height: 200

            MouseArea {
                id: dragger
                anchors.fill: parent
                drag {
                    target: opDrag
                    axis: Drag.YAxis
                }

                property bool dragActive: drag.active


                onDragActiveChanged: {
                    if(drag.active){
                        console.log("HEHE")
                    } else {
                        console.log("HEHE3")
                    }
                }
            }

            // TODO: get color of operator
            // TODO: Change color on drag
            color: "#ff961d"
        }


    }

}
