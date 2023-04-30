import QtQuick

Rectangle {
    id: root

    property alias text: label.text
    property bool enabled: false
    signal pressed

    width: 100
    height: 40
    radius: 4
    state: enabled ? "checked" : "unchecked"

    states: [
        State {
            name: "checked"
            PropertyChanges {
                target: root
                color: "#28ff00ff"
            }
            PropertyChanges {
                target: light
                color: "#ff00ff"
            }
        },
        State {
            name: "unchecked"
            PropertyChanges {
                target: root
                color: "#289E9E9E"
            }
            PropertyChanges {
                target: light
                color: "#7f7c7b"
            }
        }
    ]

    transitions: [
        Transition {
            to: "checked,unchecked"
            ColorAnimation {
                easing.type: Easing.OutQuad
                duration: 200
            }
        }
    ]

    MultiPointTouchArea{
        id: cearseToggle
        anchors.fill: parent
        onPressed: {
            root.pressed()
        }
    }
    Rectangle {
        id: light
        width: 6
        height: 16
        radius: 2
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 8
    }

    Text {
        id: label
        color: "#f0f0f0"
        font.pixelSize: 13
        font.weight: Font.DemiBold
        verticalAlignment: Text.AlignVCenter
        anchors.verticalCenter: parent.verticalCenter
        anchors.left: light.right
        anchors.leftMargin: 14
    }
}
