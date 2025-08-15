#include "std.h"
#include "bbaudio.h"
#include "../MultiLang/MultiLang.h"

/*
gxAudio* gx_audio;

static inline void debugSound(gxSound* s, const char* function) {
	if (!gx_audio->verifySound(s)) ErrorLog(function, MultiLang::sound_not_exist);
}

static gxSound* loadSound(BBStr* f, bool use_3d) {
	std::string t = *f; delete f;
	return gx_audio ? gx_audio->loadSound(t, use_3d) : 0;
}

static gxChannel* playMusic(BBStr* f, bool use_3d, int mode) {
	std::string t = *f; delete f;
	return gx_audio ? gx_audio->playFile(t, use_3d, mode) : 0;
}

int bbVerifySound(gxSound* sound) {
	return (bool)gx_audio->verifySound(sound);
}

gxSound* bbLoadSound(BBStr* f) {
	return loadSound(f, false);
}

void bbFreeSound(gxSound* sound) {
	if (!sound) return;
	debugSound(sound, "FreeSound");
	gx_audio->freeSound(sound);
}

void bbLoopSound(gxSound* sound) {
	if (!sound) return;
	debugSound(sound, "LoopSound");
	sound->setLoop(true);
}

void bbSoundPitch(gxSound* sound, int pitch) {
	if (!sound) return;
	debugSound(sound, "SoundPitch");
	sound->setPitch(pitch);
}

void bbSoundVolume(gxSound* sound, float volume) {
	if (!sound) return;
	debugSound(sound, "SoundVolume");
	sound->setVolume(volume);
}

void bbSoundPan(gxSound* sound, float pan) {
	if (!sound) return;
	debugSound(sound, "SoundPan");
	sound->setPan(pan);
}

gxChannel* bbPlaySound(gxSound* sound) {
	if (!sound) return 0;
	debugSound(sound, "PlaySound");
	return sound->play();
}

gxChannel* bbPlayMusic(BBStr* f, int mode) {
	return playMusic(f, false, mode);
}

gxChannel* bbPlayCDTrack(int track, int mode) {
	return gx_audio ? gx_audio->playCDTrack(track, mode) : 0;
}

void bbStopChannel(gxChannel* channel) {
	if (!channel) return;
	channel->stop();
}

void bbPauseChannel(gxChannel* channel) {
	if (!channel) return;
	channel->setPaused(true);
}

void bbResumeChannel(gxChannel* channel) {
	if (!channel) return;
	channel->setPaused(false);
}

void bbChannelPitch(gxChannel* channel, int pitch) {
	if (!channel) return;
	channel->setPitch(pitch);
}

void bbChannelVolume(gxChannel* channel, float volume) {
	if (!channel) return;
	channel->setVolume(volume);
}

void bbChannelPan(gxChannel* channel, float pan) {
	if (!channel) return;
	channel->setPan(pan);
}

int bbChannelPlaying(gxChannel* channel) {
	return channel ? channel->isPlaying() : 0;
}

gxSound* bbLoad3DSound(BBStr* f) {
	return loadSound(f, true);
}*/

bool audio_create() {
	//gx_audio = gx_runtime->openAudio(0);
	return true;
}

bool audio_destroy() {
	//if (gx_audio) gx_runtime->closeAudio(gx_audio);
	//gx_audio = 0;
	return true;
}

void audio_link(void(*rtSym)(const char*, void*)) {
	/*
	rtSym("%VerifySound%sound", bbVerifySound);
	rtSym("%LoadSound$filename", bbLoadSound);
	rtSym("FreeSound%sound", bbFreeSound);
	rtSym("LoopSound%sound", bbLoopSound);
	rtSym("SoundPitch%sound%pitch", bbSoundPitch);
	rtSym("SoundVolume%sound#volume", bbSoundVolume);
	rtSym("SoundPan%sound#pan", bbSoundPan);
	rtSym("%PlaySound%sound", bbPlaySound);
	rtSym("%PlayMusic$midifile%mode=0", bbPlayMusic);
	rtSym("%PlayCDTrack%track%mode=1", bbPlayCDTrack);
	rtSym("StopChannel%channel", bbStopChannel);
	rtSym("PauseChannel%channel", bbPauseChannel);
	rtSym("ResumeChannel%channel", bbResumeChannel);
	rtSym("ChannelPitch%channel%pitch", bbChannelPitch);
	rtSym("ChannelVolume%channel#volume", bbChannelVolume);
	rtSym("ChannelPan%channel#pan", bbChannelPan);
	rtSym("%ChannelPlaying%channel", bbChannelPlaying);
	rtSym("%Load3DSound$filename", bbLoad3DSound);
	*/
}