
/***************************************************************************
                          resource.cpp  -  description
                             -------------------
    begin                : Sun Mar 17 2002
    copyright            : (C) 2002 by Yaz0r
    email                : yaz0r@yaz0r.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "lba.h"

int
  LBA_engine::loadDataFileToPtr(char *fileName, short int arg_4, byte ** ptr)	// recheck
{
    FILE *resourceFile;
    int headerSize;
    int offToData;
    int dataSize;
    int compressedSize;
    short int mode;
    unsigned char *temp;

    resourceFile = openResource(fileName);

    if (!resourceFile)
	return (-1);

    readResourceData(resourceFile, (char *) &headerSize, 4);

    if (arg_4 >= headerSize / 4) {
	closeResource(resourceFile);
	return (-1);
    }

    fseek(resourceFile, arg_4 * 4, SEEK_SET);
    readResourceData(resourceFile, (char *) &offToData, 4);

    fseek(resourceFile, offToData, SEEK_SET);
    readResourceData(resourceFile, (char *) &dataSize, 4);
    readResourceData(resourceFile, (char *) &compressedSize, 4);
    readResourceData(resourceFile, (char *) &mode, 2);

    if (!allocHQRMemory(dataSize, ptr))
	return (-1);

    if (mode <= 0)		// uncompressed
    {
	readResourceData(resourceFile, (char *) *ptr, dataSize);
    } else {
	if (mode == 1)		// compressed
	{
	    temp = (unsigned char *) malloc(compressedSize);

	    readResourceData(resourceFile, (char *) temp, compressedSize);
	    decompress(dataSize, *ptr, temp);
	    free(temp);
	} else {
	   // implementer qq chose de manquant là...
	}
    }

    fclose(resourceFile);
    return (dataSize);
}

/*
 * should be getResourceSize 
 */
int LBA_engine::prepareResource(char *fileName, int index)
{
    FILE *file;
    int headerSize;
    int dataSize;
    int offToData;

    file = openResource((char *) fileName);

    if (file == NULL)
	return (0);

    readResourceData(file, (char *) &headerSize, 4);

    if (index >= headerSize / 4) {
	closeResource(file);
	return (0);
    }

    fseek(file, index * 4, SEEK_SET);
    readResourceData(file, (char *) &offToData, 4);

    fseek(file, offToData, SEEK_SET);
    readResourceData(file, (char *) &dataSize, 4);

    fclose(file);

    return (dataSize);
}
