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

SDL_Surface *sdl_screen; // that's the SDL global object for the screen
SDL_Color sdl_colors[256];

OSystem::OSystem(int argc, char* argv[]) // that's the creator of the system dependent object used for the SDL port
{
  unsigned char* keyboard;
  int size;

  if( SDL_Init(SDL_INIT_VIDEO) < 0 )
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

  sdl_screen = SDL_SetVideoMode(640, 480, 8, SDL_HWSURFACE);

  if ( sdl_screen == NULL )
  {
    fprintf(stderr, "Couldn't set 640x480x8 video mode: %s\n", SDL_GetError());
    exit(1);
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

  SDL_SetColors(sdl_screen, sdl_colors, i,1);
}

void OSystem::setPalette(byte* palette)
{
  int i;
  int j=0;

  for(i=0;i<256;i++) // c'est pas super optimise...
  {
    sdl_colors[i].r=palette[j++];
    sdl_colors[i].g=palette[j++];
    sdl_colors[i].b=palette[j++];
  }

  SDL_SetPalette(sdl_screen, SDL_LOGPAL|SDL_PHYSPAL, sdl_colors, 0, 256);
}

void OSystem::drawBufferToScreen(unsigned char* videoBuffer)
{
  int i;
  int j;

  SDL_LockSurface(sdl_screen);

  for(i=0;i<640;i++)
    for(j=0;j<480;j++)
      putpixel(i,j,videoBuffer[i+640*j]);


  SDL_UnlockSurface(sdl_screen);

  SDL_UpdateRect(sdl_screen, 0, 0, 0, 0);

}

void OSystem::refresh(unsigned char* videoBuffer, int left, int top, int right, int bottom)
{
  int i;
  int j;

  // todo: faire un vrais implementation de la chose avec les bords

  SDL_LockSurface(sdl_screen);

  for(i=top;i<bottom;i++)
    for(j=left;j<right;j++)
      putpixel(j,i,videoBuffer[j+640*i]);

  SDL_UnlockSurface(sdl_screen);

  SDL_UpdateRect(sdl_screen, left, top, right-left, bottom-top);
}

