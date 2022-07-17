/**
    This file is testing purpose only.

*/
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

#define AppName "Receiver"

// structure for message queue
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

std::thread* listenThreads[MAX_PROC_NAME];


char* readShm(char* path, size_t size) {
    // ftok to generate unique key
    key_t key = ftok(path,65);
    if (key == -1) {// error case
         int errsv = errno;
         printf("[%s]readShm %s getting key is failed, error code = %d \n", AppName, path, errsv);
         return nullptr;
    }

    // shmget returns an identifier in shmid
    int shmid = shmget(key,size + 1,0666|IPC_CREAT);
    if (shmid == -1) {// error case
         int errsv = errno;
         printf("[%s]readShm %s get sharedmemory ID is failed, error code = %d \n", AppName, path, errsv);
         return nullptr;
    }


    // shmat to attach to shared memory
    char *shm = (char*) shmat(shmid,(void*)0,0);
    if (shmid == -1) {// error case
         int errsv = errno;
         printf("[%s]readShm %s attach sharedmemory ID is failed, error code = %d \n", AppName, path, errsv);
         return nullptr;
    }

    char* result = new char[size + 1];

    std::memcpy(result, shm, size);
    result[size] = 0;

    // destroy the shared memory
    int value = shmctl(shmid,IPC_RMID,NULL);
    if (value == -1) {// error case
         int errsv = errno;
         printf("[%s]createShm %s destroy sharedmemory ID is failed, error code = %d \n", AppName, path, errsv);
    }

    return result;
}

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


void printMsg(int AppID, mesg_buffer* msg) {
    if (msg == nullptr) {
        printf("Cannot print, message is null!!! \n");
        return;
    }

    switch (AppID) {
    case IDP_HMI:
    {
        if (msg->mesg_type == IDM_RES_SPEECH_HMI_GET_CONFIGURATION)
            printf("Data is the language configuration: \n");
        else {
            printf("Data isn't the language configuration, cannot parse!!! \n");
            break;
        }
        LangugeInfo_t langInfo;
        std::memcpy(&langInfo, msg->mesg_text, sizeof (langInfo));
        printf("Current language (%d) is %s \n", langInfo.current_language,
               ( langInfo.current_language == LANGUAGE_VIETNAMESE ? "Vietnamese" :
               ( langInfo.current_language == LANGUAGE_ENGLISH ? "English": "Unknown")));
        printf("Supported language (%d):", langInfo.supported_language);
        if (langInfo.supported_language & LANGUAGE_VIETNAMESE ) printf(" Vietnamese ");
        if (langInfo.supported_language & LANGUAGE_ENGLISH) printf(" English ");
        printf("\n");
    }
        break;

    case IDP_WATCHDOG:
    {
        if (msg->mesg_type == IDM_REQ_HMI_WATCHDOG_I_AM_ALIVE)
            printf("Data is HMI watchdog status \n");
        else {
            printf("Data isn't HMI watchdog status, cannot parse!!! \n");
            break;
        }
        eStatus status;
        std::memcpy(&status, msg->mesg_text, sizeof (status));
        printf("HMI watchdog status is: %d \n", status);
    }
        break;

    case IDP_TTS_SERVICE:
    {
        if (msg->mesg_type == IDM_REQ_SPEECH_TTS_CONVERT_TEXT)
            printf("Data is TTS info \n");
        else {
            printf("Data isn't TTS info, cannot parse!!! \n");
            break;
        }
        TtsInfo_t ttsMsg;
        std::memcpy(&ttsMsg, msg->mesg_text, sizeof (ttsMsg));
        printf("Text language is: %d \n", ttsMsg.language);
        printf("Text is: %s \n", ttsMsg.text);
    }
        break;

    case IDP_AUDIO_SERVICE:
    {
        if (msg->mesg_type == IDM_REQ_SPEECH_AUDIO_PLAYBACK_ACTION)
            printf("Data is playback action info \n");
        else {
            printf("Data isn't playback action info, cannot parse!!! \n");
            break;
        }
        eActionValue playbackAction;
        std::memcpy(&playbackAction, msg->mesg_text, sizeof (playbackAction));
        printf("Playback action is: %d \n", playbackAction);
    }
        break;

    case IDP_MEDIA_SERVICE:
    {
        if (msg->mesg_type == IDM_REQ_SPEECH_MEDIA_GET_LIST_BY_TYPE)
            printf("Data is type info \n");
        else {
            printf("Data isn't type info, cannot parse!!! \n");
            break;
        }
        eMusicType musicType;
        std::memcpy(&musicType, msg->mesg_text, sizeof (musicType));
        printf("Music type is: %d \n", musicType);
    }
        break;

    case IDP_SPEECH_SERVICE:
    {
        if (msg->mesg_type == IDM_RES_MEDIA_SPEECH_LIST_BY_TYPE) {
            printf("Data is list by type info: \n");
            TypeList_t typeList;
            std::memcpy(&typeList, msg->mesg_text, sizeof (typeList));
            printf("Type is: %d \n", typeList.type);
            printf("Count is: %d \n", typeList.count);
            printf("Shm path is: %s \n", typeList.shmPath);

            TypeInfo_t* data = (TypeInfo_t*) readShm(typeList.shmPath, sizeof (TypeInfo_t)* typeList.count);

            if (data != nullptr) {
                for (int i = 0; i <typeList.count; i ++) {
                    printf("Infor[%d]: Idex = %d, Name = %s\n", i + 1, data[i].index, data[i].name);
                }

                delete [] data;
            }
        } else if (msg->mesg_type == IDM_RES_TTS_SPEECH_CONVERT_TEXT) {
            TtsResponse_t ttsRes;
            std::memcpy(&ttsRes, msg->mesg_text, sizeof (TtsResponse_t));
            printf("Result is: %d \n", ttsRes.result);
            printf("Mp3 Path is: %s \n", ttsRes.mp3FilePath);

            SongInfo_t* info = new SongInfo_t();
            std::memcpy(info->songTitle, "Test title", sizeof (info->songTitle));
            std::memcpy(info->Folder, "Test Folder", sizeof (info->Folder));
            std::memcpy(info->Artist, "Test Artist", sizeof (info->Artist));
            std::memcpy(info->Album, "Test Album", sizeof (info->Album));
            std::memcpy(info->path, ttsRes.mp3FilePath, sizeof (info->path));

            mesg_buffer* audioMsg = new mesg_buffer();
            audioMsg->mesg_type = IDM_REQ_SPEECH_AUDIO_PLAY_A_FILE;
            std::memcpy(audioMsg->mesg_text, info, sizeof (SongInfo_t));

            sendMsg(mqs[IDP_AUDIO_SERVICE]->mqid, audioMsg, sizeof (mesg_buffer));
            delete info;
            delete audioMsg;

        } else {
            printf("Cannot parse!!! \n");
            break;
        }


    }
        break;
    default:
        printf("Wrong app ID");
        break;

    }
}

