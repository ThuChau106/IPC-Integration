/**
 * @file IpcHead.h
 *
 * @brief This file provides data structure of TTS's messages
 *
 * @author Tung Duc Nguyen
 * email:  tung2.nguyen@lge.com
 *
 */


#ifndef _DTU_INTERN_TTS_DATA_H
#define _DTU_INTERN_TTS_DATA_H


#define TEXT_MAX_LEN   300

#include <stdint.h>
#include "HmiData.h"

struct TtsInfo_t {
    eLanguageValue language;
    char text[TEXT_MAX_LEN];
};

enum eTTSResponse {
    SUCESSFULLY_TRANSFER_TO_SPEECH =    0x01,
    FAILED_TO_TRANSFER_TO_SPEECH =      0x02,
    SERVICE_IS_NOT_READY =              0x03,
    CANNOT_OPEN_FILE =                  0x04,
};

struct TtsResponse_t {
    eTTSResponse result;
    char mp3FilePath[SONG_PATH_MAX_LEN];
};

#endif
