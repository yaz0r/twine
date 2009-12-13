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

#include "mainLoop.h"
#include "music.h"
#include "samples.h"

#ifdef PCLIKE
# include <SDL/SDL_mixer.h>
# include <SDL/SDL.h>
#endif

SDL_CD *cdrom;

void openCD(void)
{
	cdrom = NULL;
	if (SDL_CDNumDrives() > 0)
		cdrom = SDL_CDOpen(0);
}

void closeCD(void)
{
	SDL_CDClose(cdrom);
}

void playMusic(int musicNum) {
	if (!playCDtrack(musicNum))
		playMidi(musicNum);

	return;
}

char playCDtrack(int trackNumber) {
#ifdef PCLIKE
	freezeTime();

	Mix_FadeOutMusic(500);

	if (cdrom != NULL)
	{
		if (SDL_CDStatus(cdrom) == CD_PLAYING)
			SDL_CDStop(cdrom);
		if (CD_INDRIVE(SDL_CDStatus(cdrom)))
			SDL_CDPlayTracks(cdrom, trackNumber, 0, 1, 0);
		return 1;
	}
	else
		return 0;

	unfreezeTime();
#else
	return 0;
#endif
}

void stopMusic(void) {
}


