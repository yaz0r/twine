#include "lba.h"

#ifdef MEM_DEBUG

#define NUNMEMORYBLOCKS 10000

int currentAllocatedMemory = 0;
int staticMemoryUsage = 0;

struct memoryBlockStruct
{
	void* ptr;
	int size;
	bool used;
};

memoryBlockStruct memoryBlockArray[NUNMEMORYBLOCKS];

void initMemorySystem()
{
	int i;

	for(i=0;i<NUNMEMORYBLOCKS;i++)
	{
		memoryBlockArray[i].used = false;
	}
}

byte *Malloc(int size)
{
	int i;
	byte *ptr;

	for(i=0;i<NUNMEMORYBLOCKS;i++)
	{
		if(memoryBlockArray[i].used == false)
			break;
	}

	if(i==NUNMEMORYBLOCKS && memoryBlockArray[NUNMEMORYBLOCKS].used)
	{
		printf("All blocks are used !\n");
		assert(true);
	}

    ptr = (byte *) malloc(size);

    if (!ptr)
	{
	    printf("Can't alloc %d!\n", size);
	    exit(1);
	}

	memoryBlockArray[i].ptr = ptr;
	memoryBlockArray[i].size = size;
	memoryBlockArray[i].used = true;

	currentAllocatedMemory += size;

    return (ptr);
}

void Free(void* ptr)
{
	int i;

	for(i=0;i<NUNMEMORYBLOCKS;i++)
	{
		if(memoryBlockArray[i].used == true && memoryBlockArray[i].ptr == ptr)
		{
			free(memoryBlockArray[i].ptr);
			currentAllocatedMemory -= memoryBlockArray[i].size;
			memoryBlockArray[i].used = false;
			break;
		}
	}
}

#endif