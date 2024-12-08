#ifndef GXCHANNEL_H
#define GXCHANNEL_H
#include "../soloud/soloud.h"

class gxChannel
{
public:
    gxChannel(SoLoud::Soloud& soloud) : soloud(&soloud) {}
    virtual ~gxChannel();
    virtual void stop() = 0;
    virtual void setPaused(bool paused) = 0;
    virtual void setPitch(int pitch) = 0;
    virtual void setVolume(float volume) = 0;
    virtual void setPan(float pan) = 0;
    virtual void set3d(float x, float y, float z, float vx, float vy, float vz, float dist) = 0;
    virtual bool isPlaying() = 0;
protected:
    SoLoud::Soloud* soloud = nullptr;
};

#endif