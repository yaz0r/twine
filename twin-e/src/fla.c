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

#include "images.h"
#include "lba.h"

streamReader fla_streamReader;

int flaSampleTable[100];

void PlayAnimFla(char *flaName)
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
	strcpy(buffer, "fla/");
	strcat(buffer, flaName);
#else
	strcpy(buffer, flaName);
#endif
    strcat(buffer, ".fla");

	if (InitFla(buffer) && strcmp(flaHeaderData.version, "V1.3") == 0)
    {
		SetBackPal();
        currentFrame = 0;
        syncTime = oldSyncTime = SDL_GetTicks();

        for (;;)
        {
			if (skipIntro)
				break;

            syncTime = SDL_GetTicks();
            if (syncTime - oldSyncTime < 30) /* TODO: good value ? */
                continue;

			DrawNextFrameFla();

		    osystem_draw320x200BufferToScreen((unsigned char*)flaBuffer);

			readKeyboard();

            oldSyncTime = syncTime + (syncTime - oldSyncTime - 30);
    	    currentFrame++;
    		if (currentFrame == numOfFrameInFLA)
	    		break;
		}

		FadeToBlack(flaPalette);
		HQ_StopSample();
		SetBackPal();
		Cls();
	}
}

int InitFla(char* file) {
	int i;

	streamReader_open(&fla_streamReader, file, 0);

	workVideoBufferCopy = workVideoBuffer;

	if (samplesLoaded) {
		printf("Sample loaded in InitFla\n");
		//exit(1);
	}

	streamReader_get(&fla_streamReader, &flaHeaderData.version, 6);
	streamReader_get(&fla_streamReader, &flaHeaderData.numOfFrames, 4);
	streamReader_get(&fla_streamReader, &flaHeaderData.speed, 1);
	streamReader_get(&fla_streamReader, &flaHeaderData.var1, 1);
	streamReader_get(&fla_streamReader, &flaHeaderData.var2, 2);
	streamReader_get(&fla_streamReader, &flaHeaderData.var3, 2);

	numOfFrameInFLA = flaHeaderData.numOfFrames;

	printf("%d frames\n", numOfFrameInFLA);

	flahVar2 = flaHeaderData.var2;
	flahVar3 = flaHeaderData.var3;
	flaSpeed = flaHeaderData.speed;

	streamReader_get(&fla_streamReader, &samplesInFla, 4);

	samplesInFla &= 0xFFFF; // TODO: remove this cheat

	printf("%d samples in fla\n", samplesInFla);

	for (i = 0;i < samplesInFla;i++) {
		short int var0;
		short int var1;

		streamReader_get(&fla_streamReader, &var0, 2);
		streamReader_get(&fla_streamReader, &var1, 2);

		flaSampleTable[i] = var0;
	}

	return 1;
}

struct flaSampleStruct {
	short int sampleNum;
	short int freq;
	short int repeat;
	char dummy;
	unsigned char x;
	unsigned char y;
};

typedef struct flaSampleStruct flaSampleStruct;

void DrawNextFrameFla() {
	char* ptr;
	unsigned int currentOpcodeGlob;
	unsigned char currentOpcode;

	int var_C;
	streamReader_get(&fla_streamReader, &frameData.videoSize, 1);
	streamReader_get(&fla_streamReader, &frameData.dummy, 1);
	streamReader_get(&fla_streamReader, &frameData.frameVar0, 4);

	var_C = 0;

	runFLAscriptVar0 = frameData.videoSize;

	streamReader_get(&fla_streamReader, workVideoBufferCopy, frameData.frameVar0);

	if (var_C >= runFLAscriptVar0)
		return;

	ptr = (char*)workVideoBufferCopy;

	do {
		currentOpcode = *(unsigned char*)ptr;
		ptr += 2;
		currentOpcodeGlob = *(unsigned short int*)ptr;
		ptr += 2;

		/*printf("Size=%d\n",currentOpcodeGlob);

		printf("Opcode: %d\n",currentOpcode-1);*/

		switch (currentOpcode - 1) {
		case 0: { // load palette
				short int numOfColor = READ_LE_S16(ptr);
				short int startColor = READ_LE_S16((ptr + 2));

				copyStringToString(ptr + 4, flaPalette + startColor*3, numOfColor*3);

				lastNumOfColor = numOfColor;
				lastStartColor = startColor;

				convertPalToRGBA((byte*)flaPalette, (byte*)flaPaletteRGBA);
				osystem_setPalette320x200((byte*)flaPaletteRGBA);

				break;
			}
		case 1: {
				printf("Fade/music opcode in fla\n");
				break;
			}
		case 2: { // play sample
				flaSampleStruct header;
				memcpy(&header, ptr, sizeof(flaSampleStruct));
				playSampleFla(header.sampleNum, /* header.freq, */header.repeat/*, header.x, header.y*/);
				break;
			}
		case 4: { // stop sample
				break;
			}
		case 5: { // draw delat frame
				UpdateFrame(ptr, 320);
				break;
			}
		case 7: { // draw key frame
				DrawFrame(ptr, 320, flahVar3);
				break;
			}
		default: {
				printf("Unhandled fla opcode %d!\n", currentOpcode - 1);
				//exit(1);
				break;
			}
		}

		var_C++;
		ptr += currentOpcodeGlob;

	} while (var_C < runFLAscriptVar0);

}

void DrawFrame(char* ptr, int width, int height) {
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

void UpdateFrame(char* ptr, int width) {
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

