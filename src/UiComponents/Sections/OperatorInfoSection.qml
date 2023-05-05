import QtQuick
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import OperatorWaveView
import "../Core"

Rectangle {
    // Operator info box
    id: operatorInfo

    property bool isOperatorSelected: false
    property var selectedOperator: null
    property bool coarseCheck: true
    property bool fineCheck: false
    property int operatorId: selectedOperator ? selectedOperator.idProp : 0

    // Properties for coarse tuning
    property real base_frequency: 100.0
    property real semitones: 42.5
    property real minSemiTones: -42.5
    property real newFreq: 0

    color: "#212121"
    radius: 3

    Connections {
        function onAmpChanged(amp) {
            ampText.text = amp + ""
            var color = selectedOperator.getColorForOperator()
            opWaveView.setColor(color)
            opDrag.color = color.alpha(0.5).darker(3)
        }
        function onFreqChanged(freq) {
            freqText.text = parseFloat(freq).toFixed(1) + ""
            var color = selectedOperator.getColorForOperator()
            opWaveView.setColor(color)
            opDrag.color = color.alpha(0.5).darker(3)
        }
        function onOperatorDeselected(deselected) {
            isOperatorSelected = false;
        }
        function onOperatorSelected(operator) {
            if (!operator)
                return;
            selectedOperator = operator;
            isOperatorSelected = true;
            freqText.text = parseFloat(operator.freqProp).toFixed(1) + "";
            ampText.text = operator.ampProp + "";
            var color = selectedOperator.getColorForOperator();
            opWaveView.setColor(color);
            opDrag.color = color.alpha(0.5).darker(3);
            lfoOperatorFreqAmountText.text = `${parseFloat(selectedOperator.frequencyLfoAmount * 100).toFixed(0)}%`;
            lfoOperatorAmpAmountText.text = `${parseFloat(selectedOperator.amplitudeLfoAmount * 100).toFixed(0)}%`;
        }

        target: controller
    }
    function updateSemitone(){
        //         // ln(x/100)/ln(2) * semitones = currentSemitone
        // Math.log(x/100)/Math.log(2) * 42
        // This function is used to update the current semitone of the operator
        // whenever fine tuning is happening.
        var check = Math.log(selectedOperator.getFreq()/100)/Math.log(2) * semitones
        if (check > semitones) {
            selectedOperator.setSemiTone(semitones)
        } else if (check < minSemiTones) {
            selectedOperator.setSemiTone(minSemiTones)
        }
        else {
            selectedOperator.setSemiTone(check)
        }
    }

    function coarseTune() {
        // This coarsely tunes the operator by semitones.
        var new_frequency = base_frequency * Math.pow(
                    2, (selectedOperator.getSemiTone() / semitones))
        console.log(new_frequency)
        if (new_frequency > 200) {
            return 200.0
        }
        else if (new_frequency > 0 && new_frequency < 1) {
            return 1
        }
        return new_frequency
    }

    // Box title
    Rectangle {
        id: opContainer
        anchors.fill: parent
        color: parent.color
        enabled: isOperatorSelected || opContainerVisibleTransition.running
        height: parent.height - 4
        radius: 3
        visible: isOperatorSelected || opContainerVisibleTransition.running
        width: parent.width - 4
        state: isOperatorSelected ? "visible" : "invisible"

        states: [
            State {
                name: "invisible"
                PropertyChanges {
                    target: opContainer
                    opacity: 0
                }
            },
            State {
                name: "visible"
                PropertyChanges {
                    target: opContainer
                    opacity: 1
                }
            }
        ]

        transitions: [
            Transition {
                id: opContainerVisibleTransition
                to: "invisible,visible"
                NumberAnimation {
                    properties: "opacity"
                    easing.type: Easing.OutQuad
                    duration: 150
                }
            }
        ]

        Rectangle {
            id: leftColumn
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
                            operatorInfo.coarseCheck = false
                            operatorInfo.fineCheck = true
                        }
                    }
                    ToggleButton {
                        id: coarse
                        enabled: operatorInfo.coarseCheck
                        text: qsTr("Coarse")

                        onPressed: {
                            operatorInfo.coarseCheck = true
                            operatorInfo.fineCheck = false
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
                                text: `${selectedOperator ? parseFloat(selectedOperator.frequencyLfoAmount * 100).toFixed(0) : 0}%`
                                verticalAlignment: Text.AlignVCenter
                                width: 32
                            }
                            Slider {
                                id: lfoOperatorFreqAmountSlider
                                from: 0
                                height: 40
                                to: Math.log2(100)
                                value: selectedOperator ? Math.log2(selectedOperator.frequencyLfoAmount) : 0
                                width: 96

                                onMoved: {
                                    const value = Math.pow(2, lfoOperatorFreqAmountSlider.value) / 100;
                                    // Make sure it's actually possible to set the value to zero
                                    controller.setOperatorLfoFrequency(selectedOperator.idProp, value <= 0.01 ? 0 : value);

                                    // It would make more sense to add a signal for when frequencyLfoAmount changes, so the text would update automatically,
                                    // however, for some reason the Operator is created on another thread (event though it's just being created inside paint() of operatorView)
                                    // which will cause qt to crash if you have a signal for when the properties in Operator change
                                    lfoOperatorFreqAmountText.text = `${parseFloat(selectedOperator.frequencyLfoAmount * 100).toFixed(0)}%`;
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
                                text: `${selectedOperator ? parseFloat(selectedOperator.amplitudeLfoAmount * 100).toFixed(0) : 0}%`
                                verticalAlignment: Text.AlignVCenter
                                width: 32
                            }
                            Slider {
                                id: lfoOperatorAmpAmountSlider
                                from: 0
                                height: 40
                                to: Math.log2(100)
                                value: selectedOperator ? Math.log2(selectedOperator.amplitudeLfoAmount) : 0
                                width: 96

                                onMoved: {
                                    const value = Math.pow(2, lfoOperatorAmpAmountSlider.value) / 100;
                                    // Make sure it's actually possible to set the value to zero
                                    controller.setOperatorLfoAmplitude(selectedOperator.idProp, value <= 0.01 ? 0 : value);

                                    lfoOperatorAmpAmountText.text = `${parseFloat(selectedOperator.amplitudeLfoAmount * 100).toFixed(0)}%`;
                                }
                            }
                        }
                    }
                }
            }
        }

        Text {
            id: freqText
            anchors.horizontalCenter: opDrag.horizontalCenter
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
                anchors.left: parent.horizontalCenter
                anchors.leftMargin: 50
                color: parent.color
                font.family: "Noto Sans"
                font.pixelSize: 16
                height: 21
                horizontalAlignment: Text.AlignHCenter
                text: "+"
                verticalAlignment: Text.AlignVCenter
                width: 50
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
                            if (coarseCheck) {
                                let current = selectedOperator.getSemiTone()
                                if ((current + 1) > semitones){
                                   selectedOperator.setSemiTone(semitones)
                                } else {
                                    selectedOperator.setSemiTone(current + 1)
                                }
                                newFreq = operatorInfo.coarseTune()
                                selectedOperator.updateFrequency(newFreq)
                            } else {
                                selectedOperator.setFrequency(5)
                                updateSemitone()

                            }
                            controller.changeFrequency(
                                        selectedOperator.idProp,
                                        selectedOperator.freqProp)
                            interval = 100
                        }
                    }
                    MultiPointTouchArea {
                        id: freqPlusButton
                        anchors.fill: parent

                        onPressed: {
                            plusFreq.color = "pink"
                            if (coarseCheck) {          
                                let current = selectedOperator.getSemiTone()
                                if ((current + 1) > semitones){
                                   selectedOperator.setSemiTone(semitones)
                                } else {
                                    selectedOperator.setSemiTone(current + 1)
                                }
                                newFreq = operatorInfo.coarseTune()
                                selectedOperator.updateFrequency(newFreq)
                            } else {
                                selectedOperator.setFrequency(1)
                                updateSemitone()

                            }
                            controller.changeFrequency(
                                        selectedOperator.idProp,
                                        selectedOperator.freqProp)
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
            Text {
                id: minFreq
                anchors.right: parent.horizontalCenter
                anchors.rightMargin: 50
                color: parent.color
                font.family: "Noto Sans"
                font.pixelSize: 16
                height: 21
                horizontalAlignment: Text.AlignHCenter
                text: "-"
                verticalAlignment: Text.AlignVCenter
                width: 50
                y: -3

                Timer {
                    id: freqDecTimer
                    interval: 500
                    repeat: true

                    onTriggered: {
                        if (coarseCheck) {
                            let current = selectedOperator.getSemiTone()
                            if ((current - 1) < minSemiTones){
                               selectedOperator.setSemiTone(minSemiTones)
                            } else {
                                selectedOperator.setSemiTone(current - 1)
                            }
                            newFreq = operatorInfo.coarseTune()
                            selectedOperator.updateFrequency(newFreq)
                        } else {
                            selectedOperator.setFrequency(-5)
                            updateSemitone()
                        }
                        controller.changeFrequency(selectedOperator.idProp,
                                                   selectedOperator.freqProp)

                        interval = 100
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
                            minFreq.color = "pink"
                            if (coarseCheck) {
                                let current = selectedOperator.getSemiTone()
                                if ((current - 1) < minSemiTones){
                                   selectedOperator.setSemiTone(minSemiTones)
                                } else {
                                    selectedOperator.setSemiTone(current - 1)
                                }
                                newFreq = operatorInfo.coarseTune()
                                selectedOperator.updateFrequency(newFreq)
                            } else {
                                selectedOperator.setFrequency(-1)
                                updateSemitone()

                            }
                            controller.changeFrequency(
                                        selectedOperator.idProp,
                                        selectedOperator.freqProp)
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
            anchors.left: opDrag.right
            anchors.leftMargin: 8
            color: "gray"
            font.family: "Noto Sans"
            font.pixelSize: 16
            horizontalAlignment: Text.AlignHCenter
            text: "0"
            verticalAlignment: Text.AlignVCenter
            y: 189

            Text {
                id: ampPlus
                anchors.bottom: parent.verticalCenter
                anchors.bottomMargin: 50
                color: parent.color
                font.family: "Noto Sans"
                font.pixelSize: 16
                height: 22
                horizontalAlignment: Text.AlignHCenter
                text: qsTr("+")
                verticalAlignment: Text.AlignVCenter
                width: 9
                x: 0

                Timer {
                    id: ampIncTimer
                    interval: 500
                    repeat: true

                    onTriggered: {
                        selectedOperator.setAmplitude(2)
                        controller.changeAmplitude(selectedOperator.idProp,
                                                   selectedOperator.ampProp)
                        interval = 100
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
                            ampPlus.color = "pink"
                            selectedOperator.setAmplitude(1)
                            controller.changeAmplitude(selectedOperator.idProp,
                                                       selectedOperator.ampProp)
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
                anchors.top: parent.verticalCenter
                anchors.topMargin: 50
                color: parent.color
                font.family: "Noto Sans"
                font.pixelSize: 16
                horizontalAlignment: Text.AlignHCenter
                text: qsTr("-")
                verticalAlignment: Text.AlignVCenter
                x: 2

                Timer {
                    id: ampDecTimer
                    interval: 500
                    repeat: true

                    onTriggered: {
                        selectedOperator.setAmplitude(-2)
                        controller.changeAmplitude(selectedOperator.idProp,
                                                   selectedOperator.ampProp)
                        interval = 100
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
                            ampMin.color = "pink"
                            selectedOperator.setAmplitude(-1)
                            controller.changeAmplitude(selectedOperator.idProp,
                                                       selectedOperator.ampProp)
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
        // Operator box
        Rectangle {
            id: opDrag
            color: "lightgray" // Placeholder while colors are off on the wave
            height: 273
            width: parent.width - leftColumn.width - 350
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
                    var point = touchPoints[0]
                    opDrag.border.color = "pink"
                    opDrag.border.width = 3
                    opDrag.width = parent.width + 1
                    opDrag.height = parent.height + 1
                    offset = Qt.point(point.x, point.y)
                    lastX = point.sceneX
                    lastY = point.sceneY
                }
                onReleased: {
                    opDrag.border.width = 0
                    opDrag.width = parent.width - 1
                    opDrag.height = parent.height - 1
                    horiDrag = false
                    vertiDrag = false
                }
                onTouchUpdated: {
                    if (selectedOperator) {
                        // TODO add multipliers !
                        var freq = selectedOperator.getFreq()
                        var amp = selectedOperator.getAmp()
                        if (touchPoints.length > 0) {
                            var touchPoint = touchPoints[0]
                            var xDelta = touchPoint.sceneX - lastX
                            var yDelta = touchPoint.sceneY - lastY
                            lastX = touchPoint.sceneX
                            lastY = touchPoint.sceneY
                            if (!horiDrag && !vertiDrag) {
                                if (Math.abs(xDelta) > Math.abs(yDelta)) {
                                    horiDrag = true
                                } else if (Math.abs(yDelta) > Math.abs(
                                               xDelta)) {
                                    vertiDrag = true
                                }
                            } else if (horiDrag && Math.abs(yDelta) > Math.abs(
                                           xDelta)) {
                                vertiDrag = true
                                horiDrag = false
                            } else if (vertiDrag && Math.abs(xDelta) > Math.abs(
                                           yDelta)) {
                                horiDrag = true
                                vertiDrag = false
                            }
                            if (horiDrag) {
                                if (xDelta > 0) {
                                    if (operatorInfo.fineCheck) {
                                        if (Math.abs(xDelta) > 10) {
                                            selectedOperator.setFrequency(5)
                                        } else {
                                            selectedOperator.setFrequency(0.1)
                                        }
                                        updateSemitone()
                                    } else {
                                        // Coarse tuning happens here
                                        if (xDelta > 2) {
                                            let current = selectedOperator.getSemiTone()
                                            if ((current + 1) > semitones){
                                               selectedOperator.setSemiTone(semitones)
                                            } else {
                                                selectedOperator.setSemiTone(current + 1)
                                            }
                                            newFreq = coarseTune()
                                            selectedOperator.updateFrequency(newFreq)
                                        }
                                    }
                                    controller.changeFrequency(
                                                selectedOperator.idProp,
                                                selectedOperator.freqProp)
                                } else if (xDelta < 0) {
                                    if (operatorInfo.fineCheck) {
                                        if (xDelta < -10) {
                                            selectedOperator.setFrequency(-5)
                                        } else {
                                            selectedOperator.setFrequency(-0.1)
                                        }
                                        updateSemitone()
                                    } else {
                                        // Coarse tuning happens here
                                        if (xDelta < -2) {
                                            let current = selectedOperator.getSemiTone()
                                            if ((current - 1) < minSemiTones){
                                               selectedOperator.setSemiTone(minSemiTones)
                                            } else {
                                                selectedOperator.setSemiTone(current - 1)
                                            }
                                            newFreq = coarseTune()
                                            selectedOperator.updateFrequency(newFreq)
                                        }
                                    }
                                    controller.changeFrequency(
                                                selectedOperator.idProp,
                                                selectedOperator.freqProp)
                                }
                            } else if (vertiDrag) {
                                // TODO: Make add multiplier?
                                if (yDelta < 0) {
                                    if (yDelta < -2) {
                                        selecteOperator.setAmplitude(5)
                                    } else {
                                        selectedOperator.setAmplitude(1)
                                    }
                                    controller.changeAmplitude(
                                                selectedOperator.idProp,
                                                selectedOperator.ampProp)
                                } else if (yDelta > 0) {
                                    if (yDelta > 2) {
                                        selectedOperator.setAmplitude(-5)
                                    } else {
                                        selectedOperator.setAmplitude(-1)
                                    }
                                    controller.changeAmplitude(
                                                selectedOperator.idProp,
                                                selectedOperator.ampProp)
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
