#ifndef _DTU_INTERN_SHM_H
#define _DTU_INTERN_SHM_H


#include "../message/IpcHeader.h"

namespace lg_dtu {

    class SharedMemory {
    public:
        char* readShm(const char * path, size_t size);
        bool writeShm(const char* path, const char* data, size_t size);
    };

}

#endif
