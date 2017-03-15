import QtQuick 2.5
import QtQuick.Window 2.2
import QtQuick.Controls 1.4
import QtQuick.Dialogs 1.0
import QGraph 1.1
//import "./filedialog.qml"

ApplicationWindow {
    visible: true;
    width: 640;
    height: 400;
    title: qsTr("Graph Analyse");

    Graph{
        id : graph
    }

    FileDialog {
        id: filebrowser
        title: "Please choose a file"
        folder: shortcuts.home
        onAccepted: {
            console.log("You chose: " + filebrowser.fileUrls)
            menu_openfile.fileChosen()
        }
        onRejected: {
            console.log("Canceled")
        }
        //Component.onCompleted: visible = true
    }

    menuBar: MenuBar {
            Menu {
                title: "   File   "
                MenuItem {
                    id: menu_openfile
                    text: "Open..."
                    signal fileChosen
                    onTriggered: {
                          filebrowser.open()
//                        var com = Qt.createComponent("filedialog.qml");
//                        var filebrowser = com.createObject("fileDialog");
//                        console.log("here:"+com.url);
//                        console.log("here:"+filebrowser.fileUrls.fileName);
//                        graph.url = filebrowser.fileUrls;
//                        console.log(graph.node_amount);
//                        console.log(graph.url);
                    }
                    onFileChosen: {
                        var file = filebrowser.fileUrl.toString()
                        file = file.replace("file:///","")
                        file = file.replace(/\//g,'\\')
                        console.log("here:"+file)
                        graph.url = file;
                      //  console.log(graph.url+"|end");
                        console.log(graph.node_amount);
                    }

                }
                MenuItem { text: "Close" }
            }
            Menu {
                title: "   Centralarity Analyse   "
                MenuItem { text: "PageRank" }
            }
            Menu{
                title: "   Community Detection   "
                MenuItem {text: "Gravain-Newman Algorithm"}
                MenuItem {text: "FastUfolding Algorithm"}
            }
    }

    ListModel {
        id :graph_list
        ListElement {
            v1 : "0"
            v2 : "1 2 3 4 5 6 7"
            weight: "0"
        }
    }
    ListModel{
        id : graph_partition
    }
    ListModel{
        id: centrality
    }

    TableView {
        id : adjacent_table
        width: parent.width*2/3
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        TableViewColumn{
            role:"v1"
            title:"V1"
            width: adjacent_table.width/3
        }
        TableViewColumn{
            role: "v2"
            title:"V2"
            width: adjacent_table.width/3
        }
        TableViewColumn{
            role: "weight"
            title: "Weight"
            width: adjacent_table.width/3
        }
        model: graph_list
//        visible: false
    }
    TableView{
        id : partition_view
        width: parent.width*1/3
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        TableViewColumn{
            role : "cmty"
            title : "Community ID"
            width :partition_view.width/3
        }
        TableViewColumn{
            role : "members"
            title : "Members"
            width :partition_view.width/2
        }
        TableViewColumn{
            role : "size"
            title : "Comunity Size"
            width :partition_view.width/6
        }
        model:graph_partition
//        visible: false
    }
    TableView{
        id : keynode_view
        width: parent.width*1/3
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        TableViewColumn{
            role : "num"
            title : "NO."
            width :keynode_view.width/3
        }
        TableViewColumn{
            role : "node"
            title : "Node ID"
            width :keynode_view.width/3
        }
        TableViewColumn{
            role : "pr"
            title : "PageRank"
            width :keynode_view.width/3
        }
        model:centrality
        visible: false
    }
}

