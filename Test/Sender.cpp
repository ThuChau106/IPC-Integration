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
#define AppName "Speech"

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
    }

    const char * mqPath;
    int appID;
    bool isReady;
    key_t key;
    int mqid;
}*mqs[MAX_PROC_NAME];

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

int createShm(char* path, char* data, size_t size) {
    // ftok to generate unique key
    key_t key = ftok(path,65);
    if (key == -1) {// error case
         int errsv = errno;
         printf("[%s]createShm %s getting key is failed, error code = %d \n", AppName, path, errsv);
         return -1;
    }
    int shmid = shmget(key,1,0666|IPC_CREAT);
    if (shmid == -1) {// error case
         int errsv = errno;
         printf("[%s]createShm %s get sharedmemory ID is failed, error code = %d \n", AppName, path, errsv);
    }

    // destroy the shared memory
    int value = shmctl(shmid,IPC_RMID,NULL);
    if (value == -1) {// error case
         int errsv = errno;
         printf("[%s]createShm %s destroy sharedmemory ID is failed, error code = %d \n", AppName, path, errsv);
    }

    //try to recreate SHM
    shmid = shmget(key,size + 1,0666|IPC_CREAT);
    if (shmid == -1) {// error case
         int errsv = errno;
         printf("[%s]writeShm %s get sharedmemory ID is failed, error code = %d \n", AppName, path, errsv);
    }

    return shmid;
}

