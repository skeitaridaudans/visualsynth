import QtQuick
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import OperatorView
import OperatorPresetsView
import PButton

Rectangle {
    id: operatorrectangle
    color: "#212121"
    radius: 3

    Connections {
        function onShowPresetsChanged(show) {
            presetB.updateOpen();
        }

        target: controller
    }
    OperatorView {
        id: boxes
        anchors.top: parent.top
        height: parent.height - 50
        width: parent.width
    }
    PresetButton {
        id: presetB
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 50
        height: 50
        width: 250

        onClicked: {
            controller.deselectOperator();
            controller.showPresets = !controller.showPresets;
            presetB.updateOpen();
        }
    }
    Rectangle {
        id: presetsContainer
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: presetB.bottom
        anchors.topMargin: 16
        color: "#323232"
        height: 450
        radius: 8
        visible: controller.showPresets || presetVisibleTransition.running
        width: 450
        state: controller.showPresets ? "visible" : "invisible"
        transform: Translate {
            id: presetsContainerTranslate
        }

        states: [
            State {
                name: "invisible"
                PropertyChanges {
                    target: presetsContainer
                    opacity: 0
                    height: 400
                }
                PropertyChanges {
                    target: presetsContainerTranslate
                    y: -20
                }
            },
            State {
                name: "visible"
                PropertyChanges {
                    target: presetsContainer
                    opacity: 1
                    height: 450
                }
                PropertyChanges {
                    target: presetsContainerTranslate
                    y: 0
                }
            }
        ]

        transitions: [
            Transition {
                id: presetVisibleTransition
                to: "invisible,visible"
                NumberAnimation {
                    properties: "opacity"
                    easing.type: Easing.OutQuad
                    duration: 300
                }
                NumberAnimation {
                    properties: "height,y"
                    easing.type: Easing.OutBack
                    duration: 300
                }
            }
        ]

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
                enabled: presetsContainer.visible
            }
        }
    }
}
