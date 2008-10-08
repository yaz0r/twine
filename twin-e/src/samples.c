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

#include "lba.h"

#ifdef PCLIKE
//#include <SDL_mixer.h>
#endif

struct sampleStructData {
	int sampleIdx;
};

#ifdef PCLIKE
struct sampleStructData sampleTable[16];
#endif

void playSample(int sampleNum, int freq, int repeat, int x, int y) {
	/*  char filename[256];
	  Mix_Chunk * sample;
	  FILE* fhandle;


	  sample=Mix_LoadWAV_RW(SDL_RWFromMem(HQR_Get(HQR_Samples,sampleNum), Size_HQR("SAMPLES.HQR", sampleNum)),0);

	  if(sample == NULL)
	  {
	    printf("Mix_LoadWAV(\"%s\"): %s\n", filename, Mix_GetError());
	  }

	  Mix_PlayChannel(0,sample,repeat-1);*/
}

void playSampleFla(int sampleNum, int freq, int repeat, int x, int y) {
	/*  char filename[256];
	  Mix_Chunk * sample;

	  sprintf(filename,"fla/flasamp/flasamp%02d.voc",sampleNum+1);

	  sample=Mix_LoadWAV(filename);
	  Mix_PlayChannel(0,sample,repeat-1);
	*/
}

void soundInit() {
#ifdef PCLIKE
//  int audio_buffers=512;

//  Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,audio_buffers);
//  Mix_AllocateChannels(8);
#endif
}

#define midiFileName "Midi_mi_win"

//Mix_Music * sample = NULL;

void playMidi(int musicNum) {
	/* char filename[256];
	 FILE* fhandle;

	 if(sample != NULL)
	 {
	   Mix_FreeMusic(sample);
	 }

	// if(musicNum == 9)
	   return;

	 sprintf(filename,"%s/%s%02d.midi",midiFileName,midiFileName,musicNum);

	 fhandle = fopen(filename,"rb");

	 if(!fhandle)
	 {
	   char sourceName[256];
	   char* temp;
	   unsigned int midiSize;

	   mkdir(midiFileName);
	   fhandle = fopen(filename,"wb+");

	   sprintf(sourceName, "%s.hqr", midiFileName);

	   midiSize = Size_HQR(sourceName, musicNum);
	   temp = (char*)malloc(midiSize);

	   Load_HQR(sourceName, temp, musicNum);
	   fwrite(temp, midiSize,1, fhandle);
	   fclose(fhandle);
	   free(temp);
	 }

	 sample=Mix_LoadMUS(filename);

	 if(sample == NULL)
	 {
	   printf("Mix_LoadMUS(\"%s\"): %s\n", filename, Mix_GetError());
	 }
	 Mix_PlayMusic(sample,0);
	*/
}
