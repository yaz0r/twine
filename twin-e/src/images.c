#include "lba.h"

void LBA_engine::displayAdelineLogo(void)
{
    playMidi(31);
    loadImageToPtr("ress.hqr", videoBuffer2, 27);
    copyToBuffer(videoBuffer2, videoBuffer1);
    loadImageToPtr("ress.hqr", palette, 28);
    convertPalToRGBA(palette, paletteRGBA);
    blackToWhite();
    osystem->drawBufferToScreen(videoBuffer1);
    fadeIn(paletteRGBA);
   // SDL_Delay(2000);
}

void LBA_engine::copyToBuffer(byte * source, byte * destination)
{

    memcpy(destination, source, 307200);
}

void LBA_engine::fadeIn(byte * palette)
{
    int i;

    for (i = 0; i < 100; i += 3)
	{
	    adjustPalette(255, 255, 255, palette, i);
	    readKeyboard();
	}
}

void LBA_engine::adjustPalette(byte R, byte G, byte B, byte * palette, int intensity)
{
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

    for (i = 0; i < 256; i++)
	{
	    *newR = remapComposante(R, palette[counter], 100, local);
	    *newG = remapComposante(G, palette[counter + 1], 100, local);
	    *newB = remapComposante(B, palette[counter + 2], 100, local);
	    *newA = 0;

	    newR += 4;
	    newG += 4;
	    newB += 4;
	    newA += 4;

	    counter += 4;
	}

    osystem->setPalette(localPalette);
}

int LBA_engine::remapComposante(int modifier, int color, int param, int intensity)
{
    if (!param)
	return (color);
    return (((color - modifier) * intensity) / param) + modifier;
}

void LBA_engine::loadImageAndPalette(int imageNumber)
{
    loadImageToPtr("ress.hqr", videoBuffer2, imageNumber);
    copyToBuffer(videoBuffer2, videoBuffer1);
    loadImageToPtr("ress.hqr", palette, imageNumber + 1);
    convertPalToRGBA(palette, paletteRGBA);
    osystem->drawBufferToScreen(videoBuffer1);
    fadeIn2((char *) paletteRGBA);
}

void LBA_engine::loadImageCrossFade(int imageNumber)
{
    loadImageToPtr("ress.hqr", videoBuffer2, imageNumber);
    copyToBuffer(videoBuffer2, videoBuffer1);
    loadImageToPtr("ress.hqr", palette, imageNumber + 1);
    convertPalToRGBA(palette, paletteRGBA);

    osystem->crossFade((char *) videoBuffer1, (char *) paletteRGBA);
}

void LBA_engine::fadeOut(char *palette)
{
    int i;

    if (palReseted == 0)
	{
	    for (i = 100; i >= 0; i -= 3)
		{
		    adjustPalette(0, 0, 0, (byte *) palette, i);
		    readKeyboard();
		}
	}

    palReseted = 1;
}

void LBA_engine::fadeIn2(char *palette)
{
    int i;

    for (i = 0; i <= 100; i += 3)
	{
	    adjustPalette(0, 0, 0, (byte *) palette, i);
	    readKeyboard();
	}

    palReseted = 0;

}

void LBA_engine::blackToWhite(void)
{
    byte palette[1024];
    int i;

    for (i = 0; i < 256; i += 3)
	{
	    memset(palette, i, 1024);

	    osystem->setPalette(palette);
	    readKeyboard();
	}
}

void LBA_engine::resetPalette(void)
{
   // int i;

   /*
    * for(i=0;i<768;i++) palette[i]=0; 
    */

    memset(palette, 0, 768);
    memset(paletteRGBA, 0, 1024);

    osystem->setPalette(paletteRGBA);

    palReseted = 1;
}

void LBA_engine::resetVideoBuffer1(void)
{
   /*
    * int i;
    * 
    * for(i=0;i<307200;i++) { videoBuffer1[i]=0; }
    */

    memset(videoBuffer1, 0, 307200);
}

void LBA_engine::convertPalToRGBA(byte * palSource, byte * palDest)
{
    int i;

    int *colorDest = (int *) palDest;

    for (i = 0; i < 256; i++)
	{
	    *(colorDest++) = (*(int *) palSource);	// little optimisation trick
	    palSource += 3;
	}

}
