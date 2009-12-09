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

#ifndef _HQRLIB_
#define _HQRLIB_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct hqr_entry_s {
	char fileName[12];
	char preloadedResource;

	/* non preloaded data */
	int size1;
	int remainingSize;
	short int numEntriesMax;
	short int numCurrentlyUsedEntries;
	unsigned char *ptr;

	/* preloaded data */
	unsigned char** ptrArray;
	int* sizeArray;

} hqr_entry;

typedef struct subHqr_s {
	short int index;
	unsigned int offFromPtr;
	unsigned int size;
	int lastAccessedTime;
} subHqr;

extern short int HQR_Flag;
extern volatile int lba_time;

void HQR_Destroy_Ressource(hqr_entry* resource);
int Load_HQR(char *resourceName, unsigned char *ptr, int imageNumber);
hqr_entry* HQR_Init_Ressource(char *fileName, int sizeOfBuffer, int numOfEntriesInBuffer);
int HQR_RemoveEntryFromHQR(hqr_entry * hqrPtr, int var);
unsigned char *HQR_Get(hqr_entry * hqrPtr, short int arg_4);
int Size_HQR(char *fileName, int index);
int HQRM_Load(char *fileName, short int arg_4, unsigned char ** ptr);
void HQR_Reset_Ressource(hqr_entry * ptr);
void HQR_Expand(int decompressedSize, unsigned char *destination, unsigned char *source);
int HQR_GetNumEntry(/*char* fileName*/);
hqr_entry* HQR_Init_RessourcePreload(char *resourceName);
unsigned char *HQR_GetCopy(hqr_entry * hqrPtr, short int arg_4);

#endif

