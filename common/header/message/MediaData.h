/**
 * @file IpcHead.h
 *
 * @brief This file provides data structure of Media's messages
 *
 * @author Tung Duc Nguyen
 * email:  tung2.nguyen@lge.com
 *
 */


#ifndef _DTU_INTERN_MEDIA_DATA_H
#define _DTU_INTERN_MEDIA_DATA_H

#include <stdint.h>
#include "HmiData.h"


/**
 * @brief The information of a song
 */
struct SongInfo_t {
    char songTitle[SONG_TITLE_MAX_LEN];
    char Album[ALBUM_MAX_LEN];
    char Artist[ARTIST_MAX_LEN];
    char Folder[FOLDER_NAME_MAX_LEN];
    char path[SONG_PATH_MAX_LEN];
};

struct TypeInfo_t {
    char name[NAME_MAX_LEN];
    uint8_t index;
};

struct TypeList_t {
    eResPlayByTypeCode result;
    char shmPath[SHM_PATH_MAX_LEN];
    uint8_t count;
    eMusicType type;
};

struct SongList_t {
    eResPlayByTypeCode result;
    char shmPath[SHM_PATH_MAX_LEN];
    uint8_t count;
};

#endif
