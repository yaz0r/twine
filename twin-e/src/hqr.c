#include "lba.h"

hqr_entry *LBA_engine::load_hqr(char *fileName, int a, int b)
{
    FILE *hqr_file;
    hqr_entry *hqr_ptr;
    byte *dataPtr;

    hqr_file = fileOpen(fileName);
    if (!hqr_file)
	return (0);

    hqr_ptr = (hqr_entry *) malloc(b * sizeof(subHqr) + sizeof(hqr_entry));
    if (!hqr_ptr)
	return (0);

    dataPtr = (byte *) malloc(a + 500);
    if (!dataPtr)
	return (0);

    strcpy(hqr_ptr->fileName, fileName);

    hqr_ptr->size1 = a;
    hqr_ptr->remainingSize = a;
    hqr_ptr->b = b;
    hqr_ptr->unk = 0;
    hqr_ptr->ptr = dataPtr;

    fclose(hqr_file);

    return (hqr_ptr);
}

int LBA_engine::loadImageToPtr(char *resourceName, byte * ptr, int imageNumber)	// en fait, c'est pas vraiment une image, ca peut etre n'importe quelle data...
{
    FILE *resourceFile;
    int headerSize;
    int offToImage;
    int dataSize;
    int compressedSize;
    short int mode;

    resourceFile = openResource(resourceName);

    if (!resourceFile)
	return (0);

    readResourceData(resourceFile, (char *) &headerSize, 4);

    if (imageNumber >= headerSize / 4)
	{
	    closeResource(resourceFile);
	    return (0);
	}

    fseek(resourceFile, imageNumber * 4, SEEK_SET);
    readResourceData(resourceFile, (char *) &offToImage, 4);

    fseek(resourceFile, offToImage, SEEK_SET);
    readResourceData(resourceFile, (char *) &dataSize, 4);
    readResourceData(resourceFile, (char *) &compressedSize, 4);
    readResourceData(resourceFile, (char *) &mode, 2);

    if (mode <= 0)		// uncompressed Image
	{
	    readResourceData(resourceFile, (char *) ptr, dataSize);
	}
    else
	{
	    if (mode == 1)	// compressed Image
		{
		    readResourceData(resourceFile, (char *) (ptr + dataSize - compressedSize + 500),
				     compressedSize);
		    decompress(dataSize, ptr, (ptr + dataSize - compressedSize + 500));
		}
	    else
		{
		    return (0);
		}
	}

    return (dataSize);
}

byte *LBA_engine::loadImageToMemory(char *fileName, short int imageNumber)
{
    FILE *resourceFile;
    int headerSize;
    int offToImage;
    int dataSize;
    int compressedSize;
    short int mode;
    byte *ptr;

    resourceFile = openResource(fileName);

    if (!resourceFile)
	return (0);

    readResourceData(resourceFile, (char *) &headerSize, 4);

    if (imageNumber >= headerSize / 4)
	{
	    closeResource(resourceFile);
	    return (0);
	}

    fseek(resourceFile, imageNumber * 4, SEEK_SET);
    readResourceData(resourceFile, (char *) &offToImage, 4);

    fseek(resourceFile, offToImage, SEEK_SET);
    readResourceData(resourceFile, (char *) &dataSize, 4);
    readResourceData(resourceFile, (char *) &compressedSize, 4);
    readResourceData(resourceFile, (char *) &mode, 2);

    ptr = (byte *) malloc(dataSize + 500);
    if (mode <= 0)		// uncompressed Image
	{
	    readResourceData(resourceFile, (char *) ptr, dataSize);
	}
    else
	{
	    if (mode == 1)	// compressed Image
		{
		    readResourceData(resourceFile, (char *) (ptr + dataSize - compressedSize + 500),
				     compressedSize);
		    decompress(dataSize, ptr, (ptr + dataSize - compressedSize + 500));
		}
	    else
		{
		    free(ptr);
		    closeResource(resourceFile);
		    return (0);
		}
	}

    closeResource(resourceFile);

    return (ptr);
}

unsigned char *LBA_engine::getHqrdataPtr(hqr_entry * hqrPtr, short int arg_4)
{

    FILE *fin;
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

    hqrdata = (subHqr *) ((unsigned char *) hqrPtr + sizeof(hqr_entry));

    hqrdataPtr = findSubHqr(arg_4, hqrPtr->unk, hqrdata);

    if (hqrdataPtr)
	{
	    hqrdataPtr->lastAccessedTime = time;
	    drawInventoryVar = 0;
	    return (hqrdataPtr->offFromPtr + hqrPtr->ptr);
	}
   // printf ("Need to load: %s-%d\n", hqrPtr->fileName, arg_4);

    prepareResource(hqrPtr->fileName, arg_4);

    fin = openResource(hqrPtr->fileName);

    if (!fin)
	return (0);

    readResourceData(fin, (char *) &headerSize, 4);

    if (arg_4 >= headerSize / 4)
	{
	    closeResource(fin);
	    return (0);
	}

    fseek(fin, arg_4 * 4, SEEK_SET);
    readResourceData(fin, (char *) &offToData, 4);

    fseek(fin, offToData, SEEK_SET);
    readResourceData(fin, (char *) &dataSize, 4);
    readResourceData(fin, (char *) &compressedSize, 4);
    readResourceData(fin, (char *) &mode, 2);

    dataSize2 = dataSize;

   // ici, test sur la taille de dataSize

    ltime = time;

    while (dataSize2 >= hqrPtr->remainingSize || hqrPtr->unk >= hqrPtr->b)	// pour retirer les elements les plus vieux jusqu'a ce qu'on ai de la place
	{
	    var_4 = 0;
	    temp2 = 0;

	    for (i = 0; i < hqrPtr->unk; i++)
		{
		    if (temp2 <= ltime - hqrdata[i].lastAccessedTime)
			{
			    temp2 = ltime - hqrdata[var_4].lastAccessedTime;
			    var_4 = i;
			}
		}

	    drawInventory2(hqrPtr, var_4);

	}

    ptr = hqrPtr->ptr + hqrPtr->size1 - hqrPtr->remainingSize;

    if (mode <= 0)		// uncompressed
	{
	    readResourceData(fin, (char *) ptr, dataSize);
	}
    else
	{
	    if (mode == 1)	// compressed
		{
		    readResourceData(fin, (char *) (ptr + dataSize - compressedSize + 500),
				     compressedSize);
		    decompress(dataSize, ptr, (ptr + dataSize - compressedSize + 500));
		}

	    else
		{
		    closeResource(fin);
		    return (0);
		}
	}

    closeResource(fin);

    hqrdataPtr = &hqrdata[hqrPtr->unk];

    hqrdataPtr->index = arg_4;
    drawInventoryVar = 1;
    hqrdataPtr->lastAccessedTime = time;
    hqrdataPtr->offFromPtr = hqrPtr->size1 - hqrPtr->remainingSize;
    hqrdataPtr->size = dataSize2;

    hqrPtr->unk++;
    hqrPtr->remainingSize -= dataSize2;

    return (ptr);

}

byte LBA_engine::allocHQMemory(int size)
{
    if (HQMemory)
	return (0);

    HQMemory = (byte *) malloc(size + 500);

    if (!HQMemory)
	return (0);

    HQMemorySize = size;
    HQMemorySize2 = size;
    HQMemory2 = HQMemory;
    return (1);
}
