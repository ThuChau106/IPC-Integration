#include <ao/ao.h>
#include <mpg123.h>
#include <mutex>
#include <condition_variable>
#include <iostream>
#include <thread>

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
#include <cstring>
#include <unistd.h>
#define AppName "Audio"

#define BITS 8
#define MAX_SONG_COUNT 1000

std::mutex m;
std::condition_variable cv;

std::mutex mutex_source;
std::condition_variable cv_source;

uint32_t songPos, songCount;
char* songs[MAX_SONG_COUNT];

bool play = false;
bool cancel = false;
bool next = false;
bool previous = false;
bool stop = false;

mpg123_handle *mh;
unsigned char *buffer;
size_t buffer_size;
size_t done;
int err;

int driver;
ao_device *dev;

ao_sample_format format;
int channels, encoding;
long rate;



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


void playSong() {
    while (mpg123_read(mh, buffer, buffer_size, &done) == MPG123_OK) {

        if (!play) {
            std::unique_lock<std::mutex> lk(m);
            printf("Pause!!!\n");
            cv.wait(lk);
            lk.unlock();
        }
        printf("next or previous :  %d - %d \n", next , previous);
        if (next || previous) {
            printf("next or previous\n");
            break;
        }

        ao_play(dev, (char*) buffer, done);
    }
}

void loadSong() {
    //get path
    char* path = nullptr;
    std::unique_lock<std::mutex> lk(mutex_source);
    if (previous == true) {
         if (songPos > 0) songPos --;
    } else {
        songPos ++;
    }

    printf("Current pos = %d, max song = %d\n", songPos, songCount);
    while (songPos < songCount && songs[songPos] == nullptr) {
        songPos ++;
    }
    while (songPos >= songCount) {
        printf("Waiting for new source\n");
        cv_source.wait(lk);
        printf("New souce came\n");
        while (songPos < songCount && songs[songPos] == nullptr) {
            songPos ++;
        }
        printf("Current pos = %d, max song = %d", songPos, songCount);
    }
    path = songs[songPos];
    lk.unlock();

    previous = false;
    next = false;
     printf("Current song is: %s\n", path);

    //load song
    mpg123_open(mh, path);
    mpg123_getformat(mh, &rate, &channels, &encoding);

    /* set the output format and open the output device */
    format.bits = mpg123_encsize(encoding) * BITS;
    format.rate = rate;
    format.channels = channels;
    format.byte_format = AO_FMT_NATIVE;
    format.matrix = 0;
    dev = ao_open_live(driver, &format, NULL);
}

void playMusic() {
    while(true) {
        loadSong();
        printf("Start to play a song\n");
        playSong();
        printf("Finish a song\n");
        mpg123_close(mh);
        if (cancel) break;
    }
    pthread_exit(NULL);
}


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
}* audioMQ, *speechMQ;

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
    case IDM_REQ_SPEECH_AUDIO_PLAY_A_FILE:
    {
        SongInfo_t* info = new SongInfo_t();

        std::memcpy(info, msg->mesg_text, sizeof (SongInfo_t));

        printf("Song title: %s\n", info->songTitle);
        printf("Song Album: %s\n", info->Album);
        printf("Song Artist: %s\n", info->Artist);
        printf("Song Folder: %s\n", info->Folder);
        printf("Song path: %s\n", info->path);

        std::unique_lock<std::mutex> lk_play(m);
        // play = true;
        // next = true;//send signal to move to next song
        cv.notify_one();
        lk_play.unlock();

        // for (int i = 0; i < songCount; i ++){
        //     delete songs[i];
        // }

        std::unique_lock<std::mutex> lk(mutex_source);
        //songPos = 0;
        songCount = songPos + 2;

        songs[songPos+1] = new char[sizeof(info->path) + 1];
        char newPath[400];
        sprintf(newPath, "%s%s", "/home/thu/Documents/lg_intern/HMI-build/", info->path);

        std::memcpy(songs[songPos+1], newPath, sizeof (info->path));
        songs[songPos+1][sizeof (info->path)] = 0;
        cv_source.notify_one();

        next = true;//send signal to move to next song
        play = true;
        lk.unlock();

        delete info;

        eResPlayMp3 result;
        result = eResPlayMp3::SUCESS;

        mesg_buffer* ms = new mesg_buffer();
        ms->mesg_type = IDM_RES_AUDIO_SPEECH_PLAY_A_FILE;
        memcpy(ms->mesg_text, &result, sizeof (eResPlayMp3));

        sendMsg(speechMQ->mqid, ms, sizeof (mesg_buffer));

        delete ms;
    }
        break;


    case IDM_REQ_SPEECH_AUDIO_PLAY_A_LIST:
    {

        TypeList_t* info = new TypeList_t();

        std::memcpy(info, &msg->mesg_text, sizeof (TypeList_t));
        printf("info->count: %d\n", info->count);
        printf("info->result: %d\n", info->result);
        printf("info->shmPath: %s\n", info->shmPath);
        printf("info->type: %d\n", info->type);
        std::unique_lock<std::mutex> lk_play(m);
        play = true;
        next = true;//send signal to move to next song
        cv.notify_one();
        lk_play.unlock();
        for (int i = 0; i < songCount; i ++)
            delete songs[i];
        SongInfo_t* data = (SongInfo_t*)readShm(info->shmPath, info->count * sizeof (SongInfo_t));
        printf("Line 315\n");
        std::unique_lock<std::mutex> lk(mutex_source);
        songPos = 0;
        songCount = info->count;
        for (int i = 0; i < songCount; i ++) {
            songs[i] = new char[sizeof(data[i].path) + 1];
            std::memcpy(songs[i], data[i].path, sizeof (data[i].path));
            songs[i][sizeof (data[i].path)] = 0;
        }
        cv_source.notify_one();
        lk.unlock();

        delete info;

        eResPlayMp3 result;
        result = eResPlayMp3::SUCESS;

        mesg_buffer* ms = new mesg_buffer();
        ms->mesg_type = IDM_RES_AUDIO_SPEECH_PLAY_A_LIST;
        memcpy(ms->mesg_text, &result, sizeof (eResPlayMp3));

        sendMsg(speechMQ->mqid, ms, sizeof (mesg_buffer));

        delete ms;
    }
        break;


    case IDM_REQ_SPEECH_AUDIO_PLAYBACK_ACTION:
    {
        eActionValue info;
        char info1;
        std::memcpy(&info1, msg->mesg_text, sizeof (char));
        printf("info is: %c \n", info1);
        info = eActionValue(info1);
        if (info == ACTION_PAUSE) {
            play = false;
        } else if (info == ACTION_RESUME) {
            std::unique_lock<std::mutex> lk_play(m);
            play = true;
            cv.notify_one();
            lk_play.unlock();
        } else {

            if (info == ACTION_NEXT) {
                std::unique_lock<std::mutex> lk_play(m);
                play = true;
                next = true;
                cv.notify_one();
                lk_play.unlock();
            } else if (info == ACTION_PREVIOUS) {
                if (songPos >= songCount && songCount > 0) {
                    std::unique_lock<std::mutex> lk(mutex_source);
                    songPos = songCount - 1;
                    cv_source.notify_one();
                    lk.unlock();
                } else {
                    std::unique_lock<std::mutex> lk_play(m);
                    play = true;
                    previous = true;
                    cv.notify_one();
                    lk_play.unlock();
                }

            }

        }

        eResPlayMp3 result;
        result = eResPlayMp3::SUCESS;

        mesg_buffer* ms = new mesg_buffer();
        ms->mesg_type = IDM_RES_AUDIO_SPEECH_PLAYBACK_ACTION;
        memcpy(ms->mesg_text, &result, sizeof (eResPlayMp3));

        sendMsg(speechMQ->mqid, ms, sizeof (mesg_buffer));

        delete ms;
    }
        break;
    default:
        printf("The command %d is not in scope!!!\n", msg->mesg_type);
        break;

    }


}


