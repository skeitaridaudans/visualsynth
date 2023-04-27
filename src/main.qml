import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQml 2.0
import OperatorView
import OperatorPresetsView
import AmpEnvGraphView
import OutputWaveView
import OperatorWaveView

import "UiComponents"
import "UiComponents/Sections"

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

    Timer {
        id: alertControllerUpdate
        interval: 10
        repeat: true
        running: true

        onTriggered: alertController.update()
    }

    Row {
        spacing: 0
        anchors.fill: parent

        // The left column
        Column {
            spacing: 0
            height: parent.height
            width: parent.width / 2

            OperatorViewSection {
                id: operatorViewSection
                height: parent.height - 140
                width: parent.width
            }

            ConnectionSection {
                id: connectionSection
                height: 140
                width: parent.width
            }
        }

        // The right column
        Column {
            spacing: 0
            height: parent.height
            width: parent.width / 2

            OperatorInfoSection {
                id: operatorInfoSection
                height: 377
                width: parent.width
            }

            GlobalLfoOptionsSection {
                id: globalLfoOptionsSection
                height: 100
                width: parent.width
            }

            AmpEnvSection {
                id: ampEnvGraphViewSection
                width: parent.width
                height: 316
            }
        }
    }

    //OutputWaveSection {
    //    id: outputWaveSection
    //    height: 185
    //    width: 1922
    //    x: 0
    //    y: 898
    //}

    // This must under everything (except alert) so that it's not possible to click on anything behind the dialog
    TextInputDialog {
        anchors.fill: parent
    }

    BottomAlertView {

    }
}
