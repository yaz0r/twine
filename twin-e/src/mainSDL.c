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

#if USE_SDL_MIXER == 1
#include <SDL_mixer.h>
#endif

#include "mainLoop.h"
#include "hqr.h"
#include "images.h"

#include "osystem.h"

#define SPEED 15

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

SDL_Surface *sdl_buffer;
SDL_Surface *sdl_buffer320x200;
SDL_Surface *sdl_screen;

char breakMainLoop = 0;

void os_mainLoop(void) {
	unsigned long int t_start, t_actual;
	SDL_Event event;

	while (!breakMainLoop)
	{
		while (SDL_PollEvent(&event))
			if (event.type == SDL_QUIT)
				breakMainLoop = 1;

		mainLoopInteration();

		t_start = t_actual = SDL_GetTicks();
		if (t_actual < t_start + SPEED)
			SDL_Delay(t_start + SPEED - t_actual);

		lba_time++;
	}
}

void os_quit(void) {
	breakMainLoop = 1;
}

void os_delay(int time) {
   SDL_Delay(time);
}

int os_init()
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_CDROM | (USE_SDL_MIXER ? SDL_INIT_AUDIO:0)) < 0) {
		fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}

	atexit(SDL_Quit);

#if USE_SDL_MIXER == 1
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

	return 0;
}

void os_setPalette(byte * palette)
{
	SDL_SetColors(sdl_buffer, (SDL_Color *)palette, 0, 256);

	SDL_BlitSurface(sdl_buffer, NULL, sdl_screen, NULL);
	SDL_UpdateRect(sdl_screen, 0, 0, 0, 0);
}

void os_setPalette320x200(byte * palette)
{
	SDL_SetColors(sdl_buffer320x200, (SDL_Color *)palette, 0, 256);
}


void os_flip()
{
	SDL_BlitSurface(sdl_buffer, NULL, sdl_screen, NULL);
	SDL_UpdateRect(sdl_screen, 0, 0, 0, 0);
}

void os_draw320x200BufferToScreen()
{
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
	sdl_buffer320x200 = SDL_CreateRGBSurfaceFrom(buffer, width, height, 8, 320, 0, 0, 0, 0);
}

void os_initBuffer(char *buffer, int width, int height) {
	sdl_buffer = SDL_CreateRGBSurfaceFrom(buffer, width, height, 8, 640, 0, 0, 0, 0);
}

void os_crossFade(char *buffer, char *palette) {
	int i;
	SDL_Surface *fadeSurface
		= SDL_CreateRGBSurfaceFrom(buffer, 640, 480, 8, 640, 0, 0, 0, 0);
	SDL_SetColors(fadeSurface, (SDL_Color *) palette, 0, 256);

	for (i = 0; i < 7; i++) {
		SDL_SetAlpha(fadeSurface, SDL_SRCALPHA | SDL_RLEACCEL, i * 32);
		SDL_BlitSurface(fadeSurface, NULL, sdl_screen, NULL);
		SDL_UpdateRect(sdl_screen, 0, 0, 0, 0);
		SDL_Delay(50);
	}

	SDL_FreeSurface(fadeSurface);
}

void os_fullScreen()
{
	fullscreen ^= 1;

	sdl_screen = SDL_SetVideoMode(640, 480, 32, SDL_SWSURFACE | (fullscreen ? SDL_FULLSCREEN:0));
	SDL_SetColors(sdl_screen, (SDL_Color*)palette, 0, 256);
	SDL_ShowCursor(!fullscreen);

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

