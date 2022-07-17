#ifndef _DTU_INTERN_MQ_H
#define _DTU_INTERN_MQ_H


#include "../GlobalHeader.h"
namespace lg_dtu {

// structure for message queue
struct mesg_buffer {
    long mesg_type;
    char mesg_data[MESSAGE_MAX_LEN];
};


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
};

class MessageQueue {
public:
    void MessageQueue(MqInfor* infor);
    bool sendMsg(mesg_buffer* msg, int size);

};
}
#endif
