/****************************************************************************
** Meta object code from reading C++ file 'MediaAppMain.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.12.8)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../media_hmi/MediaAppMain.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'MediaAppMain.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.12.8. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_MediaAppMain_t {
    QByteArrayData data[63];
    char stringdata0[1135];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_MediaAppMain_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_MediaAppMain_t qt_meta_stringdata_MediaAppMain = {
    {
QT_MOC_LITERAL(0, 0, 12), // "MediaAppMain"
QT_MOC_LITERAL(1, 13, 23), // "textMessageAudioChanged"
QT_MOC_LITERAL(2, 37, 0), // ""
QT_MOC_LITERAL(3, 38, 11), // "textMessage"
QT_MOC_LITERAL(4, 50, 22), // "currentLanguageChanged"
QT_MOC_LITERAL(5, 73, 6), // "m_lang"
QT_MOC_LITERAL(6, 80, 16), // "songsListChanged"
QT_MOC_LITERAL(7, 97, 22), // "std::list<songsStruct>"
QT_MOC_LITERAL(8, 120, 7), // "newList"
QT_MOC_LITERAL(9, 128, 13), // "addSongToList"
QT_MOC_LITERAL(10, 142, 4), // "song"
QT_MOC_LITERAL(11, 147, 6), // "artist"
QT_MOC_LITERAL(12, 154, 5), // "album"
QT_MOC_LITERAL(13, 160, 9), // "clearList"
QT_MOC_LITERAL(14, 170, 16), // "sendButtonSignal"
QT_MOC_LITERAL(15, 187, 3), // "cmd"
QT_MOC_LITERAL(16, 191, 9), // "setLangue"
QT_MOC_LITERAL(17, 201, 16), // "playTypeFunction"
QT_MOC_LITERAL(18, 218, 18), // "listSelectFunction"
QT_MOC_LITERAL(19, 237, 5), // "value"
QT_MOC_LITERAL(20, 243, 16), // "textMessageAudio"
QT_MOC_LITERAL(21, 260, 15), // "currentLanguage"
QT_MOC_LITERAL(22, 276, 16), // "currentSongsList"
QT_MOC_LITERAL(23, 293, 14), // "eLanguageValue"
QT_MOC_LITERAL(24, 308, 19), // "LANGUAGE_VIETNAMESE"
QT_MOC_LITERAL(25, 328, 16), // "LANGUAGE_ENGLISH"
QT_MOC_LITERAL(26, 345, 12), // "eActionValue"
QT_MOC_LITERAL(27, 358, 12), // "ACTION_PAUSE"
QT_MOC_LITERAL(28, 371, 13), // "ACTION_RESUME"
QT_MOC_LITERAL(29, 385, 15), // "ACTION_PREVIOUS"
QT_MOC_LITERAL(30, 401, 11), // "ACTION_NEXT"
QT_MOC_LITERAL(31, 413, 10), // "eMusicType"
QT_MOC_LITERAL(32, 424, 17), // "MUSIC_TYPE_ARTIST"
QT_MOC_LITERAL(33, 442, 16), // "MUSIC_TYPE_ALBUM"
QT_MOC_LITERAL(34, 459, 17), // "MUSIC_TYPE_FOLDER"
QT_MOC_LITERAL(35, 477, 18), // "eResPlayByTypeCode"
QT_MOC_LITERAL(36, 496, 6), // "SUCESS"
QT_MOC_LITERAL(37, 503, 18), // "FAILED_TO_GET_LIST"
QT_MOC_LITERAL(38, 522, 10), // "WRONG_TYPE"
QT_MOC_LITERAL(39, 533, 20), // "SERVICE_IS_NOT_READY"
QT_MOC_LITERAL(40, 554, 11), // "eResPlayMp3"
QT_MOC_LITERAL(41, 566, 6), // "FAILED"
QT_MOC_LITERAL(42, 573, 19), // "SERIVE_IS_NOT_READY"
QT_MOC_LITERAL(43, 593, 18), // "eResChangeLanguage"
QT_MOC_LITERAL(44, 612, 11), // "WRONG_VALUE"
QT_MOC_LITERAL(45, 624, 7), // "eStatus"
QT_MOC_LITERAL(46, 632, 19), // "STATUS_INITIALIZING"
QT_MOC_LITERAL(47, 652, 12), // "STATUS_READY"
QT_MOC_LITERAL(48, 665, 17), // "STATUS_PROCESSING"
QT_MOC_LITERAL(49, 683, 14), // "STATUS_CLOSING"
QT_MOC_LITERAL(50, 698, 15), // "eHmiResponseMsg"
QT_MOC_LITERAL(51, 714, 25), // "IDM_RES_HMI_MESSAGE_START"
QT_MOC_LITERAL(52, 740, 37), // "IDM_RES_SPEECH_HMI_PLAY_MUSIC..."
QT_MOC_LITERAL(53, 778, 35), // "IDM_RES_SPEECH_HMI_GET_LIST_B..."
QT_MOC_LITERAL(54, 814, 35), // "IDM_RES_SPEECH_HMI_CHANGED_LA..."
QT_MOC_LITERAL(55, 850, 33), // "IDM_RES_SPEECH_HMI_PLAY_SONG_..."
QT_MOC_LITERAL(56, 884, 34), // "IDM_RES_SPEECH_HMI_PLAYBACK_A..."
QT_MOC_LITERAL(57, 919, 36), // "IDM_RES_SPEECH_HMI_GET_CONFIG..."
QT_MOC_LITERAL(58, 956, 35), // "IDM_RES_SPEECH_HMI_TTS_CONVER..."
QT_MOC_LITERAL(59, 992, 36), // "IDM_RES_SPEECH_HMI_AUDIO_PLAY..."
QT_MOC_LITERAL(60, 1029, 33), // "IDM_RES_SPEECH_HMI_GET_LIST_B..."
QT_MOC_LITERAL(61, 1063, 34), // "IDM_RES_SPEECH_HMI_PLAY_LIST_..."
QT_MOC_LITERAL(62, 1098, 36) // "IDM_RES_SPEECH_HMI_PROCESSING..."

    },
    "MediaAppMain\0textMessageAudioChanged\0"
    "\0textMessage\0currentLanguageChanged\0"
    "m_lang\0songsListChanged\0std::list<songsStruct>\0"
    "newList\0addSongToList\0song\0artist\0"
    "album\0clearList\0sendButtonSignal\0cmd\0"
    "setLangue\0playTypeFunction\0"
    "listSelectFunction\0value\0textMessageAudio\0"
    "currentLanguage\0currentSongsList\0"
    "eLanguageValue\0LANGUAGE_VIETNAMESE\0"
    "LANGUAGE_ENGLISH\0eActionValue\0"
    "ACTION_PAUSE\0ACTION_RESUME\0ACTION_PREVIOUS\0"
    "ACTION_NEXT\0eMusicType\0MUSIC_TYPE_ARTIST\0"
    "MUSIC_TYPE_ALBUM\0MUSIC_TYPE_FOLDER\0"
    "eResPlayByTypeCode\0SUCESS\0FAILED_TO_GET_LIST\0"
    "WRONG_TYPE\0SERVICE_IS_NOT_READY\0"
    "eResPlayMp3\0FAILED\0SERIVE_IS_NOT_READY\0"
    "eResChangeLanguage\0WRONG_VALUE\0eStatus\0"
    "STATUS_INITIALIZING\0STATUS_READY\0"
    "STATUS_PROCESSING\0STATUS_CLOSING\0"
    "eHmiResponseMsg\0IDM_RES_HMI_MESSAGE_START\0"
    "IDM_RES_SPEECH_HMI_PLAY_MUSIC_BY_TYPE\0"
    "IDM_RES_SPEECH_HMI_GET_LIST_BY_TYPE\0"
    "IDM_RES_SPEECH_HMI_CHANGED_LANGUAGE\0"
    "IDM_RES_SPEECH_HMI_PLAY_SONG_LIST\0"
    "IDM_RES_SPEECH_HMI_PLAYBACK_ACTION\0"
    "IDM_RES_SPEECH_HMI_GET_CONFIGURATION\0"
    "IDM_RES_SPEECH_HMI_TTS_CONVERT_TEXT\0"
    "IDM_RES_SPEECH_HMI_AUDIO_PLAY_A_FILE\0"
    "IDM_RES_SPEECH_HMI_GET_LIST_BY_ID\0"
    "IDM_RES_SPEECH_HMI_PLAY_LIST_BY_ID\0"
    "IDM_RES_SPEECH_HMI_PROCESSING_STATUS"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_MediaAppMain[] = {

 // content:
       8,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       3,   90, // properties
       8,  102, // enums/sets
       0,    0, // constructors
       0,       // flags
       5,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   59,    2, 0x06 /* Public */,
       4,    1,   62,    2, 0x06 /* Public */,
       6,    1,   65,    2, 0x06 /* Public */,
       9,    3,   68,    2, 0x06 /* Public */,
      13,    0,   75,    2, 0x06 /* Public */,

 // methods: name, argc, parameters, tag, flags
      14,    1,   76,    2, 0x02 /* Public */,
      16,    1,   79,    2, 0x02 /* Public */,
      17,    1,   82,    2, 0x02 /* Public */,
      18,    2,   85,    2, 0x02 /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    3,
    QMetaType::Void, QMetaType::Int,    5,
    QMetaType::Void, 0x80000000 | 7,    8,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString,   10,   11,   12,
    QMetaType::Void,

 // methods: parameters
    QMetaType::Void, QMetaType::Int,   15,
    QMetaType::Void, QMetaType::Int,   15,
    QMetaType::Void, QMetaType::Int,   15,
    QMetaType::Void, QMetaType::Int, QMetaType::Int,   15,   19,

 // properties: name, type, flags
      20, QMetaType::QString, 0x00495103,
      21, QMetaType::Int, 0x00495103,
      22, 0x80000000 | 7, 0x0049510b,

 // properties: notify_signal_id
       0,
       1,
       2,

 // enums: name, alias, flags, count, data
      23,   23, 0x0,    2,  142,
      26,   26, 0x0,    4,  146,
      31,   31, 0x0,    3,  154,
      35,   35, 0x2,    4,  160,
      40,   40, 0x2,    3,  168,
      43,   43, 0x2,    3,  174,
      45,   45, 0x0,    4,  180,
      50,   50, 0x0,   12,  188,

 // enum data: key, value
      24, uint(MediaAppMain::LANGUAGE_VIETNAMESE),
      25, uint(MediaAppMain::LANGUAGE_ENGLISH),
      27, uint(MediaAppMain::ACTION_PAUSE),
      28, uint(MediaAppMain::ACTION_RESUME),
      29, uint(MediaAppMain::ACTION_PREVIOUS),
      30, uint(MediaAppMain::ACTION_NEXT),
      32, uint(MediaAppMain::MUSIC_TYPE_ARTIST),
      33, uint(MediaAppMain::MUSIC_TYPE_ALBUM),
      34, uint(MediaAppMain::MUSIC_TYPE_FOLDER),
      36, uint(MediaAppMain::eResPlayByTypeCode::SUCESS),
      37, uint(MediaAppMain::eResPlayByTypeCode::FAILED_TO_GET_LIST),
      38, uint(MediaAppMain::eResPlayByTypeCode::WRONG_TYPE),
      39, uint(MediaAppMain::eResPlayByTypeCode::SERVICE_IS_NOT_READY),
      36, uint(MediaAppMain::eResPlayMp3::SUCESS),
      41, uint(MediaAppMain::eResPlayMp3::FAILED),
      42, uint(MediaAppMain::eResPlayMp3::SERIVE_IS_NOT_READY),
      36, uint(MediaAppMain::eResChangeLanguage::SUCESS),
      41, uint(MediaAppMain::eResChangeLanguage::FAILED),
      44, uint(MediaAppMain::eResChangeLanguage::WRONG_VALUE),
      46, uint(MediaAppMain::STATUS_INITIALIZING),
      47, uint(MediaAppMain::STATUS_READY),
      48, uint(MediaAppMain::STATUS_PROCESSING),
      49, uint(MediaAppMain::STATUS_CLOSING),
      51, uint(MediaAppMain::IDM_RES_HMI_MESSAGE_START),
      52, uint(MediaAppMain::IDM_RES_SPEECH_HMI_PLAY_MUSIC_BY_TYPE),
      53, uint(MediaAppMain::IDM_RES_SPEECH_HMI_GET_LIST_BY_TYPE),
      54, uint(MediaAppMain::IDM_RES_SPEECH_HMI_CHANGED_LANGUAGE),
      55, uint(MediaAppMain::IDM_RES_SPEECH_HMI_PLAY_SONG_LIST),
      56, uint(MediaAppMain::IDM_RES_SPEECH_HMI_PLAYBACK_ACTION),
      57, uint(MediaAppMain::IDM_RES_SPEECH_HMI_GET_CONFIGURATION),
      58, uint(MediaAppMain::IDM_RES_SPEECH_HMI_TTS_CONVERT_TEXT),
      59, uint(MediaAppMain::IDM_RES_SPEECH_HMI_AUDIO_PLAY_A_FILE),
      60, uint(MediaAppMain::IDM_RES_SPEECH_HMI_GET_LIST_BY_ID),
      61, uint(MediaAppMain::IDM_RES_SPEECH_HMI_PLAY_LIST_BY_ID),
      62, uint(MediaAppMain::IDM_RES_SPEECH_HMI_PROCESSING_STATUS),

       0        // eod
};

