#ifndef AUDIO_H
#define AUDIO_H

#include "miniaudio.h"

struct SoundEntry { ma_sound snd; bool loaded = false; };

extern ma_engine gAudioEngine;
extern bool gAudioReady;

extern SoundEntry sndTheme;
extern SoundEntry sndMissileLaunch;
extern SoundEntry sndExplosion;
extern SoundEntry sndDroneHum;
extern SoundEntry sndEarthquake;
extern SoundEntry sndAlarm;
extern SoundEntry sndCowMoo;
extern SoundEntry sndBombDrop;

void initAudio();
void playSound(SoundEntry& e, float volume = 1.0f);
void stopSound(SoundEntry& e);
void cleanupAudio();

#endif // AUDIO_H
