#include "HQRlib.h"

#include "type.h"
#include "streamReader.h"

#ifndef PCLIKE 
#include <shinobi.h>
#endif

streamReader fileReader;

int HQR_File_checkIfFileExist(char *fileName)
{
#ifdef PCLIKE
    FILE *fileHandle;

    fileHandle = fopen(fileName, "rb");

    if (fileHandle)
	{
        fclose(fileHandle);
        return(1);
	}

    return (0);
#else
    GDFS fileHandle;

    fileHandle = gdFsOpen(fileName,NULL);

    if (fileHandle)
	{
        gdFsClose(fileHandle);
        return(1);
	}

    return (0);
#endif
}

#ifdef PCLIKE 
FILE* HQR_File_OpenRead(char *fileName)
{
    FILE *fileHandle;

    if (!fileName)
	return (NULL);

    fileHandle = fopen(fileName, "rb");

    if (!fileHandle)
	{
	    printf("%s can't be found !\n", fileName);
	}

    return (fileHandle);
}
#else
// DC version
GDFS HQR_File_OpenRead(char *fileName)
{
	/*GDFS fs;
    FILE *fileHandle;

    if (!fileName)
	{
		return(NULL);
	}

	fs = gdFsOpen(fileName, NULL);

	return(fs);*/
	return(NULL);
}
#endif

#ifdef PCLIKE 
int HQR_File_Read(FILE * resourceFile, char *ptr, int length)
{
    if (!resourceFile)
	return (1);

    fread((char *) ptr, length, 1, resourceFile);

    return (0);
}
#else
//DC
int HQR_File_Read(GDFS resourceFile, char *ptr, int length)
{
	unsigned char buffer[2048];
	int numSectors;
	int lastSectorLength;

	numSectors = length/2048;
	lastSectorLength = length%2048;

	if(numSectors)
	{
		gdFsRead(resourceFile, numSectors, ptr);

		ptr+= 2048*numSectors;
	}

	// read last sector

	gdFsRead(resourceFile, 1, buffer);
	memcpy(ptr,buffer,lastSectorLength);

    return (0);
}
#endif

#ifdef PCLIKE 
void HQR_File_Close(FILE * resourceFile)
{
    fclose(resourceFile);
}
#else
//DC
void HQR_File_Close(GDFS resourceFile)
{
	gdFsClose(resourceFile);
}
#endif

void HQR_Destroy_Ressource(hqr_entry* resource)
{
    Free(resource->ptr);
    Free(resource);
}

hqr_entry* HQR_Init_Ressource(char *fileName, int sizeOfBuffer, int numOfEntriesInBuffer)
{
    hqr_entry *hqr_ptr;
    unsigned char *dataPtr;

    if( !HQR_File_checkIfFileExist( fileName ) )
    	return (NULL);

    hqr_ptr = (hqr_entry *) Malloc(numOfEntriesInBuffer * sizeof(subHqr) + sizeof(hqr_entry));

    if (!hqr_ptr)
	    return (NULL);

    dataPtr = (unsigned char *) Malloc(sizeOfBuffer + 500);

    if (!dataPtr)
	    return (NULL);

    strcpy(hqr_ptr->fileName, fileName);
	hqr_ptr->preloadedResource = false;

    hqr_ptr->size1 = sizeOfBuffer;
    hqr_ptr->remainingSize = sizeOfBuffer;
    hqr_ptr->numEntriesMax = numOfEntriesInBuffer;
    hqr_ptr->numCurrentlyUsedEntries = 0;
    hqr_ptr->ptr = dataPtr;

    return (hqr_ptr);
}

long int getFileSize(char* resourceName)
{
#ifdef PCLIKE
	FILE* fHandle;
	int size;

	fHandle = fopen(resourceName, "rb");
	fseek(fHandle,0L,SEEK_END);
	size = ftell(fHandle);
	fclose(fHandle);
	return size;
#else
	GDFS fs;
	signed long int length;
	fs = gdFsOpen(resourceName, NULL);
	gdFsGetFileSize( fs, &length);
	gdFsClose(fs);
	return length;
#endif
}

