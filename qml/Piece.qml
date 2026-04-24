import QtQuick

Item {
    id: piece

    property string svgPath: ""
    property bool isSelected: false

    Image {
        anchors.centerIn: parent
        source: piece.svgPath
        width: parent.width * 0.6
        height: width
        fillMode: Image.PreserveAspectFit
        smooth: true
        scale: piece.isSelected ? 1.1 : 1.0
        Behavior on scale { NumberAnimation { duration: 120 } }
    }
}