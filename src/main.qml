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
import "UiComponents/Core"

Window {
    id: window

    property string borderColor : "gray"
    property int borderWidth: 2
    Material.accent: Material.Purple
    Material.theme: Material.Dark
    color: "#212121"
    height: 1080
    title: qsTr("VisualSynth")
    visible: true
    width: 1920

    Component.onCompleted: controller.runInitialSynthConnection()

    Timer {
        id: alertControllerUpdate
        interval: 10
        repeat: true
        running: true

        onTriggered: alertController.update()
    }
    Row {
        anchors.fill: parent
        spacing: 0

        // The left column
        Column {
            height: parent.height
            spacing: 0
            width: parent.width / 2

            ConnectionSection {
                id: connectionSection
                height: 80
                width: parent.width

                CustomBorder {
                    sides: ["top", "left", "bottom"]
                }
            }
            OperatorViewSection {
                id: operatorViewSection
                height: parent.height - 80
                width: parent.width

                CustomBorder {
                    sides: ["bottom", "left"]
                }
            }
        }

        // The right column
        Column {
            height: parent.height
            spacing: 0
            width: parent.width / 2

            OperatorInfoSection {
                id: operatorInfoSection
                height: 400
                width: parent.width

                CustomBorder {
                    sides: ["top", "right", "left"]
                }
            }
            GlobalLfoOptionsSection {
                id: globalLfoOptionsSection
                height: 100
                width: parent.width

                CustomBorder {
                    sides: ["top", "right", "left"]
                }
            }
            AmpEnvSection {
                id: ampEnvGraphViewSection
                height: parent.height * (8 / 9) - 500
                width: parent.width

                CustomBorder {
                    sides: ["top", "right", "left", "bottom"]
                }
            }
            OutputWaveSection {
                id: outputWaveSection
                height: parent.height / 9
                width: parent.width

                CustomBorder {
                    sides: ["right", "left"]
                }
            }
        }
    }

    // This must under everything (except alert) so that it's not possible to click on anything behind the dialog
    TextInputDialog {
        anchors.fill: parent
    }
    BottomAlertView {
    }
}
