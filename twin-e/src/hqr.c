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
