#include "MediaAppMain.h"
#include <QThread>

MediaAppMain::MediaAppMain()
{
    qRegisterMetaType<std::list<songsStruct>>("std::list<songsStruct>");
    mqs[IDP_HMI]            = new MqInfor(MSQ_HMI,                   IDP_HMI);
    mqs[IDP_SPEECH_SERVICE] = new MqInfor(MSQ_SPEECH_SERVICE,        IDP_SPEECH_SERVICE);
    if (mqs[IDP_SPEECH_SERVICE] != nullptr) {
        openMQ(mqs[IDP_SPEECH_SERVICE]);
        if (mqs[IDP_SPEECH_SERVICE]->isReady) {
            printf("Open MQ %s successfully, id = %d!!!\n", mqs[IDP_SPEECH_SERVICE]->mqPath, mqs[IDP_SPEECH_SERVICE]->mqid);
        }
    }
    if (mqs[IDP_HMI] != nullptr) {
        openMQ(mqs[IDP_HMI]);
        if (mqs[IDP_HMI]->isReady) {
            printf("Open MQ %s successfully, id = %d!!!\n", mqs[IDP_HMI]->mqPath, mqs[IDP_HMI]->mqid);
        }
    }

//    if (mqs[IDP_HMI] == nullptr) {
//        printf("Cannot listen to appID = %d, data is null\n", IDP_HMI);
//    }
//    printf("Start listening to %s, mqID = %d\n", mqs[IDP_HMI]->mqPath, mqs[IDP_HMI]->mqid);
//    mesg_buffer msg;
//    mqs[IDP_HMI]->isListening = true;
    //QObject::connect(this, SIGNAL(setTextMessageAudio(QVariant)), mainWindows, SLOT(setTextMessageAudio(QVariant)));

}
MediaAppMain::~MediaAppMain()
{
//    delete myMY_IPC;
}

void MediaAppMain::AppInit()
{
}

void MediaAppMain::listSelectFunction(int cmd, int value)
{
    qDebug() << "listSelectFunction cmd: " << cmd << " --- value: " << value;
    this->message.mesg_type = IDM_REQ_HMI_SPEECH_PLAY_SONG_LIST_BY_ID;
    std::sprintf(this->message.mesg_text, "%c", char(value));
    printf("this->message.mesg_text = %d,  this->message.mesg_type = %d \n", this->message.mesg_text, this->message.mesg_type);
    this->sendMsg(mqs[IDP_SPEECH_SERVICE]->mqid, &this->message, sizeof(message));
}


void MediaAppMain::playTypeFunction(int cmd)
{
    qDebug() << "playTypeFunction cmd: " << cmd;
    this->message.mesg_type = IDM_REQ_HMI_SPEECH_PLAY_MUSIC_BY_TYPE;
    std::sprintf(this->message.mesg_text, "%c", char(cmd));
    printf("this->message.mesg_text = %d,  this->message.mesg_type = %d \n", this->message.mesg_text, this->message.mesg_type);
    this->sendMsg(mqs[IDP_SPEECH_SERVICE]->mqid, &this->message, sizeof(message));
}

void MediaAppMain::setLangue(int cmd)
{
    qDebug() << "setLangue cmd: " << cmd;
    this->message.mesg_type = IDM_REQ_HMI_SPEECH_CHANGE_LANGUAGE;
    std::sprintf(this->message.mesg_text, "%c", char(cmd));
    printf("this->message.mesg_text = %d,  this->message.mesg_type = %d \n", this->message.mesg_text, this->message.mesg_type);
    this->sendMsg(mqs[IDP_SPEECH_SERVICE]->mqid, &this->message, sizeof(message));
}

void MediaAppMain::sendButtonSignal(int cmd)
{
    qDebug() << "cmd: " << cmd;
    this->message.mesg_type = IDM_REQ_HMI_SPEECH_PLAYBACK_ACTION;
    std::sprintf(this->message.mesg_text, "%c", char(cmd));
    printf("this->message.mesg_text = %d,  this->message.mesg_type = %d \n", this->message.mesg_text, this->message.mesg_type);
    this->sendMsg(mqs[IDP_SPEECH_SERVICE]->mqid, &this->message, sizeof(message));
}




