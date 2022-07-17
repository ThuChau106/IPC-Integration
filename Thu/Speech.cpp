#include "../common/header/GlobalHeader.h"
#include "../common/header/message/IpcHeader.h"
#include "../common/header/message/TtsData.h"
#include "../common/header/message/HmiData.h"
#include "../common/header/message/MediaData.h"

#include <errno.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <thread>

#define AppName "Speech"
static eStatus status; // this variable is used to save current status of system
static eFeature feature; // this variable is used to save current feature of system
static bool isReceive; // this variabl is used to identify status receive from other services

// structure for message
struct mesg_buffer {
    long mesg_type;
    char mesg_text[1000];
} message;

// structure for message queue
struct MqInfor {
    MqInfor(const char* path, int id) {
        mqPath = path;
        appID = id;
        isReady = false;
        key = -1;
        mqid = -1;
        isContinue = true;
        isListening = false;
    }

    const char * mqPath;
    int appID;
    bool isReady;
    key_t key;
    int mqid;
    bool isContinue;
    bool isListening;
}*mqs[MAX_PROC_NAME];

// Open message queue
void openMQ(MqInfor* infor) {
    if (infor == nullptr)
        return;

    infor->isReady = false;

    // ftok to generate unique key
    infor->key = ftok(infor->mqPath, 65);

    if (infor->key == -1) {// error case
         int errsv = errno;
         printf("[%s]openMQ %s getting key is failed, error code = %d \n", AppName, infor->mqPath, errsv);
         return;
    }

    // msgget creates a message queue
    // and returns identifier
    infor->mqid = msgget(infor->key, 0666 | IPC_CREAT);
    if (infor->mqid == -1) {// error case
         int errsv = errno;
         printf("[%s]openMQ %s get message queue ID is failed, error code = %d \n", AppName, infor->mqPath, errsv);
         return;
    }

    infor->isReady = true;
}

// Send message 
bool sendMsg(int mqid, mesg_buffer* msg, int size) {
    printf("Send a message ID = %d, size = %d, type = %d \n", mqid, size, msg->mesg_type);
    int result = msgsnd(mqid, msg, size, 0);
    if (result == -1) {// error case
         int errsv = errno;
         printf("[%s]sendMsg error, error code = %d \n", AppName, errsv);
         return false;
    }

    return true;
}

// Check other services are ready or not?
bool isCheckReady(){
    int i = 1;
    while (i < 10){
        sleep(3);
        if (isReceive == true){
            return true;
        }
        printf("Counter: %d\n", i++);
    }
    return false;    
}

// Send a value message to HMI (char)
void send_ValueMessage_HMI(char value, int msg_type, char statusValue){
    message.mesg_type = msg_type;
    std::memcpy(message.mesg_text, &value, sizeof (value) + 1);
    if (sendMsg(mqs[IDP_HMI]->mqid, &message, sizeof(message))) {
        printf("Sent a message to HMI \n");
        status = eStatus(statusValue);
    }    
} 

// Send a TTSInfo_t Struct to HMI
void send_TtsInfo_TTS(TtsInfo_t msg){
    message.mesg_type = IDM_REQ_SPEECH_TTS_CONVERT_TEXT;
    std::memcpy(message.mesg_text, &msg, sizeof (TtsInfo_t));
    isReceive = false;
    if (sendMsg(mqs[IDP_TTS_SERVICE]->mqid, &message, sizeof(message))) {
        printf("Sent a message to TTS Service \n");
    }
}

// Send a ResplayByType_t struct to HMI
void send_ResplayByType_HMI(ResPlayByType_t resPlayByType, eStatus setStatus){
    message.mesg_type = IDM_RES_SPEECH_HMI_GET_LIST_BY_TYPE;
    std::memcpy(message.mesg_text, &resPlayByType, sizeof (ResPlayByType_t));
    if (sendMsg(mqs[IDP_HMI]->mqid, &message, sizeof(message))) {
        printf("Sent a message to HMI \n");
        status = setStatus;
    }
}

