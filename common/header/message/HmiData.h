/**
 * @file IpcHead.h
 *
 * @brief This file provides data structure of HMI's messages
 *
 * @author Tung Duc Nguyen
 * email:  tung2.nguyen@lge.com
 *
 */

#ifndef _DTU_INTERN_HMI_DATA_H
#define _DTU_INTERN_HMI_DATA_H

#include <stdint.h>

#define SHM_PATH_MAX_LEN    100
#define SONG_TITLE_MAX_LEN  100
#define ALBUM_MAX_LEN       100
#define ARTIST_MAX_LEN      100
#define FOLDER_NAME_MAX_LEN 100
#define SONG_PATH_MAX_LEN   300
#define NAME_MAX_LEN        50

enum eLanguageValue {
    LANGUAGE_START = 0x00,
    LANGUAGE_VIETNAMESE = 0x01,
    LANGUAGE_ENGLISH = 0x02,
    MAX_LANGUAGE = 0x09,
};

struct LangugeInfo_t {
    eLanguageValue current_language;
    uint8_t        supported_language;
};

enum eActionValue {
    ACTION_PAUSE =      0x01,
    ACTION_RESUME =     0x02,
    ACTION_PREVIOUS =   0x03,
    ACTION_NEXT =       0x04,
};

enum eMusicType {
    MUSIC_TYPE_START = 0x00,
    MUSIC_TYPE_ARTIST =     0x01,
    MUSIC_TYPE_ALBUM =      0x02,
    MUSIC_TYPE_FOLDER =     0x03,
    MAX_MUSIC_TYPE = 0x09,
};

enum class eResPlayByTypeCode {
    SUCESS =                0x00,
    FAILED_TO_GET_LIST =    0x01,
    WRONG_TYPE =            0x02,
    SERVICE_IS_NOT_READY =  0x03,
};

struct ResPlayByType_t {
    eResPlayByTypeCode result;
    char shmPath[SHM_PATH_MAX_LEN];
};

enum class eResPlayMp3 {
    SUCESS =                0x00,
    FAILED =                0x01,
    SERIVE_IS_NOT_READY =   0x02,
};

enum class eResChangeLanguage {
    SUCESS =                0x00,
    FAILED =                0x01,
    WRONG_VALUE =           0x02,
};

enum class eResPlayBackAction {
    SUCESS =                0x00,
    FAILED =                0x01,
    WRONG_VALUE =           0x02,
    SERVICE_IS_NOT_READY =  0x03,
};

enum eStatus {
    STATUS_INITIALIZING =              0x01,
    STATUS_READY =                     0x02,
    STATUS_PROCESSING =                0x03,
    STATUS_CLOSING =                   0x04,
};

enum eFeature {
    PLAY_MUSIC_BY_TYPE = 0x01,
    PLAY_MUSIC_BY_ID =   0x02,    
    PLAY_SONG_LIST =     0x03,
};

enum eProcessingStatus {
    STATUS_PROCESSING_PLAYBACK_ACTION = 0x01,
    STATUS_PROCESSING_CONVERT_TEXT =    0x02,
    STATUS_PROCESSING_GET_LIST_BY_TYPE = 0x03,
    STATUS_PROCESSING_PLAY_A_FILE = 0X04,
    STATUS_PROCESSING_GET_SONG_LIST = 0x05,
};

#endif