void MediaAppMain::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<MediaAppMain *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->textMessageAudioChanged((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->currentLanguageChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 2: _t->songsListChanged((*reinterpret_cast< std::list<songsStruct>(*)>(_a[1]))); break;
        case 3: _t->addSongToList((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QString(*)>(_a[3]))); break;
        case 4: _t->clearList(); break;
        case 5: _t->sendButtonSignal((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 6: _t->setLangue((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 7: _t->playTypeFunction((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 8: _t->listSelectFunction((*reinterpret_cast< int(*)>(_a[1])),(*reinterpret_cast< int(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (MediaAppMain::*)(QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MediaAppMain::textMessageAudioChanged)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (MediaAppMain::*)(int );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MediaAppMain::currentLanguageChanged)) {
                *result = 1;
                return;
            }
        }
        {
            using _t = void (MediaAppMain::*)(std::list<songsStruct> );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MediaAppMain::songsListChanged)) {
                *result = 2;
                return;
            }
        }
        {
            using _t = void (MediaAppMain::*)(QString , QString , QString );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MediaAppMain::addSongToList)) {
                *result = 3;
                return;
            }
        }
        {
            using _t = void (MediaAppMain::*)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&MediaAppMain::clearList)) {
                *result = 4;
                return;
            }
        }
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty) {
        auto *_t = static_cast<MediaAppMain *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: *reinterpret_cast< QString*>(_v) = _t->textMessageAudio(); break;
        case 1: *reinterpret_cast< int*>(_v) = _t->currentLanguage(); break;
        case 2: *reinterpret_cast< std::list<songsStruct>*>(_v) = _t->currentSongsList(); break;
        default: break;
        }
    } else if (_c == QMetaObject::WriteProperty) {
        auto *_t = static_cast<MediaAppMain *>(_o);
        Q_UNUSED(_t)
        void *_v = _a[0];
        switch (_id) {
        case 0: _t->setTextMessageAudio(*reinterpret_cast< QString*>(_v)); break;
        case 1: _t->setCurrentLanguage(*reinterpret_cast< int*>(_v)); break;
        case 2: _t->setCurrentSongsList(*reinterpret_cast< std::list<songsStruct>*>(_v)); break;
        default: break;
        }
    } else if (_c == QMetaObject::ResetProperty) {
    }
