#ifndef _HQRLIB_
#define _HQRLIB_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "HQMLib.h"

#ifdef _DEBUG
    #define todo(string) printf("TODO %s:%d %s\n", __FILE__, __LINE__, string)
#else // _DEBUG
    #define todo(string)
#endif // _DEBUG

#ifdef _DEBUG
    #define assert_ptr(ptr) assert((int)ptr!=0xCDCDCDCD)
#else // _DEBUG
    #define assert_ptr(ptr)
#endif // _DEBUG

struct hqr_entry
{
	char fileName[12];
	bool preloadedResource;

	// non preloaded data
	int size1;
	int remainingSize;
	short int numEntriesMax;
	short int numCurrentlyUsedEntries;
	unsigned char *ptr;

	// preloaded data
	unsigned char** ptrArray;
	int* sizeArray;
	
};

struct subHqr
{
    short int index;
    unsigned int offFromPtr;
    unsigned int size;
    int lastAccessedTime;
};

extern short int HQR_Flag;
extern volatile int time;

void HQR_Destroy_Ressource(hqr_entry* resource);
int Load_HQR(char *resourceName, unsigned char *ptr, int imageNumber);
hqr_entry* HQR_Init_Ressource(char *fileName, int sizeOfBuffer, int numOfEntriesInBuffer);
int HQR_RemoveEntryFromHQR(hqr_entry * hqrPtr, int var);
unsigned char *HQR_Get(hqr_entry * hqrPtr, short int arg_4);
unsigned char *LoadMalloc_HQR(char *fileName, short int imageNumber);
int Size_HQR(char *fileName, int index);
int HQRM_Load(char *fileName, short int arg_4, unsigned char ** ptr);
void HQR_Reset_Ressource(hqr_entry * ptr);
void HQR_Expand(int decompressedSize, unsigned char *destination, unsigned char *source);
int HQR_GetNumEntry(char* fileName);
hqr_entry* HQR_Init_RessourcePreload(char *resourceName);
unsigned char *HQR_GetCopy(hqr_entry * hqrPtr, short int arg_4);

#endif

