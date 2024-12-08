#include "gxAudio.h"
#include "../bbruntime/bbsys.h"
#include "../soloud/soloud_c.h"
#include "../soloud/soloud_wav.h"
#include "../soloud/soloud_wavstream.h"

gxAudio::SoundChannel::~SoundChannel() {
    if (!SoundChannel::isPlaying()) return;

    SoundChannel::stop();
}

void gxAudio::SoundChannel::stop() {
    if (!SoundChannel::isPlaying()) return;

    soloud->stop(handle);
}

void gxAudio::SoundChannel::setPaused(const bool paused) {
    if (!SoundChannel::isPlaying()) return;

    soloud->setPause(handle, paused);
}

void gxAudio::SoundChannel::setPitch(const int pitch) {
    if (!SoundChannel::isPlaying()) return;

    soloud->setSamplerate(handle, static_cast<float>(pitch));
}

void gxAudio::SoundChannel::setVolume(const float volume) {
    if (!SoundChannel::isPlaying()) return;

    soloud->setVolume(handle, volume);
}

void gxAudio::SoundChannel::setPan(const float pan) {
    if (!SoundChannel::isPlaying()) return;

    soloud->setPan(handle, pan);
}

void gxAudio::SoundChannel::set3d(const float x, const float y, const float z, const float vx, const float vy, const float vz, const float dist) {
    if (!SoundChannel::isPlaying()) return;

    soloud->set3dSourceParameters(handle, x * dist, y * dist, z * dist,
                                  vx * dist, vy * dist, vz * dist);
}

bool gxAudio::SoundChannel::isPlaying() {
    return soloud->isValidVoiceHandle(handle);
}

gxAudio::gxAudio() {
    soloud = new SoLoud::Soloud();

    if (soloud->init(SOLOUD_LEFT_HANDED_3D) != SoLoud::SO_NO_ERROR) {
        delete soloud;
        soloud = nullptr;
    }
}

gxAudio::~gxAudio() {
    if (!soloud) return;

    soloud->deinit();
    delete soloud;
    soloud = nullptr;
}

void gxAudio::set3dListenerConfig(const float roll, const float dopp, const float dist) {
    rolloff_factor_ = roll;
    doppler_factor_ = dopp;
    distance_factor_ = dist;
}

void gxAudio::set3dListener(const float x, const float y, const float z, const float kx, const float ky, const float kz, float jx, const float jy, const float jz,
    const float vx, const float vy, const float vz) {
    if (!soloud) return;

    soloud->set3dListenerParameters(x * distance_factor_, y * distance_factor_, z * distance_factor_, kx, ky, kz, jy, jy, jz, vx * distance_factor_, vy * distance_factor_, vz * distance_factor_);
    soloud->update3dAudio();
}

gxAudio::Sound* gxAudio::loadSound(const char* path) {
    const auto sound = new SoLoud::Wav();

    if (sound->load(path) != SoLoud::SO_NO_ERROR) {
        delete sound;
        return nullptr;
    }

    return new Sound{.source = sound};
}

gxAudio::SoundChannel* gxAudio::playSound(const Sound& sound) {
    if (!soloud) return nullptr;

    const channel_handle channel = soloud->play(*sound.source, sound.volume, sound.pan, sound.pitch != 0);
    const auto sound_channel = new SoundChannel(*soloud, channel, sound);

    if (sound.pitch != 0) {
        sound_channel->setPitch(sound.pitch);
        sound_channel->setPaused(false);
    }

    return sound_channel;
}

gxAudio::SoundChannel* gxAudio::play3dSound(const Sound& sound, const float x, const float y, const float z, const float vx, const float vy, const float vz) {
    if (!soloud) return nullptr;

    sound.source->set3dAttenuation(SoLoud::AudioSource::INVERSE_DISTANCE, rolloff_factor_);
    sound.source->set3dDopplerFactor(doppler_factor_);

    const channel_handle channel = soloud->play3d(*sound.source, x, y, z, vx, vy, vz, sound.volume, false, 0);
    const auto sound_channel = new SoundChannel(*soloud, channel, sound);

    if (sound.pitch) {
        sound_channel->setPitch(sound.pitch);
        sound_channel->setPaused(false);
    }

    return sound_channel;
}

gxAudio::StreamChannel* gxAudio::playMusic(const char* path) {
    if (!soloud) return nullptr;

    const auto stream = new SoLoud::WavStream();

    if (stream->load(path) != SoLoud::SO_NO_ERROR) {
        delete stream;
        return nullptr;
    }

    const auto sound = new Sound{.source = stream};

    const channel_handle handle = soloud->play(*sound->source);
    const auto channel = new StreamChannel{*soloud, handle, *sound};

    return channel;
}

void gxAudio::set3dChannel(gxChannel* channel, const float x, const float y, const float z, const float vx, const float vy, const float vz) {
    channel->set3d(x, y, z, vx, vy, vz, distance_factor_);
}