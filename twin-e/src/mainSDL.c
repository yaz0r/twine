/***************************************************************************
                          mainSDL.cpp  -  description
                             -------------------
    begin                : Mon Jun 3 2002
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

#include "SDL.h"
#include "SDL_thread.h"
#include "lba.h"

char* tempBuffer;
SDL_Surface *sdl_buffer;
SDL_Surface *sdl_screen; // that's the SDL global object for the screen
SDL_Color sdl_colors[256];
SDL_Surface *surfaceTable[16];

OSystem::OSystem(int argc, char* argv[]) // that's the creator of the system dependent object used for the SDL port
{
  unsigned char* keyboard;
  int size;
  int i;

	Uint32 rmask, gmask, bmask, amask;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif


  if( SDL_Init(SDL_INIT_EVERYTHING) < 0 )
  {
    fprintf(stderr,"Couldn't initialize SDL: %s\n", SDL_GetError());
    exit(1);
  }

  atexit(SDL_Quit);

  SDL_WM_SetCaption("Little Big Adventure","LBA");

  SDL_ShowCursor(SDL_DISABLE);

  SDL_EnableUNICODE(SDL_ENABLE); // not much used in fact

  SDL_PumpEvents();

  keyboard=SDL_GetKeyState(&size);

  keyboard[SDLK_RETURN]=0;

  sdl_screen = SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE);

  if ( sdl_screen == NULL )
  {
    fprintf(stderr, "Couldn't set 640x480x8 video mode: %s\n", SDL_GetError());
    exit(1);
  }

	for(i=0;i<16;i++)
	{
		surfaceTable[i]=SDL_CreateRGBSurface(SDL_SWSURFACE,640,480,32,rmask,gmask,bmask,0);
	}

}

void OSystem::putpixel(int x, int y, int pixel)
{
    int bpp = sdl_screen->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *)sdl_screen->pixels + y * sdl_screen->pitch + x * bpp;

    *p = pixel;
}

void OSystem::setColor(byte i, byte R, byte G, byte B)  // cette fonction est vraiment vraiment tres tres lente...
{
  sdl_colors[i].r=R;
  sdl_colors[i].g=G;
  sdl_colors[i].b=B;

  SDL_SetColors(sdl_buffer, sdl_colors, i,1);
}

void OSystem::setPalette(byte* palette)
{
//	int i;
	SDL_Color *sdl_colorsTemp=(SDL_Color*)palette;

	SDL_SetColors(sdl_buffer,sdl_colorsTemp,0,256);

	SDL_BlitSurface(sdl_buffer, NULL,sdl_screen,NULL);

	SDL_UpdateRect(sdl_screen, 0, 0, 0, 0);
}

void OSystem::fadeBlackToWhite()
{
	int i;

	SDL_Color colorPtr[256];

	SDL_UpdateRect(sdl_screen, 0, 0, 0, 0);

	for(i=0;i<256;i+=3)
	{
		memset(colorPtr,i,1024);
		SDL_SetPalette(sdl_screen,SDL_PHYSPAL,colorPtr,0,256);
	}
}

void OSystem::drawBufferToScreen(unsigned char* videoBuffer)
{
	SDL_BlitSurface(sdl_buffer, NULL,sdl_screen,NULL);

	SDL_UpdateRect(sdl_screen, 0, 0, 0, 0);
}

void OSystem::refresh(unsigned char* videoBuffer, int left, int top, int right, int bottom)
{
	SDL_Rect rectangle;

	rectangle.x=left;
	rectangle.y=top;
	rectangle.w=right-left;
	rectangle.h=bottom-top;

	SDL_BlitSurface(sdl_buffer, &rectangle,sdl_screen,&rectangle);

	SDL_UpdateRect(sdl_screen, left, top, right-left, bottom-top);
}

void OSystem::initBuffer(char* buffer,int width,int height)
{
	sdl_buffer=SDL_CreateRGBSurfaceFrom(buffer,width,height,8,640,0,0,0,0);
}

void OSystem::crossFade(char* buffer,char* palette)
{
	int i;
	SDL_Surface *backupSurface;
	SDL_Surface *newSurface;
	SDL_Surface *tempSurface;
	Uint32 rmask, gmask, bmask, amask;

	return;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif

	backupSurface=SDL_CreateRGBSurface(SDL_SWSURFACE,640,480,32,rmask,gmask,bmask,0);
	newSurface=SDL_CreateRGBSurface(SDL_SWSURFACE|SDL_SRCALPHA,640,480,32,rmask,gmask,bmask,0);

	tempSurface=SDL_CreateRGBSurfaceFrom(buffer,640,480,8,640,0,0,0,0);
	SDL_SetColors(tempSurface,(SDL_Color*)palette,0,256);

	SDL_BlitSurface(sdl_screen,NULL,backupSurface,NULL);
	SDL_BlitSurface(tempSurface,NULL,newSurface,NULL);

	for(i=0;i<16;i++)
	{
		SDL_BlitSurface(backupSurface,NULL,surfaceTable[i],NULL);
		SDL_SetAlpha(newSurface,SDL_SRCALPHA|SDL_RLEACCEL,i*16);
		SDL_BlitSurface(newSurface,NULL,surfaceTable[i],NULL);
	}

	for(i=0;i<16;i++)
	{
		SDL_BlitSurface(surfaceTable[i],NULL,sdl_screen,NULL);
		SDL_UpdateRect(sdl_screen,0,0,0,0);
	}

	SDL_FreeSurface(backupSurface);
	SDL_FreeSurface(newSurface);
	SDL_FreeSurface(tempSurface);
}
