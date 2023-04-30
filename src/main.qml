import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQml 2.0
import OperatorView
import OperatorPresetsView
import AmpEnvGraphView
import OutputWaveView
import PButton 1.0
import OperatorWaveView

Window {
    id: window

    property var selectedOperator: null

    Material.accent: Material.Purple
    Material.theme: Material.Dark
    color: "#212121"
    height: 1080
    title: qsTr("VisualSynth")
    visible: true
    width: 1920

    Component.onCompleted: {
        controller.setAmpEnvelopeSize(3); // controller.setReleaseAmpEnvelopeSize(3); ???
        controller.setAttackAmpEnvelopePoint(0, 0, 0);
        controller.setAttackAmpEnvelopePoint(3, 1 - (dialSustain.value / 300), 5);
        controller.setReleaseAmpEnvelopeSize(2);
        controller.setReleaseAmpEnvelopePoint(0, dialSustain.value, 0);
        controller.setReleaseAmpEnvelopePoint(1, 0, 1);
    }


    Connections {
        function onAmpChanged(amp) {
            ampText.text = amp + "";
            var color = selectedOperator.getColorForOperator();
            opWaveView.setColor(color);
            opDrag.color = color.alpha(0.5).darker(3);
        }
        function onFreqChanged(freq) {
            freqText.text = parseFloat(freq).toFixed(1) + "";
            var color = selectedOperator.getColorForOperator();
            opWaveView.setColor(color);
            opDrag.color = color.alpha(0.5).darker(3);
        }
        function onOperatorDeselected(deselected) {
            opContainer.enabled = false;
            opContainer.visible = false;
        }
        function onOperatorSelected(operator) {
            selectedOperator = operator;
            freqText.text = parseFloat(operator.freqProp).toFixed(1) + "";
            ampText.text = operator.ampProp + "";
            opContainer.enabled = true;
            opContainer.visible = true;
            var color = selectedOperator.getColorForOperator();
            opWaveView.setColor(color);
            opDrag.color = color.alpha(0.5).darker(3);
            lfoOperatorFreqAmountText.text = `${selectedOperator.frequencyLfoAmount}%`;
            lfoOperatorAmpAmountText.text = `${selectedOperator.amplitudeLfoAmount}%`;
        }
        
        function onShowPresetsChanged(show){
//            if(!show ){
                var a =  presetB.updateOpen; // Since this is updated in OperatorView.cpp we need to catch the signal.
//            }
        }

    }

    Material.theme: Material.Dark
    Material.accent: Material.Purple

        target: controller
    }
    Timer {
        id: alertControllerUpdate
        interval: 10
        repeat: true
        running: true

        onTriggered: alertController.update()
    }

    Rectangle {
        id: sinewaverectangle
        anchors.bottom: parent.bottom
        anchors.bottomMargin: -3
        border.color: "gray"
        border.width: 3
        color: "#212121"
        height: 185
        radius: 3
        width: 1922
        x: 0
        y: 898

        OutputWaveView {
            id: outputWaveView
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 10
            anchors.right: parent.right
            anchors.rightMargin: 20
            height: 100
            width: 800
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
            text: controller.isConnected ? qsTr("Connected!") : qsTr("not Connected!")
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            font.pixelSize: 18
            horizontalAlignment: Text.AlignLeft
            verticalAlignment: Text.AlignVCenter
            anchors.leftMargin: 127
            anchors.bottomMargin: 58
        }


    }

    ConnectionInfo {

    }

    Rectangle {
        id: operatorrectangle
        border.color: "gray"
        border.width: 3
        color: "#212121"
        height: 901
        radius: 3
        width: 1065
        x: 0
        y: 0

        OperatorView {
            id: boxes
            anchors.left: parent.left
            anchors.leftMargin: 50
            anchors.top: parent.top
            anchors.topMargin: 50
            height: 800
            width: 900
        }

        PresetButton {
            id:presetB
            width: 250
            height: 50

            anchors.top: parent.top
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.topMargin: 50;

            onClicked: {
//                presetB.updateOpen;
                controller.deselectOperator()
                controller.showPresets = !controller.showPresets;
            }
        }


        Rectangle {
            id: presetsContainer
            anchors.horizontalCenter: parent.horizontalCenter

            anchors.top: parent.top
            anchors.topMargin: presetB.height *2.5

            color: "#323232"
            height: 450
            radius: 8
            visible: controller.showPresets
            width: 450

            ScrollView {
                ScrollBar.horizontal.policy: ScrollBar.AlwaysOff
                ScrollBar.vertical.policy: ScrollBar.AlwaysOn
                anchors.fill: parent
                anchors.left: parent.left
                anchors.top: parent.top
                clip: true
                contentHeight: Math.max(presetsContainer.height, presetsView.calculatedContentHeight)
                contentWidth: presetsContainer.width
                wheelEnabled: false

                OperatorPresetsView {
                    id: presetsView
                    anchors.fill: parent
                    anchors.left: parent.left
                    anchors.top: parent.top
                    containerHeight: presetsContainer.height
                    containerWidth: presetsContainer.width
                    enabled: controller.showPresets
                }
            }
        }
    }
    
    Rectangle {
        // Operator info box
        id: operatorInfo

        property bool coarseCheck: true
        property bool fineCheck: false
        property int operatorId: selectedOperator ? selectedOperator.idProp : 0

        // Temporary border boundsof box
        border.color: "gray"
        border.width: 3
        color: "#212121"
        height: 377
        radius: 3
        width: 857
        x: 1065
        y: 0

        // Box title
        Rectangle {
            id: opContainer
            anchors.fill: parent
            border.color: "gray"
            border.width: 3
            color: parent.color
            enabled: selectedOperator ? true : false
            height: parent.height - 4
            radius: 3
            visible: selectedOperator ? true : false
            width: parent.width - 4

            Text {
                id: freqText
                
                anchors.horizontalCenter: opDrag.horizontalCenter
                anchors.left: parent.left
                anchors.leftMargin: 510
                anchors.right: parent.right
                anchors.rightMargin: 319
                color: "gray"
                font.family: "Noto Sans"
                font.pixelSize: 16
                height: 10
                horizontalAlignment: Text.AlignHCenter
                text: "0"
                verticalAlignment: Text.AlignVCenter
                width: 12
                x: 1064
                y: 342

                Text {
                    id: plusFreq
                    color: parent.color
                    font.family: "Noto Sans"
                    font.pixelSize: 16
                    height: 21
                    horizontalAlignment: Text.AlignHCenter
                    text: "+"
                    verticalAlignment: Text.AlignVCenter
                    width: 50
                    x: 82
                    y: -3

                    Rectangle {
                        id: freqPlusTouchpoint
                        color: "#ffffff"
                        height: 32
                        opacity: 0
                        width: 50
                        x: 0
                        y: 0

                        Timer {
                            id: freqIncTimer
                            interval: 500
                            repeat: true

                            onTriggered: {
                                selectedOperator.setFrequency(5);
                                controller.changeFrequency(selectedOperator.idProp, selectedOperator.freqProp);
                                interval = 100;
                            }
                        }
                        MultiPointTouchArea {
                            id: freqPlusButton
                            anchors.fill: parent

                            onPressed: {
                                plusFreq.color = "pink";
                                selectedOperator.setFrequency(1);
                                controller.changeFrequency(selectedOperator.idProp, selectedOperator.freqProp);
                                freqIncTimer.start();
                            }
                            onReleased: {
                                plusFreq.color = plusFreq.parent.color;
                                freqIncTimer.interval = 500;
                                freqIncTimer.stop();
                            }
                        }
                    }
                }
                Text {
                    id: minFreq
                    color: parent.color
                    font.family: "Noto Sans"
                    font.pixelSize: 16
                    height: 21
                    horizontalAlignment: Text.AlignHCenter
                    text: "-"
                    verticalAlignment: Text.AlignVCenter
                    width: 50
                    x: -94
                    y: -3

                    Timer {
                        id: freqDecTimer
                        interval: 500
                        repeat: true

                        onTriggered: {
                            selectedOperator.setFrequency(-5);
                            controller.changeFrequency(selectedOperator.idProp, selectedOperator.freqProp);
                            interval = 100;
                        }
                    }
                    Rectangle {
                        id: minFreqTouchpoint
                        color: "#ffffff"
                        height: 26
                        opacity: 0
                        width: 50
                        x: 0
                        y: 0

                        MultiPointTouchArea {
                            id: freqMinButton
                            anchors.fill: parent

                            onPressed: {
                                minFreq.color = "pink";
                                selectedOperator.setFrequency(-1);
                                controller.changeFrequency(selectedOperator.idProp, selectedOperator.freqProp);
                                freqDecTimer.start();
                            }
                            onReleased: {
                                minFreq.color = minFreq.parent.color;
                                freqDecTimer.interval = 500;
                                freqDecTimer.stop();
                            }
                        }
                    }
                }
            }
            Text {
                id: ampText
                color: "gray"
                font.family: "Noto Sans"
                font.pixelSize: 16
                horizontalAlignment: Text.AlignHCenter
                text: "0"
                verticalAlignment: Text.AlignVCenter
                x: 828
                y: 189

                Text {
                    id: ampPlus
                    color: parent.color
                    font.family: "Noto Sans"
                    font.pixelSize: 16
                    height: 22
                    horizontalAlignment: Text.AlignHCenter
                    text: qsTr("+")
                    verticalAlignment: Text.AlignVCenter
                    width: 9
                    x: 0
                    y: -60

                    Timer {
                        id: ampIncTimer
                        interval: 500
                        repeat: true

                        onTriggered: {
                            selectedOperator.setAmplitude(2);
                            controller.changeAmplitude(selectedOperator.idProp, selectedOperator.ampProp);
                            interval = 100;
                        }
                    }
                    Rectangle {
                        id: ampIncTouchpoint
                        color: "#ffffff"
                        height: 38
                        opacity: 0
                        width: 30
                        x: -5
                        y: -7

                        MultiPointTouchArea {
                            id: ampPlusButton
                            anchors.fill: parent

                            onPressed: {
                                ampPlus.color = "pink";
                                selectedOperator.setAmplitude(1);
                                controller.changeAmplitude(selectedOperator.idProp, selectedOperator.ampProp);
                                ampIncTimer.start();
                            }
                            onReleased: {
                                ampPlus.color = ampPlus.parent.color;
                                ampIncTimer.interval = 500;
                                ampIncTimer.stop();
                            }
                        }
                    }
                }
                Text {
                    id: ampMin
                    color: parent.color
                    font.family: "Noto Sans"
                    font.pixelSize: 16
                    horizontalAlignment: Text.AlignHCenter
                    text: qsTr("-")
                    verticalAlignment: Text.AlignVCenter
                    x: 2
                    y: 72

                    Timer {
                        id: ampDecTimer
                        interval: 500
                        repeat: true

                        onTriggered: {
                            selectedOperator.setAmplitude(-2);
                            controller.changeAmplitude(selectedOperator.idProp, selectedOperator.ampProp);
                            interval = 100;
                        }
                    }
                    Rectangle {
                        id: ampMinTouchPoint
                        color: "#ffffff"
                        height: 28
                        opacity: 0
                        width: 30
                        x: -8
                        y: 0

                        MultiPointTouchArea {
                            id: ampMinButton
                            anchors.fill: parent

                            onPressed: {
                                ampMin.color = "pink";
                                selectedOperator.setAmplitude(-1);
                                controller.changeAmplitude(selectedOperator.idProp, selectedOperator.ampProp);
                                ampDecTimer.start();
                            }
                            onReleased: {
                                ampMin.color = ampMin.parent.color;
                                ampDecTimer.interval = 500;
                                ampDecTimer.stop();
                            }
                        }
                    }
                }
            }
            Text {
                id: operatorName
                color: "#f0f0f0"
                font.family: "Noto Sans"
                font.pixelSize: 32
                font.weight: Font.Bold
                text: qsTr("Operator " + (selectedOperator ? selectedOperator.idProp + 1 : ""))
                x: 32
                y: 16
            }
            // Operator box
            Rectangle {
                id: opDrag
                color: "lightgray" // Placeholder while colors are off on the wave
                height: 273
                width: 598
                x: 224
                y: 63

                //color: "#414141"
                Rectangle {
                    id: opsinewaverectangle
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 1
                    color: parent.color
                    height: parent.height - 2
                    radius: 3
                    width: parent.width - 4
                    x: 2
                    y: 1

                    OperatorWaveView {
                        id: opWaveView
                        anchors.bottom: parent.bottom
                        anchors.right: parent.right
                        anchors.rightMargin: 2
                        height: parent.height - 3
                        //    		        anchors.rightMargin: 20
                        //                            anchors.bottomMargin: 10
                        width: parent.width - 3
                    }
                }

                // Make it respond to geastures
                MultiPointTouchArea {
                    property var drag: parent
                    property bool horiDrag: false
                    property real lastX: 0
                    property real lastY: 0
                    property var offset: null
                    property bool vertiDrag: false

                    anchors.bottomMargin: 0
                    anchors.fill: parent
                    anchors.leftMargin: 0
                    anchors.rightMargin: 8
                    anchors.topMargin: 0

                    touchPoints: [
                        TouchPoint {
                            id: touch1
                        }
                    ]

                    onPressed: {
                        var point = touchPoints[0];
                        opDrag.border.color = "pink";
                        opDrag.border.width = 3;
                        opDrag.width = parent.width + 1;
                        opDrag.height = parent.height + 1;
                        offset = Qt.point(point.x, point.y);
                        point.startSceneX = touchPoint.sceneX;
                        point.startSceneY = touchPoint.sceneY;
                        lastX = point.sceneX;
                        lastY = point.sceneY;
                    }
                    onReleased: {
                        opDrag.border.width = 0;
                        opDrag.width = parent.width - 1;
                        opDrag.height = parent.height - 1;
                        horiDrag = false;
                        vertiDrag = false;
                    }
                    onTouchUpdated: {
                        if (selectedOperator) {
                            // TODO add multipliers !
                            var freq = selectedOperator.getFreq();
                            var amp = selectedOperator.getAmp();
                            if (touchPoints.length > 0) {
                                var touchPoint = touchPoints[0];
                                var xDelta = touchPoint.sceneX - lastX;
                                var yDelta = touchPoint.sceneY - lastY;
                                lastX = touchPoint.sceneX;
                                lastY = touchPoint.sceneY;
                                if (!horiDrag && !vertiDrag) {
                                    if (Math.abs(xDelta) > Math.abs(yDelta)) {
                                        horiDrag = true;
                                    } else if (Math.abs(yDelta) > Math.abs(xDelta)) {
                                        vertiDrag = true;
                                    }
                                } else if (horiDrag && Math.abs(yDelta) > Math.abs(xDelta)) {
                                    vertiDrag = true;
                                    horiDrag = false;
                                } else if (vertiDrag && Math.abs(xDelta) > Math.abs(yDelta)) {
                                    horiDrag = true;
                                    vertiDrag = false;
                                }
                                if (horiDrag) {
                                    if (xDelta > 0) {
                                        if (operatorInfo.fineCheck) {
                                            if (Math.abs(xDelta) > 10) {
                                                selectedOperator.setFrequency(5);
                                            } else {
                                                selectedOperator.setFrequency(0.1);
                                            }
                                        } else {
                                            selectedOperator.setFrequency(10);
                                        }
                                        controller.changeFrequency(selectedOperator.idProp, selectedOperator.freqProp);
                                    } else if (xDelta < 0) {
                                        if (operatorInfo.fineCheck) {
                                            if (xDelta < -10) {
                                                selectedOperator.setFrequency(-5);
                                            } else {
                                                selectedOperator.setFrequency(-0.1);
                                            }
                                        } else {
                                            selectedOperator.setFrequency(-10);
                                        }
                                        controller.changeFrequency(selectedOperator.idProp, selectedOperator.freqProp);
                                    }
                                } else if (vertiDrag) {
                                    if (yDelta < 0) {
                                        selectedOperator.setAmplitude(1);
                                        controller.changeAmplitude(selectedOperator.idProp, selectedOperator.ampProp);
                                    } else if (yDelta > 0) {
                                        selectedOperator.setAmplitude(-1);
                                        controller.changeAmplitude(selectedOperator.idProp, selectedOperator.ampProp);
                                    }
                                }
                            }
                        }
                    }
                }
            }
            Text {
                id: freqControlTitle
                anchors.left: parent.left
                anchors.leftMargin: 32
                anchors.top: operatorName.bottom
                anchors.topMargin: 16
                color: "#9E9E9E"
                font.family: "Noto Sans"
                font.pointSize: 14
                font.weight: Font.DemiBold
                text: "Frequency Control"
            }
            Rectangle {
                id: freqControlBox
                anchors.left: freqControlTitle.left
                anchors.leftMargin: 8
                anchors.top: freqControlTitle.bottom
                anchors.topMargin: 16
                color: "transparent"
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
                anchors.left: parent.left
                anchors.leftMargin: 32
                anchors.top: freqControlBox.bottom
                anchors.topMargin: 16
                color: "#9E9E9E"
                font.family: "Noto Sans"
                font.pointSize: 14
                font.weight: Font.DemiBold
                text: "LFO"
            }
            Rectangle {
                id: lfoOperatorOptionsBox
                anchors.left: lfoOperatorOptionsTitle.left
                anchors.leftMargin: 8
                anchors.top: lfoOperatorOptionsTitle.bottom
                anchors.topMargin: 8
                color: "transparent"
                height: lfoOperatorOptionsColumn.height

                Column {
                    id: lfoOperatorOptionsColumn
                    spacing: 4

                    Column {
                        spacing: 2

                        Text {
                            id: frequencyAmount
                            color: "#757575"
                            font.family: "Noto Sans"
                            font.pointSize: 12
                            text: "Frequency"
                        }
                        Row {
                            spacing: 8

                            Text {
                                id: lfoOperatorFreqAmountText
                                anchors.topMargin: 8
                                color: "#616161"
                                height: 40
                                text: `${selectedOperator ? selectedOperator.frequencyLfoAmount : 0}%`
                                verticalAlignment: Text.AlignVCenter
                                width: 32
                            }
                            Slider {
                                id: lfoOperatorFreqAmountSlider
                                from: 0
                                height: 40
                                to: 100
                                value: selectedOperator ? selectedOperator.frequencyLfoAmount : 0
                                width: 96

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
                            font.family: "Noto Sans"
                            font.pointSize: 12
                            text: "Amplitude"
                        }
                        Row {
                            spacing: 8

                            Text {
                                id: lfoOperatorAmpAmountText
                                anchors.topMargin: 8
                                color: "#616161"
                                height: 40
                                text: `${selectedOperator ? selectedOperator.amplitudeLfoAmount : 0}%`
                                verticalAlignment: Text.AlignVCenter
                                width: 32
                            }
                            Slider {
                                id: lfoOperatorAmpAmountSlider
                                from: 0
                                height: 40
                                to: 100
                                value: selectedOperator ? selectedOperator.amplitudeLfoAmount : 0
                                width: 96

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
        id: lfoOptions
        anchors.right: parent.right
        anchors.rightMargin: 0
        anchors.top: operatorInfo.bottom
        anchors.topMargin: 0
        color: "transparent"
        height: 100
        width: 857

        Text {
            id: lfoOptionsTitle
            anchors.left: parent.left
            anchors.leftMargin: 24
            anchors.top: parent.top
            anchors.topMargin: 8
            color: "#ffffff"
            font.family: "Noto Sans"
            font.pointSize: 18
            font.weight: Font.Bold
            text: "LFO"
            verticalAlignment: Text.AlignTop
        }
        ToggleButton {
            id: lfoEnabled
            anchors.left: lfoOptionsTitle.left
            anchors.leftMargin: 0
            anchors.top: lfoOptionsTitle.bottom
            anchors.topMargin: 8
            enabled: controller.isLfoEnabled
            text: qsTr("Enabled")

            onPressed: {
                controller.setLfoEnabled(!controller.isLfoEnabled);
            }
        }
        Text {
            id: lfoRateTitle
            anchors.left: lfoOptionsTitle.right
            anchors.leftMargin: 96
            anchors.top: parent.top
            anchors.topMargin: 8
            color: "#9E9E9E"
            font.family: "Noto Sans"
            font.pointSize: 14
            font.weight: Font.DemiBold
            height: lfoOptionsTitle.height
            horizontalAlignment: Text.AlignLeft
            text: "Rate"
            verticalAlignment: Text.AlignBottom
        }
        Text {
            id: lfoRateValue
            anchors.left: lfoRateTitle.left
            anchors.leftMargin: 8
            anchors.top: lfoRateTitle.bottom
            anchors.topMargin: 8
            color: "#757575"
            height: 40
            horizontalAlignment: Text.AlignLeft
            text: `${controller.lfoFrequency / 10} Hz`
            verticalAlignment: Text.AlignVCenter
            width: 32
        }
        Slider {
            id: lfoSlider
            anchors.left: lfoRateValue.right
            anchors.leftMargin: 16
            anchors.top: lfoRateTitle.bottom
            anchors.topMargin: 8
            from: 1
            height: 40
            stepSize: 1
            to: 200
            value: controller.lfoFrequency
            width: 400

            onMoved: {
                controller.setLfoFrequency(lfoSlider.value);
            }
        }
    }
    AmpEnvGraphItem {
        id: ampEnvGraphView
        anchors.bottom: parent.bottom
        anchors.right: parent.right
        anchors.top: lfoOptions.bottom
        anchors.topMargin: 0
        height: ampEnvGraphView.H
        width: ampEnvGraphView.W
    }
    Rectangle {
        id: dialContainer
        anchors.bottom: parent.bottom
        anchors.bottomMargin: ampEnvGraphView.anchors.bottomMargin - dialContainer.height + ampEnvGraphView.bW
        anchors.right: parent.right
        anchors.rightMargin: ampEnvGraphView.anchors.rightMargin
        border.color: ampEnvGraphView.bColor
        border.width: ampEnvGraphView.bW
        color: "transparent"
        enabled: false
        height: 140
        visible: false
        width: ampEnvGraphView.width

        Dial {
            //Attack Implementation
            id: dialAttack

            property real commonValue

            anchors.bottom: parent.bottom
            anchors.bottomMargin: 35
            from: ampEnvGraphView.graphMinW
            height: 85
            to: ampEnvGraphView.graphMaxW * 1 / 4
            value: ampEnvGraphView.graphMaxW * 1 / 4
            width: 85
            x: dialContainer.width * 0 / 4 + width / 2

            onValueChanged: {
                ampEnvGraphView.attack = Qt.point(dialAttack.value, ampEnvGraphView.graphMinH);
                controller.setAttackAmpEnvelopePoint(1, 1, dialAttack.value / 1000);
            }

            Label {
                id: label
                text: qsTr("Attack")
                x: (parent.width - width) / 2
                y: (parent.height) + (height / 2)
            }
        }
        Dial {
            //Decay Implementation
            id: dialDecay

            property real commonValue

            anchors.bottom: parent.bottom
            anchors.bottomMargin: 35
            from: ampEnvGraphView.attack.x
            height: 85
            to: ampEnvGraphView.graphMaxW * 2 / 4
            value: ampEnvGraphView.graphMaxW * 2 / 4
            width: 85
            x: dialContainer.width * 1 / 4 + width / 2

            onValueChanged: {
                ampEnvGraphView.decay = Qt.point(dialDecay.value, ampEnvGraphView.decay.y);
                controller.setAttackAmpEnvelopePoint(2, 1 - (dialSustain.value / 300), ((dialDecay.value - 100) / 500));
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

            property real commonValue

            anchors.bottom: parent.bottom
            anchors.bottomMargin: 35
            from: ampEnvGraphView.graphMaxH - ampEnvGraphView.bW * 2
            height: 85
            to: ampEnvGraphView.graphMinH + ampEnvGraphView.bW
            value: (from - to) / 2
            width: 85
            x: dialContainer.width * 2 / 4 + width / 2

            onValueChanged: {
                ampEnvGraphView.sustain = Qt.point(ampEnvGraphView.sustain.x, value);
                controller.setAttackAmpEnvelopePoint(2, 1 - (dialSustain.value / 300), ((dialDecay.value - 100) / 500));
                controller.setAttackAmpEnvelopePoint(3, 1 - (dialSustain.value / 300), 5);
                controller.setReleaseAmpEnvelopePoint(0, 1 - (dialSustain.value / 300), 0); //release changes when sustain changes
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

            property real ble: dialContainer.width
            property real commonValue

            anchors.bottom: parent.bottom
            anchors.bottomMargin: 35
            height: 85

            //         from: and to: cannot be used because it limits the full range the dials use to set the release?  //
            //from: ampEnvGraphView.graphMaxW * 3/4
            //to: ampEnvGraphView.graphMaxW * 4/4 - 2*ampEnvGraphView.bW
            value: ampEnvGraphView.graphMaxW * 3 / 4
            width: 85
            x: dialContainer.width * 3 / 4 + width / 2

            onValueChanged: {
                ampEnvGraphView.release = Qt.point((0 - (ble * (dialRelease.value + 0.1) / 4) + ble), 0 - dialSustain.value);
                //ampEnvGraphView.release = Qt.point(ble * dialRelease.value, dialSustain.value);
                controller.setReleaseAmpEnvelopePoint(0, 1 - (dialSustain.value / 300), 0);
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

    // This must under everything (except alert) so that it's not possible to click on anything behind the dialog
    TextInputDialog {
    }

    Rectangle {
        id: rectangle1
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 62
        anchors.horizontalCenter: parent.horizontalCenter
        color: alertController.alertColor
        height: 55
        radius: 10
        state: alertController.alertVisibleState
        width: 510
        y: 965

        states: [
            State {
                name: "invisible"

                PropertyChanges {
                    anchors.bottomMargin: -53
                    opacity: 0
                    target: rectangle1
                }
            },
            State {
                name: "visible"

                PropertyChanges {
                    anchors.bottomMargin: 62
                    opacity: 1
                    target: rectangle1
                }
            }
        ]
        transitions: [
            Transition {
                to: "invisible"

                NumberAnimation {
                    duration: 400
                    easing.type: Easing.InOutQuad
                    loops: 1
                    properties: "anchors.bottomMargin,opacity"
                }
            },
            Transition {
                to: "visible"

                NumberAnimation {
                    duration: 400
                    easing.type: Easing.InOutQuad
                    loops: 1
                    properties: "anchors.bottomMargin,opacity"
                }
            }
        ]

        Text {
            id: text1
            anchors.fill: parent
            color: "#ffffff"
            font.pixelSize: 22
            horizontalAlignment: Text.AlignHCenter
            text: alertController.alertText
            verticalAlignment: Text.AlignVCenter
        }
    }

}
