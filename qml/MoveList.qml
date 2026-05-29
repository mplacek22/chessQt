import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

Rectangle {
    id: root
    color: palette.base
    clip: true

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 8
        spacing: 0

        // Header
        Rectangle {
            Layout.fillWidth: true
            height: 32
            color: "transparent"

            RowLayout {
                anchors.fill: parent
                spacing: 8

                Text {
                    Layout.preferredWidth: 36
                    text: "#"
                    color: palette.placeholderText
                    font.pixelSize: 14
                    font.family: "Monospace"
                    horizontalAlignment: Text.AlignHCenter
                }
                Text {
                    Layout.fillWidth: true
                    text: "white"
                    color: palette.placeholderText
                    font.pixelSize: 14
                    font.family: "Monospace"
                }
                Text {
                    Layout.fillWidth: true
                    text: "black"
                    color: palette.placeholderText
                    font.pixelSize: 14
                    font.family: "Monospace"
                }
            }
        }

        Rectangle { Layout.fillWidth: true; height: 1; color: palette.light }

        // Moves list
        ListView {
            id: listView
            Layout.fillWidth: true
            Layout.fillHeight: true
            model: gameController.movesList
            clip: true
            spacing: 0

            ScrollBar.vertical: ScrollBar { policy: ScrollBar.AsNeeded }

            onCountChanged: positionViewAtEnd()

            delegate: Rectangle {
                width: listView.width
                height: 28
                color: index % 2 === 0 ? "transparent" : palette.alternateBase

                // Highlight last move
                Rectangle {
                    anchors.fill: parent
                    color: "#9EBC9F"
                    opacity: index === listView.count - 1 ? 0.25 : 0
                    Behavior on opacity { NumberAnimation { duration: 200 } }
                }

                readonly property int colNum:   36   // must match header "#" width
                readonly property int colLeft:  8    // left margin
                readonly property int colWhite: Math.floor((width - colNum - colLeft) / 2)

                Text {
                    x: colLeft
                    width: colNum
                    anchors.verticalCenter: parent.verticalCenter
                    text: (index + 1) + "."
                    color: palette.placeholderText
                    font.pixelSize: 12
                    font.family: "Monospace"
                    horizontalAlignment: Text.AlignRight
                }

                // White move
                Text {
                    x: colLeft + colNum + 8
                    width: colWhite - 8
                    anchors.verticalCenter: parent.verticalCenter
                    text: modelData.split("  ")[0].replace(/^\d+\.\s*/, "")
                    color: palette.text
                    font.pixelSize: 13
                    font.family: "Monospace"
                    font.bold: index === listView.count - 1
                }

                // Black move
                Text {
                    x: colLeft + colNum + colWhite
                    width: parent.width - x - colLeft
                    anchors.verticalCenter: parent.verticalCenter
                    text: modelData.split("  ")[1] ?? ""
                    color: palette.text
                    font.pixelSize: 13
                    font.family: "Monospace"
                    font.bold: index === listView.count - 1 && modelData.split("  ").length > 1
                }
            }
        }
    }
}