void MediaAppMain::printMsg(int AppID, mesg_buffer* msg) {
    if (msg == nullptr) {
        printf("Cannot print, message is null!!! \n");
        return;
    }

    switch (AppID) {
    case IDP_HMI:
    {
         printf("message = %ld, type = %ld \n",  msg->mesg_text[0], msg->mesg_type);

         char mText[1000];
         std::sprintf(mText, "message = %ld, type = %ld \n", msg->mesg_text[0], msg->mesg_type);
         //QString m_text = "111111";
         //setTextMessageAudio(mText);

         switch (msg->mesg_type)
         {
             case IDM_RES_SPEECH_HMI_PROCESSING_STATUS:
             {
                setTextMessageAudio("IDM_RES_SPEECH_HMI_PROCESSING_STATUS");
             }
                 break;
             case IDM_RES_HMI_MESSAGE_START:
             {
                setTextMessageAudio("IDM_RES_HMI_MESSAGE_START");
             }
                 break;
             case IDM_RES_SPEECH_HMI_PLAY_MUSIC_BY_TYPE:
             {
                setTextMessageAudio("IDM_RES_SPEECH_HMI_PLAY_MUSIC_BY_TYPE");
             }
                 break;
             case IDM_RES_SPEECH_HMI_GET_LIST_BY_TYPE:
             {
                setTextMessageAudio("IDM_RES_SPEECH_HMI_GET_LIST_BY_TYPE");
             }
                 break;
             case IDM_RES_SPEECH_HMI_CHANGED_LANGUAGE:
             {
                setTextMessageAudio("IDM_RES_SPEECH_HMI_CHANGED_LANGUAGE");
             }
                 break;
             case IDM_RES_SPEECH_HMI_PLAY_SONG_LIST:
             {
                setTextMessageAudio("IDM_RES_SPEECH_HMI_PLAY_SONG_LIST");
             }
                 break;
             case IDM_RES_SPEECH_HMI_PLAYBACK_ACTION:
             {
                setTextMessageAudio("IDM_RES_SPEECH_HMI_PLAYBACK_ACTION");
             }
                 break;
             case IDM_RES_SPEECH_HMI_GET_CONFIGURATION:
             {
                setTextMessageAudio("IDM_RES_SPEECH_HMI_GET_CONFIGURATION");
             }
                 break;
             case IDM_RES_SPEECH_HMI_TTS_CONVERT_TEXT:
             {
                setTextMessageAudio("IDM_RES_SPEECH_HMI_TTS_CONVERT_TEXT");
             }
                 break;
             case IDM_RES_SPEECH_HMI_AUDIO_PLAY_A_FILE:
             {
                setTextMessageAudio("IDM_RES_SPEECH_HMI_AUDIO_PLAY_A_FILE");
             }
                 break;
             case IDM_RES_SPEECH_HMI_GET_LIST_BY_ID:
             {
                setTextMessageAudio("IDM_RES_SPEECH_HMI_GET_LIST_BY_ID");
             }
                 break;
             case IDM_RES_SPEECH_HMI_PLAY_LIST_BY_ID:
             {
                setTextMessageAudio("IDM_RES_SPEECH_HMI_PLAY_LIST_BY_ID");
             }
                 break;
         }
    }
        break;
    case IDP_WATCHDOG:
    {
    }
        break;

    case IDP_TTS_SERVICE:
    {
    }
        break;

    case IDP_AUDIO_SERVICE:
    {
    }
        break;

    case IDP_MEDIA_SERVICE:
    {
    }
        break;

    case IDP_SPEECH_SERVICE:
    {
    }
        break;
    default:
        printf("Wrong app ID");
        break;
    }
}

