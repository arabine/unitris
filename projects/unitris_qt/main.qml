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
            imageDisplay.source = unitris.tick(0);
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 4
        focus: true

//        Keys.onPressed: {
//             console.log("Pressed!");
//        }

        Keys.onPressed: {
            console.log("Released!");
            var keyEvent = 0;
        /*
#define KEY_LEFT    1
#define KEY_RIGHT   2
#define KEY_UP      4
#define KEY_DOWN    8
        */
            if (event.key === Qt.Key_Left) {
                keyEvent = 1;
                event.accepted = true;
            } else if (event.key === Qt.Key_Right) {
                keyEvent = 2;
                event.accepted = true;
            } else if (event.key === Qt.Key_Up) {
                keyEvent = 4;
                event.accepted = true;
            } else if (event.key === Qt.Key_Down) {
                keyEvent = 8;
                event.accepted = true;
            }

            if (keyEvent !== 0) {
                imageDisplay.source = unitris.tick(keyEvent);
            }
        }


        Label {
            Layout.alignment: Qt.AlignHCenter
            text: "Use keyboard arrow keys to move the tetromino, up to rotate, down to drop"
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