void rcvMsg(int appID) {
    if (mqs[appID] == nullptr) {
        printf("Cannot listen to appID = %d, data is null\n", appID);
    }

    printf("Start listening to %s, mqID = %d\n", mqs[appID]->mqPath, mqs[appID]->mqid);
    mesg_buffer msg;
    mqs[appID]->isListening = true;
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

            printMsg(appID, &msg);
        }
    }

    printf("Stop listening, isContinue = %s, isReady = %s \n",
           (mqs[appID]->isContinue ? "true" : "false"),
           (mqs[appID]->isReady ? "true" : "false"));

    pthread_exit(NULL);

}

int main()
{
    mqs[IDP_HMI]            = new MqInfor(MSQ_HMI,                   IDP_HMI);
    mqs[IDP_SPEECH_SERVICE] = new MqInfor(MSQ_SPEECH_SERVICE,        IDP_SPEECH_SERVICE);
    mqs[IDP_MEDIA_SERVICE]  = new MqInfor(MSQ_MEDIA_SERVICE,         IDP_MEDIA_SERVICE);
    mqs[IDP_TTS_SERVICE]    = new MqInfor(MSQ_TTS_SERVICE,           IDP_TTS_SERVICE);
    mqs[IDP_AUDIO_SERVICE]  = new MqInfor(MSQ_AUDIO_SERVICE,         IDP_AUDIO_SERVICE);
    mqs[IDP_WATCHDOG]       = new MqInfor(MSQ_WATCHDOG,              IDP_WATCHDOG);

    for (int i = 0; i < MAX_PROC_NAME; i ++) {
        if (mqs[i] != nullptr) {
            openMQ(mqs[i]);
            if (mqs[i]->isReady) {
                printf("Open MQ %s successfully, mqid = %d!!!\n", mqs[i]->mqPath, mqs[i]->mqid);
                if (i != IDP_AUDIO_SERVICE && i != IDP_TTS_SERVICE)
                    listenThreads[i] = new std::thread(rcvMsg, i);
                else
                    mqs[i]->isListening = true;
            }
        }
    }

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

    int selection;
    do {
        printf("Enter 0 to stop listening...");
        std::cin>>selection;
    } while (selection != 0);

    //send requests to stop listening
    for (int i = 0; i < MAX_PROC_NAME; i ++) {
        if (mqs[i] != nullptr) {
            mqs[i]->isContinue = false;
        }
    }

    //wait for threads stop
    for (int i = 0; i < MAX_PROC_NAME; i ++) {
        if (listenThreads[i] != nullptr) {
            listenThreads[i]->join();
        }
    }

    //release memory
    for (int i = 0; i < MAX_PROC_NAME; i ++) {
        if (mqs[i] != nullptr) {
            delete listenThreads[i];
            delete mqs[i];
        }
    }

    return 0;
}