void fileRead(char* resourceName, long int size, unsigned char* buffer)
{
#ifdef PCLIKE
	FILE* fHandle;
	fHandle = fopen(resourceName, "rb");
	fread(buffer,size,1,fHandle);
	fclose(fHandle);
#else
	if(!streamReader_open( &fileReader, (int8*) resourceName ))
		return;

	streamReader_get( &fileReader, buffer, size );

	streamReader_close( &fileReader );

#endif
}

hqr_entry* HQR_Init_RessourcePreload(char *resourceName)
{
    hqr_entry *hqr_ptr;
    unsigned char **ptrTable;
	int fileSize;
	unsigned char* buffer;
	int numEntries;
	int i;

    if( !HQR_File_checkIfFileExist( resourceName ) )
    	return (NULL);

    hqr_ptr = (hqr_entry *) Malloc(sizeof(hqr_entry));

    if (!hqr_ptr)
	    return (NULL);

	fileSize = getFileSize( resourceName );
	buffer = (unsigned char*)Malloc(fileSize);
	fileRead(resourceName,fileSize,buffer);

	numEntries = (*(unsigned int*)buffer)/4; // always alligned

	ptrTable = (unsigned char**)Malloc(numEntries*sizeof(unsigned char*));
	hqr_ptr->sizeArray = (int*)Malloc(numEntries*sizeof(int));

	for(i=0;i<numEntries-1;i++)
	{
		int offsetToData;
		int dataSize;
		int compressedSize;
		short int mode;

		offsetToData = *(unsigned int*)(buffer+4*i); // always alligned

		dataSize = READ_LE_U32(buffer+offsetToData);
		compressedSize = READ_LE_U32(buffer+offsetToData+4);
		mode = READ_LE_U16(buffer+offsetToData+8);

		ptrTable[i] = (unsigned char*) Malloc(dataSize);
		hqr_ptr->sizeArray[i] = dataSize;

		if(mode == 0) // uncompressed data
		{			
			memcpy(ptrTable[i], buffer+offsetToData+10,dataSize);
		}
		else
		if(mode == 1) // compressed data
		{
			HQR_Expand(dataSize, ptrTable[i], buffer+offsetToData+10);
		}
	}

	free(buffer);

    strcpy(hqr_ptr->fileName, resourceName);
	hqr_ptr->preloadedResource = true;

	hqr_ptr->ptrArray=ptrTable;

    return (hqr_ptr);
}

int Load_HQR(char *resourceName, unsigned char* ptr, int imageNumber)
{
    int headerSize;
    int offToImage;
    int dataSize;
    int compressedSize;
    short int mode;

	if(!streamReader_open( &fileReader, (int8*) resourceName ))
		return 0;

	streamReader_get( &fileReader, &headerSize, 4 );

	if(imageNumber >= headerSize /4)
	{
		streamReader_close( &fileReader );
		return 0;
	}

	streamReader_seek( &fileReader, imageNumber * 4 );
	streamReader_get( &fileReader, &offToImage, 4 );

	streamReader_seek( &fileReader, offToImage );
	streamReader_get( &fileReader, &dataSize , 4 );
	streamReader_get( &fileReader, &compressedSize , 4 );
	streamReader_get( &fileReader, &mode , 2 );

	if( mode <= 0 )
	{
		streamReader_get( &fileReader, ptr, dataSize );
	}
	else
	if( mode == 1 )
	{
		unsigned char* compressedDataPtr;

		compressedDataPtr = ptr + dataSize - compressedSize + 500;

		streamReader_get( &fileReader, compressedDataPtr, compressedSize);
		HQR_Expand(dataSize, (unsigned char*)ptr, compressedDataPtr);
	}

    streamReader_close( &fileReader );

    return (dataSize);
}

int HQR_GetNumEntry(char* fileName)
{
#ifdef PCLIKE 
    FILE *resourceFile;
#else
	//DC
	GDFS resourceFile;
#endif
 //   resourceFile = HQR_File_OpenRead(fileName);
    int headerSize;

    if (!resourceFile)
	return (0);

    HQR_File_Read(resourceFile, (char *) &headerSize, 4);

    return(headerSize/4);
}

