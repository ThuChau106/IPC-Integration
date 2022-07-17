import QtQuick 2.9
import QtQuick.Window 2.2
import QtQuick.Controls 1.6
import com.MediaAppMain 1.0
//import com.MY_IPC 1.0

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("MEDIA PLAYER")
    id: mainWindows

    MediaAppMain {
        id: myMediaAppMain
    }

    property int    current_langue_TTS: 0
    property int    current_mode_play: 0
    property string olderTextFromCPP: "Mess1"
    property string textFromCPP: "Mess2"
    property var listSong

//    onTextFromCPPChanged: {
//        console.log("onTextFromCPPChanged ", textFromCPP);
//    }

    Connections {
        target: MediaAppMainCore
        onTextMessageAudioChanged: {
            console.log("setTextMessageAudio Connections ", textMessage);
            olderTextFromCPP = textFromCPP;
            textFromCPP = textMessage;
        }
        onCurrentLanguageChanged: {
            current_langue_TTS = m_lang
        }
        onClearList: {
            console.log("onClearList newList ");
            contactsModel.clear();
        }
        onAddSongToList:{
            console.log("onAddSongToList onAddSongToList ");
            contactsModel.append({"Song": song, "Artist": artist, "Album": album})
        }
    }

    Rectangle {
        id: rectangle
        x: 30
        y: 10
        width: 572
        height: 60
        color: "#34eef9"

        Text {
            id: text_voice_message
            x: 31
            y: 10
            width: 510
            height: 33
//            text: qsTr("Text when receive message from service")
            text: " Old: " + olderTextFromCPP + "\n" + "New: " + textFromCPP
            font.pixelSize: 12
            Component.onCompleted: {
                console.log("onCompleted ", textFromCPP);
            }
        }
    }

    Item {
        x: 176
        y: 139
        width: 446
        height: 232

        ListView {
            id: contactsView
//            x: 176
//            y: 139
            width: 446
            height: 232
            spacing: 10
            model: contactsModel
            delegate: contactDelegate
            highlight: Rectangle { color: "lightsteelblue"; radius: 5; focus: true}
        }

        Component {
            id: contactDelegate
            Item {
                width: 400; height: 50
                Column {
                    Text { text: '<b>Song:</b> ' + Song }
                    Text { text: '<b>Artist:</b> ' + Artist }
                    Text { text: '<b>Album:</b> ' + Album }
                }
                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        contactsView.currentIndex = index
                        console.log("bt_play ");
                        myMediaAppMain.listSelectFunction(current_mode_play, index);
                    }
                }
            }
        }
        ListModel {
            id: contactsModel
            ListElement {
                Song: "Bill Smith11"
                Artist: "Engineer"
                Album: "Album"
            }
            ListElement {
                Song: "John Brown"
                Artist: "Engineer"
                Album: "Album"
            }
            ListElement {
                Song: "Sam Wise"
                Artist: "Manager"
                Album: "Album"
            }
        }
    }

    Button {
        id: bt_play
        x: 59
        y: 139
        width: 100
        height: 25
        text: qsTr("Play")
        onClicked: {
            console.log("bt_play ");
            myMediaAppMain.sendButtonSignal(MediaAppMain.ACTION_RESUME);
        }
    }

    Button {
        id: bt_pause
        x: 59
        y: 173
        width: 100
        height: 25
        text: qsTr("Pause")
        onClicked: {
            console.log("bt_pause ");
            myMediaAppMain.sendButtonSignal(MediaAppMain.ACTION_PAUSE);
        }
    }

    Button {
        id: bt_next
        x: 59
        y: 207
        width: 100
        height: 25
        text: qsTr("Next")
        onClicked: {
            console.log("bt_next ");
            myMediaAppMain.sendButtonSignal(MediaAppMain.ACTION_NEXT);
        }
    }

    Button {
        id: bt_prev
        x: 59
        y: 241
        width: 100
        height: 25
        text: qsTr("Previous")
        onClicked: {
            console.log("bt_prev ");
            myMediaAppMain.sendButtonSignal(MediaAppMain.ACTION_PREVIOUS);
        }
    }

    Text {
        id: text_current_langue
        x: 20
        y: 435
        width: 150
        height: 35
        text: qsTr((("Current langue is ") + ((current_langue_TTS == 0) ? "undefined" : ( (current_langue_TTS == 1) ? "Vietnamese" : "English"))))
        font.pixelSize: 12
    }

    Button {
        id: bt_setLangVN
        x: 20
        y: 375
        width: 150
        height: 25
        text: qsTr("Set langue to Viet Nam")
        onClicked: {
          console.log("bt_setLangVN ");
            myMediaAppMain.setLangue(MediaAppMain.LANGUAGE_VIETNAMESE);
        }
    }

    Button {
        id: bt_setLangEN
        x: 20
        y: 405
        width: 150
        height: 25
        text: qsTr("Set langue to English")
        onClicked: {
          console.log("bt_setLangEN ");
          myMediaAppMain.setLangue(MediaAppMain.LANGUAGE_ENGLISH);
        }
    }

    Button {
        id: bt_artist
        x: 184
        y: 395
        width: 100
        height: 25
        text: qsTr("Play Artist")
        onClicked: {
          console.log("bt_artist ");
          myMediaAppMain.playTypeFunction(MediaAppMain.MUSIC_TYPE_ARTIST);
            current_mode_play = MediaAppMain.MUSIC_TYPE_ARTIST;
        }
    }

    Button {
        id: bt_album
        x: 336
        y: 395
        width: 100
        height: 25
        text: qsTr("Play Album")
        onClicked: {
          console.log("bt_album ");
          myMediaAppMain.playTypeFunction(MediaAppMain.MUSIC_TYPE_ALBUM);
            current_mode_play = MediaAppMain.MUSIC_TYPE_ALBUM;
        }
    }

    Button {
        id: bt_folder
        x: 486
        y: 395
        width: 100
        height: 25
        text: qsTr("Open Folder")
        onClicked: {
          console.log("bt_folder ");
          myMediaAppMain.playTypeFunction(MediaAppMain.MUSIC_TYPE_FOLDER);
            current_mode_play = MediaAppMain.MUSIC_TYPE_FOLDER;
        }
    }

    Text {
        id: text2
        visible: false
        x: 184
        y: 108
        width: 412
        height: 25
        text: qsTr("NAME ------------------- ARTIST ------------------ ALBUM")
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
        font.pixelSize: 12
    }
}
