#include "lba.h"
#include <sdl_mixer.h>

struct sampleStructData
{
	int sampleIdx;
};

sampleStructData sampleTable[16];

void playSample(int sampleNum, int freq, int repeat, int x, int y)
{
	char filename[256];

	sprintf(filename,"samples/samples%03d.voc",sampleNum+1);

	Mix_Chunk * sample=Mix_LoadWAV(filename);
	Mix_PlayChannel(0,sample,repeat-1);
}

void playSampleFla(int sampleNum, int freq, int repeat, int x, int y)
{
	char filename[256];

	sprintf(filename,"fla/flasamp/flasamp%02d.voc",sampleNum+1);

	Mix_Chunk * sample=Mix_LoadWAV(filename);
	Mix_PlayChannel(0,sample,repeat-1);
}

void soundInit()
{
	int audio_buffers=512;

	Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,audio_buffers);
	Mix_AllocateChannels(8);
}

void playMidi(int musicNum)
{
	char filename[256];

	sprintf(filename,"midi_mi_win/midi_mi_win%02d.mid",musicNum+1);

	Mix_Music * sample=Mix_LoadMUS(filename);
	Mix_PlayMusic(sample,0);
}
