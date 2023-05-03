import QtQuick
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import OutputWaveView

Rectangle {
    color: "#212121"
    radius: 3

    OutputWaveView {
        id: outputWaveView
        anchors.fill: parent
    }
}
