#ifndef MEDIAAPPMAIN_H
#define MEDIAAPPMAIN_H

#include <QObject>
#include <QQmlError>
#include <QQuickView>

#include "../common/header/GlobalHeader.h"
#include "../common/header/message/IpcHeader.h"
#include "../common/header/message/TtsData.h"
#include "../common/header/message/HmiData.h"
#include "../common/header/message/MediaData.h"
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <errno.h>
#include <stdio.h>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <thread>
#include <QThread>
#include <list>

#define AppName "HMI"

struct songsStruct
{
    QString Song ;
    QString Artist ;
    QString Album ;
};

class MediaAppMain : public QObject{
   Q_OBJECT
    Q_PROPERTY(QString textMessageAudio READ textMessageAudio WRITE setTextMessageAudio NOTIFY textMessageAudioChanged)
    Q_PROPERTY(int currentLanguage READ currentLanguage WRITE setCurrentLanguage NOTIFY currentLanguageChanged)
    Q_PROPERTY(std::list<songsStruct> currentSongsList READ currentSongsList WRITE setCurrentSongsList NOTIFY songsListChanged)
public:
    MediaAppMain();
//    explicit MediaAppMain(QObject *parent = 0);
    ~MediaAppMain();


    Q_INVOKABLE void sendButtonSignal(int cmd);
    Q_INVOKABLE void setLangue(int cmd);
    Q_INVOKABLE void playTypeFunction(int cmd);
    Q_INVOKABLE void listSelectFunction(int cmd, int value);

    enum eLanguageValue {
        LANGUAGE_VIETNAMESE = 0x01,
        LANGUAGE_ENGLISH = 0x02,
    };

    struct LangugeInfo_t {
        eLanguageValue current_language;
        uint8_t        supported_language;
    };

    enum eActionValue : int8_t{
        ACTION_PAUSE =      0x01,
        ACTION_RESUME =     0x02,
        ACTION_PREVIOUS =   0x03,
        ACTION_NEXT =       0x04,
    };

    enum eMusicType {
        MUSIC_TYPE_ARTIST =     0x01,
        MUSIC_TYPE_ALBUM =      0x02,
        MUSIC_TYPE_FOLDER =     0x03,
    };

    enum class eResPlayByTypeCode {
        SUCESS =                0x00,
        FAILED_TO_GET_LIST =    0x01,
        WRONG_TYPE =            0x02,
        SERVICE_IS_NOT_READY =  0x03,
    };

    struct ResPlayByType_t {
        eResPlayByTypeCode result;
        char shmPath[SHM_PATH_MAX_LEN];
    };

    enum class eResPlayMp3 {
        SUCESS =                0x00,
        FAILED =                0x01,
        SERIVE_IS_NOT_READY =   0x02,
    };

    enum class eResChangeLanguage {
        SUCESS =                0x00,
        FAILED =                0x01,
        WRONG_VALUE =           0x02,
    };

    enum eStatus {
        STATUS_INITIALIZING =   0x01,
        STATUS_READY =          0x02,
        STATUS_PROCESSING =     0x03,
        STATUS_CLOSING =        0x04,
    };

    enum eHmiResponseMsg {
        IDM_RES_HMI_MESSAGE_START = 0x2000,
        IDM_RES_SPEECH_HMI_PLAY_MUSIC_BY_TYPE,
        IDM_RES_SPEECH_HMI_GET_LIST_BY_TYPE,
        IDM_RES_SPEECH_HMI_CHANGED_LANGUAGE,
        IDM_RES_SPEECH_HMI_PLAY_SONG_LIST,
        IDM_RES_SPEECH_HMI_PLAYBACK_ACTION,
        IDM_RES_SPEECH_HMI_GET_CONFIGURATION,
        IDM_RES_SPEECH_HMI_TTS_CONVERT_TEXT,
        IDM_RES_SPEECH_HMI_AUDIO_PLAY_A_FILE,
        IDM_RES_SPEECH_HMI_GET_LIST_BY_ID,
        IDM_RES_SPEECH_HMI_PLAY_LIST_BY_ID,
        IDM_RES_SPEECH_HMI_PROCESSING_STATUS,
    };

    Q_ENUM(eLanguageValue)
    Q_ENUM(eActionValue)
    Q_ENUM(eMusicType)
    Q_ENUM(eResPlayByTypeCode)
    Q_ENUM(eResPlayMp3)
    Q_ENUM(eResChangeLanguage)
    Q_ENUM(eStatus)
    Q_ENUM(eHmiResponseMsg)

    struct mesg_buffer {
        long mesg_type;
        char mesg_text[1000];
    } message;

    struct MqInfor {
        MqInfor(const char* path, int id) {
            mqPath = path;
            appID = id;
            isReady = false;
            key = -1;
            mqid = -1;
        }

        const char * mqPath;
        int appID;
        bool isReady;
        key_t key;
        int mqid;
        bool isContinue;
        bool isListening;
    }*mqs[MAX_PROC_NAME];

    void AppInit();
    //    char* readShm(char* path, size_t size);
    //    int createShm(char* path, char* data, size_t size);
    //    bool writeShm(char* path, char* data, size_t size);
    void printMsg(int AppID, mesg_buffer* msg);
    void rcvMsg(int appID);
    void openMQ(MqInfor* infor);
    bool sendMsg(int mqid, mesg_buffer* msg, int size);

    QString textMessageAudio();
    void setTextMessageAudio(QString textMessage);

    int currentLanguage();
    void setCurrentLanguage(int m_lang);

    std::list<songsStruct> currentSongsList();
    void setCurrentSongsList(std::list<songsStruct> newList);

signals:
    void textMessageAudioChanged(QString textMessage);
    void currentLanguageChanged(int m_lang);
    void songsListChanged(std::list<songsStruct> newList);
    void addSongToList(QString song, QString artist, QString album);
    void clearList();

private:
    QString m_textMessageAudio = "";
    int m_currentLanguage = 0;
    std::list<songsStruct> m_currentSongsList = {};

};


#endif // MEDIAAPPMAIN_H
