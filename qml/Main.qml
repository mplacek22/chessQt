// import QtQuick 2.15
// import QtQuick.Controls 2.15

// ApplicationWindow {
//     id: root
//     visible: true
//     width: 800
//     height: 600
//     title: "Chess"

//     Row {
//         anchors.fill: parent

//         Board {
//             id: board
//             width: parent.height
//             height: parent.height

//             onSquareClicked: function(file, rank) {
//                 chessController.onSquareClicked(file, rank)
//             }
//         }

//         MoveList {
//             id: moveList
//             width: parent.width - board.width
//             height: parent.height

//             model: chessController.moveModel
//         }
//     }
// }
// Main.qml
import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Chess 1.0

Window {
    id: root
    width: 700
    height: 780
    visible: true
    title: "Chess"
    color: "#2b2b2b"

    // gameController is injected via setContextProperty in main.cpp
    Component.onCompleted: gameController.startGame()

    Connections {
        target: gameController
        function onGameOver(winner) {
            resultText.text = winner + " wins!"
            resultOverlay.visible = true
        }
    }

    ColumnLayout {
        anchors.centerIn: parent
        spacing: 16

        // Status bar
        Rectangle {
            Layout.fillWidth: true
            height: 44
            radius: 8
            color: "#3c3c3c"

            Text {
                anchors.centerIn: parent
                text: gameController.currentPlayer === "white"
                      ? "⬜  White to move"
                      : "⬛  Black to move"
                color: "#e8e8e8"
                font.pixelSize: 16
                font.family: "serif"
            }
        }

        // The board itself
        Board {
            id: chessBoard
            width: 560
            height: 560
            model: gameController.board

            onSquareClicked: (row, col) => {
                // BoardModel.selectSquare() handles move logic on the C++ side
                // and emits boardChanged() / moveExecuted() back to QML
            }
        }

        // Buttons
        RowLayout {
            Layout.alignment: Qt.AlignHCenter
            spacing: 12

            Button {
                text: "New Game"
                onClicked: gameController.restartGame()
            }
        }
    }

    // Game-over overlay
    Rectangle {
        id: resultOverlay
        anchors.fill: parent
        color: "#aa000000"
        visible: false
        radius: 4

        Column {
            anchors.centerIn: parent
            spacing: 20

            Text {
                id: resultText
                anchors.horizontalCenter: parent.horizontalCenter
                color: "white"
                font.pixelSize: 36
                font.bold: true
            }

            Button {
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Play Again"
                onClicked: {
                    resultOverlay.visible = false
                    gameController.restartGame()
                }
            }
        }
    }
}