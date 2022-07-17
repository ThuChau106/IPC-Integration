#include <stdlib.h>
#include "../common/header/GlobalHeader.h"
#include "../common/header/message/IpcHeader.h"
#include "../common/header/message/TtsData.h"
#include "../common/header/message/HmiData.h"
#include "../common/header/message/MediaData.h"


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
#include <stdlib.h>
#include <filesystem>


#define AppName "MediaService"
#define MAX_TEST_SET 10

std::string* listActualSong;
int actualSongCount;

int testSet = 0;


struct TestInfo_t{
    TestInfo_t(int artCount, int albCount, int folCount) {
        ArtistCount = artCount;
        AlbumCount = albCount;
        FolderCount = folCount;
    }
    int ArtistCount;
    int AlbumCount;
    int FolderCount;
} *testSetInfo[MAX_TEST_SET];



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
}* mediaMQ, *speechMQ;


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

    switch (msg->mesg_type) {
        case IDM_REQ_SPEECH_MEDIA_GET_LIST_BY_TYPE:
        {
            char type;
            std::memcpy(&type, msg->mesg_text, sizeof (type));
            switch (type) {
            case MUSIC_TYPE_ARTIST:
            {
                TypeInfo_t *data = new TypeInfo_t[testSetInfo[testSet]->ArtistCount];
                for (int i = 0; i < testSetInfo[testSet]->ArtistCount; i ++) {
                    data[i].index = i;
                    sprintf((char*)data[i].name, "Artist name %02d", i + 1);
                    printf("Artist name is: %s \n", (char*)data[i].name);
                }
                 writeShm(SHM_MEDIA_TYPE_CONTENT_LIST, (char*)data, sizeof (TypeInfo_t)* testSetInfo[testSet]->ArtistCount);
                 delete data;


                TypeList_t typeList;
                typeList.type = MUSIC_TYPE_ARTIST;
                typeList.count = testSetInfo[testSet]->ArtistCount;
                std::memcpy(typeList.shmPath, SHM_MEDIA_TYPE_CONTENT_LIST, sizeof (typeList.shmPath));


                mesg_buffer* msg = new mesg_buffer();
                msg->mesg_type = IDM_RES_MEDIA_SPEECH_LIST_BY_TYPE;
                std::memcpy(msg->mesg_text, &typeList, sizeof (typeList));

                sendMsg(speechMQ->mqid, msg, sizeof (mesg_buffer));

                delete msg;

            }

                break;
            case MUSIC_TYPE_ALBUM:
            {
                TypeInfo_t *data = new TypeInfo_t[testSetInfo[testSet]->AlbumCount];
                for (int i = 0; i < testSetInfo[testSet]->AlbumCount; i ++) {
                    data[i].index = i;
                    sprintf((char*)data[i].name, "Album name %02d", i + 1);
                }
                 writeShm(SHM_MEDIA_TYPE_CONTENT_LIST, (char*)data, sizeof (TypeInfo_t)* testSetInfo[testSet]->AlbumCount);
                 delete[] data;


                TypeList_t typeList;
                typeList.type = MUSIC_TYPE_ALBUM;
                typeList.count = testSetInfo[testSet]->AlbumCount;
                std::memcpy(typeList.shmPath, SHM_MEDIA_TYPE_CONTENT_LIST, sizeof (typeList.shmPath));


                mesg_buffer* msg = new mesg_buffer();
                msg->mesg_type = IDM_RES_MEDIA_SPEECH_LIST_BY_TYPE;
                std::memcpy(msg->mesg_text, &typeList, sizeof (typeList));

                sendMsg(speechMQ->mqid, msg, sizeof (mesg_buffer));
                delete msg;

            }
                break;
            case MUSIC_TYPE_FOLDER:
            {
                TypeInfo_t *data = new TypeInfo_t[testSetInfo[testSet]->FolderCount];
                for (int i = 0; i < testSetInfo[testSet]->FolderCount; i ++) {
                    data[i].index = i;
                    sprintf((char*)data[i].name, "Folder name %02d", i + 1);
                }
                 writeShm(SHM_MEDIA_TYPE_CONTENT_LIST, (char*)data, sizeof (TypeInfo_t)* testSetInfo[testSet]->FolderCount);
                 delete[] data;


                TypeList_t typeList;
                typeList.type = MUSIC_TYPE_FOLDER;
                typeList.count = testSetInfo[testSet]->FolderCount;
                std::memcpy(typeList.shmPath, SHM_MEDIA_TYPE_CONTENT_LIST, sizeof (typeList.shmPath));


                mesg_buffer* msg = new mesg_buffer();
                msg->mesg_type = IDM_RES_MEDIA_SPEECH_LIST_BY_TYPE;
                std::memcpy(msg->mesg_text, &typeList, sizeof (typeList));

                sendMsg(speechMQ->mqid, msg, sizeof (mesg_buffer));
                delete msg;

            }
                break;
            default:
            {
                printf("Receive wrong data = %d", (int)type);
            }
                break;
            }
        }
        break;

        case IDM_REQ_SPEECH_MEDIA_GET_SONG_LIST:
        {
            TypeList_t list;

            std::memcpy(list.shmPath, SHM_MEDIA_SONG_LIST, sizeof (list.shmPath));

            int cnt = rand() % 100 + 1;
            printf("cnt is: %d\n", cnt);
            list.count = cnt;

            SongInfo_t* data = new SongInfo_t[cnt];

            for (int i = 0; i < cnt; i ++) {
                sprintf((char*)data[i].Album, "Album name %02d", i + 1);
                printf("Album name: %s \n", (char*)data[i].Album);
                sprintf((char*)data[i].Artist, "Artist name %02d", i + 1);
                printf("Artist name: %s \n", (char*)data[i].Artist);
                sprintf((char*)data[i].Folder, "Folder name %02d", i + 1);
                printf("Folder name: %s \n", (char*)data[i].Folder);
                sprintf((char*)data[i].songTitle, "songTitle name %02d", i + 1);
                printf("songTitle name: %s \n", (char*)data[i].songTitle);
                sprintf((char*)data[i].path, listActualSong[i%actualSongCount].c_str());
                printf("path: %s\n",(char*)data[i].path );
            }
            
            writeShm(list.shmPath, (char*) data, sizeof (SongInfo_t) * cnt);

            delete [] data;

            mesg_buffer* ms = new mesg_buffer();

            ms->mesg_type = IDM_RES_MEDIA_SPEECH_GET_SONG_LIST;

            std::memcpy(ms->mesg_text, &list, sizeof (list) + 1);
            // //ms->mesg_text = message.mesg_text;
            // int count = list.count;
            // printf("Number of song is: %d\n", count);
            // TypeList_t list11;
            // std::memcpy(&list11, ms->mesg_text, sizeof (TypeList_t));
            //     printf("msg->mesg_text: %s\n",list11.shmPath);
            //     printf("&list: %ld\n",&list);

            sendMsg(speechMQ->mqid, ms, sizeof (mesg_buffer));


        }
        break;

        case IDM_REQ_TEST_SET_TEST_CASE:
        {
            int tc;
            memcpy(&tc, msg->mesg_text, sizeof (tc));
            testSet = tc % 4;
             printf("Test case has been set to %d!!!\n", testSet);
        }
        break;

        default:
        printf("Wrong command!!!\n");
        break;
    }
}


