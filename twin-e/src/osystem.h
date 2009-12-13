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

#include "SDL.h"

int osystem_init();

extern int osystem_mouseRight;
extern int osystem_mouseLeft;
extern char fullscreen;
extern char breakMainLoop;

void osystem_mainLoop(void);
void osystem_delay();
void osystem_crossFade(char *buffer, char *palette);
void osystem_initBuffer(char *buffer, int width, int height);
void osystem_initVideoBuffer(char *buffer, int width, int height);
void osystem_setPalette(byte * palette);
void osystem_setPalette320x200(byte * palette);
void osystem_flip();
void osystem_draw320x200BufferToScreen();
void osystem_copyBlockPhys(int left, int top, int right, int bottom);
void osystem_getMouseStatus(mouseStatus* mouseData);

void osystem_fullScreen();

