/***************************************************************************
                          vox.cpp  -  description
                             -------------------
    begin                : Tue Feb 12 2002
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

int LBA_engine::setVoxFileAtDigit(int index)
{
  int temp=0;
	int temp2=0;
	int nEntry;
	int temp3=0;

//	int ptrCurrentEntry;
//	int ptrNextEntry;

//	short int *localTextBuf=(short int*)bufText;
	short int *localOrderBuf=(short int*)bufOrder;

//	int localIndex=index;

	if(!voxFileHandle)
		return(0);

	nEntry=numTextEntry;

  goto e2;	
	
e1:

	temp2=0;
	temp2=*(localOrderBuf);
	localOrderBuf++;
	if(temp2==index)
		goto e3;
	temp++;

e2:
	
	temp2=0;
	temp2=nEntry;
	if(temp<temp2)
		goto e1;
 	temp=-1;

e3:
	
	if(temp!=-1)
		goto ex;
	temp=0;
	numTextEntry=nEntry;
	return(0);

ex:

	temp3=bufMemoSeek[temp*4];

	if(temp3==0)
		return(0);
	
	fseek(voxFileHandle,temp3,0);
	readBufferSpeak(voxFileHandle);
			
	return(1);
}

int LBA_engine::loadVox(int index)
{
	return(0);
}