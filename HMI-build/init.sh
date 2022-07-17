rm MSQ_HMI
rm MSQ_SPEECH_SERVICE
rm MSQ_MEDIA_SERVICE
rm MSQ_TTS_SERVICE
rm MSQ_AUDIO_SERVICE
rm MSQ_WATCHDOG
rm SHM_MEDIA_TYPE_CONTENT_LIST
rm SHM_MEDIA_SONG_LIST
rm tts
rm audio
rm sender
rm receiver
rm media
rm -rf voice
mkdir voice

touch MSQ_HMI
touch MSQ_SPEECH_SERVICE
touch MSQ_MEDIA_SERVICE
touch MSQ_TTS_SERVICE
touch MSQ_AUDIO_SERVICE
touch MSQ_WATCHDOG
touch SHM_MEDIA_TYPE_CONTENT_LIST
touch SHM_MEDIA_SONG_LIST

gcc -o tts ../TTS/gTTS.cpp -lpthread -lstdc++
gcc -o media ../Media/Media.cpp -lpthread -lstdc++
gcc -O2 -o audio ../Audio/playsong.cpp -lmpg123 -lao -lstdc++ -lpthread
gcc -o speech ../Thu/Speech.cpp -lpthread -lstdc++