unsigned char *LoadMalloc_HQR(char *fileName, short int imageNumber)
{
#ifdef PCLIKE 
    FILE *resourceFile;
    int headerSize;
    int offToImage;
    int dataSize;
    int compressedSize;
    short int mode;
    unsigned char *ptr;

    resourceFile = HQR_File_OpenRead(fileName);

    if (!resourceFile)
	return (0);

    HQR_File_Read(resourceFile, (char *) &headerSize, 4);

    if (imageNumber >= headerSize / 4)
	{
	    HQR_File_Close(resourceFile);
	    return (0);
	}

    fseek(resourceFile, imageNumber * 4, SEEK_SET);
    HQR_File_Read(resourceFile, (char *) &offToImage, 4);

    fseek(resourceFile, offToImage, SEEK_SET);
    HQR_File_Read(resourceFile, (char *) &dataSize, 4);
    HQR_File_Read(resourceFile, (char *) &compressedSize, 4);
    HQR_File_Read(resourceFile, (char *) &mode, 2);

    ptr = (unsigned char *) Malloc(dataSize + 500);
    if (mode <= 0)		// uncompressed Image
	{
	    HQR_File_Read(resourceFile, (char *) ptr, dataSize);
	}
    else
	{
	    if (mode == 1)	// compressed Image
		{
		    HQR_File_Read(resourceFile, (char *) (ptr + dataSize - compressedSize + 500),
				     compressedSize);
		    HQR_Expand(dataSize, ptr, (ptr + dataSize - compressedSize + 500));
		}
	    else
		{
		    Free(ptr);
		    HQR_File_Close(resourceFile);
		    return (0);
		}
	}

    HQR_File_Close(resourceFile);

    return (ptr);
#else
	//DC
	unsigned char* ptr;
	HQRM_Load(fileName,imageNumber, &ptr);

	return (ptr);
#endif
}

subHqr *findSubHqr(int arg_0, int arg_4, subHqr * arg_8)
{
    subHqr *temp;
    int i;

    if (arg_4 == 0)
	return (0);

    temp = arg_8;

    for (i = 0; i < arg_4; i++)
	{
	    if (temp->index == arg_0)
		return (temp);

	    temp++;
	}

    return (0);

}

