import QtQuick
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15

Rectangle {
    id: dialog
    x: 710
    y: 295
    width: 600
    height: 280
    color: "#323232"
    radius: 8
    anchors.horizontalCenter: parent.horizontalCenter
    anchors.verticalCenter: parent.verticalCenter
    visible: dialogController.isVisible

    Material.theme: Material.Dark
    Material.accent: Material.Purple

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
            placeholderText: "Name"
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
        y: 223
        width: 244
        height: 49
        color: "#323232"
        anchors.bottom: parent.bottom
        anchors.horizontalCenterOffset: 149
        anchors.bottomMargin: 8
        anchors.horizontalCenter: parent.horizontalCenter

        Button {
            id: okButton
            width: 96
            text: dialogController.confirmButtonText
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            font.weight: Font.DemiBold
            font.family: "Noto Sans"
            anchors.rightMargin: 0
            anchors.bottomMargin: 0
            anchors.topMargin: 0
            onReleased: dialogController.submit()

            background: Rectangle {
                id: okButtonBackground
                color: "#3F51B5"
                radius: 4
                state: okButton.hovered ? "hovered" : okButton.pressed ? "pressed" : "none"

                states: [
                    State {
                        name: "none"
                        PropertyChanges {
                            target: okButtonBackground
                            color: "#3F51B5"
                        }
                    },
                    State {
                        name: "hovered"
                        PropertyChanges {
                            target: okButtonBackground
                            color: "#303F9F"
                        }
                    },
                    State {
                        name: "pressed"
                        PropertyChanges {
                            target: okButtonBackground
                            color: "#1A237E"
                        }
                    }
                ]

                transitions: [
                    Transition {
                        to: "none,hovered,pressed"
                        ColorAnimation {
                            easing.type: Easing.OutQuad
                            duration: 260
                        }
                    }

                ]
            }
        }

        Button {
            id: cancelButton
            width: 96
            text: dialogController.cancelButtonText
            anchors.right: okButton.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            font.weight: Font.DemiBold
            font.family: "Noto Sans"
            anchors.rightMargin: 8
            anchors.bottomMargin: 0
            anchors.topMargin: 0
            onReleased: dialogController.cancel()

            background: Rectangle {
                id: cancelButtonBackground
                color: "#FAFAFA"
                radius: 4
                state: cancelButton.hovered ? "hovered" : cancelButton.pressed ? "pressed" : "none"

                states: [
                    State {
                        name: "none"
                        PropertyChanges {
                            target: cancelButtonBackground
                            opacity: 0.0
                        }
                    },
                    State {
                        name: "hovered"
                        PropertyChanges {
                            target: cancelButtonBackground
                            opacity: 0.2
                        }
                    },
                    State {
                        name: "pressed"
                        PropertyChanges {
                            target: cancelButtonBackground
                            opacity: 0.4
                        }
                    }
                ]

                transitions: [
                    Transition {
                        to: "none,hovered,pressed"
                        NumberAnimation {
                            properties: "opacity"
                            easing.type: Easing.OutQuad
                            duration: 260
                        }
                    }

                ]
            }
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