// Send mp3 file to Audio
void send_mp3File_Audio(SongInfo_t* info){
    message.mesg_type = IDM_REQ_SPEECH_AUDIO_PLAY_A_FILE;
    std::memcpy(message.mesg_text, info, sizeof (SongInfo_t));
    isReceive = false;
    if (sendMsg(mqs[IDP_AUDIO_SERVICE]->mqid, &message, sizeof(message))) {
        printf("Sent a message to Audio \n");
    }   
}

// Send a SongList_t struct to HMI
void send_SongList_HMI(TypeList_t songList){
    message.mesg_type = IDM_RES_SPEECH_HMI_GET_LIST_BY_ID;
    std::memcpy(message.mesg_text, &songList, sizeof (message.mesg_text) + 1);
    if (sendMsg(mqs[IDP_HMI]->mqid, &message, sizeof(message))) {
        printf("Sent a message to HMI \n");
        if (songList.result != eResPlayByTypeCode::SUCESS){
            status = eStatus::STATUS_READY;
        }
    }
}

// Send a value message to Audio
void send_ValueMessage_Audio(char actionValue, int msg_type){
    message.mesg_type = msg_type;
    std::memcpy(message.mesg_text, &actionValue, sizeof (actionValue) + 1);
    isReceive = false;
    if (sendMsg(mqs[IDP_AUDIO_SERVICE]->mqid, &message, sizeof(message))) {
        printf("Sent a message to Audio Service \n");
    }
}

// Send a value message to Media (char)
void send_ValueMessage_Media(char musicType, int msg_type){
    message.mesg_type = msg_type;
    std::memcpy(message.mesg_text, &musicType, sizeof (musicType) + 1);
    isReceive = false;
    if (sendMsg(mqs[IDP_MEDIA_SERVICE]->mqid, &message, sizeof(message))) {
        printf("Sent a message to Media Service \n");
    } 
}

// Handle message - get current configuration
void h_reqGetConfiguration_HMI(){
    printf("Sending a message: response HMI about get configuration\n");
    // read file current_language.txt
    FILE * fp = NULL;
    fp = fopen("../config/current_language.txt", "r");
    if (fp == NULL){
        printf("The file current_language.txt can not opened\n");
        char current_language = eLanguageValue::LANGUAGE_START;
        send_ValueMessage_HMI(current_language, IDM_RES_SPEECH_HMI_GET_CONFIGURATION, eStatus::STATUS_READY);
    }else {
        char current_language = eLanguageValue(fgetc(fp));
        send_ValueMessage_HMI(current_language, IDM_RES_SPEECH_HMI_GET_CONFIGURATION, eStatus::STATUS_READY);
    }
}

// Speech receive a message: HMI request Speech to get current configutation of system
void reqGetConfiguration_HMI(mesg_buffer* msg){
    if (status == eStatus::STATUS_READY){
        status = eStatus::STATUS_PROCESSING;
        printf("Data is HMI request Speech to get configuration\n");
        h_reqGetConfiguration_HMI();
    }
    else{
        printf("The system is busy\nData is HMI request Speech to get configuration\n");
    }
}

// Identify play music type by artist, album or folder.
char* requestText(eMusicType type){
    char *s;
    if (type == eMusicType::MUSIC_TYPE_ALBUM) s = "Which album do you want to play?";
    else if (type == eMusicType::MUSIC_TYPE_ARTIST) s = "Which artist do you want to play?";
        else if (type == eMusicType::MUSIC_TYPE_FOLDER) s = "Which folder do you want to play?";
    return s;
}

// Speech request TTS to convert text to speech
void reqTTSCovertText(char *s){
    printf("Sending a message:Speech request TTS Service to convert text \n");
    {
        TtsInfo_t msg;
        FILE * fp = NULL;
        fp = fopen("../config/current_language.txt", "r");
        if (fp == NULL){
            printf("The file current_language.txt can not opened\n");
            send_ValueMessage_HMI(eTTSResponse::CANNOT_OPEN_FILE, IDM_RES_SPEECH_HMI_TTS_CONVERT_TEXT, eStatus::STATUS_READY);
        }else{
            msg.language = eLanguageValue(fgetc(fp)-48);
            printf("Language is: %d\n", msg.language);
            fclose(fp);
            sprintf(msg.text, "%s", s);
            printf("Language: %d\n ", msg.language);
            printf("Text: %s\n", msg.text);

            char processingStatus = eProcessingStatus::STATUS_PROCESSING_CONVERT_TEXT;
            send_ValueMessage_HMI(processingStatus,IDM_RES_SPEECH_HMI_PROCESSING_STATUS, eStatus::STATUS_PROCESSING);

            send_TtsInfo_TTS(msg);

            if (isCheckReady() == 0){
                eTTSResponse result = eTTSResponse::SERVICE_IS_NOT_READY;
                send_ValueMessage_HMI(result, IDM_RES_SPEECH_HMI_TTS_CONVERT_TEXT, eStatus::STATUS_READY);
            }           
        }
    }
}

