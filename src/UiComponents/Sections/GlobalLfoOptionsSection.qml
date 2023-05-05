import QtQuick
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15

import "../Core"

Rectangle {
    id: lfoOptions
    color: "transparent"

    Text {
        id: lfoOptionsTitle
        anchors.left: parent.left
        anchors.leftMargin: 24
        anchors.top: parent.top
        anchors.topMargin: 8
        color: "#ffffff"
        font.family: "Noto Sans"
        font.pointSize: 18
        font.weight: Font.Bold
        text: "LFO"
        verticalAlignment: Text.AlignTop
    }
    ToggleButton {
        id: lfoEnabled
        anchors.left: lfoOptionsTitle.left
        anchors.leftMargin: 0
        anchors.top: lfoOptionsTitle.bottom
        anchors.topMargin: 8
        enabled: controller.isLfoEnabled
        text: qsTr("Enabled")

        onPressed: {
            controller.setLfoEnabled(!controller.isLfoEnabled);
        }
    }
    Text {
        id: lfoRateTitle
        anchors.left: lfoOptionsTitle.right
        anchors.leftMargin: 96
        anchors.top: parent.top
        anchors.topMargin: 8
        color: "#9E9E9E"
        font.family: "Noto Sans"
        font.pointSize: 14
        font.weight: Font.DemiBold
        height: lfoOptionsTitle.height
        horizontalAlignment: Text.AlignLeft
        text: "Rate"
        verticalAlignment: Text.AlignBottom
    }
    Text {
        id: lfoRateValue
        anchors.left: lfoRateTitle.left
        anchors.leftMargin: 8
        anchors.top: lfoRateTitle.bottom
        anchors.topMargin: 8
        color: "#757575"
        height: 40
        horizontalAlignment: Text.AlignLeft
        text: `${parseFloat(controller.lfoFrequency).toFixed(2)} Hz`
        verticalAlignment: Text.AlignVCenter
        width: 32
    }
    Slider {
        id: lfoSlider
        anchors.left: lfoRateValue.right
        anchors.leftMargin: 16
        anchors.top: lfoRateTitle.bottom
        anchors.topMargin: 8
        from: Math.log2(1)
        height: 40
        to: Math.log2(20)
        value: Math.log2(controller.lfoFrequency)
        width: 400

        onMoved: {
            controller.setLfoFrequency(Math.pow(2, lfoSlider.value));
        }
    }
}
