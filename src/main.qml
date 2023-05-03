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
    property string borderColor : "gray"
    property int borderWidth: 2
    Material.accent: Material.Purple
    Material.theme: Material.Dark
    color: "#212121"
    height: 1080
    title: qsTr("VisualSynth")
    visible: true
    width: 1920

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

                Item {
                    width: parent.width
                    height: parent.height
                    Rectangle {
                        width: parent.width
                        height: borderWidth
                        color: borderColor
                        anchors.top: parent.top
                    }
                    Rectangle {
                        height: parent.height
                        width: borderWidth
                        color: borderColor
                        anchors.left: parent.left
                    }
                    Rectangle {
                        width: parent.width
                        height: borderWidth
                        color: borderColor
                        anchors.bottom: parent.bottom
                    }
                }

            }
            OperatorViewSection {
                id: operatorViewSection
                height: parent.height - 80
                width: parent.width

                Item {
                    width: parent.width
                    height: parent.height
                    Rectangle {
                        width: parent.width
                        height: borderWidth
                        color: borderColor
                        anchors.bottom: parent.bottom
                    }
                    Rectangle {
                        height: parent.height
                        width: borderWidth
                        color: borderColor
                        anchors.left: parent.left
                    }
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

                Item {
                    width: parent.width
                    height: parent.height

                    Rectangle {
                        width: parent.width
                        height: borderWidth
                        color: borderColor
                        anchors.top: parent.top
                    }

                    Rectangle {
                        height: parent.height
                        width: borderWidth
                        color: borderColor
                        anchors.right: parent.right
                    }

                    Rectangle {
                        height: parent.height
                        width: borderWidth
                        color: borderColor
                        anchors.left: parent.left
                    }

                    Rectangle {
                        width: parent.width
                        height: borderWidth
                        color: borderColor
                        anchors.bottom: parent.bottom
                    }
                }
            }
            GlobalLfoOptionsSection {
                id: globalLfoOptionsSection
                height: 100
                width: parent.width

                Item {
                    width: parent.width
                    height: parent.height
                    Rectangle {
                        width: borderWidth
                        height: parent.height
                        color: borderColor
                        anchors.left: parent.left
                    }
                    Rectangle {
                        width: borderWidth
                        height: parent.height
                        color: borderColor
                        anchors.right: parent.right
                    }

                }
            }
            AmpEnvSection {
                id: ampEnvGraphViewSection
                height: parent.height * (6 / 7) - 500
                width: parent.width
            }
            OutputWaveSection {
                id: outputWaveSection
                height: parent.height / 7
                width: parent.width

                Item{
                    width: parent.width
                    height: parent.height

                    Rectangle {
                        width: borderWidth
                        height: parent.height
                        color: borderColor
                        anchors.right: parent.right
                    }
                    Rectangle {
                        width: borderWidth
                        height: parent.height
                        color: borderColor
                        anchors.left: parent.left
                    }
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
