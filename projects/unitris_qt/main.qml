import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.4

Window {
    id: unitrisGame
    visible: true
    width: 640
    height: 480
    title: qsTr("Unitris - Qt version")
    color: "#00BFFF"
    property bool started: false

    Timer {
        id: gameTick
        interval: 50; running: false; repeat: true
        onTriggered: {
            imageDisplay.source = unitris.tick();
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 4

        Label {
            Layout.alignment: Qt.AlignHCenter
            text: "Arrow keys to move the tetromino, up to rotate, space to drop"
        }

        Image {
            id: imageDisplay
            Layout.alignment: Qt.AlignHCenter
            Layout.fillHeight: true
            Layout.fillWidth: true
            fillMode: Image.PreserveAspectFit
        }

        RowLayout {
            spacing: 4
            Layout.fillWidth: true
            Item {
                Layout.fillWidth: true
            }

            Button {
                text: "Start"
                onClicked: {
                    if (!unitrisGame.started)
                    {
                        unitris.start();
                    }
                    gameTick.start();
                }
            }
            Button {
                text: "Pause"
                onClicked: {
                    gameTick.stop();
                }
            }
        }
    }
}
