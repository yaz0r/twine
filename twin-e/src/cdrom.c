/***************************************************************************
                          cdrom.cpp  -  description
                             -------------------
    begin                : Tue Feb 12 2002
    copyright            : (C) 2002 by Yaz0r
    email                : yaz0r@yaz0r.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "lba.h"
#include "SDL.h"

int LBA_engine::checkCD(char* string)
{
//	int cur_track;
//	int min, sec, frame;
	int numOfCDROM;
	int cdNum;

	return(0);

	numOfCDROM=SDL_CDNumDrives();

	printf("Found %d CDROM devices\n",numOfCDROM);

	if(!numOfCDROM){
  	/* None found */
  	fprintf(stderr, "No CDROM devices available\n");
		exit(-1);
	}

	for(cdNum=0;cdNum<numOfCDROM;cdNum++)
	{	

		printf("Testing drive %s\n",SDL_CDName(cdNum));		

		/* Open the default drive */
//		cdrom=SDL_CDOpen(cdNum);

		/* Did if open? Check if cdrom is NULL */
		/*		if(!cdrom){
 	 		fprintf(stderr, "Couldn't open drive: %s\n", SDL_GetError());
		}
		else*/
		{
			/* Print Volume info */
	/*		SDL_CDStatus(cdrom);
			if(cdrom->numtracks==10)
			{
				printf("Assuming that it is LBA cd...\n");
				return(1);
			}*/
		}
	//	SDL_CDClose(cdrom);
	}   
	return(0);
}


void LBA_engine::stopCD(void)
{
//	SDL_CDStop(cdrom);
}

