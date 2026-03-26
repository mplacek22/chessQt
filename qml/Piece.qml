// import QtQuick 2.15

// Item {
//     property string piece: ""

//     Text {
//         anchors.centerIn: parent
//         text: {
//             const map = {
//                 "wK": "♔", "wQ": "♕", "wR": "♖", "wB": "♗", "wN": "♘", "wP": "♙",
//                 "bK": "♚", "bQ": "♛", "bR": "♜", "bB": "♝", "bN": "♞", "bP": "♟"
//             }
//             return map[piece] ?? ""
//         }
//         font.pixelSize: parent.width * 0.75
//         color: piece.startsWith("w") ? "white" : "#1a1a1a"
//         style: Text.Outline
//         styleColor: piece.startsWith("w") ? "#1a1a1a" : "white"
//         horizontalAlignment: Text.AlignHCenter
//         verticalAlignment: Text.AlignVCenter
//         width: parent.width
//         height: parent.height
//     }
// }
// Piece.qml
import QtQuick 2.15

Item {
    id: root

    property string pieceType: ""   // "King", "Queen", "Rook", etc.
    property string pieceColor: ""  // "white" or "black"
    property bool isSelected: false

    // Map piece type + color to unicode chess symbols
    readonly property var pieceSymbols: ({
        "white": { King:"♔", Queen:"♕", Rook:"♖", Bishop:"♗", Knight:"♘", Pawn:"♙" },
        "black": { King:"♚", Queen:"♛", Rook:"♜", Bishop:"♝", Knight:"♞", Pawn:"♟" }
    })

    Text {
        anchors.centerIn: parent
        text: (pieceType && pieceColor)
              ? (root.pieceSymbols[pieceColor]?.[pieceType] ?? "")
              : ""
        font.pixelSize: parent.width * 0.7
        font.family: "serif"
        style: Text.Outline
        styleColor: root.pieceColor === "white" ? "#222" : "#eee"
        color: root.pieceColor === "white" ? "#f0f0f0" : "#1a1a1a"

        scale: root.isSelected ? 1.1 : 1.0
        Behavior on scale { NumberAnimation { duration: 120 } }
    }
}