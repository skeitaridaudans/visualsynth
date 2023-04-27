import QtQuick
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15

Rectangle {
    id: connectionInfoContainer
    anchors.left: parent.left
    anchors.top: operatorrectangle.bottom

    Rectangle {
        id: connectionInfo
        anchors.left: parent.left
        anchors.leftMargin: 40
        anchors.top: parent.top
        anchors.topMargin: 32
        height: 60

        BusyIndicator {
            id: connectingIndicator
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.verticalCenter: parent.verticalCenter
            height: 34
            running: controller.isConnecting
            width: 34
        }
        Rectangle {
            id: connectedRoundButton
            anchors.left: parent.left
            anchors.leftMargin: 0
            anchors.verticalCenter: parent.verticalCenter
            color: controller.isConnected ? "green" : "red"
            height: 34
            radius: 50
            visible: !controller.isConnecting
            width: 34
        }
        Text {
            id: connectedText
            anchors.left: parent.left
            anchors.leftMargin: 48
            anchors.verticalCenter: parent.verticalCenter
            color: "#ffffff"
            font.pixelSize: 18
            height: 27
            horizontalAlignment: Text.AlignLeft
            text: controller.connectionStateText
            verticalAlignment: Text.AlignVCenter
            width: 141
        }
        Button {
            id: connectButton
            anchors.left: connectedText.right
            anchors.leftMargin: 8
            anchors.verticalCenter: parent.verticalCenter
            font.family: "Noto Sans"
            font.weight: Font.DemiBold
            height: 48
            text: controller.isConnected ? "Disconnect" : "Connect"
            width: 96
            enabled: !controller.isConnecting

            background: Rectangle {
                id: connectButtonBackground
                color: "#3F51B5"
                radius: 4
                state: connectButton.pressed ? "pressed" : connectButton.hovered ? "hovered" : "none"

                states: [
                    State {
                        name: "none"

                        PropertyChanges {
                            color: controller.isConnected ? "#F44336" : controller.isConnecting ? "#616161" : "#910887"
                            target: connectButtonBackground
                        }
                    },
                    State {
                        name: "hovered"

                        PropertyChanges {
                            color: controller.isConnected ? "#D32F2F" : "#800080"
                            target: connectButtonBackground
                        }
                    },
                    State {
                        name: "pressed"

                        PropertyChanges {
                            color: controller.isConnected ? "#B71C1C" : "#55006b"
                            target: connectButtonBackground
                        }
                    }
                ]
                transitions: [
                    Transition {
                        to: "none,hovered,pressed"

                        ColorAnimation {
                            duration: 260
                            easing.type: Easing.OutQuad
                        }
                    }
                ]
            }

            onReleased: !controller.isConnected ? controller.showConnectDialog() : controller.disconnect()
        }
    }
}
