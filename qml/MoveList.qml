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

                RowLayout {
                    anchors.fill: parent
                    anchors.leftMargin: 4
                    anchors.rightMargin: 4
                    spacing: 0

                    Text {
                        Layout.preferredWidth: 32
                        text: (index + 1) + "."
                        color: palette.placeholderText
                        font.pixelSize: 12
                        font.family: "Monospace"
                        horizontalAlignment: Text.AlignRight
                    }

                    // White move (before the two spaces)
                    Text {
                        Layout.fillWidth: true
                        Layout.leftMargin: 8
                        text: modelData.split("  ")[0].replace(/^\d+\.\s*/, "")
                        color: palette.text
                        font.pixelSize: 13
                        font.family: "Monospace"
                        font.bold: index === listView.count - 1
                    }

                    // Black move (after the two spaces)
                    Text {
                        Layout.fillWidth: true
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
}