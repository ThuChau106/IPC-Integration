/**
 * @file GloablHeader.h
 *
 * @brief This file provides data structure for Global data
 *
 * @author Tung Duc Nguyen
 * email:  tung2.nguyen@lge.com
 *
 */


#ifndef _DTU_INTERN_GLOBAL_H
#define _DTU_INTERN_GLOBAL_H


#define MESSAGE_MAX_LEN         1000;


enum eProcID {
    IDP_HMI = 			0x01,
    IDP_SPEECH_SERVICE = 	0x02,
    IDP_MEDIA_SERVICE = 	0x03,
    IDP_AUDIO_SERVICE = 	0x04,
    IDP_TTS_SERVICE = 		0x05,
    IDP_WATCHDOG =              0x10,

    MAX_PROC_NAME
};

enum CodeId
{
        GLOBAL_STATE_CODE = MAX_PROC_NAME + 1
};

enum TimerId
{
        GLOBAL_TIMER_CODE = GLOBAL_STATE_CODE + 1
};


#endif
