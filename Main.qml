import QtQuick
import App 

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Oscilloscope (TP2 Client)")

    Oscilloscope {
        anchors.fill: parent
        model: appState.sensorsModel
    }
}
