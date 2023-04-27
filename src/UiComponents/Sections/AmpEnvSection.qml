import QtQuick
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import AmpEnvGraphView

Rectangle {
    color: "transparent"

    AmpEnvGraphItem {
        id: ampEnvGraphView
        anchors.fill: parent
    }
    Rectangle {
        id: dialContainer
        border.color: ampEnvGraphView.bColor
        border.width: ampEnvGraphView.bW
        color: "transparent"
        enabled: false
        height: 0
        width: 0
        visible: false

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
}
