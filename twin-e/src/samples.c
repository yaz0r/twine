/*
Copyright (C) 2002-2010 The TwinE team

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

Mix_Chunk *sample = NULL;

void playSample(int sampleNum, int repeat)
{
	int sz;
	unsigned char *ptr;
	SDL_RWops *rw;

	printf("Playing sample %d\n", sampleNum);

	sz = Size_HQR("samples.hqr", sampleNum);
	ptr = malloc(sizeof(unsigned char) * sz);

	Load_HQR("samples.hqr", ptr, sampleNum);

	if (ptr[0] != 'C')                                                                                                                                      
		ptr[0] = 'C';

	rw = SDL_RWFromMem(ptr, sz);
	sample = Mix_LoadWAV_RW(rw, 1);

	Mix_PlayChannel(-1, sample, repeat - 1);

	free(ptr);
}

void playSampleFla(int sampleNum, int repeat)
{
	int sz;
	unsigned char *ptr;
	SDL_RWops *rw;

	printf("Playing sample %d\n", sampleNum);

	sz = Size_HQR("fla/flasamp.hqr", sampleNum);
	ptr = malloc(sizeof(unsigned char) * sz);

	Load_HQR("fla/flasamp.hqr", ptr, sampleNum);

	if (ptr[0] != 'C')                                                                                                                                      
		ptr[0] = 'C';

	rw = SDL_RWFromMem(ptr, sz);
	sample = Mix_LoadWAV_RW(rw, 1);

	Mix_PlayChannel(-1, sample, repeat - 1);

	free(ptr);
}

void stopSampleFla(void)
{
	printf("Stopping samples\n");
	Mix_HaltChannel(-1);
	Mix_FreeChunk(sample);
	sample = NULL;
}

void soundInit()
{
	const int audio_buffers = 512;

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, audio_buffers);
	Mix_AllocateChannels(8);
}

Mix_Music *music = NULL;

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

	music = Mix_LoadMUS(filename);

	if (music == NULL)
		printf("Mix_LoadMUS(\"%s\"): %s\n", filename, Mix_GetError());

	Mix_PlayMusic(music, -1);
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

