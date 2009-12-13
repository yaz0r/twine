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

#include "room.h"
#include "hqr.h"
#include "main.h"
#include "mainLoop.h"

#include "cube.h"

unsigned char *currentGrid;
unsigned char *currentBll;

void initGrid(short int roomNumber)
{
	int gridSize;
	int bllSize;
	int brickDataSize;

	gridSize = Size_HQR(DATADIR "lba_gri.hqr", roomNumber);
	currentGrid = malloc(gridSize + 1000);
	Load_HQR("lba_gri.hqr", currentGrid, roomNumber);

	bllSize = Size_HQR(DATADIR "lba_bll.hqr", roomNumber);
    currentBll = malloc(bllSize + 1000);
	Load_HQR("lba_bll.hqr", currentBll, roomNumber);

	brickDataSize = loadBrk(gridSize);
	CreateMaskGph();

	createCube();
}

void MixteMapToCube(byte* gridPtr) {
	int var_8 = 0;
	int var_4 = 0;

	int i;
	int j;
	char* ptr;

	for (i = 0;i < 64;i++) {
		int posInBufCube = var_4;
		ptr = (char*)gridPtr + var_8;


		for (j = 0;j < 64;j++) {
			unsigned char* ptr2 = (unsigned char*)(gridPtr + READ_LE_S16(ptr));
			ptr += 2;
			MixteColonne(ptr2, bufCube + posInBufCube);
			posInBufCube += 50;
		}

		var_8 += 0x80;
		var_4 += 3200;
	}
}

void MixteColonne(unsigned char *gridEntry, unsigned char *dest) {

	int numIteration;
	int temp1;
	int temp2;

	int temp3;

	int i;
	unsigned short int *source;
	unsigned short int *arrive;

	temp1 = *(gridEntry++);

	do {
		temp2 = *(gridEntry++);

		numIteration = (temp2 & 0x3F) + 1;

		source = (unsigned short int *) gridEntry;
		arrive = (unsigned short int *) dest;

		if (!(temp2 & 0xC0)) {
			for (i = 0; i < numIteration; i++)
				(arrive++);
		} else if (temp2 & 0x40) {
			for (i = 0; i < numIteration; i++)
				*(arrive++) = *(source++);
		} else {
			temp3 = *(source++);
			for (i = 0; i < numIteration; i++)
				*(arrive++) = temp3;
		}

		gridEntry = (unsigned char *) source;
		dest = (unsigned char *) arrive;

	} while (--temp1);
}

void IncrustGrm(int gridNumber) {
	byte* gridPtr;

	HQRM_Load("lba_gri.hqr", gridNumber + 120, (unsigned char**)&gridPtr);

	if (!gridPtr) {
		printf("arg grm not found in lba_gri\n");
		exit(1);
	}

	MixteMapToCube(gridPtr);

	free(gridPtr);
	requestBackgroundRedraw = 1;
}

// this unpack the grid to the cube buffer
void createCube(void) {
	int var2 = 0;
	int ptr1;
	int ptr2;
	int i;
	int j;

	for (j = 0;j < 64;j++) {
		ptr1 = var2;
		ptr2 = j << 6;

		for (i = 0;i < 64;i++) {
			addCubeEntry(currentGrid +  READ_LE_U16(currentGrid + 2 * (i + ptr2)), bufCube + ptr1);
			ptr1 += 50;
		}

		var2 += 3200;
	}
}

// this unpack a vertical column from the grid to the cube buffer
void addCubeEntry(unsigned char *gridEntry, unsigned char *dest) {

	int numIteration;
	int temp1;
	int temp2;

	int temp3;

	int i;
	unsigned short int *source;
	unsigned short int *arrive;

	temp1 = *(gridEntry++);

	do {
		temp2 = *(gridEntry++);

		numIteration = (temp2 & 0x3F) + 1;

		source = (unsigned short int *) gridEntry;
		arrive = (unsigned short int *) dest;

		if (!(temp2 & 0xC0)) {
			for (i = 0; i < numIteration; i++)
				WRITE_LE_U16(arrive++, 0);
		} else if (temp2 & 0x40) {
			for (i = 0; i < numIteration; i++)
				WRITE_LE_U16(arrive++, READ_LE_U16(source++));
		} else {
			temp3 = READ_LE_U16(source++);
			for (i = 0; i < numIteration; i++)
				WRITE_LE_U16(arrive++, temp3);
		}

		gridEntry = (unsigned char *) source;
		dest = (unsigned char *) arrive;

	} while (--temp1);
}

// build the masks for the bricks
int CalcGraphMsk(unsigned char *buffer, unsigned char *ptr) {
	unsigned int *ptrSave = (unsigned int *)ptr;
	unsigned char *ptr2;
	unsigned char *esi;
	unsigned char *edi;
	byte iteration, ch, numOfBlock, ah, bl, al, bh;
	int ebx;

	//buffer = (unsigned int *) (READ_LE_U32(buffer + var) + (unsigned char *) buffer);

	ebx = READ_LE_U32(buffer);   // on ecrit le flag de la brique
	buffer += 4;
	WRITE_LE_U32(ptr, ebx);
	ptr += 4;

	bh = (ebx & 0x0000FF00) >> 8;

	esi = (unsigned char *) buffer;
	edi = (unsigned char *) ptr;

	iteration = 0;
	ch = 0;

	do {
		numOfBlock = 0;
		ah = 0;
		ptr2 = edi;

		edi++;

		bl = *(esi++);

		if (*(esi) & 0xC0) { // the first time isn't skip. the skip size is 0 in that case
			*edi++ = 0;
			numOfBlock++;
		}

		do {
			al = *esi++;
			iteration = al;
			iteration &= 0x3F;
			iteration++;

			if (al & 0x80) {
				ah += iteration;
				esi++;
			} else if (al & 0x40) {
				ah += iteration;
				esi += iteration;
			} else { // skip
				if (ah) {
					*edi++ = ah; // write down the number of pixel passed so far
					numOfBlock++;
					ah = 0;
				}
				*(edi++) = iteration; //write skip
				numOfBlock++;
			}
		} while (--bl > 0);

		if (ah) {
			*edi++ = ah;
			numOfBlock++;

			ah = 0;
		}

		*ptr2 = numOfBlock;
	} while (--bh > 0);

	return ((int)((unsigned char *) edi - (unsigned char *) ptrSave));
}
