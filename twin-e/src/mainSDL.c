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

#include <SDL.h>
#include "lba.h"

#ifdef USE_SDL_MIXER
#include <SDL_mixer.h>
#endif

#include <SDL_ttf.h>

#include "mainLoop.h"
#include "hqr.h"
#include "images.h"

#include "osystem.h"

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
Uint32 rmask = 0xff000000;
Uint32 gmask = 0x00ff0000;
Uint32 bmask = 0x0000ff00;
Uint32 amask = 0x000000ff;
#else
Uint32 rmask = 0x000000ff;
Uint32 gmask = 0x0000ff00;
Uint32 bmask = 0x00ff0000;
Uint32 amask = 0xff000000;
#endif

char fullscreen;

char *tempBuffer;
SDL_Surface *sdl_buffer;
SDL_Surface *sdl_buffer320x200;
SDL_Surface *sdl_bufferStretch;
SDL_Surface *sdl_bufferRGBA;
SDL_Surface *sdl_screen;  // that's the SDL global object for the screen
SDL_Color sdl_colors[256];
SDL_Surface *surfaceTable[16];

SDL_Event event;

TTF_Font *font;

char breakMainLoop = 0;

void os_mainLoop(void) {
#define SPEED 15              /* Ticks per Frame */
#define SLEEP_MIN 2          /* Minimum time a sleep takes, usually 2*GRAN */
#define SLEEP_GRAN 1         /* Granularity of sleep */

long int t_start, t_left;
long unsigned int t_end;
long int q = 0;                   /* Dummy */

	while (!breakMainLoop)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT) {
				breakMainLoop = 1;
				break;
			}
		}

		t_start = SDL_GetTicks();

		mainLoopInteration();
		t_end = t_start + SPEED;
		t_left = t_start - SDL_GetTicks() + SPEED;

		if (t_left > SLEEP_MIN) {
			SDL_Delay(t_left - SLEEP_GRAN);
		}
		while (SDL_GetTicks() < t_end) {
			q++;
	    };
		lba_time++;
	}
}

void os_delay(int time) {
   SDL_Delay(time);
}

int os_init()
{
	int i;

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_CDROM | (USE_SDL_MIXER ? SDL_INIT_AUDIO:0)) < 0) {
		fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}

	atexit(SDL_Quit);

#ifdef USE_SDL_MIXER
	/* open 44.1KHz, signed 16bit, system byte order,
	 *      stereo audio, using 1024 byte chunks */
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
		printf("Mix_OpenAudio: %s\n", Mix_GetError());
		exit(2);
	}

	Mix_AllocateChannels(16);
#endif

	SDL_WM_SetCaption("Little Big Adventure: TwinEngine", "LBA");

	sdl_screen = SDL_SetVideoMode(640, 480, 32, SDL_SWSURFACE);

	if (sdl_screen == NULL) {
		fprintf(stderr, "Couldn't set 640x480x8 video mode: %s\n", SDL_GetError());
		exit(1);
	}

	for (i = 0; i < 16; i++) {
		surfaceTable[i] =
			SDL_CreateRGBSurface(SDL_SWSURFACE, 640, 480, 32, rmask, gmask, bmask, 0);
	}

	return 0;
}

void os_setPalette(byte * palette)
{
	SDL_Color *sdl_colorsTemp = (SDL_Color *) palette;

	SDL_SetColors(sdl_buffer, sdl_colorsTemp, 0, 256);

	SDL_BlitSurface(sdl_buffer, NULL, sdl_screen, NULL);

	SDL_UpdateRect(sdl_screen, 0, 0, 0, 0);
}

void os_setPalette320x200(byte * palette)
{
	SDL_Color *sdl_colorsTemp = (SDL_Color *) palette;

	SDL_SetColors(sdl_buffer320x200, sdl_colorsTemp, 0, 256);
}

void os_flip()
{
	SDL_BlitSurface(sdl_buffer, NULL, sdl_screen, NULL);
	SDL_UpdateRect(sdl_screen, 0, 0, 0, 0);
}

