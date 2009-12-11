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

#include "hqr.h"

#include "type.h"
#include "streamReader.h"

#ifdef DREAMCAST
#include <shinobi.h>
#endif

volatile int lba_time;
short int HQR_Flag;
streamReader fileReader;

hqr_entry* HQR_Init_Ressource(char* fileName, int sizeOfBuffer, int numOfEntriesInBuffer) {
	hqr_entry *hqr_ptr;
	unsigned char *dataPtr;

	char newFileName[255];
	strcpy(newFileName, DATADIR);
	strcat(newFileName, fileName);

    FILE* f = NULL;
	if (!(f = fopen(newFileName, "rb"))) {
        printf("HQR: ERROR: file '%s' does not exist !\n", fileName);
		return NULL;
    }
    fclose(f);

	hqr_ptr = malloc(numOfEntriesInBuffer * sizeof(subHqr) + sizeof(hqr_entry));
	dataPtr = malloc(sizeOfBuffer + 500); /* TODO: why 500 ? */

	if (!hqr_ptr || !dataPtr)
		return NULL;

	strcpy(hqr_ptr->fileName, newFileName);
	hqr_ptr->preloadedResource = false;

	hqr_ptr->size1 = sizeOfBuffer;
	hqr_ptr->remainingSize = sizeOfBuffer;
	hqr_ptr->numEntriesMax = numOfEntriesInBuffer;
	hqr_ptr->numCurrentlyUsedEntries = 0;
	hqr_ptr->ptr = dataPtr;


	return hqr_ptr;
}

int Load_HQR(char *resourceName, unsigned char* ptr, int imageNumber) {
	unsigned int headerSize;
	unsigned int offToImage;
	unsigned int dataSize;
	unsigned int compressedSize;
	unsigned short int mode;

	char newFileName[255];
	strcpy(newFileName, DATADIR);
	strcat(newFileName, resourceName);

	if (!streamReader_open(&fileReader, newFileName, 1))
		return 0;

	streamReader_get(&fileReader, &headerSize, 4);
	headerSize = convertDWFromLE(headerSize);

	if ((unsigned int)imageNumber >= headerSize / 4) {
		streamReader_close(&fileReader);
		return 0;
	}

	streamReader_seek(&fileReader, imageNumber * 4);
	streamReader_get(&fileReader, &offToImage, 4);
	offToImage = convertDWFromLE(offToImage);

	streamReader_seek(&fileReader, offToImage);
	streamReader_get(&fileReader, &dataSize , 4);
	dataSize = convertDWFromLE(dataSize);
	streamReader_get(&fileReader, &compressedSize , 4);
	compressedSize = convertDWFromLE(compressedSize);
	streamReader_get(&fileReader, &mode , 2);
	mode = convertWFromLE(mode);

	if (mode <= 0)
		streamReader_get(&fileReader, ptr, dataSize);
	else if (mode == 1)
    {
		unsigned char* compressedDataPtr;

		compressedDataPtr = malloc(compressedSize + 500);

		streamReader_get(&fileReader, compressedDataPtr, compressedSize);
		HQR_Expand(dataSize, (unsigned char*)ptr, compressedDataPtr);

		free(compressedDataPtr);
	}

	streamReader_close(&fileReader);

	return (dataSize);
}

subHqr *findSubHqr(int arg_0, int arg_4, subHqr * arg_8) {
	subHqr *temp;
	int i;

	if (arg_4 == 0)
		return (0);

	temp = arg_8;

	for (i = 0; i < arg_4; i++) {
		if (temp->index == arg_0)
			return (temp);

		temp++;
	}

	return (0);

}

