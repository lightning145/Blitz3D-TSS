#include "std.h"
#include "bbaudio.h"

#include "../MultiLang/MultiLang.h"
#include "../soloud/soloud_wav.h"
#include "../soloud/soloud_wavstream.h"

gxAudio* gx_audio;

set<gxAudio::Sound*> sounds;
set<gxChannel*> channels;

bool bbVerifySound(gxAudio::Sound* sound) {
    return sounds.contains(sound);
}

bool bbVerifyChannel(gxChannel* channel) {
    return channels.contains(channel);
}

static void debugSound(gxAudio::Sound* s, const char* function) {
    if (!sounds.contains(s)) ErrorLog(function, MultiLang::sound_not_exist)
}

static void debugChannel(gxChannel* channel, const char* function) {
    if (!channels.contains(channel)) ErrorLog(function, MultiLang::sound_not_exist)
}

gxAudio::Sound* bbLoadSound(const BBStr* path) {
    gxAudio::Sound* sound = gx_audio->loadSound(path->c_str());

    if (sound != nullptr) {
        sounds.insert(sound);
    }

    delete path;
    return sound;
}

void bbFreeSound(gxAudio::Sound* sound) {
    if (!bbVerifySound(sound)) return;

    sounds.erase(sound);

    delete sound;
}

void bbLoopSound(gxAudio::Sound* sound) {
    if (!sound) return;

    debugSound(sound, "LoopSound");

    sound->source->setLooping(true);
}

void bbSoundVolume(gxAudio::Sound* sound, const float volume) {
    if (!sound) return;

    debugSound(sound, "SoundVolume");

    sound->source->setVolume(volume);
}

void bbSoundPan(gxAudio::Sound* sound, const float pan) {
    if (!sound) return;

    debugSound(sound, "SoundPan");

    sound->pan = pan;
}

void bbSoundPitch(gxAudio::Sound* sound, const int pitch) {
    if (!sound) return;

    debugSound(sound, "SoundPitch");

    sound->pitch = pitch;
}

gxChannel* bbPlaySound(gxAudio::Sound* sound) {
    if (!sound) return nullptr;

    debugSound(sound, "PlaySound");

    gxChannel* channel = gx_audio->playSound(*sound);

    if (channel != nullptr) {
        channels.insert(channel);
    }

    return channel;
}

gxChannel* bbPlayMusic(const BBStr* path) {
    gxChannel* channel = gx_audio->playMusic(path->c_str());

    if (channel != nullptr) {
        channels.insert(channel);
    }

    return channel;
}

void bbStopChannel(gxChannel* channel) {
    if (!channel || !bbVerifyChannel(channel)) return;

    channel->stop();
    channels.erase(channel);

    delete channel;
}

void bbPauseChannel(gxChannel* channel) {
    if (!channel) return;

    debugChannel(channel, "PauseChannel");

    channel->setPaused(true);
}

void bbResumeChannel(gxChannel* channel) {
    if (!channel) return;

    debugChannel(channel, "ResumeChannel");

    channel->setPaused(false);
}

void bbChannelVolume(gxChannel* channel, const float volume) {
    if (!channel) return;

    debugChannel(channel, "ChannelVolume");

    channel->setVolume(volume);
}

void bbChannelPan(gxChannel* channel, const float pan) {
    if (!channel) return;

    debugChannel(channel, "ChannelPan");

    channel->setPan(pan);
}

void bbChannelPitch(gxChannel* channel, const int pitch) {
    if (!channel) return;

    debugChannel(channel, "ChannelPitch");

    channel->setPitch(pitch);
}

bool bbChannelPlaying(gxChannel* channel) {
    return bbVerifyChannel(channel) && channel->isPlaying();
}

gxChannel* bbPlay3dSound(gxAudio::Sound* sound, const float x, const float y, const float z, const float vx, const float vy, const float vz) {
    if (!sound) return nullptr;

    debugSound(sound, "Play3dSound");

    gxChannel* channel = gx_audio->play3dSound(*sound, x, y, z, vx, vy, vz);

    if (channel != nullptr) {
        channels.insert(channel);
    }

    return channel;
}

void bbSet3dChannel(gxChannel* channel, const float x, const float y, const float z, const float vx, const float vy, const float vz) {
    if (!channel) return;

    debugChannel(channel, "Set3dChannel");

    gx_audio->set3dChannel(channel, x, y, z, vx, vy, vz);
}

void bbSet3dListenerConfig(const float roll, const float dopp, const float dist) {
    gx_audio->set3dListenerConfig(roll, dopp, dist);
}

void bbSet3dListener(const float x, const float y, const float z, const float kx, const float ky, const float kz, const float jx, const float jy, const float jz, const float vx,
    const float vy, const float vz) {
    gx_audio->set3dListener(x, y, z, kx, ky, kz, jx, jy, jz, vx, vy, vz);
}

bool audio_create() {
    gx_audio = new gxAudio();

    return true;
}

bool audio_destroy() {
    if (!gx_audio) return true;

    delete gx_audio;

    return true;
}

void audio_link(void(*rtSym)(const char*, void*)) {
    rtSym("%VerifySound%sound", bbVerifySound);
    rtSym("%LoadSound$filename", bbLoadSound);
    rtSym("FreeSound%sound", bbFreeSound);
    rtSym("LoopSound%sound", bbLoopSound);
    rtSym("SoundPitch%sound%pitch", bbSoundPitch);
    rtSym("SoundVolume%sound#volume", bbSoundVolume);
    rtSym("SoundPan%sound#pan", bbSoundPan);
    rtSym("%PlaySound%sound", bbPlaySound);
    rtSym("%PlayMusic$midifile%mode=1", bbPlayMusic);
    //rtSym("%PlayCDTrack%track%mode=1", bbPlayCDTrack);
    rtSym("StopChannel%channel", bbStopChannel);
    rtSym("PauseChannel%channel", bbPauseChannel);
    rtSym("ResumeChannel%channel", bbResumeChannel);
    rtSym("ChannelPitch%channel%pitch", bbChannelPitch);
    rtSym("ChannelVolume%channel#volume", bbChannelVolume);
    rtSym("ChannelPan%channel#pan", bbChannelPan);
    rtSym("%ChannelPlaying%channel", bbChannelPlaying);
    rtSym("%Load3DSound$filename", bbLoadSound);
}