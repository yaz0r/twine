#include "lba.h"
/*
-----------------
memory usage list:
-----------------

data:

- fic				  11k		 82 files
- inventory			  60k		 28 files
- text				 170k		 56 files  (french only)
- scenes			 250k		120 files
- sprites			 290k		118 files
- animes			 440k		516 files
- body				 620k		132 files
- samples			3600k		230 files

-----------------------------------------
Special:

- bricks			3900k		unpacked, used to speed up cube loading
-----------------------------------------

total:				9341k



*/


void InitBufferCube(void)
{
   // todo: mettre les codes d'erreur

    bufCube = (byte *) Malloc(204800);	// 204800 = 64*64*25*2
    bufferBrick = (byte *) Malloc(361472);
}