unsigned char *HQR_Get(hqr_entry * hqrPtr, short int arg_4) {
	unsigned int headerSize;
	unsigned int offToData;
	unsigned int dataSize;
	unsigned int compressedSize;
	short int mode;

	short int var_4;
	int ltime;
	int temp2;
	unsigned char *ptr;

	int i;

	int dataSize2;
	subHqr *hqrdata;
	subHqr *hqrdataPtr;

	if (arg_4 < 0)
		return 0;

	if (hqrPtr->preloadedResource)
		return hqrPtr->ptrArray[arg_4];

	hqrdata = (subHqr *)((unsigned char *) hqrPtr + sizeof(hqr_entry));

	hqrdataPtr = findSubHqr(arg_4, hqrPtr->numCurrentlyUsedEntries, hqrdata);

	if (hqrdataPtr) {
		hqrdataPtr->lastAccessedTime = lba_time;
		HQR_Flag = 0;
		return (hqrdataPtr->offFromPtr + hqrPtr->ptr);
	}

	Size_HQR(hqrPtr->fileName, arg_4);

	if (!streamReader_open(&fileReader, hqrPtr->fileName, 1))
		return 0;

	streamReader_get(&fileReader, &headerSize, 4);
	headerSize = convertDWFromLE(headerSize);

	if ((unsigned int)arg_4 >= headerSize / 4) {
		streamReader_close(&fileReader);
		return 0;
	}

	streamReader_seek(&fileReader, arg_4 * 4);
	streamReader_get(&fileReader, &offToData, 4);
	offToData = convertDWFromLE(offToData);

	streamReader_seek(&fileReader, offToData);
	streamReader_get(&fileReader, &dataSize, 4) ;
	dataSize = convertDWFromLE(dataSize);
	streamReader_get(&fileReader, &compressedSize, 4) ;
	compressedSize = convertDWFromLE(compressedSize);
	streamReader_get(&fileReader, &mode, 2) ;
	mode = convertWFromLE(mode);

	dataSize2 = dataSize;

// ici, test sur la taille de dataSize

	ltime = lba_time;

	while (dataSize2 >= hqrPtr->remainingSize || hqrPtr->numCurrentlyUsedEntries >= hqrPtr->numEntriesMax) { // pour retirer les elements les plus vieux jusqu'a ce qu'on ai de la place
		var_4 = 0;
		temp2 = 0;

		for (i = 0; i < hqrPtr->numCurrentlyUsedEntries; i++) {
			if (temp2 <= ltime - hqrdata[i].lastAccessedTime) {
				temp2 = ltime - hqrdata[var_4].lastAccessedTime;
				var_4 = i;
			}
		}

		HQR_RemoveEntryFromHQR(hqrPtr, var_4);

	}

	ptr = hqrPtr->ptr + hqrPtr->size1 - hqrPtr->remainingSize;

	if (mode <= 0) {  // uncompressed
		streamReader_get(&fileReader, ptr, dataSize);
	} else
		if (mode == 1) { // compressed
			streamReader_get(&fileReader, ptr + dataSize - compressedSize + 500, compressedSize);
			HQR_Expand(dataSize, ptr, (ptr + dataSize - compressedSize + 500));
		}

	streamReader_close(&fileReader);

	hqrdataPtr = &hqrdata[hqrPtr->numCurrentlyUsedEntries];

	hqrdataPtr->index = arg_4;
	HQR_Flag = 1;
	hqrdataPtr->lastAccessedTime = lba_time;
	hqrdataPtr->offFromPtr = hqrPtr->size1 - hqrPtr->remainingSize;
	hqrdataPtr->size = dataSize2;

	hqrPtr->numCurrentlyUsedEntries++;
	hqrPtr->remainingSize -= dataSize2;

	return (ptr);
}

unsigned char *HQR_GetCopy(hqr_entry * hqrPtr, short int arg_4) {
	unsigned char* newPtr;

	if (hqrPtr->preloadedResource) {
		newPtr = malloc(hqrPtr->sizeArray[arg_4]);
		memcpy(newPtr, hqrPtr->ptrArray[arg_4], hqrPtr->sizeArray[arg_4]);
		return newPtr;
	}

	return NULL;
}

void HQR_Reset_Ressource(hqr_entry * ptr) {
	ptr->remainingSize = ptr->size1;
	ptr->numCurrentlyUsedEntries = 0;
}

