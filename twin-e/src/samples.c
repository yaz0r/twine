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

struct sampleStructData
{
  int sampleIdx;
};

#ifdef PCLIKE
//sampleStructData sampleTable[16];
#endif

void playSample(int sampleNum, int freq, int repeat, int x, int y)
{
/*  char filename[256];

  sprintf(filename,"samples/samples%03d.voc",sampleNum+1);

  Mix_Chunk * sample=Mix_LoadWAV(filename);
  Mix_PlayChannel(0,sample,repeat-1);*/
}

void playSampleFla(int sampleNum, int freq, int repeat, int x, int y)
{
/*  char filename[256];

  sprintf(filename,"fla/flasamp/flasamp%02d.voc",sampleNum+1);

  Mix_Chunk * sample=Mix_LoadWAV(filename);
  Mix_PlayChannel(0,sample,repeat-1);*/
}

void soundInit()
{
#ifdef PCLIKE
//  int audio_buffers=512;

//  Mix_OpenAudio(44100,MIX_DEFAULT_FORMAT,2,audio_buffers);
//  Mix_AllocateChannels(8);
#endif
}

void playMidi(int musicNum)
{
  /*char filename[256];

  sprintf(filename,"midi_mi_win/midi_mi_win%02d.mid",musicNum+1);

  Mix_Music * sample=Mix_LoadMUS(filename);
  Mix_PlayMusic(sample,0);*/
}