void MediaAppMain::rcvMsg(int appID) {
    if (mqs[appID] == nullptr) {
        printf("Cannot listen to appID = %d, data is null\n", appID);
    }
    printf("Start listening to %s, mqID = %d\n", mqs[appID]->mqPath, mqs[appID]->mqid);
    mesg_buffer msg;
    mqs[appID]->isListening = true;
    while (1)
    {
        int result = msgrcv(mqs[appID]->mqid, &msg, sizeof(msg), 0, IPC_NOWAIT);
        if (result == -1) {// error case
             int errsv = errno;
             if (ENOMSG != errno) {//Not is an empty queue
                printf("[%s]sendMsg error, error code = %d \n", AppName, errsv);
                //break;
             } else {
                 usleep(20000); //sleep for 20ms
             }
        } else {
            printf("Received a message ID = %d, size = %d \n", mqs[appID]->mqid, result);
            printMsg(appID, &msg);
        }
        sleep(0.1);
    }
    printf("Stop listening, isContinue = %s, isReady = %s \n",
           (mqs[appID]->isContinue ? "true" : "false"),
           (mqs[appID]->isReady ? "true" : "false"));
    //pthread_exit(NULL);
}

void MediaAppMain::openMQ(MqInfor* infor) {
    if (infor == nullptr)
        return;
    infor->isReady = false;
    infor->key = ftok(infor->mqPath, 65);
    if (infor->key == -1) {// error case
         int errsv = errno;
         printf("[%s]openMQ %s getting key is failed, error code = %d \n", AppName, infor->mqPath, errsv);
         return;
    }
    infor->mqid = msgget(infor->key, 0666 | IPC_CREAT);
    if (infor->mqid == -1) {// error case
         int errsv = errno;
         printf("[%s]openMQ %s get message queue ID is failed, error code = %d \n", AppName, infor->mqPath, errsv);
         return;
    }
    infor->isReady = true;
}

bool MediaAppMain::sendMsg(int mqid, mesg_buffer* msg, int size) {
    printf("Send a message ID = %d, size = %d, type = %ld \n", mqid, size, msg->mesg_type);
    int result = msgsnd(mqid, msg, size, 0);
    if (result == -1) {// error case
         int errsv = errno;
         printf("[%s]sendMsg error, error code = %d \n", AppName, errsv);
         return false;
    }
    return true;
}


QString MediaAppMain::textMessageAudio()
{
    return m_textMessageAudio;
}

void MediaAppMain::setTextMessageAudio(QString textMessage)
{
    //qDebug() << "setTextMessageAudio textMessage = " << textMessage;
    if(m_textMessageAudio != textMessage)
    {
        m_textMessageAudio = textMessage;
        setCurrentLanguage((m_currentLanguage+1));
        emit clearList();
        songsStruct a;
        a.Album = "asdadada";
        a.Artist = "sadsasd";
        a.Song = "asdasdsaas";
        addSongToList(a.Album, a.Artist, a.Song);
        a.Album = "asdadada";
        a.Artist = "sadsasd";
        a.Song = "asdasdsaas";
        addSongToList(a.Album, a.Artist, a.Song);
        a.Album = "asdadada";
        a.Artist = QString(m_currentLanguage);
        a.Song = "asdasdsaas";
        addSongToList(a.Album, a.Artist, a.Song);
        addSongToList(a.Album, a.Artist, a.Song);
        addSongToList(a.Album, a.Artist, a.Song);

        //qDebug() << "setTextMessageAudio m_textMessageAudio changed to " << m_textMessageAudio;
        emit textMessageAudioChanged(textMessage);
    }
    return;
}


void MediaAppMain::setCurrentLanguage(int m_lang)
{
    qDebug() << "changeLanguage to: " << m_lang;
    if(m_lang != m_currentLanguage)
    {
        m_currentLanguage = m_lang;
        emit currentLanguageChanged(m_currentLanguage);
    }
}

int MediaAppMain::currentLanguage()
{
    return m_currentLanguage;
}



void MediaAppMain::setCurrentSongsList(std::list<songsStruct> newList)
{
    //qDebug() << "songsListChanged to: " << newList;
    //if(newList == m_currentSongsList)
    {
        m_currentSongsList = newList;
        emit songsListChanged(m_currentSongsList);
    }
}

std::list<songsStruct> MediaAppMain::currentSongsList()
{
    return m_currentSongsList;
}
