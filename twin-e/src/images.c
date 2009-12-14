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

#include "samples.h"
#include "hqr.h"
#include "main.h"

#include "images.h"

byte palette[256 * 3];
byte paletteRGBA[256 * 4];

short int palReseted;

void AdelineLogo(void) {
	playMidi(31);
	Load_HQR("ress.hqr", workVideoBuffer, 27);
	CopyScreen(workVideoBuffer, frontVideoBuffer);
	Load_HQR("ress.hqr", palette, 28);
	convertPalToRGBA(palette, paletteRGBA);
	blackToWhite();
	os_flip(frontVideoBuffer);
	fadeIn(paletteRGBA);
#ifdef PCLIKE
	os_delay(6000);
#endif
}

void fadeIn(byte * palette) {
	int i = 100;

	for (i = 0; i < 100; i += 3)
		adjustPalette(255, 255, 255, palette, i);
}

void adjustPalette(byte R, byte G, byte B, byte * palette, int intensity) {
	byte localPalette[1024];
	byte *newR;
	byte *newG;
	byte *newB;
	byte *newA;

	int local;
	int counter = 0;
	int i;

	local = intensity;

	newR = &localPalette[0];
	newG = &localPalette[1];
	newB = &localPalette[2];
	newA = &localPalette[3];

	for (i = 0; i < 256; i++) {
		*newR = RegleTrois32(R, palette[counter], 100, local);
		*newG = RegleTrois32(G, palette[counter + 1], 100, local);
		*newB = RegleTrois32(B, palette[counter + 2], 100, local);
		*newA = 0;

		newR += 4;
		newG += 4;
		newB += 4;
		newA += 4;

		counter += 4;
	}

	os_setPalette(localPalette);
}

int RegleTrois32(int modifier, int color, int param, int intensity) {
	if (!param)
		return (color);
	return (((color - modifier) * intensity) / param) + modifier;
}

void RessPict(int imageNumber) {
	Load_HQR("ress.hqr", workVideoBuffer, imageNumber);
	CopyScreen(workVideoBuffer, frontVideoBuffer);
	Load_HQR("ress.hqr", palette, imageNumber + 1);
	convertPalToRGBA(palette, paletteRGBA);
	os_flip(frontVideoBuffer);
	FadeToPal((char *) paletteRGBA);
}

void loadImageCrossFade(int imageNumber) {
	Load_HQR("ress.hqr", workVideoBuffer, imageNumber);
	CopyScreen(workVideoBuffer, frontVideoBuffer);
	Load_HQR("ress.hqr", palette, imageNumber + 1);
	convertPalToRGBA(palette, paletteRGBA);

	os_crossFade((char *) frontVideoBuffer, (char *) paletteRGBA);
}

void FadeToBlack(char *palette) {
	int i = 0;

	if (palReseted == 0)
		for (i = 100; i >= 0; i -= 3)
			adjustPalette(0, 0, 0, (byte *) palette, i);

	palReseted = 1;
}

void FadeToPal(char *palette) {
	int i = 100;

	for (i = 0; i <= 100; i += 3)
		adjustPalette(0, 0, 0, (byte *) palette, i);

	os_setPalette((byte*)palette);

	palReseted = 0;

}

void blackToWhite(void)
{
	byte palette[1024];
	int i;

	for (i = 255; i >= 0; i -= 3)
	{
		memset(palette, i, 1024);
		os_setPalette(palette);
	}
}

void SetBackPal(void)
{
	memset(palette, 0, 768);
	memset(paletteRGBA, 0, 1024);

	os_setPalette(paletteRGBA);

	palReseted = 1;
}

void convertPalToRGBA(byte * palSource, byte * palDest) {
	int i;

	for (i = 0; i < 256; i++) {
		palDest[0] = palSource[0];
		palDest[1] = palSource[1];
		palDest[2] = palSource[2];
		palDest += 4;
		palSource += 3;
	}
}
