// import QtQuick 2.15

// Item {
//     id: board

//     property int labelSize: 20
//     property int squareSize: (Math.min(width, height) - labelSize) / 8

//     // // Drag state
//     // property int dragFromFile: -1
//     // property int dragFromRank: -1
//     // property string dragPiece: ""
//     // property real dragX: 0
//     // property real dragY: 0
//     // property bool isDragging: false

//     signal squareClicked(int file, int rank)

//     Item {
//         id: boardContainer
//         anchors.left: parent.left
//         anchors.top: parent.top
//         anchors.leftMargin: labelSize
//         anchors.topMargin: 0

//         implicitWidth: squareSize * 8
//         implicitHeight: squareSize * 8

//         Grid {
//             ranks: 8
//             columns: 8
//             anchors.fill: parent

//             Repeater {
//                 model: 64

//                 Rectangle {
//                     width: board.squareSize
//                     height: board.squareSize

//                     property int file: index % 8
//                     property int rank: 7 - Math.floor(index / 8)

//                     color: (file + rank) % 2 === 0 ? "#656256" : "#828F7B"

//                     // Selection highlight
//                     Rectangle {
//                             anchors.fill: parent
//                             color: "#9EBC9F"
//                             visible: chessController.selectedIndex === (rank * 8 + file)
//                             z: 1
//                         }

//                     MouseArea {
//                         anchors.fill: parent
//                         onClicked: chessController.selectSquare(file, rank)
//                     }

//                     Piece {
//                         anchors.fill: parent
//                         piece: chessController.pieceAt(file, rank)
//                         z: 2
//                     }
//                 }
//             }
//         }
//     }

//     Column {
//         anchors.right: boardContainer.left
//         anchors.top: boardContainer.top
//         spacing: 0

//         Repeater {
//             model: 8

//             Rectangle {
//                 width: labelSize
//                 height: board.squareSize
//                 color: "transparent"

//                 Text {
//                     anchors.centerIn: parent
//                     text: (8 - index)
//                     color: "white"
//                     font.pixelSize: 12
//                 }
//             }
//         }
//     }

//     rank {
//         anchors.top: boardContainer.bottom
//         anchors.left: boardContainer.left
//         spacing: 0

//         Repeater {
//             model: 8

//             Rectangle {
//                 width: board.squareSize
//                 height: board.labelSize
//                 color: "transparent"

//                 Text {
//                     anchors.centerIn: parent
//                     text: String.fromCharCode(65 + index) // A–H
//                     color: "white"
//                     font.pixelSize: 12
//                 }
//             }
//         }
//     }
// }

// Board.qml
import QtQuick 2.15
import Chess 1.0

Item {
    id: board

    property var model: null
    property int labelSize: 20
    signal squareClicked(int rank, int file)

    GridView {
        id: grid
        anchors.fill: parent
        model: board.model
        cellWidth:  width  / 8
        cellHeight: height / 8

        delegate: Rectangle {
            id: square
            width:  grid.cellWidth
            height: grid.cellHeight

            // Checkerboard coloring
            readonly property bool isLight: (index % 8 + Math.floor(index / 8)) % 2 === 0
            color: {
                if (model.isHighlighted) return "#aac465"   // legal-move hint
                if (model.isSelected)   return "#f6f669"    // selected square
                return isLight ? "#f0d9b5" : "#b58863"
            }

            Behavior on color { ColorAnimation { duration: 100 } }

            Piece {
                anchors.fill: parent
                anchors.margins: 4
                pieceType:  model.pieceType   ?? ""
                pieceColor: model.pieceColor  ?? ""
                isSelected: model.isSelected  ?? false
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    const file = index % 8
                    const rank = Math.floor(index / 8)
                    board.model.selectSquare(rank, file)
                    board.squareClicked(rank, file)
                }
            }
        }
    }
    Row {
        anchors.right: grid.left
        anchors.top: grid.top
        spacing: 0

        Repeater {
            model: 8

            Rectangle {
                width: labelSize
                height: board.squareSize
                color: "transparent"

                Text {
                    anchors.centerIn: parent
                    text: (8 - index)
                    color: "white"
                    font.pixelSize: 12
                }
            }
        }
    }
}



//     rank {
//         anchors.top: boardContainer.bottom
//         anchors.left: boardContainer.left
//         spacing: 0

//         Repeater {
//             model: 8

//             Rectangle {
//                 width: board.squareSize
//                 height: board.labelSize
//                 color: "transparent"

//                 Text {
//                     anchors.centerIn: parent
//                     text: String.fromCharCode(65 + index) // A–H
//                     color: "white"
//                     font.pixelSize: 12
//                 }
//             }
//         }
//     }