/***************************************************************************
                          input.cpp  -  description
                             -------------------
    begin                : Sun Feb 3 2002
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
#include <SDL.h>

void LBA_engine::readKeyboard(void)
{
    SDL_Event event;
    int localKey;
    int i;
    int find = 0;
    short int temp;
    unsigned char temp2;
    char found = 0;
    int size;
    int j;

    localKey = 0;

    printTextVar12 = 0;
    key1 = 0;
    skipIntro = 0;

    unsigned char *keyboard;

    while (SDL_PollEvent(&event))
	{
	    switch (event.type)
		{
		case SDL_QUIT:
		    exit(0);
		    break;

		case SDL_MOUSEBUTTONDOWN:
		    if (event.button.button == 0)
			{
			    osystem->mouseRight = 1;
			}
		    else
			{
			    osystem->mouseLeft = 1;
			}
		    break;

		default:
		    break;
		}
	}

   // case SDL_KEYDOWN:
   /*
    * if(event.key.keysym.sym == SDLK_UP) printTextVar12|=2; if(event.key.keysym.sym == SDLK_DOWN)
    * printTextVar12|=1; if(event.key.keysym.sym == SDLK_RETURN) key1=0; 
    */

    SDL_PumpEvents();

    keyboard = SDL_GetKeyState(&size);

    for (j = 0; j < size; j++)
	{
	    if (keyboard[j])
		{
		    if (j == SDLK_ESCAPE)
			localKey = 0x1;
		    if (j == SDLK_RETURN)
			localKey = 0x1C;
		    if (j == SDLK_LCTRL)
			localKey = 0x1D;
		    if (j == SDLK_LSHIFT)
			localKey = 0x2A;
		    if (j == SDLK_RSHIFT)
			localKey = 0x36;
		    if (j == SDLK_SPACE)
			localKey = 0x39;
		    if (j == SDLK_UP)
			localKey = 0x48;
		    if (j == SDLK_LEFT)
			localKey = 0x4B;
		    if (j == SDLK_RIGHT)
			localKey = 0x4D;
		    if (j == SDLK_DOWN)
			localKey = 0x50;
		    if (j == SDLK_SPACE)
			localKey = 0x39;

		    if (j == SDLK_p)
			localKey = 'p';
		    if (j == SDLK_h)
			localKey = 'h';
		}

	    if (event.key.keysym.sym == SDLK_ESCAPE)
		localKey = 0x1;
	    if (event.key.keysym.sym == SDLK_RETURN)
		localKey = 0x1C;
	    if (event.key.keysym.sym == SDLK_LSHIFT)
		localKey = 0x2A;
	    if (event.key.keysym.sym == SDLK_RSHIFT)
		localKey = 0x36;
	    if (event.key.keysym.sym == SDLK_SPACE)
		localKey = 0x39;
	    if (event.key.keysym.sym == SDLK_UP)
		localKey = 0x48;
	    if (event.key.keysym.sym == SDLK_DOWN)
		localKey = 0x50;
	    if (event.key.keysym.sym == SDLK_LEFT)
		localKey = 0x4B;
	    if (event.key.keysym.sym == SDLK_RIGHT)
		localKey = 0x4D;
	    if (event.key.keysym.sym == SDLK_LALT)
		localKey = 0x1D;
	    if (event.key.keysym.sym == SDLK_SPACE)
		localKey = 0x39;

	    if (event.key.keysym.sym == SDLK_p)
		localKey = 'p';

	    for (i = 0; i < 28; i++)
		{
		    if (scanCodeTab1[i] == localKey)
			{
			    find = i;
			    found = 1;
			}
		}

	    if (found != 0)
		{
		    temp = scanCodeTab2[find];
		    temp2 = temp & 0x00FF;

		    if (temp2 == 0)
			{
			    if (!(localKey & 0x80))
				{
				    printTextVar12 |= (temp & 0xFF00) >> 8;
				}
			    else
				{
				    printTextVar12 &= -((temp & 0xFF00) >> 8);
				}
			}
		    else
			{
			   // mode non géré ici
			    key1 |= (temp & 0xFF00) >> 8;
			}
		}
	    else
		{
		   // no géré ici aussi
		    skipIntro = localKey;
		}

	    currentKey = event.key.keysym.scancode;

	   // printf("unicode= %X\n",event.key.keysym.unicode);
	   // printf("scancode= %X\n",skipIntro);
	}
}

short int LBA_engine::getKeyboardChar(void)
{
    do
	{
	    readKeyboard();
	}
    while (currentKey == 36);
    return (currentKey);
}