#endif // QT_NO_PROPERTIES
}

QT_INIT_METAOBJECT const QMetaObject MediaAppMain::staticMetaObject = { {
    &QObject::staticMetaObject,
    qt_meta_stringdata_MediaAppMain.data,
    qt_meta_data_MediaAppMain,
    qt_static_metacall,
    nullptr,
    nullptr
} };


const QMetaObject *MediaAppMain::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MediaAppMain::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_MediaAppMain.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int MediaAppMain::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
#ifndef QT_NO_PROPERTIES
    else if (_c == QMetaObject::ReadProperty || _c == QMetaObject::WriteProperty
            || _c == QMetaObject::ResetProperty || _c == QMetaObject::RegisterPropertyMetaType) {
        qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyDesignable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyScriptable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyStored) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyEditable) {
        _id -= 3;
    } else if (_c == QMetaObject::QueryPropertyUser) {
        _id -= 3;
    }
#endif // QT_NO_PROPERTIES
    return _id;
}

// SIGNAL 0
void MediaAppMain::textMessageAudioChanged(QString _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void MediaAppMain::currentLanguageChanged(int _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void MediaAppMain::songsListChanged(std::list<songsStruct> _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void MediaAppMain::addSongToList(QString _t1, QString _t2, QString _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void MediaAppMain::clearList()
{
    QMetaObject::activate(this, &staticMetaObject, 4, nullptr);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
