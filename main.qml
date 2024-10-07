import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.3
import QtQuick.Dialogs 1.3
import CustomPlot 1.0
import QtQuick.Controls.Styles 1.4

Window {
    width: 800
    height: 600
    visible: true
    title: qsTr("Histogram")
property QtObject uiManager: UiManager
    Connections {
         target: UiManager
         onShowWarning:{
             messageDialog.setText(str)
             messageDialog.visible=true
         }
         onChangeStatus:
         {
             progressBar.value = value/100.;
         }
    }

    GridLayout {
        id: gridLayout
        anchors.fill: parent
        rows: 3
        columns: 2
        CustomPlotItem {
            id: customPlot
            Layout.columnSpan: 2
            Layout.preferredHeight: 400
            Layout.preferredWidth: parent.width-20
            Component.onCompleted: {

                initCustomPlot()
                UiManager.setPlot(this);
            }
        }
        ProgressBar {
            id: progressBar
            value: 0
            Layout.columnSpan: 2
            Layout.preferredHeight: 60
            Layout.preferredWidth: parent.width-20
            Layout.alignment: Qt.AlignCenter
            contentItem: Item {
                anchors.fill: parent
                Rectangle{
                    anchors.fill: parent
                    color: "gray"
                    radius: 10
                }
                Rectangle{
                    width: progressBar.width*progressBar.value
                    height: progressBar.height
                    color: "#00B000"
                    radius: 10
                }
            }
        }
        Button {
            id: buttonPath
            text: qsTr("Choose a path")
            Layout.alignment: Qt.AlignCenter
            Layout.bottomMargin: 15
            onClicked: {
                fileDialog.visible = true
            }
        }
        Button {
            id: buttonStartParse
            text: qsTr("Start")
            Layout.alignment: Qt.AlignCenter
            Layout.bottomMargin: 15
            onClicked: {
                progressBar.value = 0;
                uiManager.startParse(uiManager.getPath());
            }
        }
    }
    MessageDialog {
        id: messageDialog
           title: qsTr("Warning")
           text: ""
           onAccepted: {
               console.log("And of course you could only agree.")
               this.visible = false
           }
           Component.onCompleted: visible = false
    }
    FileDialog {
        id: fileDialog
        title: qsTr("Please choose a file")
        folder: shortcuts.home
        onAccepted: {
            uiManager.setPath(fileUrl);
            fileDialog.visible = false
        }
        onRejected: {
            fileDialog.visible = false
        }
        Component.onCompleted: visible = false
    }
}
