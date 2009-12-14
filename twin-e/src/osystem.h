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

#include "config.h"

#ifdef USE_SDL
#include <SDL.h>
#include "mainSDL.h"
#endif

int os_init();

extern char fullscreen;
extern char breakMainLoop;

void os_mainLoop(void);
void os_delay(int time);
void os_crossFade(char *buffer, char *palette);
void os_initBuffer(char *buffer, int width, int height);
void os_initVideoBuffer(char *buffer, int width, int height);
void os_setPalette(byte * palette);
void os_setPalette320x200(byte * palette);
void os_flip();
void os_draw320x200BufferToScreen();
void os_copyBlockPhys(int left, int top, int right, int bottom);

void os_fullScreen();
char os_isPressed(int key);
unsigned int os_getTicks();

