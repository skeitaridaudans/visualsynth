import QtQuick
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import SinViewItem
import OperatorView
// This does not work
Window {
    id: window
    width: 1920
    height: 1080
    visible: true
    title: qsTr("VisualSynth")
    color: "#212121"

    Material.theme: Material.Dark
    Material.accent: Material.Purple

    Rectangle {
        id: rectangle
        y: 745
        width: 351
        height: 123
        color: "#212121"
            border.color: "gray"
            border.width: 3
            radius: 3
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.leftMargin: 1489
        anchors.bottomMargin: 212

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
            onMoved: waveView.setAmplitude(dial1.value)
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

    OperatorView {
        id: boxes
        anchors.left: parent.left
        anchors.leftMargin: 50
        anchors.top: parent.top
        anchors.topMargin: 50
        width: 900
        height: 800

    }


//    Button {
//        id: button
//        x: 717
//        width: 224
//        height: 96
//        text: qsTr("Send note")
//        anchors.right: parent.right
//        anchors.top: parent.top
//        font.pointSize: 16
//        anchors.rightMargin: 979
//        anchors.topMargin: 957
//        onPressed: {
//            controller.noteOn(60)
//        }
//        onReleased: {
//            controller.noteOff(60)
//        }
//    }

    Rectangle {
        // Operator info box
        id: operatorInfo
        x: 1212
        y: 100
        width: 608
        height: 277
        color: "#212121"
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

        // Frequency text
        Text {
            id: freqText
            x: 12
            y: 79
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
            y: 79
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
            y: 163
            width: 80
            height: 43
            text: qsTr("Amp:")
            font.pixelSize: 32
            color: "#f0f0f0"
        }
        // Amp value box
        Rectangle {
            id: ampValueBox
            x: 98
            y: 158
            width: 219
            height: 65
            color: "#222222"
            border.color: "#f0f0f0"
            border.width: 3
            // Amp value text
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
        // Operator box
        Rectangle {
            id: opDrag
            x: 359
            y: 38
            width: 200
            height: 200
            // TODO: get color of operator
            // TODO: Change color on drag
            color: "#ff961d"
            // Make it respond to geastures
            MultiPointTouchArea{
                anchors.fill: parent
                anchors.rightMargin: 0
                anchors.bottomMargin: 0
                anchors.leftMargin: 0
                anchors.topMargin: 0
                property var drag: parent
                property var offset : null

                function dragMove(holder, point){
                    if (point && drag) {
                        consnole.log("oh we be draggin");
                    }
                }

                onPressed: {
                    var point = touchPoints[0];
                    console.log("Oh we be pressin");
                    parent.width = 250;

                    offset = Qt.point(point.x, point.y);
                }

                onReleased: {
                    parent.width = 200;
                    console.log("Oh we be releasing");
                }


            }


        }


    }

}
    RadioButton {
        id: radioButton
        checked: controller.isConnected()
        x: 64
        y: 938
        text: qsTr("CONNECTED")
    }
}

