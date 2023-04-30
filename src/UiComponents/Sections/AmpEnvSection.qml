import QtQuick
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import AmpEnvGraphView

Rectangle {
    color: "transparent"

    AmpEnvGraphItem {
        id: ampEnvGraphView
        anchors.fill: parent
    }
}