// Handle message - request change language
void h_reqChangeLanguage_HMI(char changed_language){
    status = eStatus::STATUS_PROCESSING;
    printf("Sending a message: request change language\n");
    eResChangeLanguage resChangeLanguage;
    if (changed_language > eLanguageValue::LANGUAGE_START && changed_language < eLanguageValue::MAX_LANGUAGE){
        FILE * fp = NULL;
        fp = fopen("../config/current_language.txt", "w");
        if (fp == NULL){
            resChangeLanguage = eResChangeLanguage::FAILED;
            printf("The file current_language.txt can not opened\n");
            send_ValueMessage_HMI(char(resChangeLanguage), IDM_RES_SPEECH_HMI_CHANGED_LANGUAGE,eStatus::STATUS_READY);
        }else {
            fprintf(fp,"%d",changed_language);
            resChangeLanguage = eResChangeLanguage::SUCESS;
            fclose(fp);
            send_ValueMessage_HMI(char(resChangeLanguage), IDM_RES_SPEECH_HMI_CHANGED_LANGUAGE, eStatus::STATUS_PROCESSING);
            reqTTSCovertText("Changed language successfully");
        }

    }
    else {
        resChangeLanguage = eResChangeLanguage::WRONG_VALUE;
        send_ValueMessage_HMI(char(resChangeLanguage), IDM_RES_SPEECH_HMI_CHANGED_LANGUAGE,eStatus::STATUS_READY);
    }
}

// Speech receive a message - HMI request Speech to change language
void reqChangeLanguage_HMI(mesg_buffer* msg){
    if (status == eStatus::STATUS_READY){
        printf("Data is HMI request speech to change language\n");
        char changed_language;
        std::memcpy(&changed_language, msg->mesg_text, sizeof (changed_language));
        printf("Changed language is: %d \n", changed_language);   
        h_reqChangeLanguage_HMI(changed_language);
    }
    else{
        printf("The system is busy\nData is HMI request speech to change language\n"); 
    }
}

// Handle message - request to play music by type
void h_reqPlayMusicByType_HMI(char musicType){
    printf("Sending a message: response play music by type \n");
    {
        if (musicType > eMusicType::MUSIC_TYPE_START && musicType < eMusicType::MAX_MUSIC_TYPE){ 

            char processingStatus = eProcessingStatus::STATUS_PROCESSING_GET_LIST_BY_TYPE;
            send_ValueMessage_HMI(processingStatus, IDM_RES_SPEECH_HMI_PROCESSING_STATUS, eStatus::STATUS_PROCESSING);

            send_ValueMessage_Media(musicType, IDM_REQ_SPEECH_MEDIA_GET_LIST_BY_TYPE);

            if (isCheckReady() == 0){
                ResPlayByType_t resPlayByType;
                resPlayByType.result = eResPlayByTypeCode::SERVICE_IS_NOT_READY;
                send_ResplayByType_HMI(resPlayByType, eStatus::STATUS_READY);
            }
        }
        else{
            ResPlayByType_t resPlayByType;
            resPlayByType.result = eResPlayByTypeCode::WRONG_TYPE;
            send_ResplayByType_HMI(resPlayByType, eStatus::STATUS_READY);
        }
    }
}

// HMI request speech to play music by type
void reqPlayMusicByType_HMI(mesg_buffer* msg){
    if (status == eStatus::STATUS_READY)
    {
        feature = eFeature::PLAY_MUSIC_BY_TYPE;
        printf("Data is HMI request Speech to play music by Type");    
        char musicType;
        std::memcpy(&musicType, msg->mesg_text, sizeof (musicType));
        printf("Music type is: %d \n", musicType);
        h_reqPlayMusicByType_HMI(musicType);
    }
    else {
        printf("The system is busy\nData is HMI request Speech to play music by Type");   
    }

}

