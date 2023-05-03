import QtQuick
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import AmpEnvGraphView

Rectangle {
    color: "transparent"
    border.color: "gray"
    border.width: 3
    radius: 3

    AmpEnvGraphItem {
        id: ampEnvGraphView
        anchors.fill: parent
    }
}
