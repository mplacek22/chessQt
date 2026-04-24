import QtQuick 2.15
import QtQuick.Controls 2.15
import Chess 1.0

Popup {
    id: root
    modal: true
    focus: true
    padding: 40
    anchors.centerIn: Overlay.overlay

    closePolicy: Popup.NoAutoClose

    background: Rectangle {
        color: palette.base
        radius: 10
    }

    Column {
        anchors.centerIn: parent
        spacing: 16

        Text {
            text: "Choose promotion piece type"
            anchors.horizontalCenter: parent.horizontalCenter
            color: palette.text
            font.family: "Monospace"
            font.pointSize: 14
        }

        Row {
            spacing: 12
            anchors.horizontalCenter: parent.horizontalCenter

            Repeater {
                model: [
                    Enums.PieceType.QUEEN,
                    Enums.PieceType.ROOK,
                    Enums.PieceType.BISHOP,
                    Enums.PieceType.KNIGHT
                ]

                PieceButton {
                    pieceType: modelData
                    pieceColor: gameController.currentPlayer
                    onClicked: {
                        gameController.promotePawn(modelData)
                        root.close()
                    }
                }
            }
        }
    }
}