import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQml 2.0
import SinViewItem
import OperatorView
import OperatorPresetsView
import AmpEnvGraphView
import OutputWaveView

Window {
    id: window
    width: 1920
    height: 1080
    visible: true
    title: qsTr("VisualSynth")
    color: "#212121"
    property var selectedOperator: null

    Component.onCompleted: {
        controller.setAmpEnvelopeSize(3); // controller.setReleaseAmpEnvelopeSize(3); ???
        controller.setAttackAmpEnvelopePoint(0, 0, 0);
        controller.setAttackAmpEnvelopePoint(3, 1-(dialSustain.value/300), 5);

        controller.setReleaseAmpEnvelopeSize(2);
        controller.setReleaseAmpEnvelopePoint(0 , dialSustain.value ,0);
        controller.setReleaseAmpEnvelopePoint(1, 0, 1);
    }

    Connections {

        target: controller
        function onOperatorSelected(operator) {

            selectedOperator = operator
            freqText.text = parseFloat(operator.freqProp).toFixed(1) + ""
            ampText.text = operator.ampProp + ""

            opContainer.enabled = true
            opContainer.visible = true

            var color = selectedOperator.getColorForOperator();

            opWaveView.setFrequency(operator.freqProp);
            opWaveView.setAmplitude(operator.ampProp);
            opWaveView.setColor(color);
            opDrag.color = color.alpha(0.5).darker(3);

            lfoOperatorFreqAmountText.text = `${selectedOperator.frequencyLfoAmount}%`;
            lfoOperatorAmpAmountText.text = `${selectedOperator.amplitudeLfoAmount}%`;
        }

        function onOperatorDeselected(deselected){
            opContainer.enabled = false;
            opContainer.visible = false;
        }

        function onFreqChanged(freq){
            freqText.text = parseFloat(freq).toFixed(1) + ""
            waveView.setFrequency(freq);

            opWaveView.setFrequency(freq);
            var color = selectedOperator.getColorForOperator();
            opWaveView.setColor(color);
            opDrag.color = color.alpha(0.5).darker(3);
        }

        function onAmpChanged(amp) {
            ampText.text = amp + ""
            opWaveView.setAmplitude(amp);
            var color = selectedOperator.getColorForOperator();
            opWaveView.setColor(color);
            opDrag.color = color.alpha(0.5).darker(3);
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
        OutputWaveView {
            id: outputWaveView
            anchors.right: parent.right
            anchors.bottom: parent.bottom
            anchors.rightMargin: 20
            anchors.bottomMargin: 10
            width: 800
            height: 100
        }


        Rectangle {
            id: connectedRoundButton
            x: 88
            y: 94
            width: 34
            height: 34
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            anchors.leftMargin: 88
            anchors.bottomMargin: 54
            color: controller.isConnected ? "green" : "red"
            radius: 50
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

        Button {
            id: button1
            width: 250
            height: 50
            text: qsTr("Presets")

            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 50
            onPressed: {
                controller.deselectOperator()
                controller.showPresets = !controller.showPresets;
            }
        }

        Rectangle {
            id: presetsContainer
            visible: controller.showPresets

            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: button1.height*2

            width: 450
            height: 450
            color: "#323232"
            radius: 8

            OperatorPresetsView {
                id: presetsView
               anchors.left: parent.left
                anchors.top: parent.top
                anchors.fill: parent
                enabled: controller.showPresets
            }
        }


    }


    Rectangle {
        // Operator info box
        id: operatorInfo
        x: 1065
        y: 0
        width: 857
        height: 377
        color: "#212121"
        // Temporary border boundsof box
        border.color: "gray"
        border.width: 3
        radius: 3
        property int operatorId: selectedOperator ? selectedOperator.idProp : 0

        property bool fineCheck: false
        property bool coarseCheck: true

        // Box title
        Rectangle {
            id: opContainer
            enabled: selectedOperator ? true : false
            visible: selectedOperator ? true : false
            color: parent.color
            width: parent.width - 4
            height: parent.height - 4
            anchors.fill: parent
            border.color: "gray"
            border.width: 3
            radius: 3
            Text{
                id: freqText
                x: 1064
                y: 342
                width: 12
                height: 10
                text: "0"
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.horizontalCenter: opDrag.horizontalCenter
                anchors.rightMargin: 319
                anchors.leftMargin: 510
                color: "gray"
                font.pixelSize: 16
                font.family: "Noto Sans"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                Text{
                    id: plusFreq
                    x: 82
                    y: -3
                    width: 50
                    height: 21
                    text: "+"
                    font.pixelSize: 16
                    color: parent.color
                    font.family: "Noto Sans"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter

                    Rectangle {
                        id: freqPlusTouchpoint
                        x: 0
                        y: 0
                        width: 50
                        height: 32
                        color: "#ffffff"
                        opacity: 0

                        Timer {
                            id: freqIncTimer
                            interval: 500
                            repeat: true

                            onTriggered: {
                                selectedOperator.setFrequency(5)
                                controller.changeFrequency(selectedOperator.idProp, selectedOperator.freqProp)
                                interval = 100
                            }
                        }

                        MultiPointTouchArea{
                            id: freqPlusButton
                            anchors.fill: parent
                            onPressed : {
                                plusFreq.color = "pink"
                                selectedOperator.setFrequency(1)
                                controller.changeFrequency(selectedOperator.idProp, selectedOperator.freqProp);
                                freqIncTimer.start()

                            }
                            onReleased: {
                                plusFreq.color = plusFreq.parent.color
                                freqIncTimer.interval = 500
                                freqIncTimer.stop()
                            }
                        }
                    }
                }
                Text{
                    id: minFreq
                    x: -94
                    y: -3
                    width: 50
                    height: 21
                    text: "-"
                    font.pixelSize: 16
                    color: parent.color
                    font.family: "Noto Sans"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    Timer{
                        id: freqDecTimer
                        interval: 500
                        repeat: true

                        onTriggered: {
                            selectedOperator.setFrequency(-5)
                            controller.changeFrequency(selectedOperator.idProp, selectedOperator.freqProp)
                            interval = 100
                        }
                    }
                    Rectangle {
                        id: minFreqTouchpoint
                        x: 0
                        y: 0
                        width: 50
                        height: 26
                        color: "#ffffff"
                        opacity: 0
                        MultiPointTouchArea{
                            id: freqMinButton
                            anchors.fill: parent
                            onPressed : {
                                minFreq.color = "pink"
                                selectedOperator.setFrequency(-1)
                                controller.changeFrequency(selectedOperator.idProp, selectedOperator.freqProp);
                                freqDecTimer.start()

                            }
                            onReleased: {
                                minFreq.color = minFreq.parent.color
                                freqDecTimer.interval = 500
                                freqDecTimer.stop()
                            }
                        }
                    }
                }
            }
            Text {
                id: ampText
                x: 828
                y: 189
                text: "0"
                color: "gray"
                font.pixelSize: 16
                font.family: "Noto Sans"
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                Text {
                    id: ampPlus
                    x: 0
                    y: -60
                    width: 9
                    height: 22
                    text: qsTr("+")
                    font.pixelSize: 16
                    color: parent.color
                    font.family: "Noto Sans"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    Timer {
                        id: ampIncTimer
                        interval: 500
                        repeat: true

                        onTriggered: {
                            selectedOperator.setAmplitude(2)
                            controller.changeAmplitude(selectedOperator.idProp, selectedOperator.ampProp)
                            interval = 100
                        }
                    }

                    Rectangle {
                        id: ampIncTouchpoint
                        x: -5
                        y: -7
                        width: 30
                        height: 38
                        color: "#ffffff"
                        opacity: 0
                        MultiPointTouchArea{
                            id: ampPlusButton
                            anchors.fill: parent
                            onPressed : {
                                ampPlus.color = "pink"
                                selectedOperator.setAmplitude(1)
                                controller.changeAmplitude(selectedOperator.idProp, selectedOperator.ampProp);
                                ampIncTimer.start()

                            }
                            onReleased: {
                                ampPlus.color = ampPlus.parent.color
                                ampIncTimer.interval = 500
                                ampIncTimer.stop()
                            }

                        }
                    }
                }

                Text {
                    id: ampMin
                    x: 2
                    y: 72
                    text: qsTr("-")
                    font.pixelSize: 16
                    color: parent.color
                    font.family: "Noto Sans"
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                    Timer {
                        id: ampDecTimer
                        interval: 500
                        repeat: true

                        onTriggered: {
                            selectedOperator.setAmplitude(-2)
                            controller.changeAmplitude(selectedOperator.idProp, selectedOperator.ampProp)
                            interval = 100
                        }
                    }
                    Rectangle {
                        id: ampMinTouchPoint
                        x: -8
                        y: 0
                        width: 30
                        height: 28
                        color: "#ffffff"
                        opacity: 0
                        MultiPointTouchArea{
                            id: ampMinButton
                            anchors.fill: parent
                            onPressed : {
                                ampMin.color = "pink"
                                selectedOperator.setAmplitude(-1)
                                controller.changeAmplitude(selectedOperator.idProp, selectedOperator.ampProp);
                                ampDecTimer.start()
                            }
                            onReleased: {
                                ampMin.color = ampMin.parent.color
                                ampDecTimer.interval = 500
                                ampDecTimer.stop()
                            }

                        }
                    }
                }
            }

            Text {
                id: operatorName
                x: 32
                y: 16
                text: qsTr("Operator " + (selectedOperator ? selectedOperator.idProp+1 : ""))
                color: "#f0f0f0"
                font.pixelSize: 32
                font.family: "Noto Sans"
                font.weight: Font.Bold
            }
            // Operator box
            Rectangle {
                id: opDrag
                x: 224
                y: 63
                width: 598
                height: 273
                color: "lightgray" // Placeholder while colors are off on the wave
                //color: "#414141"


                Rectangle {
                    id: opsinewaverectangle
                    x: 2
                    y: 1
                    width: parent.width - 4
                    height: parent.height - 2
                    color: parent.color
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 1
                    radius: 3
                    SinWaveItem {
                        id: opWaveView
                        anchors.right: parent.right
                        anchors.rightMargin: 2
                        anchors.bottom: parent.bottom
                        //    		        anchors.rightMargin: 20
                        //                            anchors.bottomMargin: 10
                        width: parent.width - 3
                        height: parent.height - 3
                    }
                }

                // Make it respond to geastures
                MultiPointTouchArea{
                    anchors.fill: parent
                    anchors.rightMargin: 8
                    anchors.bottomMargin: 0
                    anchors.leftMargin: 0
                    anchors.topMargin: 0
                    property var drag: parent
                    property var offset : null
                    property bool horiDrag: false
                    property bool vertiDrag: false
                    property real lastX: 0
                    property real lastY: 0
                    touchPoints: [
                        TouchPoint {id: touch1}
                    ]

                    onTouchUpdated: {
                        if (selectedOperator) {
                            // TODO add multipliers !
                            var freq = selectedOperator.getFreq();
                            var amp = selectedOperator.getAmp();
                            if(touchPoints.length > 0){

                                var touchPoint = touchPoints[0]
                                var xDelta = touchPoint.sceneX - lastX;
                                var yDelta = touchPoint.sceneY - lastY;
                                lastX = touchPoint.sceneX;
                                lastY = touchPoint.sceneY;


                                if (!horiDrag && !vertiDrag){
                                    if (Math.abs(xDelta) > Math.abs(yDelta)){
                                        horiDrag = true
                                    } else if (Math.abs(yDelta) > Math.abs(xDelta)) {
                                        vertiDrag = true;
                                    }
                                } else if (horiDrag && Math.abs(yDelta) > Math.abs(xDelta)){
                                    vertiDrag = true;
                                    horiDrag = false;
                                } else if ( vertiDrag && Math.abs(xDelta) > Math.abs(yDelta)){
                                    horiDrag = true;
                                    vertiDrag = false;
                                }

                                if(horiDrag) {
                                    if (xDelta > 0){
                                        if(operatorInfo.fineCheck){
                                            if (Math.abs(xDelta) > 10){
                                                selectedOperator.setFrequency(5)
                                            }
                                            else{
                                                selectedOperator.setFrequency(0.1)
                                            }
                                        } else {
                                            selectedOperator.setFrequency(10)
                                        }

                                        controller.changeFrequency(selectedOperator.idProp, selectedOperator.freqProp);

                                    } else if (xDelta < 0){
                                        if(operatorInfo.fineCheck){
                                            if (xDelta < -10){
                                                selectedOperator.setFrequency(-5)
                                            }
                                            else{
                                                selectedOperator.setFrequency(-0.1)
                                             }
                                        } else {
                                            selectedOperator.setFrequency(-10)
                                        }

                                        controller.changeFrequency(selectedOperator.idProp, selectedOperator.freqProp);
                                    }
                                } else if(vertiDrag) {
                                    if (yDelta < 0) {
                                        selectedOperator.setAmplitude(1)
                                        controller.changeAmplitude(selectedOperator.idProp, selectedOperator.ampProp);

                                    } else if (yDelta > 0){
                                        selectedOperator.setAmplitude(-1)
                                        controller.changeAmplitude(selectedOperator.idProp, selectedOperator.ampProp);
                                    }
                                }
                            }
                        }
                    }



                    onPressed: {
                        var point = touchPoints[0]

                        opDrag.border.color = "pink"
                        opDrag.border.width = 3
                        opDrag.width = parent.width + 1
                        opDrag.height = parent.height + 1
                        offset = Qt.point(point.x, point.y);
                        point.startSceneX = touchPoint.sceneX
                        point.startSceneY = touchPoint.sceneY
                        lastX = point.sceneX
                        lastY = point.sceneY
                    }

                    onReleased: {
                        opDrag.border.width = 0
                        opDrag.width = parent.width - 1
                        opDrag.height = parent.height - 1
                        horiDrag = false;
                        vertiDrag = false;

                    }

                }

            }

            Text {
                id: freqControlTitle
                color: "#9E9E9E"
                text: "Frequency Control"
                font.family: "Noto Sans"
                anchors.top: operatorName.bottom
                anchors.topMargin: 16
                anchors.left: parent.left
                anchors.leftMargin: 32
                font.pointSize: 14
                font.weight: Font.DemiBold
            }

            Rectangle {
                id: freqControlBox
                color: "transparent"
                anchors.top: freqControlTitle.bottom
                anchors.topMargin: 16
                anchors.left: freqControlTitle.left
                anchors.leftMargin: 8
                height: freqControlBoxColumn.height

                Column {
                    id: freqControlBoxColumn
                    spacing: 8

                    ToggleButton {
                        id: fine
                        enabled: operatorInfo.fineCheck
                        text: qsTr("Fine")
                        onPressed: {
                            operatorInfo.coarseCheck = false;
                            operatorInfo.fineCheck = true;
                        }
                    }

                    ToggleButton {
                        id: coarse
                        enabled: operatorInfo.coarseCheck
                        text: qsTr("Coarse")
                        onPressed: {
                            operatorInfo.coarseCheck = true;
                            operatorInfo.fineCheck = false;
                        }
                    }
                }
            }

            Text {
                id: lfoOperatorOptionsTitle
                color: "#9E9E9E"
                text: "LFO"
                font.family: "Noto Sans"
                anchors.top: freqControlBox.bottom
                anchors.topMargin: 16
                anchors.left: parent.left
                anchors.leftMargin: 32
                font.pointSize: 14
                font.weight: Font.DemiBold
            }

            Rectangle {
                id: lfoOperatorOptionsBox
                color: "transparent"
                anchors.top: lfoOperatorOptionsTitle.bottom
                anchors.topMargin: 8
                anchors.left: lfoOperatorOptionsTitle.left
                anchors.leftMargin: 8
                height: lfoOperatorOptionsColumn.height

                Column {
                    id: lfoOperatorOptionsColumn
                    spacing: 4

                    Column {
                        spacing: 2

                        Text {
                            id: frequencyAmount
                            color: "#757575"
                            text: "Frequency"
                            font.family: "Noto Sans"
                            font.pointSize: 12
                        }

                        Row {
                            spacing: 8

                            Text {
                                id: lfoOperatorFreqAmountText
                                text: `${selectedOperator ? selectedOperator.frequencyLfoAmount : 0}%`
                                color: "#616161"
                                height: 40
                                width: 32
                                verticalAlignment: Text.AlignVCenter
                                anchors.topMargin: 8
                            }
                            Slider {
                                id: lfoOperatorFreqAmountSlider
                                height: 40
                                width: 96

                                from: 0
                                to: 100
                                value: selectedOperator ? selectedOperator.frequencyLfoAmount : 0
                                onMoved: {
                                    controller.setOperatorLfoFrequency(selectedOperator.idProp, lfoOperatorFreqAmountSlider.value);

                                    // It would make more sense to add a signal for when frequencyLfoAmount changes, so the text would update automatically,
                                    // however, for some reason the Operator is created on another thread (event though it's just being created inside paint() of operatorView)
                                    // which will cause qt to crash if you have a signal for when the properties in Operator change
                                    lfoOperatorFreqAmountText.text = `${selectedOperator.frequencyLfoAmount}%`;
                                }
                            }
                        }
                    }

                    Column {
                        spacing: 2

                        Text {
                            id: amplitudeAmount
                            color: "#757575"
                            text: "Amplitude"
                            font.family: "Noto Sans"
                            font.pointSize: 12
                        }
                        Row {
                            spacing: 8

                            Text {
                                id: lfoOperatorAmpAmountText
                                text: `${selectedOperator ? selectedOperator.amplitudeLfoAmount : 0}%`
                                color: "#616161"
                                height: 40
                                width: 32
                                verticalAlignment: Text.AlignVCenter
                                anchors.topMargin: 8
                            }

                            Slider {
                                id: lfoOperatorAmpAmountSlider
                                height: 40
                                width: 96

                                from: 0
                                to: 100
                                value: selectedOperator ? selectedOperator.amplitudeLfoAmount : 0
                                onMoved: {
                                    controller.setOperatorLfoAmplitude(selectedOperator.idProp, lfoOperatorAmpAmountSlider.value);
                                    lfoOperatorAmpAmountText.text = `${selectedOperator.amplitudeLfoAmount}%`;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    Rectangle {
        id: rectangle1
        y: 965
        width: 510
        height: 55
        color: alertController.alertColor
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
            color: "#ffffff"
            text: alertController.alertText
            anchors.fill: parent
            font.pixelSize: 22
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
        }
    }

    Rectangle {
        id: lfoOptions
        width: 857
        height: 100
        color: "transparent"

        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.top: operatorInfo.bottom
        anchors.topMargin: 0

        Text {
            id: lfoOptionsTitle
            color: "#ffffff"
            text: "LFO"
            font.family: "Noto Sans"
            anchors.left: parent.left
            anchors.leftMargin: 24
            anchors.topMargin: 8
            anchors.top: parent.top
            verticalAlignment: Text.AlignTop
            font.pointSize: 18
            font.weight: Font.Bold
        }

        ToggleButton {
            id: lfoEnabled
            anchors.top: lfoOptionsTitle.bottom
            anchors.topMargin: 8
            anchors.left: lfoOptionsTitle.left
            anchors.leftMargin: 0
            enabled: controller.isLfoEnabled
            text: qsTr("Enabled")
            onPressed: {
                controller.setLfoEnabled(!controller.isLfoEnabled);
            }
        }

        Text {
            id: lfoRateTitle
            color: "#9E9E9E"
            text: "Rate"
            height: lfoOptionsTitle.height
            font.family: "Noto Sans"
            anchors.left: lfoOptionsTitle.right
            anchors.leftMargin: 96
            anchors.topMargin: 8
            anchors.top: parent.top
            verticalAlignment: Text.AlignBottom
            horizontalAlignment: Text.AlignLeft
            font.pointSize: 14
            font.weight: Font.DemiBold
        }

        Text {
            id: lfoRateValue
            text: `${controller.lfoFrequency / 10} Hz`
            color: "#757575"
            height: 40
            width: 32
            anchors.left: lfoRateTitle.left
            anchors.leftMargin: 8
            anchors.top: lfoRateTitle.bottom
            anchors.topMargin: 8
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
        }

        Slider {
            id: lfoSlider
            width: 400
            height: 40
            anchors.left: lfoRateValue.right
            anchors.leftMargin: 16
            anchors.top: lfoRateTitle.bottom
            anchors.topMargin: 8

            from: 1
            to: 200
            stepSize: 1
            value: controller.lfoFrequency
            onMoved: {
                controller.setLfoFrequency(lfoSlider.value);
            }
        }
    }

    AmpEnvGraphItem{
        id: ampEnvGraphView
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.top: lfoOptions.bottom
        anchors.topMargin: 0

        width: ampEnvGraphView.W
        height: ampEnvGraphView.H
    }


    Rectangle {
        id: dialContainer
        width: ampEnvGraphView.width
        height: 140
        enabled: false
        visible: false
        color: "transparent"
        border.width: ampEnvGraphView.bW
        border.color: ampEnvGraphView.bColor

        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.rightMargin:ampEnvGraphView.anchors.rightMargin
        anchors.bottomMargin: ampEnvGraphView.anchors.bottomMargin - dialContainer.height +ampEnvGraphView.bW


        Dial { //Attack Implementation
            id: dialAttack

            width: 85
            height: 85
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 35
            x: dialContainer.width * 0/4 + width/2




            from: ampEnvGraphView.graphMinW
            to: ampEnvGraphView.graphMaxW * 1/4
            value: ampEnvGraphView.graphMaxW * 1/4

            property real commonValue;
            onValueChanged: {
                ampEnvGraphView.attack = Qt.point(dialAttack.value,ampEnvGraphView.graphMinH);
                controller.setAttackAmpEnvelopePoint(1, 1, dialAttack.value/1000);
            }

            Label {
                id: label
                text: qsTr("Attack")

                x: (parent.width - width) / 2
                y: (parent.height) + (height / 2)
            }
        }

        Dial { //Decay Implementation
            id: dialDecay

            width: 85
            height: 85
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 35
            x: dialContainer.width * 1/4 + width/2

            from: ampEnvGraphView.attack.x
            to: ampEnvGraphView.graphMaxW * 2/4
            value: ampEnvGraphView.graphMaxW * 2/4

            property real commonValue;
            onValueChanged: {
                ampEnvGraphView.decay = Qt.point(dialDecay.value,ampEnvGraphView.decay.y);
                controller.setAttackAmpEnvelopePoint(2, 1 - (dialSustain.value/300), ((dialDecay.value - 100)/500));

            }

            Label {
                id: label1
                text: qsTr("Decay")

                x: (parent.width - width) / 2
                y: (parent.height) + (height / 2)
            }

        }

        Dial {
            id: dialSustain

            width: 85
            height: 85
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 35
            x: dialContainer.width * 2/4 + width/2



            from:ampEnvGraphView.graphMaxH - ampEnvGraphView.bW*2
            to: ampEnvGraphView.graphMinH + ampEnvGraphView.bW
            value: (from - to) / 2

            property real commonValue;
            onValueChanged: {
                ampEnvGraphView.sustain = Qt.point(ampEnvGraphView.sustain.x,value);
                controller.setAttackAmpEnvelopePoint(2, 1 - (dialSustain.value/300), ((dialDecay.value - 100)/500))
                controller.setAttackAmpEnvelopePoint(3, 1 - (dialSustain.value/300), 5);
                controller.setReleaseAmpEnvelopePoint(0, 1 - (dialSustain.value/300), 0) //release changes when sustain changes

            }

            Label {
                id: label2
                text: qsTr("Sustain")

                x: (parent.width - width) / 2
                y: (parent.height) + (height / 2)
            }

        }

        Dial {
            id: dialRelease

            width: 85
            height: 85
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 35
            x: dialContainer.width * 3/4 + width/2
            property real ble: dialContainer.width;

            //         from: and to: cannot be used because it limits the full range the dials use to set the release?  //
            //from: ampEnvGraphView.graphMaxW * 3/4
            //to: ampEnvGraphView.graphMaxW * 4/4 - 2*ampEnvGraphView.bW
            value: ampEnvGraphView.graphMaxW * 3/4

            property real commonValue;

            onValueChanged: {

                ampEnvGraphView.release = Qt.point((0 - (ble * (dialRelease.value + 0.1) / 4) + ble), 0 - dialSustain.value); //
                //ampEnvGraphView.release = Qt.point(ble * dialRelease.value, dialSustain.value);

                controller.setReleaseAmpEnvelopePoint(0, 1 - (dialSustain.value/300), 0)
                controller.setReleaseAmpEnvelopePoint(1, 0, dialRelease.value);
            }

            Label {
                id: label3
                text: qsTr("Release")

                x: (parent.width - width) / 2
                y: (parent.height) + (height / 2)

            }

        }


    }





    Rectangle {
        id: textInputDialog
        x: 710
        y: 295
        width: 600
        height: 280
        color: "#323232"
        radius: 8
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
        visible: dialogController.isVisible

        Text {
            id: title
            y: 8
            width: 543
            height: 36
            color: "#ffffff"
            text: dialogController.title
            font.pixelSize: 26
            verticalAlignment: Text.AlignVCenter
            anchors.horizontalCenter: parent.horizontalCenter
            font.family: "Noto Sans"
        }


        Rectangle {
            id: textInputContainer
            y: 153
            width: 543
            height: 32
            color: "#212121"
            anchors.horizontalCenterOffset: 0
            anchors.horizontalCenter: parent.horizontalCenter

            TextInput {
                id: textInput
                x: 87
                y: 6
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.fill: parent
                color: "#ffffff"
                text: dialogController.value
                font.pixelSize: 16
                verticalAlignment: Text.AlignVCenter
                anchors.rightMargin: 10
                anchors.leftMargin: 10
                selectionColor: "#99144b"
                onTextChanged: dialogController.value = text
                onAccepted: dialogController.submit()
            }
        }

        Rectangle {
            id: actionButtonsContainer
            y: 222
            width: 302
            height: 39
            color: "#323232"
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 19
            anchors.horizontalCenter: parent.horizontalCenter

            Button {
                id: okButton
                width: 96
                text: dialogController.confirmButtonText
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 0
                anchors.topMargin: 0
                anchors.leftMargin: 2
                onPressed: dialogController.submit()
            }

            Button {
                id: cancelButton
                width: 96
                text: dialogController.cancelButtonText
                anchors.right: parent.right
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.rightMargin: 2
                anchors.bottomMargin: 0
                anchors.topMargin: 0
                onPressed: dialogController.cancel()
            }
        }

        Text {
            id: content
            y: 50
            width: 543
            height: 97
            color: "#ffffff"
            text: dialogController.text
            font.pixelSize: 12
            verticalAlignment: Text.AlignTop
            anchors.horizontalCenterOffset: 1
            font.family: "Noto Sans"
            anchors.horizontalCenter: parent.horizontalCenter
        }


    }
}