int HQR_RemoveEntryFromHQR(hqr_entry * hqrPtr, int var) {
	subHqr *subPtr;

	int lvar;

	unsigned char *source;
	unsigned char *dest;
	int size;
	int retVal;

	todo("implement HQR_RemoveEntryFromHqr");
	HQR_Reset_Ressource(hqrPtr);
	return(var);

	lvar = var;

	subPtr = (subHqr *)(hqrPtr + sizeof(hqr_entry) + var * sizeof(subHqr));

	retVal = subPtr->size;

	if (hqrPtr->numCurrentlyUsedEntries - 1 > lvar) {
		source = hqrPtr->ptr + subPtr->offFromPtr;

		dest = source + retVal;
		size = hqrPtr->ptr + hqrPtr->size1 - dest;

		memmove(source, dest, size);

		printf("HQR_RemoveEntryFromHQR\n");
		exit(1);
		// implementer la suite
	}

	hqrPtr->numCurrentlyUsedEntries--;

	hqrPtr->remainingSize = retVal;

	return (retVal);
}

int HQRM_Load(char *fileName, short int arg_4, unsigned char ** ptr) { // recheck
	unsigned int headerSize;
	unsigned int offToData;
	unsigned int dataSize;
	unsigned int compressedSize;
	unsigned short int mode;
	unsigned char *temp;

	char newFileName[255];
	strcpy(newFileName, DATADIR);
	strcat(newFileName, fileName);

	if (!streamReader_open(&fileReader, newFileName, 1))
		return(-1);

	streamReader_get(&fileReader, &headerSize, 4);
	headerSize = convertDWFromLE(headerSize);

	if ((unsigned int)arg_4 >= headerSize / 4) {
		streamReader_close(&fileReader);
		return (-1);
	}

	streamReader_seek(&fileReader, arg_4 * 4);
	streamReader_get(&fileReader, &offToData, 4);
	offToData = convertDWFromLE(offToData);

	streamReader_seek(&fileReader, offToData);
	streamReader_get(&fileReader, &dataSize, 4);
	dataSize = convertDWFromLE(dataSize);
	streamReader_get(&fileReader, &compressedSize, 4);
	compressedSize = convertDWFromLE(compressedSize);
	streamReader_get(&fileReader, &mode , 2);
	mode = convertWFromLE(mode);

	(*ptr) = malloc(dataSize);

	if (!(*ptr)) {
		streamReader_close(&fileReader);
		return(-1);
	}

	if (mode == 0) {
		streamReader_get(&fileReader, (*ptr), dataSize);
	} else
		if (mode == 1) {
			temp = malloc(compressedSize);

			streamReader_get(&fileReader, temp, compressedSize);
			HQR_Expand(dataSize, *ptr, temp);
            free(temp);
		}

	streamReader_close(&fileReader);

	return (dataSize);
}

int Size_HQR(char *fileName, int index) {
	unsigned int headerSize;
	unsigned int dataSize;
	unsigned int offToData;

	if (!streamReader_open(&fileReader, fileName, 1)) {
		return 0;
	}

	streamReader_get(&fileReader, &headerSize, 4);
	headerSize = convertDWFromLE(headerSize);

	if ((unsigned int)index >= headerSize / 4) {
		streamReader_close(&fileReader);
		return 0;
	}

	streamReader_seek(&fileReader, index * 4);
	streamReader_get(&fileReader, &offToData, 4);
	offToData = convertDWFromLE(offToData);
	streamReader_seek(&fileReader, offToData);
	streamReader_get(&fileReader, &dataSize, 4);
	dataSize = convertDWFromLE(dataSize);
	streamReader_close(&fileReader);

	return (dataSize);
}

void HQR_Expand(int decompressedSize, unsigned char *destination, unsigned char *source) {
	char loop;
	char indic;
	unsigned char *jumpBack;
	int size;
	unsigned short int temp;
	int i;

	do {
		loop = 8;
		indic = *(source++);
		do {
			if (!(indic & 1)) {
				temp = (*(source + 1)) * 256 + *(source);
				source += 2;
				size = temp & 0x0F;
				size += 2;
				decompressedSize -= size;
				jumpBack = destination - (temp >> 4) - 1;
				for (i = 0; i < size; i++) {
					*(destination++) = *(jumpBack++);
				}
				if (decompressedSize <= 0)
					return;
				loop--;
			} else {
				*(destination++) = *(source++);
				loop--;
				decompressedSize--;
				if (decompressedSize <= 0)
					return;
			}
			indic >>= 1;
		} while (loop);
	} while (decompressedSize);
	return;
}