// Handle message - reponse get list of artist/folder/album
void h_resGetListByType_Media(TypeList_t typeList){
    printf("Sending a message: Speech response HMI about get list by type\n");
    ResPlayByType_t resPlayByType;
    if (typeList.result == eResPlayByTypeCode::FAILED_TO_GET_LIST){
        resPlayByType.result = eResPlayByTypeCode::FAILED_TO_GET_LIST;  
        send_ResplayByType_HMI(resPlayByType, eStatus::STATUS_READY);
    }
    else if (typeList.shmPath != ""){
        resPlayByType.result = eResPlayByTypeCode::SUCESS;
        strncpy(resPlayByType.shmPath, typeList.shmPath, sizeof(typeList.shmPath));
        send_ResplayByType_HMI(resPlayByType, status);
        reqTTSCovertText(requestText(typeList.type));
    }

}

// Speech receive message from Media – Media response Speech about get list of artist/folder/album
void resPlayMusicByType_Media(mesg_buffer* msg){
    printf("Data is media response Speech about get list by type: \n");
    TypeList_t typeList;
    std::memcpy(&typeList, msg->mesg_text, sizeof (typeList));
    h_resGetListByType_Media(typeList);    
}

// Handle message - reponse convert text to speech
void h_resConvertText_TTS(TtsResponse_t ttsRes){
    printf("Sending a message: Response TTS convert \n");
    eTTSResponse result = ttsRes.result;
    if (result == eTTSResponse::FAILED_TO_TRANSFER_TO_SPEECH){
        send_ValueMessage_HMI(result, IDM_RES_SPEECH_HMI_TTS_CONVERT_TEXT, eStatus::STATUS_READY);
    }
    else if (result == eTTSResponse::SUCESSFULLY_TRANSFER_TO_SPEECH){
        SongInfo_t* info = new SongInfo_t();
        strncpy(info->path, ttsRes.mp3FilePath, sizeof(ttsRes.mp3FilePath));

        char processingStatus = eProcessingStatus::STATUS_PROCESSING_PLAY_A_FILE;
        send_ValueMessage_HMI(processingStatus,IDM_RES_SPEECH_HMI_PROCESSING_STATUS, eStatus::STATUS_PROCESSING);
        
        send_mp3File_Audio(info);

        if (isCheckReady() == 0){
            eResPlayMp3 result = eResPlayMp3::SERIVE_IS_NOT_READY;
            send_ValueMessage_HMI((char)result, IDM_RES_SPEECH_HMI_AUDIO_PLAY_A_FILE, eStatus::STATUS_READY);
        }
    }
}

// Speech receive message from TTS - TTS response Speech about convert text to speech
void resConvertText_TTS(mesg_buffer* msg){
    TtsResponse_t ttsRes;
    std::memcpy(&ttsRes, msg->mesg_text, sizeof (ttsRes));
    printf("Result is: %d \n", ttsRes.result);
    printf("Mp3 Path is: %s \n", ttsRes.mp3FilePath);
    h_resConvertText_TTS(ttsRes);
}

// Handle message - reponse play audio
void h_resPlayFile_Audio(char result){
    if (feature == eFeature::PLAY_MUSIC_BY_ID && result == char(eResPlayMp3::SUCESS)){
        printf("Test 384\n");
        send_ValueMessage_HMI(result,  IDM_RES_SPEECH_HMI_AUDIO_PLAY_A_FILE, eStatus::STATUS_PROCESSING); 
    }
    else {
        printf("Test 388\n");
        send_ValueMessage_HMI(result,  IDM_RES_SPEECH_HMI_AUDIO_PLAY_A_FILE, eStatus::STATUS_READY);    
        printf("Status is : %d\n", status);     
    }
}

// Speech receive message - Audio response Speech about play audio
void resPlayFile_Audio(mesg_buffer* msg){
    char result;
    std::memcpy(&result, msg->mesg_text, sizeof (result));   
    printf("Result is: %d \n", result);
    h_resPlayFile_Audio(result);
    
}

