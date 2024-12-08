#ifndef GXAUDIO_H
#define GXAUDIO_H

#include "gxchannel.h"
#include "../soloud/soloud.h"

typedef unsigned int channel_handle;

class gxAudio
{
public:

    struct Sound
    {
        SoLoud::AudioSource* source;
        float volume = 1.0f;
        float pan = 0.0f;
        int pitch = 0;

        ~Sound() {delete source;}
    };

    struct SoundChannel : gxChannel
    {
        channel_handle handle;
        const Sound* sound;

        SoundChannel(SoLoud::Soloud& soloud, const channel_handle handle, const Sound& sound) : gxChannel(soloud), handle(handle), sound(&sound) {
        }

        ~SoundChannel() override;

        void stop() override;
        void setPaused(bool paused) override;
        void setPitch(int pitch) override;
        void setVolume(float volume) override;
        void setPan(float pan) override;
        void set3d(float x, float y, float z, float vx, float vy, float vz, float dist) override;
        bool isPlaying() override;
    };

    struct StreamChannel : SoundChannel
    {
        StreamChannel(SoLoud::Soloud& soloud, const channel_handle handle, const Sound& sound): SoundChannel(soloud, handle, sound) { }
    };

    SoLoud::Soloud* soloud = nullptr;

    gxAudio();
    ~gxAudio();

    void set3dListenerConfig(float roll, float dopp, float dist);
    void set3dListener(float x, float y, float z, float kx, float ky, float kz, float jx, float jy, float jz, float vx, float vy, float vz);
    Sound* loadSound(const char* path);
    SoundChannel* playSound(const Sound& sound);
    SoundChannel* play3dSound(const Sound& sound, float x, float y, float z, float vx, float vy, float vz);
    StreamChannel* playMusic(const char* path);
    void set3dChannel(gxChannel* channel, float x, float y, float z, float vx, float vy, float vz);

private:
    float rolloff_factor_ = 1.0f;
    float doppler_factor_ = 1.0f;
    float distance_factor_ = 1.0f;
};

#endif