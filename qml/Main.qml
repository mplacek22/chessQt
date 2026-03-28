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

ApplicationWindow {
    id: root
    width: 700
    height: 780
    visible: true
    title: "Chess"
    color: "#2b2b2b"

    Component.onCompleted: gameController.startGame()

    Connections {
        target: gameController
        function onGameOver(winner) {
            resultText.text = winner + " wins!"
            resultOverlay.visible = true
        }
    }

    menuBar: MenuBar {
        Menu {
            title: qsTr("Game")
            Action {
                text: qsTr("New game")
                shortcut: StandardKey.New
                onTriggered: gameController.restartGame()
            }
            Action { text: qsTr("Open game") }
            Action { text: qsTr("Save game") }
            Action { text: qsTr("Save game as") }
            MenuSeparator { }
            Action {
                text: qsTr("Quit")
                shortcut: StandardKey.Quit
                onTriggered:Qt.quit()
            }
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

        Board {
            id: chessBoard
            width: 560
            height: 560
            model: gameController.board
            enabled: gameController.status === "in_progress"
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