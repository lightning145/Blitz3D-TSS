#ifndef BBAUDIO_H
#define BBAUDIO_H

#include "bbsys.h"

struct Sound;
typedef unsigned int Channel;

Sound* bbLoadSound(BBStr* path);
Sound* bbLoad3DSound(BBStr* path);
void bbFreeSound(Sound* sound);
void bbLoopSound(Sound* sound);
void bbSoundVolume(Sound* sound, float volume);
void bbSoundPan(Sound* sound, float pan);
void bbSoundPitch(Sound* sound, int pitch);
Channel bbPlaySound(Sound* sound);
Channel bbPlayMusic(BBStr* path);
void bbStopChannel(Channel channel);
void bbPauseChannel(Channel channel);
void bbResumeChannel(Channel channel);
void bbChannelVolume(Channel channel, float volume);
void bbChannelPan(Channel channel, float pan);
void bbChannelPitch(Channel channel, int pitch);
int bbChannelPlaying(Channel channel);

// ***** INTERNAL *****
Channel bbPlay3dSound(Sound* sound, float x, float y, float z, float vx, float vy, float vz);
void bbSet3dChannel(Channel channel, float x, float y, float z, float vx, float vy, float vz);
void bbSet3dListenerConfig(float roll, float dopp, float dist);
void bbSet3dListener(float x, float y, float z, float kx, float ky, float kz, float jx, float jy, float jz, float vx, float vy, float vz);

#endif
