import QtQuick
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import OutputWaveView

Rectangle {
    id: sinewaverectangle
    anchors.bottom: parent.bottom
    anchors.bottomMargin: 0
    border.color: "gray"
    border.width: 3
    color: "#212121"
    radius: 3

    OutputWaveView {
        id: outputWaveView
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 20
        height: 100
        width: 800
    }
}