// Handle message - play music by id
void h_reqPlayMusicByID_HMI(char id){
    printf("Sending a message to get song list by id\n");
    
    char processingStatus = eProcessingStatus::STATUS_PROCESSING_GET_SONG_LIST;
    send_ValueMessage_HMI(processingStatus,IDM_RES_SPEECH_HMI_PROCESSING_STATUS, eStatus::STATUS_PROCESSING);

    send_ValueMessage_Media(id, IDM_REQ_SPEECH_MEDIA_GET_SONG_LIST);

    if (isCheckReady() == 0){
        TypeList_t songList;
        songList.result = eResPlayByTypeCode::SERVICE_IS_NOT_READY;
        send_SongList_HMI(songList);
    }   
}

// Speech Service receives a message - HMI requets speech to play music by id
void reqPlayMusicByID_HMI(mesg_buffer* msg){
    if (status == eStatus::STATUS_READY){
        status = eStatus::STATUS_PROCESSING;
        feature = eFeature::PLAY_MUSIC_BY_ID;
        printf("Data is HMI request Speech to play music by ID\n");  
        char id;
        std::memcpy(&id, msg->mesg_text, sizeof(id));
        printf("Id is: %d \n", id);
        h_reqPlayMusicByID_HMI(id);
    }
    else {
        printf("The system is busy\nData is HMI request Speech to play music by ID\n");       
    }

}

// Handle message - get song list
void h_resGetSongListByID_Media(TypeList_t songList){
    send_SongList_HMI(songList);
    if (songList.result == eResPlayByTypeCode::SUCESS){
        reqTTSCovertText("Playing song list");
    }
}

// Speech Service receives a message - Media reponse speech about get song list
void resGetSongListByID_Media(mesg_buffer* msg){
    printf("Data is Media response about get song list by ID\n");
    TypeList_t typeList;
    std::memcpy(&typeList, msg->mesg_text, sizeof(TypeList_t));
    printf("info->count: %d\n", typeList.count);
    printf("info->result: %d\n", typeList.result);
    printf("info->shmPath: %s\n", typeList.shmPath);
    printf("info->type: %d\n", typeList.type);
    printf("List is: %s\n",typeList.shmPath);
    h_resGetSongListByID_Media(typeList);
    message.mesg_type = IDM_REQ_SPEECH_AUDIO_PLAY_A_LIST;
    std::memcpy(message.mesg_text, &typeList, sizeof (typeList));
    printf("message.mesg_text is: %s\n", message.mesg_text);
    if (sendMsg(mqs[IDP_AUDIO_SERVICE]->mqid, &message, sizeof(message))) {
        printf("Sent a message to Audio about play song list\n");
    }   
}

// Handle message - play song list 
void h_resPlayList_Audio(char result){
    if (result == char(eResPlayMp3::SUCESS)){
        printf("Test 463 \n");
        send_ValueMessage_HMI(result, IDM_RES_SPEECH_HMI_PLAY_LIST_BY_ID, eStatus::STATUS_PROCESSING);
        feature = eFeature::PLAY_SONG_LIST;
    } else{
        printf("Test 468 \n");
        send_ValueMessage_HMI(result, IDM_RES_SPEECH_HMI_PLAY_LIST_BY_ID, eStatus::STATUS_READY);        
    }

}

// Speech Service receives a messge - Audio reponse speech about play song list
void resPlayList_Audio(mesg_buffer* msg){
    char result;
    std::memcpy(&result, msg->mesg_text, sizeof (result));   
    printf("Result is: %d \n", result);
    h_resPlayList_Audio(result);
}

