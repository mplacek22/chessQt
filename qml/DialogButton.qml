import QtQuick 2.15
import QtQuick.Controls 2.15

Button {
    property string text: value
    horizontalPadding: 20
    verticalPadding: 10

    contentItem: Text {
        text: parent.text
        color: parent.enabled
               ? (parent.down || parent.highlighted
                  ? parent.palette.highlightedText
                  : parent.palette.buttonText)
               : parent.palette.mid
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

    background: Rectangle {
        radius: 8
        layer.enabled: true
        color: parent.down || parent.highlighted
               ? parent.palette.highlight
               : parent.hovered
                 ? parent.palette.midlight
                 : parent.palette.button

        border.color: parent.activeFocus
                      ? parent.palette.highlight
                      : parent.palette.mid
        border.width: parent.activeFocus ? 2 : 1

        Behavior on color {
            ColorAnimation { duration: 120 }
        }
    }
}