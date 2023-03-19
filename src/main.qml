import QtQuick
import QtQuick.Controls 2.12
import QtQuick.Controls.Material 2.12
import SinViewItem
import OperatorView
// This does not work
//import Presets

Window {
    id: window
    width: 1920
    height: 1080
    visible: true
    title: qsTr("VisualSynth")
    color: "#212121"

    //    #property var currentOp
    //    #currentOp: controller.getSelectedOperator()u

    //    function onChangeCurrentOp() {
    //        console.log("changed")
    //    }

    Material.theme: Material.Dark
    Material.accent: Material.Purple

    Timer {
        id: alertControllerUpdate
        repeat: true
        running: true
        interval: 10
        onTriggered: alertController.update()
    }

    Rectangle {
        id: rectangle
        y: 771
        width: 351
        height: 123
        color: "#212121"
        border.color: "gray"
        border.width: 3
        radius: 3
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.leftMargin: 1321
        anchors.bottomMargin: 186

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

    Rectangle {
        id: sinewaverectangle
        x: 0
        y: 898
        width: 1922
        height: 185
        color: "#212121"
        border.color: "gray"
        border.width: 3
        anchors.bottom: parent.bottom
        anchors.bottomMargin: -3
        radius: 3
        SinWaveItem {
            id: waveView
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.rightMargin: 20
            anchors.bottomMargin: 10
            width: 500
            height: 100
        }

        RoundButton {
            id: connectedRoundButton
            x: 88
            //state: connected ? "Synth_connected" : "Synth_not_connected"    // trying to make color change
            y: 94
            width: 34
            height: 34
            text: ""
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            anchors.leftMargin: 88
            anchors.bottomMargin: 54 //
            background: Rectangle {
                radius: connectedRoundButton.radius
                color: "#55ff00"
            }

        }

        Text {
            id: connectedText
            x: 128
            y: 100
            width: 141
            height: 27
            color: "#ffffff"
            text: qsTr("Connected")
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            font.pixelSize: 18
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            anchors.leftMargin: 127
            anchors.bottomMargin: 58
        }

    }

    Rectangle {
        id: operatorrectangle
        x: 0
        y: 0
        width: 1065
        height: 901
        color: "#212121"
        border.color: "gray"
        border.width: 3
        radius: 3
        OperatorView {
            id: boxes
            anchors.left: parent.left
            anchors.leftMargin: 50
            anchors.top: parent.top
            anchors.topMargin: 50
            width: 900
            height: 800
        }

        ComboBox {

            //var bleBle = qsTr(presets.getName() + "hello");
            //var bleBle = text()"helloooo";
            id: presetDropdown
            x: 412
            y: 57
            width: 262
            height: 36
            //model: ["Preset 1", "Preset 2", bleBle]  // , String(bleBle)
            textRole: "key"
            model: ListModel {
                    ListElement { key: "Init"; value: 0 }
                    ListElement { key: "Bass"; value: 1 }
                    ListElement { key: "Pad"; value: 2 }
                    ListElement { key: "Lead"; value: 3 }
                }

        }


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
    //            console.log(currentOp.freqProp)
    //            controller.noteOn(60)
    //       }
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
        border.color: "gray"
        border.width: 3
        radius: 3
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
                    text: qsTr(currentOp.ampProp + "Db")
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
                    //                property var currentOp: null
                    touchPoints: [
                        TouchPoint {id: touch1}
                    ]



                    onTouchUpdated: {
                        var currentOp = controller.getSelectedOperator();
                        if (currentOp) {
                            var freq = currentOp.getFreq();
                            var amp = currentOp.getAmp();
                            console.log("Current frequency: ", freq);
                            console.log("Current amplitude: ", amp);
                            if (touchPoints[0].x > offset.x){
                                currentOp.setFrequency(1)
                                controller.changeFrequency(currentOp.idProp, currentOp.freqProp);
                            } else if (touchPoints[0].x < offset.x){
                                currentOp.setFrequency(-1)
                                controller.changeFrequency(currentOp.idProp, currentOp.freqProp);
                            }
                            if (touchPoints[0].y < offset.y) {
                                currentOp.setAmplitude(1)
                                controller.changeAmplitude(currentOp.idProp, currentOp.ampProp);
                            } else if (touchPoints[0].y > offset.y){
                                currentOp.setAmplitude(-1)
                                controller.changeAmplitude(currentOp.idProp, currentOp.ampProp);
                            }
                        }
                    }

                    onPressed: {
                        //                    currentOp: controller.getSelectedOperator();
                        var point = touchPoints[0];
                        //                    var operator = controller.getSelectedOperator();
                        //                    console.log(operator.freqProp);
                        //                    console.log(operator);
                        parent.width = parent.width+10;
                        offset = Qt.point(point.x, point.y);
                        //                    dragMove(offset, point)
                    }

                    onReleased: {
                        parent.width = 200;
//                        var currentOp = controller.getSelectedOperator();
//                        if (currentOp) {
//                        controller.sendOperator(currentOp.idProp);
//                        }
                        console.log("Oh we be releasing");
                    }

                }

            }

        }

    }

    Text {
        id: presetsText
        x: 347
        y: 62
        width: 68
        height: 27
        color: "#ffffff"
        text: qsTr("Presets")
        font.pixelSize: 18

    }

    Rectangle {
        id: rectangle1
        y: 965
        width: 350
        height: 53
        color: "#f44336"
        radius: 10
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 62
        anchors.horizontalCenter: parent.horizontalCenter
        state: alertController.alertVisibleState

        states: [
            State {
                name: "invisible"
                PropertyChanges { target: rectangle1; anchors.bottomMargin: -53; opacity: 0 }
            },
            State {
                name: "visible"
                PropertyChanges { target: rectangle1; anchors.bottomMargin: 62; opacity: 1 }
            }
        ]

        transitions: [
            Transition {
                to: "invisible"
                NumberAnimation { properties: "anchors.bottomMargin,opacity"; easing.type: Easing.InOutQuad; duration: 400; loops: 1 }
            },
            Transition {
                to: "visible"
                NumberAnimation { properties: "anchors.bottomMargin,opacity"; easing.type: Easing.InOutQuad; duration: 400; loops: 1 }
            }
        ]

        Text {
            id: text1
            x: 0
            y: 0
            width: 350
            height: 53
            color: "#ffffff"
            text: alertController.alertText
            font.pixelSize: 22
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }
    // TODO: make colour of CONNECTED change if synth is not connected
    //    states: [
    //        State {
    //            name: "Synth_connected"
    //            PropertyChanges {
    //                target: connectedRoundButton
    //                radius: myRoundButton.radius
    //                color: "blue" //"#55ff00"
    //
    //            }
    //        },
    //        State {
    //            name: "Synth_not_connected"
    //            PropertyChanges {
    //                target: connectedRoundButton
    //                radius: myRoundButton.radius
    //                color: "gray"
    //            }
    //        }
    //    ]

}
