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
    width: 800
    height: 600
    minimumHeight: 500
    minimumWidth: 600
    visible: true
    title: "Chess"
    // color: "#2b2b2b"

    Component.onCompleted: gameController.startGame()

    palette {
        // Backgrounds
        window: "#1a1816"
        base: "#2c2a27"
        alternateBase: "#3d3a35"

        // Text
        windowText: "#c8c0b5"
        text: "#c8c0b5"
        placeholderText: "#8a847c"
        mid: "#8a847c"

        // Accent (matches white piece colour)
        highlight: "#d3b88c"
        highlightedText: "#1a1816"
        button: "#3d3a35"
        buttonText: "#c8c0b5"

        // Borders
        dark: "#141210"
        shadow: "#0d0b09"
        light: "#4a4640"
        midlight: "#3d3a35"
    }

    Connections {
        target: gameController
        function onGameOver(winner) {
            resultText.text = winner + " wins!"
            resultOverlay.visible = true
        }

        function onPendingPromotionChanged() {
            if (gameController.pendingPromotion === true) {
                promotionDialog.open()
            }
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

    SplitView {
        anchors.fill: parent
        orientation: Qt.Horizontal

        Item {
            SplitView.fillWidth: true
            SplitView.fillHeight: true

            Board {
                id: chessBoard
                anchors.centerIn: parent
                width: Math.min(parent.width, parent.height)
                height: width  // always square
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
                model: gameController.board
                enabled: gameController.isGameOngoing
            }

        }

        ColumnLayout {
            SplitView.fillWidth: true
            SplitView.fillHeight: true
            SplitView.minimumWidth: 200

            spacing: 8


            MoveList {
                Layout.fillWidth: true
                Layout.fillHeight: true
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

    PromotionDialog {
        id: promotionDialog
    }
}