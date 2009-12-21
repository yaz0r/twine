/*
Copyright (C) 2002-2004 The TwinE team

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#include <sys/stat.h>

#include "lba.h"

#include "hqr.h"

#if USE_SDL_MIXER == 1

#include <SDL_mixer.h>

void playSample(int sampleNum, int repeat) {
	char filename[MAX_PATH];
	Mix_Chunk * sample;

	sample = Mix_LoadWAV_RW(SDL_RWFromMem(HQR_Get(HQR_Samples,sampleNum), Size_HQR("SAMPLES.HQR", sampleNum)), 0);

	if (sample == NULL)
		printf("Mix_LoadWAV(\"%s\"): %s\n", filename, Mix_GetError());

	Mix_PlayChannel(0, sample, repeat - 1);
}

void playSampleFla(int sampleNum, int repeat)
{
	char filename[MAX_PATH];
	Mix_Chunk * sample;

	sprintf(filename,"fla/flasamp/flasamp%02d.voc", sampleNum + 1);

	sample = Mix_LoadWAV(filename);
	Mix_PlayChannel(0, sample, repeat - 1);
}

void soundInit()
{
	const int audio_buffers = 512;

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, audio_buffers);
	Mix_AllocateChannels(8);
}

static Mix_Music * sample = NULL;

void playMidi(int musicNum) {
	char filename[MAX_PATH];
	FILE* fhandle;
	char* temp;

	sprintf(filename, "midi/%02d.midi", musicNum);

	fhandle = fopen(filename, "r");

	if (!fhandle)
	{
		mkdir("midi", 0777);
		fhandle = fopen(filename, "w");

		temp = (char*)HQR_Get(HQR_Midi, musicNum);
		fwrite(temp, Size_HQR(HQR_Midi->fileName, musicNum), 1, fhandle);
	}
    fclose(fhandle);

	sample = Mix_LoadMUS(filename);

	if (sample == NULL)
		printf("Mix_LoadMUS(\"%s\"): %s\n", filename, Mix_GetError());

	Mix_PlayMusic(sample, 0);
}

void pauseSound() {
	Mix_PauseMusic();
}

void resumeSound() {
	Mix_ResumeMusic();
}

#else
void playSample(int sampleNum, int repeat) { }
void playSampleFla(int sampleNum, int repeat) { }
void soundInit(void) { }
void playMidi() { }
void pauseSound() { }
void resumeSound() { }
#endif