unsigned char *HQR_Get(hqr_entry * hqrPtr, short int arg_4)
{
    int headerSize;
    int offToData;
    int dataSize;
    int compressedSize;
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
	return (0);

	if(hqrPtr->preloadedResource)
	{
		return hqrPtr->ptrArray[arg_4];
	}

    hqrdata = (subHqr *) ((unsigned char *) hqrPtr + sizeof(hqr_entry));

    hqrdataPtr = findSubHqr(arg_4, hqrPtr->numCurrentlyUsedEntries, hqrdata);

    if (hqrdataPtr)
	{
	    hqrdataPtr->lastAccessedTime = lba_time;
	    HQR_Flag = 0;
	    return (hqrdataPtr->offFromPtr + hqrPtr->ptr);
	}
   // printf ("Need to load: %s-%d\n", hqrPtr->fileName, arg_4);

    Size_HQR(hqrPtr->fileName, arg_4);

	if(!streamReader_open( &fileReader, (int8*) hqrPtr->fileName ))
		return 0;

	streamReader_get( &fileReader, &headerSize, 4 );

	if( arg_4 >= headerSize / 4 )
	{
		streamReader_close( &fileReader );
		return 0;
	}

	streamReader_seek( &fileReader, arg_4 * 4 );
	streamReader_get( &fileReader, &offToData, 4 );

	streamReader_seek( &fileReader, offToData );
	streamReader_get( &fileReader, &dataSize, 4) ;
	streamReader_get( &fileReader, &compressedSize, 4) ;
	streamReader_get( &fileReader, &mode, 2) ;

    dataSize2 = dataSize;

   // ici, test sur la taille de dataSize

    ltime = lba_time;

    while (dataSize2 >= hqrPtr->remainingSize || hqrPtr->numCurrentlyUsedEntries >= hqrPtr->numEntriesMax)	// pour retirer les elements les plus vieux jusqu'a ce qu'on ai de la place
	{
	    var_4 = 0;
	    temp2 = 0;

	    for (i = 0; i < hqrPtr->numCurrentlyUsedEntries; i++)
		{
		    if (temp2 <= ltime - hqrdata[i].lastAccessedTime)
			{
			    temp2 = ltime - hqrdata[var_4].lastAccessedTime;
			    var_4 = i;
			}
		}

	    HQR_RemoveEntryFromHQR(hqrPtr, var_4);

	}

    ptr = hqrPtr->ptr + hqrPtr->size1 - hqrPtr->remainingSize;

    if (mode <= 0)		// uncompressed
	{
		streamReader_get( &fileReader, ptr, dataSize );
	}
    else
	if (mode == 1)	// compressed
	{
		streamReader_get( &fileReader, ptr + dataSize - compressedSize + 500, compressedSize );
		HQR_Expand(dataSize, ptr, (ptr + dataSize - compressedSize + 500));
	}

	streamReader_close( &fileReader );

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

unsigned char *HQR_GetCopy(hqr_entry * hqrPtr, short int arg_4)
{
	unsigned char* newPtr;

	if(hqrPtr->preloadedResource)
	{
		newPtr = (unsigned char*)Malloc(hqrPtr->sizeArray[arg_4]);
		memcpy(newPtr, hqrPtr->ptrArray[arg_4],hqrPtr->sizeArray[arg_4]);
		return newPtr;
	}

	return NULL;
}

void HQR_Reset_Ressource(hqr_entry * ptr)
{
    ptr->remainingSize = ptr->size1;
    ptr->numCurrentlyUsedEntries = 0;
}

int HQR_RemoveEntryFromHQR(hqr_entry * hqrPtr, int var)
{
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

    subPtr = (subHqr *) (hqrPtr + sizeof(hqr_entry) + var * sizeof(subHqr));

    retVal = subPtr->size;

    if (hqrPtr->numCurrentlyUsedEntries - 1 > lvar)
	{
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

int HQRM_Load(char *fileName, short int arg_4, unsigned char ** ptr)	// recheck
{
    int headerSize;
    int offToData;
    int dataSize;
    int compressedSize;
    short int mode;
    unsigned char *temp;

	if(!streamReader_open( &fileReader, (int8*)fileName ))
		return(-1);

	streamReader_get( &fileReader, &headerSize, 4);
	
	if(arg_4 >= headerSize /4)
	{
		streamReader_close( &fileReader );
		return (-1);
	}

	streamReader_seek( &fileReader, arg_4 * 4 );
	streamReader_get( &fileReader, &offToData, 4 );

	streamReader_seek( &fileReader, offToData );
	streamReader_get( &fileReader, &dataSize, 4 );
	streamReader_get( &fileReader, &compressedSize, 4 );
	streamReader_get( &fileReader, &mode , 2 );

	(*ptr) = (unsigned char*)Malloc(dataSize);

	if(!(*ptr))
	{
		streamReader_close(&fileReader);
		return(-1);
	}

	if(mode == 0)
	{
		streamReader_get( &fileReader, (*ptr), dataSize );
	}
	else
	if(mode == 1)
	{
		temp = (unsigned char *) Malloc(compressedSize);

		streamReader_get( &fileReader, temp, compressedSize );
		HQR_Expand( dataSize, *ptr, temp );
		Free(temp);
	}

	streamReader_close( &fileReader );

    return (dataSize);
}

int Size_HQR(char *fileName, int index)
{
    int headerSize;
    int dataSize;
    int offToData;

	if(!streamReader_open( &fileReader, (int8*)fileName ))
	{
		return 0;
	}

	streamReader_get( &fileReader, &headerSize, 4 );

	if(index >= headerSize /4 )
	{
		streamReader_close( &fileReader );
		return 0;
	}

	streamReader_seek( &fileReader, index * 4 );
	streamReader_get( &fileReader, &offToData, 4 );
	streamReader_seek( &fileReader, offToData );
	streamReader_get( &fileReader, &dataSize, 4);
	streamReader_close( &fileReader );

    return (dataSize);
}