void rcvMsg() {
    if (mediaMQ == nullptr) {
        printf("Cannot listen to TTS message, data is null\n");
        return;
    }

    printf("Start listening to %s, mqID = %d\n", mediaMQ->mqPath, mediaMQ->mqid);
    mesg_buffer msg;
    mediaMQ->isListening = true;
    while (mediaMQ->isContinue && mediaMQ->isReady) {
        int result = msgrcv(mediaMQ->mqid, &msg, sizeof(msg), 0, IPC_NOWAIT);
        if (result == -1) {// error case
             int errsv = errno;
             if (ENOMSG != errno) {//Not is an empty queue
                printf("[%s]sendMsg error, error code = %d \n", AppName, errsv);
                break;
             } else {
                 usleep(20000); //sleep for 20ms
             }
        } else {
            printf("Received a message ID = %d, size = %d \n", mediaMQ->mqid, sizeof(mediaMQ));

            handleMsg(&msg);
        }
    }

    printf("Stop listening, isContinue = %s, isReady = %s \n",
           (mediaMQ->isContinue ? "true" : "false"),
           (mediaMQ->isReady ? "true" : "false"));

    pthread_exit(NULL);

}

void init() {

    mediaMQ = new MqInfor(MSQ_MEDIA_SERVICE, IDP_MEDIA_SERVICE);
    openMQ(mediaMQ);
    if (mediaMQ->isReady) {
        printf("Open MQ %s successfully, id = %d!!!\n", mediaMQ->mqPath, mediaMQ->mqid);
    }

    speechMQ = new MqInfor(MSQ_SPEECH_SERVICE, IDP_SPEECH_SERVICE);
    openMQ(speechMQ);
    if (speechMQ->isReady) {
        printf("Open MQ %s successfully, id = %d!!!\n", speechMQ->mqPath, speechMQ->mqid);
    }

    testSetInfo[0] = new TestInfo_t(0,10,10);
    testSetInfo[1] = new TestInfo_t(10,0,10);
    testSetInfo[2] = new TestInfo_t(10,10,0);
    testSetInfo[3] = new TestInfo_t(10,10,10);
    testSet = 3;

    actualSongCount = 12;
    listActualSong = new std::string[actualSongCount];
    listActualSong[0] = "songs/Between The Rain Drops.mp3";
    listActualSong[1] = "songs/Can You Feel.mp3";
    listActualSong[2] = "songs/Generade.mp3";
    listActualSong[3] = "songs/Lover.mp3";
    listActualSong[4] = "songs/Love Someone.mp3";
    listActualSong[5] = "songs/Never Enough.mp3";
    listActualSong[6] = "songs/Night Changes.mp3";
    listActualSong[7] = "songs/Oah.mp3";
    listActualSong[8] = "songs/Senorita.mp3";
    listActualSong[9] = "songs/test.mp3";
    listActualSong[10] = "songs/They Dont Know About Us.mp3";
    listActualSong[11] = "songs/We Dont Talk Any More.mp3";
}


int main() {

    init();

    std::thread th(rcvMsg);
    th.join();
    return 0;
}