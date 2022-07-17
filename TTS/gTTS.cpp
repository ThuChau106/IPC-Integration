#include <stdlib.h>
#include "../common/header/GlobalHeader.h"
#include "../common/header/message/IpcHeader.h"
#include "../common/header/message/TtsData.h"
#include "../common/header/message/HmiData.h"


#include <cstring>
#include <cstdio>
#include <errno.h>
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <iostream>
#include <cstring>
#include <unistd.h>
#include <thread>

#define AppName "TTS"

#define GTTS            "gtts-cli"
#define FORMAT          "%s -l %s \"%s\" -o %s"
#define VIETNAMESE      "vi"
#define ENGLISH         "en"
#define FILENAME        "SPEECH_FROM_TEXT_"
#define TTS_FOLDER      "voice"

#define MAX_COMMAND_LEN 1000
#define MAX_MP3_PATH_LEN 50

#define MAX_FILE 10

int fileCount;
char mp3File[MAX_MP3_PATH_LEN];
char command[MAX_COMMAND_LEN];

struct mesg_buffer {
    long mesg_type;
    char mesg_text[1000];
} message;

void getFile() {
    ++ fileCount;
    fileCount = fileCount % MAX_FILE;

    sprintf(mp3File, "%s/%s%02d.mp3",TTS_FOLDER, FILENAME, fileCount);
}

void tts(TtsInfo_t* info) {
    getFile();

    //set command
    sprintf(command, FORMAT, GTTS, (info->language == LANGUAGE_VIETNAMESE ? VIETNAMESE : ENGLISH), info->text, mp3File);
    system(command);

    delete info;
}


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
}* ttsMQ, *speechMQ;

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

void handleMsg(mesg_buffer* msg) {
    if (msg == nullptr) {
        printf("Message is null!!!\n");
        return;
    }

    if (msg->mesg_type != IDM_REQ_SPEECH_TTS_CONVERT_TEXT) {
        printf("Wrong command!!!\n");
        return;
    }

    TtsInfo_t* info = new TtsInfo_t();
    std::memcpy(info, msg->mesg_text, sizeof (TtsInfo_t));

    tts(info);
    printf("Language is: %d\n", info->language);
    printf("Text is: %s\n", info->text);

    mesg_buffer* sendData = new mesg_buffer();
    sendData->mesg_type = IDM_RES_TTS_SPEECH_CONVERT_TEXT;

    TtsResponse_t* res = new TtsResponse_t();
    res->result = SUCESSFULLY_TRANSFER_TO_SPEECH;
    std::memcpy(res->mp3FilePath, mp3File, sizeof (mp3File));

    std::memcpy(sendData->mesg_text, res, sizeof (TtsResponse_t));
    printf("sending data result = %d, path = %s\n", res->result, res->mp3FilePath);
    printf("sending msg = %s\n", sendData->mesg_text);

    sendMsg(speechMQ->mqid, sendData, sizeof (mesg_buffer));
    delete  sendData;
    delete res;
}


void rcvMsg() {
    if (ttsMQ == nullptr) {
        printf("Cannot listen to TTS message, data is null\n");
        return;
    }

    printf("Start listening to %s, mqID = %d\n", ttsMQ->mqPath, ttsMQ->mqid);
    mesg_buffer msg;
    ttsMQ->isListening = true;
    while (ttsMQ->isContinue && ttsMQ->isReady) {
        int result = msgrcv(ttsMQ->mqid, &msg, sizeof(msg), 0, IPC_NOWAIT);
        if (result == -1) {// error case
             int errsv = errno;
             if (ENOMSG != errno) {//Not is an empty queue
                printf("[%s]sendMsg error, error code = %d \n", AppName, errsv);
                break;
             } else {
                 usleep(20000); //sleep for 20ms
             }
        } else {
            printf("Received a message ID = %d, size = %d \n", ttsMQ->mqid, result);

            handleMsg(&msg);
        }
    }

    printf("Stop listening, isContinue = %s, isReady = %s \n",
           (ttsMQ->isContinue ? "true" : "false"),
           (ttsMQ->isReady ? "true" : "false"));

    pthread_exit(NULL);

}

void init() {
    sprintf(command,"rm -rf %s", TTS_FOLDER);
    system(command);

    sprintf(command,"mkdir %s", TTS_FOLDER);
    system(command);

    fileCount = 0;

    ttsMQ = new MqInfor(MSQ_TTS_SERVICE, IDP_TTS_SERVICE);
    openMQ(ttsMQ);
    if (ttsMQ->isReady) {
        printf("Open MQ %s successfully, id = %d!!!\n", ttsMQ->mqPath, ttsMQ->mqid);
    }

    speechMQ = new MqInfor(MSQ_SPEECH_SERVICE, IDP_SPEECH_SERVICE);
    openMQ(speechMQ);
    if (speechMQ->isReady) {
        printf("Open MQ %s successfully, id = %d!!!\n", speechMQ->mqPath, speechMQ->mqid);
    }
}


int main() {

    init();


    std::thread th(rcvMsg);
    th.join();
    //system("gtts-cli \"hello, how are you? Do you miss me?\" -o hello.mp3");
    return 0;
}


//gtts-cli -l vi "Hello anh chị em. Anh chị em khỏe không. Cùng nói cheese nào" -o hello.mp3
