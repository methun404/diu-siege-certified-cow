#include "audio.h"
#include <iostream>

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

ma_engine gAudioEngine;
bool gAudioReady = false;

SoundEntry sndTheme;
SoundEntry sndMissileLaunch, sndExplosion, sndDroneHum, sndEarthquake, sndAlarm, sndCowMoo, sndBombDrop;

void initAudio() {
    if (ma_engine_init(NULL, &gAudioEngine) != MA_SUCCESS) {
        std::cerr << "[Audio] Engine init failed\n"; return;
    }
    gAudioReady = true;
    auto load = [&](SoundEntry& e, const char* path, bool loop) {
        if (ma_sound_init_from_file(&gAudioEngine, path, MA_SOUND_FLAG_DECODE, NULL, NULL, &e.snd) == MA_SUCCESS) {
            ma_sound_set_looping(&e.snd, loop ? MA_TRUE : MA_FALSE);
            e.loaded = true;
            std::cout << "[Audio] Loaded: " << path << "\n";
        } else {
            std::cerr << "[Audio] Failed: " << path << "\n";
        }
    };
    load(sndTheme,         "audio/theme.wav",          true);
    load(sndMissileLaunch, "audio/missile_launch.wav", false);
    load(sndExplosion,     "audio/explosion.wav",      false);
    load(sndDroneHum,      "audio/drone_hum.wav",      true);
    load(sndEarthquake,    "audio/earthquake.wav",     false);
    load(sndAlarm,         "audio/alarm.wav",          false);
    load(sndCowMoo,        "audio/cow_moo.wav",        false);
    load(sndBombDrop,      "audio/bomb_drop.wav",      false);

    // Start background music softly
    if (sndTheme.loaded) {
        ma_sound_set_volume(&sndTheme.snd, 0.22f);
        ma_sound_start(&sndTheme.snd);
    }
}

void playSound(SoundEntry& e, float volume) {
    if (!gAudioReady || !e.loaded) return;
    ma_sound_seek_to_pcm_frame(&e.snd, 0);
    ma_sound_set_volume(&e.snd, volume);
    ma_sound_start(&e.snd);
}

void stopSound(SoundEntry& e) {
    if (!gAudioReady || !e.loaded) return;
    ma_sound_stop(&e.snd);
}

void cleanupAudio() {
    if (!gAudioReady) return;
    ma_sound_uninit(&sndTheme.snd);
    ma_sound_uninit(&sndMissileLaunch.snd);
    ma_sound_uninit(&sndExplosion.snd);
    ma_sound_uninit(&sndDroneHum.snd);
    ma_sound_uninit(&sndEarthquake.snd);
    ma_sound_uninit(&sndAlarm.snd);
    ma_sound_uninit(&sndCowMoo.snd);
    ma_sound_uninit(&sndBombDrop.snd);
    ma_engine_uninit(&gAudioEngine);
}