void rcvMsg() {
    if (audioMQ == nullptr) {
        printf("Cannot listen to TTS message, data is null\n");
        return;
    }

    printf("Start listening to %s, mqID = %d\n", audioMQ->mqPath, audioMQ->mqid);
    mesg_buffer msg;
    audioMQ->isListening = true;
    while (audioMQ->isContinue && audioMQ->isReady) {
        int result = msgrcv(audioMQ->mqid, &msg, sizeof(msg), 0, IPC_NOWAIT);
        if (result == -1) {// error case
             int errsv = errno;
             if (ENOMSG != errno) {//Not is an empty queue
                printf("[%s]sendMsg error, error code = %d \n", AppName, errsv);
                break;
             } else {
                 usleep(20000); //sleep for 20ms
             }
        } else {
            printf("Received a message ID = %d, size = %d, type = %d \n", audioMQ->mqid, result, msg.mesg_type);

            handleMsg(&msg);
        }
    }

    printf("Stop listening, isContinue = %s, isReady = %s \n",
           (audioMQ->isContinue ? "true" : "false"),
           (audioMQ->isReady ? "true" : "false"));

    pthread_exit(NULL);

}

void init() {

    songPos = 0;
    songCount = 0;

    play = false;
    next = false;
    previous = false;

    audioMQ = new MqInfor(MSQ_AUDIO_SERVICE, IDP_AUDIO_SERVICE);
    openMQ(audioMQ);
    if (audioMQ->isReady) {
        printf("Open MQ %s successfully, id = %d!!!\n", audioMQ->mqPath, audioMQ->mqid);
    }

    speechMQ = new MqInfor(MSQ_SPEECH_SERVICE, IDP_SPEECH_SERVICE);
    openMQ(speechMQ);
    if (speechMQ->isReady) {
        printf("Open MQ %s successfully, id = %d!!!\n", speechMQ->mqPath, speechMQ->mqid);
    }
}

int main(int argc, char *argv[])
{
    init();

    std::thread th(rcvMsg);

    /* initializations */
    ao_initialize();
    driver = ao_default_driver_id();
    mpg123_init();
    mh = mpg123_new(NULL, &err);
    buffer_size = mpg123_outblock(mh);
    buffer = (unsigned char*) malloc(buffer_size * sizeof(unsigned char));

    /* open the file and get the decoding format */

    std::thread th_play(playMusic);

    th_play.join();
    th.join();

    /* clean up */
    free(buffer);
    ao_close(dev);

    mpg123_delete(mh);
    mpg123_exit();
    ao_shutdown();

    return 0;
}
