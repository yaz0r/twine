#include "lba.h"
#include <sdl_mixer.h>

#ifdef PCLIKE
#include "SDL.h"
#endif //PCLIKE

void PlayMusic(int musicNum)
{
    if (musicNum == -1)
	{			/* stop music */
	    fullStopMusic();
	    currentlyPlayingCDTrack = -1;
	   // playMusic2();
	    playMusciVar1 = -1;
	    return;
	}

    if (playMusicFlag == 0 && musicNum >= 1 && musicNum <= 9)
		playCDtrack(musicNum);
    else
		playMidi(musicNum);

    return;

}

void playCDtrack(int trackNumber)
{
#ifdef PCLIKE
	freezeTime();
	Mix_FadeOutMusic(500);
	currentlyPlayingMidi = -1;

	if(cdrom!=NULL)
	{
		if(SDL_CDStatus(cdrom) == CD_PLAYING)
		{
			SDL_CDStop(cdrom);
		}
	}
	currentlyPlayingCDTrack = -1;

	if(cdrom!=NULL)  
		if(CD_INDRIVE(SDL_CDStatus(cdrom)))
			SDL_CDPlayTracks(cdrom, trackNumber, 0, 1, 0);

	unfreezeTime();
#endif //PCLIKE 
}

void fullStopMusic(void)
{
	if(cdrom!=NULL)
	{
		int status = SDL_CDStatus(cdrom);
//		if(status == CD_PLAYING)
		{
			SDL_CDStop(cdrom);
		}
	}
}

void stopMusic(void)
{
    fullStopMusic();
    currentlyPlayingCDTrack = -1;
}

int IsMidiPlaying(void)
{
    return (0);
}

void FadeMusicMidi(short int arg_0)
{
}

int getCurrentlyPlayingMusic(void)
{
    if (musicPosition > musicLength)
	currentlyPlayingCDTrack = -1;

    return (currentlyPlayingCDTrack);
}
