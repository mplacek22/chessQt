import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Chess 1.0

Dialog {
    id: root
    title: "Game over"

    property int winner: -1
    property string drawCause: ""

    signal restartRequested()

    readonly property string winnerStr: {
        if (winner === -1) return ""
        return winner === Enums.WHITE ? "White" : "Black"
    }

    anchors.centerIn: Overlay.overlay
    modal: true
    closePolicy: Popup.NoAutoClose
    padding: 24
    width: 400
    standardButtons: Dialog.NoButton

    Overlay.modal: Rectangle { color: Qt.rgba(0, 0, 0, 0.4) }

    background: Rectangle {
        radius: 12
        color: palette.base
        border.color: palette.dark
    }

    contentItem : ColumnLayout {
        id: contentColumn
        spacing: 8
        width: 400

        Text {
            Layout.fillWidth: true
            horizontalAlignment: Text.AlignHCenter
            font.pointSize: 20
            font.weight: Font.Medium
            font.family: "Monospace"
            color: palette.text
            text: root.winner !== -1 ? root.winnerStr + " wins" : "Draw"
        }

        Text {
            Layout.fillWidth: true
            horizontalAlignment: Text.AlignHCenter
            font.family: "Monospace"
            font.pointSize: 14
            color: palette.highlight
            text: root.drawCause
            visible: root.winner === -1 && root.drawCause !== ""
        }
    }

    footer: DialogButtonBox {
        padding: 8
        spacing: 8
        alignment: Qt.AlignRight

        RoundButton {
            radius: 8
            text: "Restart"
            horizontalPadding: 20
            font.pixelSize: 14
            DialogButtonBox.buttonRole: DialogButtonBox.AcceptRole
            onClicked: root.restartRequested()
        }

        RoundButton {
            radius: 8
            text: "Close"
            horizontalPadding: 20
            font.pixelSize: 14
            DialogButtonBox.buttonRole: DialogButtonBox.RejectRole
        }
    }
}
