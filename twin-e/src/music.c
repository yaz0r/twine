#include "lba.h"

void
LBA_engine::playMusic (int musicNum)
{
  if (musicNum == -1)		/* stop music */
    {
      fullStopMusic ();
      currentlyPlayingMusic = -1;
//              playMusic2();
      playMusciVar1 = -1;
      return;
    }

  if (playMusicFlag == 0 && musicNum >= 1 && musicNum <= 9)
    playCDtrack (musicNum);
  else
    playMidi (musicNum);

  return;

}

void
LBA_engine::playCDtrack (int trackNumber)
{
/*	if(CD_INDRIVE(SDL_CDStatus(cdrom)))
		SDL_CDPlayTracks(cdrom, trackNumber, 0, 1, 0);*/
}

void
LBA_engine::playMidi (int musicNum)
{
}

void
LBA_engine::fullStopMusic (void)
{
}

void
LBA_engine::stopMusic (void)
{
  fullStopMusic ();
  currentlyPlayingMusic = -1;
}

int
LBA_engine::playMusicSub (void)
{
  return (0);
}

void
LBA_engine::playMusicSub2 (short int arg_0)
{
}

int
LBA_engine::getCurrentlyPlayingMusic (void)
{
  if (musicPosition > musicLength)
    currentlyPlayingMusic = -1;

  return (currentlyPlayingMusic);
}
