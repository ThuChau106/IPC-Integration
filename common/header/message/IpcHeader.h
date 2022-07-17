/**
 * @file IpcHead.h
 *
 * @brief This file provides all IPC messages in the project of DTU internship program - Speech System.
 *
 * @author Tung Duc Nguyen
 * email:  tung2.nguyen@lge.com
 *
 */


#ifndef _DTU_INTERN_MSG_H
#define _DTU_INTERN_MSG_H


#define MSQ_HMI                "MSQ_HMI"
#define MSQ_SPEECH_SERVICE     "MSQ_SPEECH_SERVICE"
#define MSQ_MEDIA_SERVICE      "MSQ_MEDIA_SERVICE"
#define MSQ_TTS_SERVICE        "MSQ_TTS_SERVICE"
#define MSQ_AUDIO_SERVICE      "MSQ_AUDIO_SERVICE"
#define MSQ_WATCHDOG           "MSQ_WATCHDOG"


#define SHM_MEDIA_SONG_LIST             "SHM_MEDIA_SONG_LIST"
#define SHM_MEDIA_TYPE_CONTENT_LIST     "SHM_MEDIA_TYPE_CONTENT_LIST"

/**
 * @brief The eHmiRequestMsgs enum provides message IDs for requests from HMI module
 */
enum eHmiRequestMsg {
    IDM_REQ_HMI_MESSAGE_START = 0x1000,
    IDM_REQ_HMI_SPEECH_PLAY_MUSIC_BY_TYPE,
    IDM_REQ_HMI_SPEECH_CHANGE_LANGUAGE,
    IDM_REQ_HMI_SPEECH_GET_CONFIGURATION,
    IDM_REQ_HMI_SPEECH_PLAY_SONG_LIST_BY_ID,
    IDM_REQ_HMI_SPEECH_PLAY_SONG_LIST_BY_VALUE,
    IDM_REQ_HMI_SPEECH_PLAYBACK_ACTION,
    IDM_REQ_HMI_WATCHDOG_I_AM_ALIVE,
};


/**
 * @brief The eHmiResponseMsgs enum provides message IDs for Response to HMI module
 */
enum eHmiResponseMsg {
    IDM_RES_HMI_MESSAGE_START = 0x2000,
    IDM_RES_SPEECH_HMI_PLAY_MUSIC_BY_TYPE,
    IDM_RES_SPEECH_HMI_GET_LIST_BY_TYPE,
    IDM_RES_SPEECH_HMI_CHANGED_LANGUAGE,
    IDM_RES_SPEECH_HMI_PLAY_SONG_LIST,
    IDM_RES_SPEECH_HMI_PLAYBACK_ACTION,
    IDM_RES_SPEECH_HMI_GET_CONFIGURATION,
    IDM_RES_SPEECH_HMI_TTS_CONVERT_TEXT,
    IDM_RES_SPEECH_HMI_AUDIO_PLAY_A_FILE,
    IDM_RES_SPEECH_HMI_GET_LIST_BY_ID,
    IDM_RES_SPEECH_HMI_PLAY_LIST_BY_ID,
    IDM_RES_SPEECH_HMI_PROCESSING_STATUS,
};


/**
 * @brief The eSpeechRequestMsgs enum provides message IDs for requests from Speech module
 */
enum eSpeechRequestMsg {
    IDM_REQ_SPEECH_MESSAGE_START = 0x3000,
    IDM_REQ_SPEECH_MEDIA_GET_LIST_BY_TYPE,
    IDM_REQ_SPEECH_TTS_CONVERT_TEXT,
    IDM_REQ_SPEECH_AUDIO_PLAY_A_FILE,
    IDM_REQ_SPEECH_AUDIO_PLAY_A_LIST,
    IDM_REQ_SPEECH_AUDIO_PLAYBACK_ACTION,
    IDM_REQ_SPEECH_MEDIA_GET_SONG_LIST,
    IDM_REQ_SPEECH_WATCHDOG_I_AM_ALIVE
};


/**
 * @brief The eSpeechResponseMsgs enum provides message IDs for response to Speech module
 */

enum eSpeechResponseMsg {
    IDM_RES_SPEECH_MESSAGE_START = 0x4000,
    IDM_RES_MEDIA_SPEECH_LIST_BY_TYPE,
    IDM_RES_TTS_SPEECH_CONVERT_TEXT,
    IDM_RES_AUDIO_SPEECH_PLAY_A_FILE,
    IDM_RES_AUDIO_SPEECH_PLAY_A_LIST,
    IDM_RES_AUDIO_SPEECH_PLAYBACK_ACTION,
    IDM_RES_MEDIA_SPEECH_GET_SONG_LIST
};


/**
 * @brief The eMediaRequestMsgs enum provides message IDs for requests from Media module
 */
enum eMediaRequestMsgs {
    IDM_REQ_MEDIA_MESSAGE_START = 0x5000,
    IDM_REQ_MEDIA_WATCHDOG_I_AM_ALIVE
};


/**
 * @brief The eMediaResponseMsgs enum provides message IDs for response to Media module
 */

enum eMediaResponseMsg {
    IDM_RES_MEDIA_MESSAGE_START = 0x6000,
};


/**
 * @brief The eTtsRequestMsgs enum provides message IDs for requests from TTS module
 */
enum eTtsRequestMsg {
    IDM_REQ_TTS_MESSAGE_START = 0x7000,
    IDM_REQ_TTS_WATCHDOG_I_AM_ALIVE
};

/**
 * @brief The eTtsResponseMsgs enum provides message IDs for response to TTS module
 */

enum eTtsResponseMsgs {
    IDM_RES_TTS_MESSAGE_START = 0x8000,
};


/**
 * @brief The eTtsRequestMsgs enum provides message IDs for requests from Audio module
 */
enum eAudioRequestMsg {
    IDM_REQ_AUDIO_MESSAGE_START = 0x9000,
    IDM_REQ_AUDIO_WATCHDOG_I_AM_ALIVE
};


/**
 * @brief The eAudioResponseMsgs enum provides message IDs for response to Audio module
 */

enum eAudioResponseMsg {
    IDM_RES_AUDIO_MESSAGE_START = 0xA000,
};


/**
 * @brief The eAudioResponseMsgs enum provides message IDs for response to Audio module
 */

enum eTestMsg {
    IDM_REQ_TEST_MESSAGE_START = 0xB000,
    IDM_REQ_TEST_SET_TEST_CASE = 0xB001,
};


#endif
