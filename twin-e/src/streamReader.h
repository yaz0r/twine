#ifndef _STREAMREADER_
#define _STREAMREADER_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "lba.h"

#ifndef PCLIKE
#ifndef PS2
#include <shinobi.h>
#endif
#endif

#define NUM_SECTOR_IN_BUFFER (3)
#define BUFFER_SIZE (2048*NUM_SECTOR_IN_BUFFER)

struct streamReader
{
#ifdef PCLIKE
	FILE* fileHandle;
#else
#ifndef PS2
	// DC
	GDFS fileHandle;
#endif
#ifdef PS2
	FILE* fileHandle;
#endif
#endif

	unsigned char buffer[BUFFER_SIZE];
	unsigned long int positionInBuffer;
	unsigned long int currentSector;
};

typedef struct streamReader streamReader;

bool streamReader_open(streamReader* pThis, const int8* fileName);
void streamReader_close(streamReader* pThis);
void streamReader_feedBuffer(streamReader* pThis);

uint8 streamReader_getU8(streamReader* pThis);
uint16 streamReader_getU16(streamReader* pThis);
uint32 streamReader_getU32(streamReader* pThis);

void streamReader_get(streamReader* pThis, void* destPtr, uint32 size);
void streamReader_seek(streamReader* pThis, uint32 seekPosition);

#endif // _STREAMREADER_

