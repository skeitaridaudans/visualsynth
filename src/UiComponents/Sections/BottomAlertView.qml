import QtQuick
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15

Rectangle {
    id: rectangle1
    anchors.bottom: parent.bottom
    anchors.bottomMargin: 62
    anchors.horizontalCenter: parent.horizontalCenter
    color: alertController.alertColor
    height: 55
    radius: 10
    state: alertController.alertVisibleState
    width: 510
    y: 965

    states: [
        State {
            name: "invisible"

            PropertyChanges {
                anchors.bottomMargin: -53
                opacity: 0
                target: rectangle1
            }
        },
        State {
            name: "visible"

            PropertyChanges {
                anchors.bottomMargin: 62
                opacity: 1
                target: rectangle1
            }
        }
    ]
    transitions: [
        Transition {
            to: "invisible"

            NumberAnimation {
                duration: 400
                easing.type: Easing.InOutQuad
                loops: 1
                properties: "anchors.bottomMargin,opacity"
            }
        },
        Transition {
            to: "visible"

            NumberAnimation {
                duration: 400
                easing.type: Easing.InOutQuad
                loops: 1
                properties: "anchors.bottomMargin,opacity"
            }
        }
    ]

    Text {
        id: text1
        anchors.fill: parent
        color: "#ffffff"
        font.pixelSize: 22
        horizontalAlignment: Text.AlignHCenter
        text: alertController.alertText
        verticalAlignment: Text.AlignVCenter
    }
}
