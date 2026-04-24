import QtQuick 2.15
import QtQuick.Controls 2.15
import Chess 1.0

Rectangle {
    id: root
    property int pieceType
    property int pieceColor

    signal clicked(int pieceType)

    width: 80
    height: 80
    radius: 8
    color: hovered ? "#757265" : "#656256"
    border.color: hovered ? "#9EBC9F" : "transparent"
    border.width: 2

    property bool hovered: false

    Behavior on color {
        ColorAnimation { duration: 100 }
    }

    Image {
        anchors.centerIn: parent
        width: parent.width * 0.6
        height: parent.height * 0.6
        source: PieceUtils.imageSource(pieceColor, pieceType)
        fillMode: Image.PreserveAspectFit
        smooth: true
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        onEntered: root.hovered = true
        onExited: root.hovered = false
        onClicked: root.clicked(root.pieceType)
    }
}