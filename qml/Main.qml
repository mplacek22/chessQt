import QtQuick 2.15
import QtQuick.Controls 2.15

ApplicationWindow {
    id: root
    visible: true
    width: 800
    height: 600
    title: "Chess"

    Row {
        anchors.fill: parent

        Board {
            id: board
            width: parent.height
            height: parent.height

            onSquareClicked: function(file, rank) {
                chessController.onSquareClicked(file, rank)
            }
        }

        MoveList {
            id: moveList
            width: parent.width - board.width
            height: parent.height

            model: chessController.moveModel
        }
    }
}