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

#include "type.h"

#define CopyScreen(src, dest) memcpy(dest, src, 307200)
#define Cls() memset(frontVideoBuffer, 0, 307200);

void AdelineLogo(void);
void blackToWhite(void);
void fadeIn(byte * palette);
void adjustPalette(byte R, byte G, byte B, byte * palette, int a);
int RegleTrois32(int modifier, int color, int param, int intensity);
void FadeToBlack(char *palette);
void RessPict(int imageNumber);
void FadeToPal(char *ptr);
void SetBackPal(void);
void convertPalToRGBA(byte * palSource, byte * palDest);
void loadImageCrossFade(int imageNumber);

extern byte palette[256 * 3];
extern byte paletteRGBA[256 * 4];



