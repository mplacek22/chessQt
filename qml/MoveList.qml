import QtQuick 2.15
import QtQuick.Controls 2.15

ListView {
    id: root

    property var model

    model: root.model

    delegate: Text {
        text: model.display
        padding: 8
    }

    ScrollBar.vertical: ScrollBar {}
}