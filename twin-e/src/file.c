#include "lba.h"

int checkIfFileExist(char *fileName)
{
    FILE *fileHandle;

    fileHandle = fopen(fileName, "rb");

    if (fileHandle)
	{
        fclose(fileHandle);
        return(1);
	}

    return (0);
}

FILE *OpenRead(char *fileName)
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

int Read(FILE * resourceFile, char *ptr, int length)
{
    if (!resourceFile)
	return (1);

    fread((char *) ptr, length, 1, resourceFile);

    return (0);

}

void Close(FILE * resourceFile)
{
    fclose(resourceFile);
}

void dumpFile(char *name, char *ptr, int size)
{
    FILE *file;

    file = fopen(name, "wb+");
    fwrite(ptr, size, 1, file);

    fclose(file);
}
