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

#include "images.h"
#include "input.h"
#include "mainMenu.h"

#include "fla.h"

streamReader fla_streamReader;
frameData fla_frameData;
FLAheader flaHeaderData;
char flaBuffer[320*200];
char flaPalette[256*3];
char flaPaletteRGBA[256*4];

int flaSampleTable[100];
byte* workVideoBufferCopy;

void playFla(char *flaName)
{
	int currentFrame;
	int syncTime;
    int oldSyncTime;
	char buffer[256];

#ifndef PCLIKE
	return;
#endif

	todo("remove");

	Cls();

	unsigned int i;
	for (i = 0; i < strlen(flaName); i++)
		if (flaName[i] == '.')
			flaName[i] = 0;

	stopMusic();
#ifdef PCLIKE
	strcpy(buffer, DATADIR);
	strcat(buffer, "fla/");
	strcat(buffer, flaName);
#else
	strcpy(buffer, flaName);
#endif

	strcat(buffer, ".fla");

	if (initFla(buffer) && strcmp(flaHeaderData.version, "V1.3") == 0)
    {
		SetBackPal();
        currentFrame = 0;
        syncTime = oldSyncTime = SDL_GetTicks();

        for (;;)
        {
			if (skipIntro)
				break;

            syncTime = SDL_GetTicks();
            if (syncTime - oldSyncTime <= 1000.0f/(flaHeaderData.speed+1))
                continue;

			drawNextFrameFla();

		    osystem_draw320x200BufferToScreen((unsigned char*)flaBuffer);

			readKeyboard();

            oldSyncTime = syncTime + (syncTime - oldSyncTime - 1000.0f/(flaHeaderData.speed+1));
    	    currentFrame++;
    		if (currentFrame == flaHeaderData.numOfFrames)
	    		break;
		}

		FadeToBlack(flaPalette);
		HQ_StopSample();
		SetBackPal();
		Cls();
	}
}

int initFla(char* file)
{
	int i;
	int samplesInFla;

	streamReader_open(&fla_streamReader, file, 0);

	workVideoBufferCopy = workVideoBuffer;

	if (samplesLoaded)
		printf("Sample loaded in initFla\n");

	streamReader_get(&fla_streamReader, &flaHeaderData.version, 6);
	streamReader_get(&fla_streamReader, &flaHeaderData.numOfFrames, 4);
	streamReader_get(&fla_streamReader, &flaHeaderData.speed, 1);
	streamReader_get(&fla_streamReader, &flaHeaderData.var1, 1);
	streamReader_get(&fla_streamReader, &flaHeaderData.var2, 2);
	streamReader_get(&fla_streamReader, &flaHeaderData.var3, 2);
 
	streamReader_get(&fla_streamReader, &samplesInFla, 4);
	samplesInFla &= 0xFFFF;

	printf("%d frames\n", flaHeaderData.numOfFrames);
	printf("%d samples\n", samplesInFla);

	for (i = 0; i < samplesInFla; i++)
	{
		short int var0;
		short int var1;

		streamReader_get(&fla_streamReader, &var0, 2);
		streamReader_get(&fla_streamReader, &var1, 2);

		flaSampleTable[i] = var0;
	}

	return 1;
}

void drawNextFrameFla()
{
	char* ptr;
	unsigned int currentOpcodeGlob;
	unsigned char currentOpcode;

	int var_C;
	streamReader_get(&fla_streamReader, &fla_frameData.videoSize, 1);
	streamReader_get(&fla_streamReader, &fla_frameData.dummy, 1);
	streamReader_get(&fla_streamReader, &fla_frameData.frameVar0, 4);
	streamReader_get(&fla_streamReader, workVideoBufferCopy, fla_frameData.frameVar0);

	ptr = (char*)workVideoBufferCopy;

	for (var_C = 0; var_C < fla_frameData.videoSize; var_C++)
	{
		currentOpcode = *(unsigned char*)ptr;
		ptr += 2;
		currentOpcodeGlob = *(unsigned short int*)ptr;
		ptr += 2;

		switch (currentOpcode - 1) {
		case 0: { // load palette
				short int numOfColor = READ_LE_S16(ptr);
				short int startColor = READ_LE_S16((ptr + 2));

				copyStringToString(ptr + 4, flaPalette + startColor*3, numOfColor*3);

				convertPalToRGBA((byte*)flaPalette, (byte*)flaPaletteRGBA);
				osystem_setPalette320x200((byte*)flaPaletteRGBA);

				break;
			}
		case 1: {
				printf("Fade/music opcode in fla\n");
				break;
			}
		case 2: { // play sample
				flaSample header;
				memcpy(&header, ptr, sizeof(flaSample));
				playSampleFla(header.sampleNum, /* header.freq, */header.repeat/*, header.x, header.y*/);
				break;
			}
		case 4: { // stop sample
				break;
			}
		case 5: { // draw delat frame
				updateFrame(ptr, 320);
				break;
			}
		case 7: { // draw key frame
				drawFrame(ptr, 320, flaHeaderData.var3);
				break;
			}
		default:
			printf("Unhandled fla opcode %d!\n", currentOpcode - 1);
			break;
		}

		ptr += currentOpcodeGlob;
	}

}

void drawFrame(char* ptr, int width, int height)
{
	char* destPtr = (char*)flaBuffer;
	char* startOfLine = destPtr;

	char fill;
    char loop;

	int i;
	int j;

	while (height-- > 0)
    {
        loop = *(ptr++);
		for (i = 0; i < loop; i++)
        {
			fill = *(ptr++);

			if (fill < 0) /* copy all pixels */
				for (j = 0; j < -fill; j++)
					*(destPtr++) = *(ptr++);
			else
            {
				for (j = 0; j < fill; j++)
					*(destPtr++) = *ptr;
				ptr++;
			}
		}

		startOfLine = destPtr = startOfLine + width;
	}
}

void updateFrame(char* ptr, int width)
{
	unsigned short int skip;
	char* destPtr;
	char* startOfLine;
	int height;

	char fill;
    char loop;

	int i, j;

    skip = READ_LE_U16(ptr);
	ptr += 2;
	skip *= width;
	startOfLine = destPtr = (char*)flaBuffer + skip;
    height = READ_LE_S16(ptr);;
	ptr += 2;

	while (height-- > 0)
    {
        loop = *(ptr++);
		for (i = 0; i < loop; i++)
        {
			destPtr += (unsigned char) *(ptr++);
			fill = *(ptr++);

			if (fill > 0) /* copy all pixels */
				for (j = 0; j < fill; j++)
					*(destPtr++) = *(ptr++);
			else /* same color for all the line */
			{
				for (j = 0; j < -fill; j++)
					*(destPtr++) = *ptr;
                ptr++;
			}
		}

		startOfLine = destPtr = startOfLine + width;
	}
}

