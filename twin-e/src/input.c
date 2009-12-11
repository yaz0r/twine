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

#include "lba.h"
#include "SDL/SDL.h"

unsigned char scanCodeTab1[] = {
	0x48,
	0x50,
	0x4B,
	0x4D,
	0x47,
	0x49,
	0x51,
	0x4F,

	0x39,
	0x1C,
	0x1D,
	0x38,
	0x53,
	0x2A,
	0x36,

	0x3B,
	0x3C,
	0x3D,
	0x3E,
	0x3F,
	0x40,
	0x41,
	0x42,
	0x43,
	0x44,
	0x57,
	0x58,
	0x2A,
	0x0
};

unsigned short int scanCodeTab2[] = {
	0x0100, // up
	0x0200, // down
	0x0400, // left
	0x0800, // right
	0x0500, // home
	0x0900, // pageup
	0x0A00, // pagedown
	0x0600, // end

	0x0101, // space bar
	0x0201, // enter
	0x0401,  // ctrl
	0x0801,  // alt
	0x1001,  // del
	0x2001,  // left shift
	0x2001,  // right shift

	0x0102,  // F1
	0x0202,  // F2
	0x0402,  // F3
	0x0802,  // F4
	0x1002,  // F5
	0x2002,  // F6
	0x4002,  // F7
	0x8002,  // F8

	0x0103,  // F9
	0x0203,  // F10
	0x0403,  // ?
	0x0803,  // ?
	0x00FF,  // left shift
	0x00FF,
	0x0,
	0x0
};

void readKeyboard(void) {
	SDL_Event event;
	int localKey;
	int i;
	int find = 0;
	short int temp;
	unsigned char temp2;
	char found = 0;
	int size;
	int j;
	unsigned char *keyboard;

	localKey = 0;

	printTextVar12 = 0;
	key1 = 0;
	skipIntro = 0;

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			exit(0);
			break;
		case SDL_MOUSEBUTTONDOWN:
			if (event.button.button == 3)
				osystem_mouseRight = 1;
			else
				osystem_mouseLeft = 1;
			break;
		default:
			break;
		}
	}

	SDL_PumpEvents();

	keyboard = SDL_GetKeyState(&size);

	for (j = 0; j < size; j++) {
		if (keyboard[j]) {
			switch (j) {
			case SDLK_ESCAPE:
				localKey = 0x1;
				break;
			case SDLK_RETURN:
				localKey = 0x1C;
				break;
			case SDLK_LSHIFT:
			case SDLK_RSHIFT:
				localKey = 0x36;
				break;
			case SDLK_LALT:
			case SDLK_RALT:
				localKey = 0x38;
				break;
			case SDLK_SPACE:
				localKey = 0x39;
				break;
			case SDLK_UP:
			case SDLK_KP8:
				localKey = 0x48;
				break;
			case SDLK_DOWN:
			case SDLK_KP2:
				localKey = 0x50;
				break;
			case SDLK_LEFT:
			case SDLK_KP4:
				localKey = 0x4B;
				break;
			case SDLK_RIGHT:
			case SDLK_KP6:
				localKey = 0x4D;
				break;
			case SDLK_LCTRL:
			case SDLK_RCTRL:
				localKey = 0x1D;
				break;
			case SDLK_PAGEUP:
				localKey = 0x49;
				break;
			case SDLK_p: // pause
				localKey = 'p';
				break;
			case SDLK_h: // holomap
				localKey = 'h';
				break;
			case SDLK_j:
				localKey = 'j';
				break;
			case SDLK_w: // Especial key to do the action
				localKey = 'w';
				break;
			case SDLK_F1:
				fkeys = 1; // F1
				break;
			case SDLK_F2:
				fkeys = 2; // F2
				break;
			case SDLK_F3:
				fkeys = 3; // F3
				break;
			case SDLK_F4:
				fkeys = 4; // F4
				break;
			case SDLK_F6:
				fkeys = 6; // F6
				break;
			case SDLK_F12:
				osystem_fullScreen();
				break;
			}

			for (i = 0; i < 28; i++) {
				if (scanCodeTab1[i] == localKey) {
					find = i;
					found = 1;
				}
			}

			if (found != 0) {
				temp = scanCodeTab2[find];
				temp2 = temp & 0x00FF;

				if (temp2 == 0) {
					if (!(localKey & 0x80)) {
						printTextVar12 |= (temp & 0xFF00) >> 8;
					} else {
						printTextVar12 &= -((temp & 0xFF00) >> 8);
					}
				} else {
					// mode non géré ici
					key1 |= (temp & 0xFF00) >> 8;
				}
			} else {
				// no géré ici aussi
				skipIntro = localKey;
			}
		}
	}
}