// Handle message - play back action
void h_reqPlayBackAction_HMI(char actionValue){
    printf("Sending a message about request play back action: \n");
    if (actionValue <= eActionValue::ACTION_NEXT && actionValue >= eActionValue::ACTION_PAUSE){
        printf("Action is: %d\n", actionValue);

        char processingStatus = eProcessingStatus::STATUS_PROCESSING_PLAYBACK_ACTION;
        send_ValueMessage_HMI(processingStatus,IDM_RES_SPEECH_HMI_PROCESSING_STATUS, eStatus::STATUS_PROCESSING);

        if (eActionValue(actionValue) == eActionValue::ACTION_PAUSE){
            printf("Hello 1111111111111111111\n");
            status = eStatus::STATUS_READY;
        }else {
            status = eStatus::STATUS_PROCESSING;
        }
        send_ValueMessage_Audio(actionValue, IDM_REQ_SPEECH_AUDIO_PLAYBACK_ACTION);

        if (isCheckReady() == 0){
            eResPlayBackAction resultPlayBackAction = eResPlayBackAction::SERVICE_IS_NOT_READY;
            send_ValueMessage_HMI((char)resultPlayBackAction, IDM_RES_SPEECH_HMI_PLAYBACK_ACTION, eStatus::STATUS_READY);
        } 
    }
    else if (actionValue == 0){
        eResPlayBackAction result = eResPlayBackAction::WRONG_VALUE; 
        send_ValueMessage_HMI((char) result, IDM_RES_SPEECH_HMI_PLAYBACK_ACTION, eStatus::STATUS_READY);
    }
}

//HMI rquest Speech to play back action
void reqPlayBackAction_HMI(mesg_buffer* msg){
    if (feature == eFeature::PLAY_SONG_LIST){
        status = eStatus::STATUS_PROCESSING;
        char actionValue;
        std::memcpy(&actionValue, msg->mesg_text, sizeof (actionValue));
        if (actionValue == '1'){
            status = eStatus::STATUS_READY;
        }
        printf("Action value is: %d \n", actionValue);
        h_reqPlayBackAction_HMI(actionValue);
    }
    else {
        printf("The system is busy! Message is request play back action from HMI\n");
    }
}

// Handle message - handle play back action
void h_resPlayBackAction_Audio(char resultPlayBackAction){
    printf("Sending a message about response play back action \n");
    printf("Status is : ............. %d\n", status);
    send_ValueMessage_HMI(resultPlayBackAction, IDM_RES_SPEECH_HMI_PLAYBACK_ACTION, status);
}

// Speech receives a message - Audio reponses speech about handle play back action
void resPlayBackAction_Audio(mesg_buffer* msg){
    printf("Data is response Audio to Speech about play back action\n");
    char resultPlayBackAction;
    std::memcpy(&resultPlayBackAction, msg->mesg_text, sizeof(resultPlayBackAction));
    printf("Result of play back action is: %d\n", resultPlayBackAction);
    h_resPlayBackAction_Audio(resultPlayBackAction);
}

// Identify where the message is received from to handle
void printMsg(int AppID, mesg_buffer* msg) {
    if (msg == nullptr) {
        printf("Cannot print, message is null!!! \n");
        return;
    }
    switch (msg->mesg_type)
    {
        case IDM_REQ_HMI_SPEECH_GET_CONFIGURATION:
            reqGetConfiguration_HMI(msg);
            break;
        case IDM_REQ_HMI_SPEECH_CHANGE_LANGUAGE:
            reqChangeLanguage_HMI(msg);
            break;
        case IDM_REQ_HMI_SPEECH_PLAY_MUSIC_BY_TYPE: 
            reqPlayMusicByType_HMI(msg);
            break;
        case IDM_RES_MEDIA_SPEECH_LIST_BY_TYPE:
            isReceive = true;
            resPlayMusicByType_Media(msg);
            break;
        case IDM_RES_TTS_SPEECH_CONVERT_TEXT:
            isReceive = true;
            resConvertText_TTS(msg);
            break;
        case IDM_RES_AUDIO_SPEECH_PLAY_A_FILE:
            isReceive = true;
            resPlayFile_Audio(msg);
            break;
        case IDM_REQ_HMI_SPEECH_PLAY_SONG_LIST_BY_ID:
            reqPlayMusicByID_HMI(msg);
            break;
        case IDM_RES_MEDIA_SPEECH_GET_SONG_LIST:
            isReceive = true;
            resGetSongListByID_Media(msg);
            break;
        case IDM_RES_AUDIO_SPEECH_PLAY_A_LIST:
            isReceive = true;
            resPlayList_Audio(msg);
            break;
        case IDM_REQ_HMI_SPEECH_PLAYBACK_ACTION:
            reqPlayBackAction_HMI(msg);
            break;
        case IDM_RES_AUDIO_SPEECH_PLAYBACK_ACTION:
            isReceive = true;
            resPlayBackAction_Audio(msg);
            break;
        default:
            printf("Cannot parse!!! \n");
            break;
    }

}

