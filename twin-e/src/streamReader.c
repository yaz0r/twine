#include "streamReader.h"

bool streamReader_open(streamReader* pThis, const int8* fileName)
{
#ifdef PCLIKE
	pThis->fileHandle = fopen((const char*)fileName,"rb");
#else
	pThis->fileHandle = gdFsOpen((char*)fileName, NULL);
#endif

	if(pThis->fileHandle)
	{
		pThis->currentSector = 0;
		streamReader_feedBuffer(pThis);
		return true;
	}
	else
	{
		return false;
	}
}

void streamReader_feedBuffer(streamReader* pThis)
{
#ifdef PCLIKE
	fread(pThis->buffer, BUFFER_SIZE, 1, pThis->fileHandle);
#else
	gdFsRead(pThis->fileHandle, NUM_SECTOR_IN_BUFFER, pThis->buffer);
#endif
	pThis->positionInBuffer = 0;
}

/*uint8 streamReader_getU8(streamReader* pThis)
{
	assert( pThis->fileHandle );
}

uint16 streamReader_getU16(streamReader* pThis)
{
	assert( pThis->fileHandle );
}

uint32 streamReader_getU32(streamReader* pThis)
{
	assert( pThis->fileHandle );
}*/

void streamReader_get(streamReader* pThis, void* destPtr, uint32 size)
{
	if(BUFFER_SIZE-pThis->positionInBuffer >= size)
	{
		memcpy(destPtr,&pThis->buffer[pThis->positionInBuffer],size);
		pThis->positionInBuffer += size;
	}
	else
	{
		// buffer isn't filled enough...
		char* tempPtr = (char*)destPtr;

		// feed what we can:
		memcpy( tempPtr, &pThis->buffer[pThis->positionInBuffer], BUFFER_SIZE-pThis->positionInBuffer );
		tempPtr+=BUFFER_SIZE-pThis->positionInBuffer;
		size-=BUFFER_SIZE-pThis->positionInBuffer;

		// feed the rest
		do
		{
			pThis->currentSector++;
			streamReader_feedBuffer( pThis );

			if(size>=BUFFER_SIZE)
			{
				memcpy(tempPtr, pThis->buffer, BUFFER_SIZE);
				tempPtr+=BUFFER_SIZE;
				size-=BUFFER_SIZE;
			}
			else
			{
				memcpy(tempPtr, pThis->buffer, size);
				pThis->positionInBuffer += size;
				size=0;
			}
		}while(size>0);
	}
}

void streamReader_seek(streamReader* pThis, uint32 seekPosition)
{
	uint32 sectorToSeek;

	sectorToSeek = seekPosition / 2048;

/*	if((sectorToSeek >= pThis->currentSector) && (sectorToSeek < pThis->currentSector + NUM_SECTOR_IN_BUFFER ))// already at the good sector
	{
		pThis->positionInBuffer = (seekPosition - (sectorToSeek*2048));
	}
	else
	{
	} */

#ifdef PCLIKE
	fseek(pThis->fileHandle, sectorToSeek * 2048, SEEK_SET );
#else
	gdFsSeek(pThis->fileHandle, sectorToSeek, GDD_SEEK_SET );
#endif

	pThis->currentSector = sectorToSeek;
	streamReader_feedBuffer( pThis );
	pThis->positionInBuffer = (seekPosition - (sectorToSeek*2048));
}

void streamReader_close(streamReader* pThis)
{
	if(pThis->fileHandle)
	{
#ifdef PCLIKE
		fclose( pThis->fileHandle );
#else
		gdFsClose( pThis->fileHandle );
#endif
	}
}