void os_draw320x200BufferToScreen()
{
	SDL_BlitSurface(sdl_buffer320x200, NULL, sdl_bufferRGBA, NULL);

	SDL_BlitSurface(sdl_buffer320x200, NULL, sdl_screen, NULL);

	SDL_UpdateRect(sdl_screen, 0, 0, 0, 0);
}

void os_copyBlockPhys(int left, int top, int right, int bottom) {
	SDL_Rect rectangle;

	rectangle.x = left;
	rectangle.y = top;
	rectangle.w = right - left + 1 ;
	rectangle.h = bottom - top + 1 ;

	SDL_BlitSurface(sdl_buffer, &rectangle, sdl_screen, &rectangle);

	SDL_UpdateRect(sdl_screen, left, top, right - left + 1, bottom - top + 1);
}

void os_initVideoBuffer(char *buffer, int width, int height) {
	sdl_bufferRGBA = SDL_CreateRGBSurface(SDL_SWSURFACE, 320, 200, 32, rmask, gmask, bmask, amask);
	sdl_buffer320x200 = SDL_CreateRGBSurfaceFrom(buffer, width, height, 8, 320, 0, 0, 0, 0);
}

void os_initBuffer(char *buffer, int width, int height) {
	sdl_buffer = SDL_CreateRGBSurfaceFrom(buffer, width, height, 8, 640, 0, 0, 0, 0);
}

void os_crossFade(char *buffer, char *palette) {
	int i;
	SDL_Surface *backupSurface;
	SDL_Surface *newSurface;
	SDL_Surface *tempSurface;

	backupSurface = SDL_CreateRGBSurface(SDL_SWSURFACE, 640, 480, 32, rmask, gmask, bmask, 0);
	newSurface =
		SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA, 640, 480, 32, rmask, gmask, bmask, 0);

	tempSurface = SDL_CreateRGBSurfaceFrom(buffer, 640, 480, 8, 640, 0, 0, 0, 0);
	SDL_SetColors(tempSurface, (SDL_Color *) palette, 0, 256);

	SDL_BlitSurface(sdl_screen, NULL, backupSurface, NULL);
	SDL_BlitSurface(tempSurface, NULL, newSurface, NULL);

	for (i = 0; i < 8; i++) {
		SDL_BlitSurface(backupSurface, NULL, surfaceTable[i], NULL);
		SDL_SetAlpha(newSurface, SDL_SRCALPHA | SDL_RLEACCEL, i * 32);
		SDL_BlitSurface(newSurface, NULL, surfaceTable[i], NULL);
		SDL_BlitSurface(surfaceTable[i], NULL, sdl_screen, NULL);
		SDL_UpdateRect(sdl_screen, 0, 0, 0, 0);
		SDL_Delay(20);

	}

	SDL_BlitSurface(newSurface, NULL, sdl_screen, NULL);
	SDL_UpdateRect(sdl_screen, 0, 0, 0, 0);

	SDL_FreeSurface(backupSurface);
	SDL_FreeSurface(newSurface);
	SDL_FreeSurface(tempSurface);
}

void os_fullScreen()
{
	SDL_FreeSurface(sdl_screen);
	sdl_screen = NULL;

	fullscreen ^= 1;

	sdl_screen = SDL_SetVideoMode(640, 480, 32, SDL_SWSURFACE | (fullscreen ? SDL_FULLSCREEN:0));
	SDL_SetColors(sdl_screen, (SDL_Color*)palette, 0, 256);
	SDL_ShowCursor(!SDL_FULLSCREEN);

	requestBackgroundRedraw = 1;
}

char os_isPressed(int key)
{
	Uint8 *keystates;

	SDL_PumpEvents();
	keystates = SDL_GetKeyState(NULL);
	return keystates[key];
}

unsigned int os_getTicks()
{
	return SDL_GetTicks();
}

