import QtQuick 2.15

Item {
    id: board

    property int labelSize: 20
    property int squareSize: (Math.min(width, height) - labelSize) / 8

    signal squareClicked(int file, int rank)

    Item {
        id: boardContainer
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.leftMargin: labelSize
        anchors.topMargin: 0

        implicitWidth: squareSize * 8
        implicitHeight: squareSize * 8

        Grid {
            rows: 8
            columns: 8
            anchors.fill: parent

            Repeater {
                model: 64

                Rectangle {
                    width: board.squareSize
                    height: board.squareSize

                    property int file: index % 8
                    property int rank: 7 - Math.floor(index / 8)

                    color: (file + rank) % 2 === 0 ? "#656256" : "#828F7B"

                    Rectangle {
                            anchors.fill: parent
                            color: "#9EBC9F"
                            visible: chessController.selectedIndex === (rank * 8 + file)
                            z: 1
                        }

                    MouseArea {
                        anchors.fill: parent
                        onClicked: chessController.selectSquare(file, rank)
                    }

                    Piece {
                        anchors.fill: parent
                        piece: chessController.pieceAt(file, rank)
                        z: 2
                    }
                }
            }
        }
    }

    Column {
        anchors.right: boardContainer.left
        anchors.top: boardContainer.top
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

    Row {
        anchors.top: boardContainer.bottom
        anchors.left: boardContainer.left
        spacing: 0

        Repeater {
            model: 8

            Rectangle {
                width: board.squareSize
                height: board.labelSize
                color: "transparent"

                Text {
                    anchors.centerIn: parent
                    text: String.fromCharCode(65 + index) // A–H
                    color: "white"
                    font.pixelSize: 12
                }
            }
        }
    }
}