std::thread* receiveThreads[MAX_PROC_NAME];
// Receive message from another
void rcvMsg(int appID) {
    if (mqs[appID] == nullptr) {
        printf("Cannot listen to appID = %d, data is null\n", appID);
    }
    printf("\nStart listening to %s, mqID = %d\n", mqs[appID]->mqPath, mqs[appID]->mqid);
    mesg_buffer msg;
    mqs[appID]->isListening = true;
    int i = 0;
    while (mqs[appID]->isContinue && mqs[appID]->isReady) {
        int result = msgrcv(mqs[appID]->mqid, &msg, sizeof(msg), 0, IPC_NOWAIT);
        if (result == -1) {// error case

             int errsv = errno;
             if (ENOMSG != errno) {//Not is an empty queue
                printf("[%s]sendMsg error, error code = %d \n", AppName, errsv);
                break;
             } else {
                 usleep(20000); //sleep for 20ms
             }
        } else {
            printf("Received a message ID = %d, size = %d \n", mqs[appID]->mqid, result);
            receiveThreads[i] = new std::thread(printMsg,appID, &msg);
            // printMsg(appID, &msg);
        }
    }

    printf("Stop listening, isContinue = %s, isReady = %s \n",
           (mqs[appID]->isContinue ? "true" : "false"),
           (mqs[appID]->isReady ? "true" : "false"));

    pthread_exit(NULL);

}

int main()
{
    status = eStatus::STATUS_INITIALIZING;
    mqs[IDP_HMI]            = new MqInfor(MSQ_HMI,                   IDP_HMI);
    mqs[IDP_SPEECH_SERVICE] = new MqInfor(MSQ_SPEECH_SERVICE,        IDP_SPEECH_SERVICE);
    mqs[IDP_MEDIA_SERVICE]  = new MqInfor(MSQ_MEDIA_SERVICE,         IDP_MEDIA_SERVICE);
    mqs[IDP_TTS_SERVICE]    = new MqInfor(MSQ_TTS_SERVICE,           IDP_TTS_SERVICE);
    mqs[IDP_AUDIO_SERVICE]  = new MqInfor(MSQ_AUDIO_SERVICE,         IDP_AUDIO_SERVICE);
    mqs[IDP_WATCHDOG]       = new MqInfor(MSQ_WATCHDOG,              IDP_WATCHDOG);
    status = eStatus::STATUS_READY;
    std::thread* listenThreads;
    for (int i = 0; i < MAX_PROC_NAME; i ++) {
        if (mqs[i] != nullptr) {
            openMQ(mqs[i]);
            if (mqs[i]->isReady) {
                printf("Open MQ %s successfully, mqid = %d!!!\n", mqs[i]->mqPath, mqs[i]->mqid);
                if (i == IDP_SPEECH_SERVICE){
                    listenThreads = new std::thread(rcvMsg, i);
                }
            }
        }
    }
    char selection;
    do {
        printf("Enter 0 to stop listening...");
        std::cin>>selection;
        if (selection == '0'){
            delete receiveThreads;
            delete listenThreads;
            printf("Exit Speech Service\n");
            exit(1);
        }
    } while (selection != 0);

    //wait for all threads listening
    for (int i = 0; i < MAX_PROC_NAME; i ++) {
        if (mqs[i] != nullptr) {
            int cnt = 1;
            while (!mqs[i]->isListening && cnt <= 3) {
                usleep(20000);
                cnt++;
            }
        }
    }

    //send requests to stop listening
    for (int i = 0; i < MAX_PROC_NAME; i ++) {
        if (mqs[i] != nullptr) {
            mqs[i]->isContinue = false;
        }
    }

    //wait for threads stop
    for (int i = 0; i < MAX_PROC_NAME; i ++) {
        if (listenThreads != nullptr) {
            listenThreads->join();
        }
    }

    //release memory
    for (int i = 0; i < MAX_PROC_NAME; i ++) {
        if (mqs[i] != nullptr) {
            delete receiveThreads;
            delete listenThreads;
            delete mqs[i];
        }
    }

    return 0;
}
