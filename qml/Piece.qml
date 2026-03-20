import QtQuick 2.15

Item {
    property string piece: ""

    Text {
        anchors.centerIn: parent
        text: {
            const map = {
                "wK": "♔", "wQ": "♕", "wR": "♖", "wB": "♗", "wN": "♘", "wP": "♙",
                "bK": "♚", "bQ": "♛", "bR": "♜", "bB": "♝", "bN": "♞", "bP": "♟"
            }
            return map[piece] ?? ""
        }
        font.pixelSize: parent.width * 0.75
        color: piece.startsWith("w") ? "white" : "#1a1a1a"
        style: Text.Outline
        styleColor: piece.startsWith("w") ? "#1a1a1a" : "white"
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
        width: parent.width
        height: parent.height
    }
}