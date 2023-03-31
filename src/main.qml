import QtQuick
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQml 2.0
import SinViewItem
import OperatorView
import AmpEnvGraphView
import QtQuick 2.15
// This does not work
//import Presets
//import QtMultimedia 5.15

Window {
    id: window
    width: 1920
    height: 1080
    visible: true
    title: qsTr("VisualSynth")
    color: "#212121"

    property var selectedOperator: null

    Connections {
        target: controller
        function onOperatorSelected(operator) {
            selectedOperator = operator
            freqValueText.text = operator.freqProp + "hz"
            ampValueText.text = operator.ampProp + ""
        }

        function onFreqChanged(freq){
            freqValueText.text = freq + " hz"
        }

        function onAmpChanged(amp) {
            ampValueText.text = amp + ""
        }
    }

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
        y: 750
        width: 628
        height: 123
        color: "#212121"
        border.color: "gray"
        border.width: 3
        radius: 3
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.leftMargin: 1212
        anchors.bottomMargin: 207

        Label {
            id: label
            x: 26
            y: 86
            text: qsTr("Attack")
           anchors.bottom: parent.bottom
            anchors.rightMargin: 20
            anchors.bottomMargin: 10
            width: 500
            height: 100
        }
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


        Button {
            id: button
            x: 81
            width: 155
            height: 53
            text: qsTr("Send note")
            anchors.right: parent.right
            anchors.top: parent.top
            font.pointSize: 16
            anchors.rightMargin: 1684
            anchors.topMargin: 927
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
        x: 1212
        y: 100
        width: 608
        height: 277
        color: "#212121"
        // Temporary border boundsof box
        border.color: "gray"
        border.width: 3
        radius: 3
        property int operatorId: selectedOerator ? selectedOperator.idProp : 0
        // Box title
        Text {
            id: operatorName
            x: 27
            y: 14
            //TODO: get ID of operator and add to it!
            text: qsTr("Operator: " + (selectedOperator ? selectedOperator.idProp+1 : ""))
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
                    text: selectedOperator ? selectedOperator.freqProp + " hz" : "0"
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
                    text: selectedOperator ? selectedOperator.ampProp + "" : "0"
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
                        if (selectedOperator) {

                            // TODO add multipliers !
                            var freq = selectedOperator.getFreq();
                            var amp = selectedOperator.getAmp();

                            if (touchPoints[0].x > offset.x){
                                selectedOperator.setFrequency(1)
                                controller.changeFrequency(selectedOperator.idProp, selectedOperator.freqProp);
                            } else if (touchPoints[0].x < offset.x){
                                selectedOperator.setFrequency(-1)
                                controller.changeFrequency(selectedOperator.idProp, selectedOperator.freqProp);
                            }
                            if (touchPoints[0].y < offset.y) {
                                selectedOperator.setAmplitude(1)
                                controller.changeAmplitude(selectedOperator.idProp, selectedOperator.ampProp);

                            } else if (touchPoints[0].y > offset.y){
                                selectedOperator.setAmplitude(-1)
                                controller.changeAmplitude(selectedOperator.idProp, selectedOperator.ampProp);
                            }
                        }
                    }

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

                    onPressed: {
                        var point = touchPoints[0];
                        parent.border.color = "pink"
                        parent.border.width = 3
                        parent.width = parent.width + 5
                        parent.height = parent.height + 5
                        offset = Qt.point(point.x, point.y);
                    }

                    onReleased: {
                        parent.border.width = 0
                        parent.width = parent.width - 5
                        parent.height = parent.height - 5

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


    Rectangle {
        id: rectangle
        y: 556
        width: 351
        height: 338
        color: "#212121"
        border.color: "gray"
        border.width: 3
        radius: 3
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.leftMargin: 1321
        anchors.bottomMargin: 186


    Item {
        id: envelopeItem
        x: 24
        y: 13
        width: 310
        height: 210

        property real attackTime: 0.01
        property real decTime: 0.3
        property real susLevel: 0.25
        property real relTime: 0.3

        Canvas {

            id: envelopeCanvas
            anchors.fill: parent

            onPaint: {
                var ctx = getContext("2d")
                var w = width
                var h = height
                ctx.clearRect(0, 0, w, h);


                // Release
                var relStart = ((4 - envelopeItem.relTime) * (w/4))
                ctx.beginPath()
                ctx.lineWidth = 5
                ctx.strokeStyle = "purple"
                ctx.moveTo(relStart, (envelopeItem.susLevel * h))
                ctx.lineTo(w, h)
                ctx.stroke()


                // Attack
                var attackEnd = (envelopeItem.attackTime * (w/4))
                ctx.beginPath()
                ctx.lineWidth = 5
                ctx.strokeStyle = "purple"
                ctx.moveTo(0, h)
                ctx.lineTo(attackEnd, 0)
                ctx.stroke()


                // Decay
                //var decayEnd = attackEnd + envelopeItem.decTime * w / (envelopeItem.attackTime + envelopeItem.decTime)
                var decayEnd = (attackEnd + (envelopeItem.decTime  * (w/4)) - envelopeItem.relTime)
                ctx.beginPath()
                ctx.lineWidth = 5
                ctx.strokeStyle = "purple"
                ctx.moveTo(attackEnd, 0)
                ctx.lineTo(decayEnd, envelopeItem.susLevel * h)
                ctx.stroke()


                // Sustain
                ctx.beginPath()
                ctx.lineWidth = 5
                ctx.strokeStyle = "purple"
                ctx.moveTo(decayEnd, (envelopeItem.susLevel) * h)
                ctx.lineTo(relStart,(envelopeItem.susLevel) * h)
                ctx.stroke()
            }
        }
    }
    Label {
        id: label
        x: 26
        y: 86
        text: qsTr("Attack")
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 16
    }


    AmpEnvGraphItem{
        id: ampEnvGraphView
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.rightMargin:80
            anchors.bottomMargin: 340
            width: 628
            height: 316
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
        from: 0.01
        //to: 0.5
        value: 0.01

        property real commonValue;
        onValueChanged: {
            console.log("Attack: " + value);
            envelopeItem.attackTime = dial.value;
            envelopeCanvas.requestPaint();
        }
    }

    Dial {
        id: dial1
        x: 95
        y: 9
        width: 72
        height: 71
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 38
        from: 0.01
        //to: 0.3
        value: 0.01

        property real commonValue;
        onValueChanged: {
            console.log("Decay: " + value);
            envelopeItem.decTime = dial1.value;
            envelopeCanvas.requestPaint();
        }
    }

    Dial {
        id: dial2
        x: 176
        y: 9
        width: 72
        height: 71
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 38
        from: 0.01
        value: 0.01

        property real commonValue;
        onValueChanged: {
            console.log("Sustain: " + value);
            envelopeItem.susLevel = dial2.value;
            envelopeCanvas.requestPaint();
        }
    }

    Dial {
        id: dial3
        x: 262
        y: 9
        width: 72
        height: 71
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 38
        from: 0.01
        //to: 0.3
        value: 0.01

        property real commonValue;
        onValueChanged: {
            console.log("Release: " + value);
            envelopeItem.relTime = dial3.value;
            envelopeCanvas.requestPaint();
        }
    }

 }

 }