bool writeShm(char* path, char* data, size_t size) {
    printf("Write to SHM %s, size = %d \n", path, size);

    int shmid = createShm(path, data,size);
    if (shmid == -1) {// error case
         return false;
    }

    // shmat to attach to shared memory
    char *shm = (char*) shmat(shmid,(void*)0,0);
    if (shm == (void*)-1) {// error case
         int errsv = errno;
         printf("[%s]writeShm %s attachment is failed, error code = %d \n", AppName, path, errsv);
         return false;
    }

    // write data to shared memory
    std::memcpy(shm, data, size);

     //detach from shared memory
    int detach = shmdt(shm);
    if (detach == -1) {// error case
         int errsv = errno;
         printf("[%s]writeShm %s detachment is failed, error code = %d \n", AppName, path, errsv);
         return false;
    }

    return true;
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
                printf("Open MQ %s successfully, id = %d!!!\n", mqs[i]->mqPath, mqs[i]->mqid);
            }
        }
    }

    int selection = 0;
    do {
        printf("Send to HMI-%d, Speech-%d, Media-%d, TTS-%d, Audio-%d, Watchdog-%d, Quit-0 \n",
               IDP_HMI, IDP_SPEECH_SERVICE, IDP_MEDIA_SERVICE, IDP_TTS_SERVICE, IDP_AUDIO_SERVICE, IDP_WATCHDOG);

        std::cin>>selection;

        if (selection > MAX_PROC_NAME) {
            printf("Wrong command, please enter other value \n");
            continue;
        }

        if ((selection < 0) || (selection > 0 && mqs[selection] == nullptr )) {
            printf("Cannot connect to %d, please enter other value \n", selection);
            continue;
        }

        switch (selection) {
            case IDP_HMI:
                printf("Sending a message to HMI \n");
                message.mesg_type = IDM_RES_SPEECH_HMI_GET_CONFIGURATION;
        {
            LangugeInfo_t langInfo;

            int currentLang;
            printf("Enter current language, %d is Vietnamese, %d is English\n", LANGUAGE_VIETNAMESE, LANGUAGE_ENGLISH);
            std::cin>>currentLang;
            langInfo.current_language = (eLanguageValue)currentLang;

            int supportedLang;
            printf("Enter supproted language, %d is Vietnamese, %d is English, %d is both\n", LANGUAGE_VIETNAMESE, LANGUAGE_ENGLISH, LANGUAGE_ENGLISH | LANGUAGE_VIETNAMESE);
            std::cin>>supportedLang;
            langInfo.supported_language = supportedLang;

            std::memcpy(message.mesg_text, &langInfo, sizeof (langInfo) + 1);

        }
                if (sendMsg(mqs[selection]->mqid, &message, sizeof(message))) {
                    printf("Sent a message to HMI \n");
                }
                break;


            case IDP_SPEECH_SERVICE:
                printf("Sending a message to Speech Service \n");
                message.mesg_type = IDM_RES_MEDIA_SPEECH_LIST_BY_TYPE;
        {
            TypeList_t typeList;

            int count;
            printf("Enter number of objects:");
            std::cin>>count;
            typeList.count = count;

            int type;
            printf("Enter type of objects:");
            std::cin>>type;
            typeList.type = (eMusicType) type;

            std::memcpy(typeList.shmPath, SHM_MEDIA_TYPE_CONTENT_LIST, sizeof (typeList.shmPath));

            std::memcpy(message.mesg_text, &typeList, sizeof (typeList) + 1);

            TypeInfo_t* data = new TypeInfo_t[count];

            for (int i = 0; i < count; i ++) {
                data[i].index = i + 1;
                sprintf(data[i].name, "Name number %02d", i + 1);
            }

            writeShm(typeList.shmPath, (char*) data, sizeof (TypeInfo_t) * count);

            delete [] data;

        }

                if (sendMsg(mqs[selection]->mqid, &message, sizeof(message))) {
                    printf("Sent a message to Speech Service \n");
                }
                break;


            case IDP_MEDIA_SERVICE:
                printf("Sending a message to Media Service \n");
                message.mesg_type = IDM_REQ_SPEECH_MEDIA_GET_LIST_BY_TYPE;
        {
            eMusicType musicType;

            int type;
            printf("Enter type of music:");
            std::cin>>type;
            musicType = (eMusicType) type;

            std::memcpy(message.mesg_text, &musicType, sizeof (musicType) + 1);

        }

                if (sendMsg(mqs[selection]->mqid, &message, sizeof(message))) {
                    printf("Sent a message to Media Service \n");
                }
                break;
            case IDP_TTS_SERVICE:

                printf("Sending a message to TTS Service \n");
                message.mesg_type = IDM_REQ_SPEECH_TTS_CONVERT_TEXT;
                {
                    TtsInfo_t msg;
                    int lang;
                    printf("Enter language of text:");
                    std::cin>>lang;
                    msg.language = (eLanguageValue) lang;

                    std::memcpy(msg.text, "This is a test text. It's so simple.", sizeof (msg.text));

                    std::memcpy(message.mesg_text, &msg, sizeof (msg) + 1);

                }
                if (sendMsg(mqs[selection]->mqid, &message, sizeof(message))) {
                    printf("Sent a message to TTS Service \n");
                }
                break;


            case IDP_AUDIO_SERVICE:
                printf("Sending a message to Audio Service \n");
            {
                int selecttt;
                std::cin>>selecttt;
                if (selecttt == 1) {
                    message.mesg_type = IDM_REQ_SPEECH_AUDIO_PLAYBACK_ACTION;
                    {
                        eActionValue playbackAction;
                        int act;
                        printf("Enter Action:");
                        std::cin>>act;
                        playbackAction = (eActionValue) act;

                        std::memcpy(message.mesg_text, &act, sizeof (act) + 1);

                    }
                } else {
                    message.mesg_type = IDM_REQ_SPEECH_AUDIO_PLAY_A_LIST;
                    {
                       TypeList_t list;

                       int cnt;
                       printf("Enter count:");
                       std::cin>>cnt;
                       list.count = cnt;

                       std::memcpy(list.shmPath, SHM_MEDIA_SONG_LIST, sizeof (list.shmPath));

                       SongInfo_t* data = new SongInfo_t[cnt];

                       for (int i = 0; i < cnt; i ++) {
                           sprintf((char*)data[i].Album, "Album name %02d", i + 1);
                           sprintf((char*)data[i].Artist, "Artist name %02d", i + 1);
                           sprintf((char*)data[i].Folder, "Folder name %02d", i + 1);
                           sprintf((char*)data[i].songTitle, "songTitle name %02d", i + 1);
                           sprintf((char*)data[i].path, "songs/test.mp3");
                       }

                       writeShm(list.shmPath, (char*) data, sizeof (SongInfo_t) * cnt);

                       delete [] data;

                       std::memcpy(message.mesg_text, &list, sizeof (list) + 1);
                    }
                }
        }

                if (sendMsg(mqs[selection]->mqid, &message, sizeof(message))) {
                    printf("Sent a message to Audio Service \n");
                }
                break;
            case IDP_WATCHDOG:
                printf("Sending a message to Watchdog \n");
                message.mesg_type = IDM_REQ_HMI_WATCHDOG_I_AM_ALIVE;
                {
                    eStatus status;
                    int stt;
                    printf("Enter Action:");
                    std::cin>>stt;
                    status = (eStatus) stt;

                    std::memcpy(message.mesg_text, &status, sizeof (status) + 1);

                }

                if (sendMsg(mqs[selection]->mqid, &message, sizeof(message))) {
                    printf("Sent a message to Watchdog \n");
                }
                break;
            default:
                printf("Quit program!!!\n");

                break;
        }

    } while (selection != 0);


    return 0;
}
