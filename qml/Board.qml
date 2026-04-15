import QtQuick 2.15
import Chess 1.0

Item {
    id: board

    property var model: null
    property int labelSize: 20
    signal squareClicked(int rank, int file)

    // Ranks
    Column {
        id: rankLabels
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: fileLabels.top
        width: labelSize
        Repeater {
            model: 8
            Text {
                width: labelSize
                height: rankLabels.height / 8
                text: (8 - index).toString()
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: labelSize * 0.7
                color: palette.text
            }
        }
    }


    GridView {
        id: grid
        anchors.left: rankLabels.right
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: fileLabels.top
        model: board.model
        cellWidth:  width  / 8
        cellHeight: height / 8
        interactive: false

        delegate: Rectangle {
            id: square
            width:  grid.cellWidth
            height: grid.cellHeight

            // Checkerboard coloring
            readonly property bool isLight: (index % 8 + Math.floor(index / 8)) % 2 === 0
            color: {
                if (model.isHighlighted) return "#9EBC9F"   // legal-move hint
                if (model.isSelected)   return "#9EBC9F"    // selected square
                return isLight ? "#828F7B" : "#656256"
            }

            Behavior on color { ColorAnimation { duration: 100 } }

            Piece {
                anchors.fill: parent
                anchors.margins: 4
                svgPath:  model.svgPath ?? ""
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

    // Files
    Row {
        id: fileLabels
        anchors.left: rankLabels.right
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        height: labelSize

        Repeater {
            model: 8
            Text {
                width: fileLabels.width / 8
                height: labelSize
                text: String.fromCharCode(97 + index)
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                font.pixelSize: labelSize * 0.7
                color: palette.text
            }
        }
    }
}