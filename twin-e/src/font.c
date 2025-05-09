/*
Copyright (C) 2002-2010 The TwinE team

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

#include "text.h"
#include "main.h"

byte *fntFont;
int interCharSpace;
int spaceLenght;
int textColor;
int stringLenght;

void SetFont(byte * font, int param2, int param1) {
	fntFont = font;
	interCharSpace = param2;
	spaceLenght = param1;
}

void drawCharacter(int X, int Y, unsigned char caractere) {
	byte sizeX;
	byte sizeY;
	byte param1;
	byte param2;
	byte *data;
	byte *screen;

	int toNextLine;
	char index;

	char usedColor;
	char number;
	char jump;

	int i;

	int tempX;
	int tempY;

	data = fntFont + convertWFromLE(*((short int *)(fntFont + caractere * 4)));

	sizeX = *(data++);
	sizeY = *(data++);
	param1 = *(data++);
	param2 = *(data++);

	X += param1;
	Y += param2;

	usedColor = textColor;

	screen = frontVideoBuffer + WINDOW_X*Y + X;

	tempX = X;
	tempY = Y;

	toNextLine = WINDOW_X - sizeX;

	for (;;) {
		index = *(data++);
		for (;;) {
			jump = *(data++);
			screen += jump;
			tempX += jump;
			/* New line */
			if (--index == 0) {
				screen += toNextLine;
				tempY++;
				tempX = X;
				sizeY--;
				if (sizeY <= 0)
					return;
				break;
			} else {
				number = *(data++);
				for (i = 0; i < number; i++) {
					if (tempX >= textWindowLeft && tempX < textWindowRight && tempY >= textWindowTop && tempY < textWindowBottom)
						frontVideoBuffer[640*tempY + tempX] = usedColor;

					screen++;
					tempX++;
				}

				if (--index == 0) {
					screen += toNextLine;
					tempY++;
					tempX = X;

					sizeY--;
					if (sizeY <= 0)
						return;
					break;
				}
			}
		}
	}
}

void Font(int X, int Y, char *string) {
	unsigned char character;

	if (fntFont == 0)   // if the font is defined
		return;

	for (character = -1; character != 0; character = (unsigned char)*(string++)) {
		if (character == 0x20) {
			X += spaceLenght;
		} else {
			stringLenght = *(fntFont + convertWFromLE(*((short int *)(fntFont + character * 4))));  // get the length of the character
			drawCharacter(X, Y, character); // draw the character on screen
			X += interCharSpace;  // add the length of the space between 2 characters
			X += stringLenght;  // add the length of the current character
		}
	}
}

