import QtQuick
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15

Rectangle {
    id: dialogBackground
    visible: dialogController.isVisible
    enabled: dialogController.isVisible
    state: dialogController.isVisible ? "visible" : "invisible"

    anchors.fill: parent
    color: "#8C000000"

    states: [
        State {
            name: "invisible"
            PropertyChanges {
                target: dialogBackground
                color: "#00000000"
            }
            PropertyChanges {
                target: dialog
                scale: 0.6
                opacity: 0
            }
        },
        State {
            name: "visible"
            PropertyChanges {
                target: dialogBackground
                color: "#8C000000"
            }
            PropertyChanges {
                target: dialog
                scale: 1
                opacity: 1
            }
        }
    ]

    transitions: [
        Transition {
            to: "visible"
            ColorAnimation {
                easing.type: Easing.OutQuad
                duration: 300
            }
            NumberAnimation {
                properties: "scale"
                easing.type: Easing.OutBack
                duration: 300
            }
            NumberAnimation {
                properties: "opacity"
                easing.type: Easing.OutQuad
                duration: 300
            }
        }
    ]

    // To prevent the user from pressing on elements behind the dialog
    MultiPointTouchArea  {
        anchors.fill: parent
    }

    Rectangle {
        id: dialog
        width: 600
        height: 280
        color: "#323232"
        radius: 8
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter

        Text {
            id: title
            width: 543
            height: 36
            color: "#ffffff"
            text: dialogController.title
            anchors.top: parent.top
            font.pixelSize: 26
            verticalAlignment: Text.AlignVCenter
            anchors.topMargin: 12
            font.weight: Font.Bold
            anchors.horizontalCenter: parent.horizontalCenter
            font.family: "Noto Sans"
        }


        Rectangle {
            id: textInputContainer
            y: 153
            width: 545
            height: 47
            color: "#212121"
            radius: 4
            anchors.horizontalCenterOffset: 1
            anchors.horizontalCenter: parent.horizontalCenter
            border.width: 2
            border.color: "transparent"
            state: textInput.activeFocus ? "focused" : "unfocused"

            states: [
                State {
                    name: "unfocused"
                    PropertyChanges {
                        target: textInputContainer
                        border.color: "transparent"
                    }
                },
                State {
                    name: "focused"
                    PropertyChanges {
                        target: textInputContainer
                        border.color: "#910887"
                    }
                }

            ]

            transitions: [
                Transition {
                    to: "unfocused,focused"
                    ColorAnimation {
                        easing.type: Easing.OutQuad
                        duration: 300
                    }
                }
            ]

            TextInput {
                id: textInput
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.fill: parent
                color: "#ffffff"
                text: dialogController.value
                font.pixelSize: 16
                verticalAlignment: Text.AlignVCenter
                anchors.rightMargin: 10
                anchors.leftMargin: 10
                selectionColor: "#910887"
                onTextChanged: dialogController.value = text
                onAccepted: dialogController.submit()

                Text {
                    visible: textInput.text === ""
                    text: dialogController.placeholderText
                    anchors.fill: parent
                    font.pixelSize: 16
                    verticalAlignment: Text.AlignVCenter
                    color: "#BDBDBD"
                }
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
                    state: okButton.pressed ? "pressed" : okButton.hovered ? "hovered" : "none"

                    states: [
                        State {
                            name: "none"
                            PropertyChanges {
                                target: okButtonBackground
                                color: "#910887"
                            }
                        },
                        State {
                            name: "hovered"
                            PropertyChanges {
                                target: okButtonBackground
                                color: "#800080"
                            }
                        },
                        State {
                            name: "pressed"
                            PropertyChanges {
                                target: okButtonBackground
                                color: "#55006b"
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
                    state: cancelButton.pressed ? "pressed" : cancelButton.hovered ? "hovered" : "none"

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
            width: 543
            height: 73
            color: "#e0e0e0"
            text: dialogController.text
            anchors.top: title.bottom
            font.pixelSize: 14
            verticalAlignment: Text.AlignTop
            anchors.topMargin: 10
            anchors.horizontalCenterOffset: 1
            font.family: "Noto Sans"
            anchors.horizontalCenter: parent.horizontalCenter
        }


    }

}
