import QtQuick
import QtQuick.Controls 2.15
import QtQuick.Controls.Material 2.15
import QtQuick.Layouts 1.3
import AmpEnvGraphView
import "../Core"

Rectangle {
    color: "transparent"

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        Text {
            id: operatorEnvTitle
            Layout.topMargin: 12
            Layout.bottomMargin: 8
            Layout.leftMargin: 24
            color: "#9E9E9E"
            font.family: "Noto Sans"
            font.pointSize: 14
            font.weight: Font.DemiBold
            text: controller.selectedOperator ? `Operator Envelope: Operator ${controller.selectedOperator.idProp + 1}` : "Operator Envelope: No operator selected"
            verticalAlignment: Text.AlignTop
        }
        AmpEnvGraphItem {
            id: operatorEnvGraphView
            Layout.preferredHeight: parent.height / 2 - operatorEnvTitle.height - 20
            Layout.preferredWidth: parent.width
            operatorEnvelope: true

            CustomBorder {
                sides: ["bottom"]
            }
        }

        Text {
            id: ampEnvTitle
            Layout.topMargin: 12 - 3
            Layout.bottomMargin: 8
            Layout.leftMargin: 24
            color: "#9E9E9E"
            font.family: "Noto Sans"
            font.pointSize: 14
            font.weight: Font.DemiBold
            text: "Global Amplitude Envelope"
            verticalAlignment: Text.AlignTop
        }
        AmpEnvGraphItem {
            id: ampEnvGraphView
            Layout.preferredHeight: parent.height / 2 - ampEnvTitle.height - 20
            Layout.preferredWidth: parent.width
            operatorEnvelope: false
        }
    }
}
