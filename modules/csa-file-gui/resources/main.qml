/****************************************************************************
**
** Copyright (C) 2014 Dinu SV.
** (contact: mail@dinusv.com)
** This file is part of C++ Snippet Assist application.
**
** GNU General Public License Usage
**
** This file may be used under the terms of the GNU General Public License
** version 3.0 as published by the Free Software Foundation and appearing
** in the file LICENSE.GPL included in the packaging of this file.  Please
** review the following information to ensure the GNU General Public License
** version 3.0 requirements will be met: http://www.gnu.org/copyleft/gpl.html.
**
****************************************************************************/


import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.0
import CSA 1.0

ApplicationWindow {
    width: 600
    height: 600
    visible : true
    color : "#262626"
    title : qsTr("C++ Snippet Assist")

    Rectangle{
        id : consoleWrap
        anchors.top  : parent.top
        anchors.left : parent.left
        anchors.topMargin: 13
        anchors.leftMargin: 80
        width : parent.width - 70
        height : 26
        color : "#262626"

        TextInput{
            id : consoleInput
            anchors.fill: parent
            anchors.margins: 5
            focus : true
            font.pixelSize: 16
            font.family : "Courier New"
            font.weight: Font.Normal
            color : "#fff"
            selectByMouse : true
            selectionColor : "#886631"
            text : command
            onTextChanged : {
                plugins.filter(consoleInput.text)
                if ( text === "" ){
                    pluginList.visible = false
                } else {
                    pluginList.visible = true
                    pluginList.currentIndex = 0
                }
            }

            cursorDelegate: Rectangle{
                width : 9
                color : "#585858"
                opacity : 0.75
            }

            Keys.onReturnPressed: {
                if ( pluginList.visible && pluginList.count > 0 ){
                    event.accepted     = true;
                    selectIndexedItem()

                } else {
                    if (configuredEngine.execute(text)){
                        text = ""
                    }
                }
            }

            Keys.onSpacePressed: {
                if ( event.modifiers & Qt.ControlModifier ){
                    event.accepted = true;
                    selectIndexedItem()
                }
            }

            Keys.onEscapePressed: {
                pluginList.visible = false
            }

            Keys.onDownPressed: {
                pluginList.incrementCurrentIndex()
                event.accepted = true
            }
            Keys.onUpPressed: {
                pluginList.decrementCurrentIndex()
                event.accepted = true
            }

            function selectIndexedItem(){
                text               = pluginList.currentItem.usage
                pluginList.visible = false

                positionCursor()
                positionCursor()
            }

            function positionCursor(){
                if ( text !== "" ){
                    var quoteFlag       = false;
                    var doubleQuoteFlag = false;
                    var selectionBegin  = -1;
                    for( var i = 0; i < text.length; ++i ){
                        switch (text[i]){
                        case '\'' :
                            if ( !doubleQuoteFlag )
                                quoteFlag = !quoteFlag;
                            if ( quoteFlag && i > cursorPosition )
                                selectionBegin = i + 1;
                            break;
                        case '"'  :
                            if ( !quoteFlag )
                                doubleQuoteFlag = !doubleQuoteFlag
                            if ( doubleQuoteFlag && i > cursorPosition )
                                selectionBegin = i + 1;
                            break;
                        }
                        if(i > cursorPosition && !quoteFlag && !doubleQuoteFlag && selectionBegin !== -1){
                            select(selectionBegin, i);
                            return;
                        }
                    }
                }
                if ( cursorPosition === 0 )
                    cursorPosition = text.length - 1;
                else
                    cursorPosition = 0;
            }

            Keys.onTabPressed: {
                positionCursor()
            }

        }
    }
    Image{
        source : "logo.png"
        anchors.top : parent.top
        anchors.topMargin: 14
        anchors.left: parent.left
        anchors.leftMargin: 8
    }

    Text{
        text: ">"
        font.family: "Arial"
        font.pixelSize: 26
        font.weight: Font.Black
        anchors.top : parent.top
        anchors.topMargin: 10
        anchors.left: parent.left
        anchors.leftMargin: 60
        color : "#474545"
    }
    Rectangle{
        id : consoleBorder
        anchors.top :parent.top
        anchors.topMargin: 43
        width : parent.width
        height : 8
        color : "#3a3a3a"
    }

    Rectangle{
        anchors.top : consoleBorder.bottom
        anchors.topMargin: 0
        width : parent.width
        anchors.left: parent.left
        height : parent.height - consoleWrap.height - 23
        clip : true

        Rectangle{
            anchors.left: parent.left
            anchors.top : parent.top
            anchors.topMargin: 0
            width : 43
            height : parent.height
            color : "#d7d7d7"
        }

        Component {
            id: contactDelegate
            Item {
                Rectangle{
                    height : parent.height
                    width : 28
                    color : syntaxTreeModel.selected === model.index ? "#aaa" : "#ccc"
                }

                Text{
                    text : model.line
                    anchors.top : parent.top
                    anchors.topMargin: 3
                    horizontalAlignment: Text.AlignRight
                    width: 22
                    color : "#555"
                }

                Rectangle{
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.leftMargin: 28
                    height : parent.height
                    width : 15
                    color : syntaxTreeModel.selected === model.index ? "#aaa" : "#ccc"
                    Text{
                        anchors.centerIn: parent
                        text: model.isCollapsible ? model.isCollapsed ? '+' : '-' : ''
                        font.pixelSize: 13
                        font.family: "Courier New"
                        color: "#000"
                    }

                    MouseArea{
                        anchors.fill: parent
                        onClicked:{
                            if ( model.isCollapsible )
                                if ( model.isCollapsed ){
                                    syntaxTreeModel.expand(model.index)
                                } else {
                                    syntaxTreeModel.collapse(model.index)
                                }
                        }
                    }
                }

                width: parent.width;
                height: 20

                Rectangle{
                    anchors.top: parent.top
                    anchors.left: parent.left
                    anchors.leftMargin: 43
                    width : parent.width - 43
                    height : parent.height
                    color : "#eeeeee"

                    Text{
                        anchors.left : parent.left
                        anchors.leftMargin: 10 + 15 * model.indent
                        anchors.top: parent.top
                        anchors.topMargin: 3
                        textFormat: Text.RichText;
                        font.pixelSize: 13
                        font.family: "Courier New"
                        color : "#000"
                        text: {
                            if ( syntaxTreeModel.selected === model.index )
                                return '<b><span style="color : #0000cc">' + model.type + ':</span> ' + model.identifier + '</b>';
                            else
                                return '<span style="color : #0000cc">' + model.type + ':</span> ' + model.identifier;
                        }
                    }

                    MouseArea{
                        anchors.fill: parent
                        onDoubleClicked: {
                            codeBase.select(model.breadcrumbs)
                        }
                    }

                }

            }
        }

        ScrollView{
            anchors.fill: parent
            ListView {
                id : syntaxTreeList
                anchors.fill: parent
                anchors.rightMargin: 2
                anchors.bottomMargin: 5
                anchors.topMargin: 0
                boundsBehavior : Flickable.StopAtBounds
                model: syntaxTreeModel
                delegate: contactDelegate
                highlight: Rectangle {
                    color: "#ccc";
                }
            }
        }

        ScrollView{
            anchors.top : parent.top
            anchors.left: parent.left
            anchors.right: parent.right

            height : parent.height > 200 ? parent.height : 200

            ListView{
                id : pluginList
                anchors.fill: parent
                anchors.rightMargin: 2
                anchors.bottomMargin: 5
                anchors.topMargin: 0
                boundsBehavior : Flickable.StopAtBounds
                model: plugins
                visible: false
                delegate: Component{

                    Rectangle{
                        property string usage: model.usage

                        width : parent.width
                        height : 30
                        color : ListView.isCurrentItem ? "#444" : "#3a3a3a"
                        Text{
                            anchors.left: parent.left
                            anchors.leftMargin: 86
                            anchors.verticalCenter: parent.verticalCenter

                            font.pixelSize: 13
                            font.family: "Courier New"

                            color: "#fafafa"
                            text: model.usage
                        }

                        MouseArea{
                            anchors.fill: parent
                        }
                    }
                }

            }
        }
    }
}

