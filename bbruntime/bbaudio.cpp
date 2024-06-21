#include "std.h"
#include "bbaudio.h"
#include "../soloud/soloud.h"
#include "../soloud/soloud_wav.h"
#include "../MultiLang/MultiLang.h"
#include "../soloud/soloud_wavstream.h"

std::set<Sound*> sounds;
SoLoud::Soloud* soloud;

float rolloffFactor = 1.0f;
float dopplerFactor = 1.0f;
float distanceFactor = 1.0f;

struct Sound {
    SoLoud::Wav* const wav;

    float volume = 1.0f;
    float pan = 0;
    int pitch = 0;

    explicit Sound(SoLoud::Wav* wav) : wav(wav) {
        sounds.insert(this);
    }

    ~Sound() {
        sounds.erase(this);
        delete wav;
    }

    Channel play() const {
        Channel channel;
        if (pitch) {
            channel = soloud->play(*wav, volume, pan, true, 0);
            soloud->setSamplerate(channel, (float)pitch);
            soloud->setPause(channel, false);
        }
        else {
            channel = soloud->play(*wav, volume, pan, false, 0);
        }
        return channel;
    }

    Channel play3d(float x, float y, float z, float vx, float vy, float vz) const {
        Channel channel;
        wav->set3dAttenuation(SoLoud::AudioSource::INVERSE_DISTANCE, rolloffFactor);
        wav->set3dDopplerFactor(dopplerFactor);
        if (pitch) {
            channel = soloud->play3d(*wav, x, y, z, vx, vy, vz, volume, true, 0);
            soloud->setSamplerate(channel, (float)pitch);
            soloud->setPause(channel, false);
        }
        else {
            channel = soloud->play3d(*wav, x, y, z, vx, vy, vz, volume, false, 0);
        }
        return channel;
    }
};

static inline void debugSound(Sound* s, std::string function) {
    if (!sounds.contains(s)) ErrorLog(function, MultiLang::sound_not_exist);
}

int bbVerifySound(Sound* sound) {
    return sounds.contains(sound);
}

Sound* bbLoadSound(BBStr* f) {
    SoLoud::Wav* wav = new SoLoud::Wav();
    SoLoud::result r = wav->load(f->c_str());
    delete f;
    return r == SoLoud::SO_NO_ERROR ? new Sound(wav) : nullptr;
}

Sound* bbLoad3DSound(BBStr* f) {
    return bbLoadSound(f);
}

void bbFreeSound(Sound* sound) {
    if (!sound) return;
    delete sound;
}

void bbLoopSound(Sound* sound) {
    if (!sound) return;
    debugSound(sound, "LoopSound");
    sound->wav->setLooping(true);
}

void bbSoundPitch(Sound* sound, int pitch) {
    if (!sound) return;
    debugSound(sound, "SoundPitch");
    sound->pitch = pitch;
}

void bbSoundVolume(Sound* sound, float volume) {
    if (!sound) return;
    debugSound(sound, "SoundVolume");
    sound->volume = volume;
}

void bbSoundPan(Sound* sound, float pan) {
    if (!sound) return;
    debugSound(sound, "SoundPan");
    sound->pan = pan;
}

uint32_t bbPlaySound(Sound* sound) {
    if (!sound) return 0;
    debugSound(sound, "PlaySound");
    return sound->play();
}

Channel bbPlayMusic(BBStr* f) {
    SoLoud::WavStream wavStream;
    SoLoud::result r = wavStream.load(f->c_str());
    delete f;
    return r == SoLoud::SO_NO_ERROR ? soloud->play(wavStream) : 0;
}

//Channel bbPlayCDTrack(int track, int mode) {
//	return gx_audio ? gx_audio->playCDTrack(track, mode) : 0;
//}

void bbStopChannel(Channel channel) {
    if (!channel) return;
    soloud->stop(channel);
}

void bbPauseChannel(Channel channel) {
    if (!channel) return;
    soloud->setPause(channel, true);
}

void bbResumeChannel(Channel channel) {
    if (!channel) return;
    soloud->setPause(channel, false);
}

void bbChannelPitch(Channel channel, int pitch) {
    if (!channel) return;
    soloud->setSamplerate(channel, (float) pitch);
}

void bbChannelVolume(Channel channel, float volume) {
    if (!channel) return;
    soloud->setVolume(channel, volume);
}

void bbChannelPan(Channel channel, float pan) {
    if (!channel) return;
    soloud->setPan(channel, pan);
}

int bbChannelPlaying(Channel channel) {
    return soloud->isValidVoiceHandle(channel);
}

Channel bbPlay3dSound(Sound* sound, float x, float y, float z, float vx, float vy, float vz) {
    if (!sound) return 0;
    debugSound(sound, "Play3dSound");
    auto sc = distanceFactor;
    return sound->play3d(x * sc, y * sc, z * sc, vx * sc, vy * sc, vz * sc);
}

void bbSet3dChannel(Channel channel, float x, float y, float z, float vx, float vy, float vz) {
    if (!channel) return;
    auto sc = distanceFactor;
    soloud->set3dSourceParameters(channel, x * sc, y * sc, z * sc, vx * sc, vy * sc, vz * sc);
}

void bbSet3dListenerConfig(float roll, float dopp, float dist) {
    if (!soloud) return;
    rolloffFactor = roll;
    dopplerFactor = dopp;
    distanceFactor = dist;
}

void bbSet3dListener(float x, float y, float z, float kx, float ky, float kz, float jx, float jy, float jz, float vx,
    float vy, float vz) {
    if (!soloud) return;
    auto sc = distanceFactor;
    soloud->set3dListenerParameters(x * sc, y * sc, z * sc, kx, ky, kz, jy, jy, jz, vx * sc, vy * sc, vz * sc);
    soloud->update3dAudio();
}

bool audio_create() {
    soloud = new SoLoud::Soloud();
    if (soloud->init() != SoLoud::SO_NO_ERROR) {
        delete soloud;
        soloud = nullptr;
    }
    return true;
}

bool audio_destroy() {
    if (!soloud) return true;
    soloud->deinit();
    delete soloud;
    soloud = nullptr;
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
    rtSym("%Load3DSound$filename", bbLoad3DSound);
}