/***************************************************************************
                          body.cpp  -  description
                             -------------------
    begin                : Fri May 10 2002
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

int SearchBody(int bodyNum, int actorNum)	// should be something like initBody
{
    actor *lactor;
    unsigned char *bodyPtr;
    unsigned char var1;
    unsigned char var2;
    unsigned char *bodyPtr2;
    unsigned char *bodyPtr3;
    unsigned char *bodyPtr4;
    short int *bodyPtr5;
    short int flag;
    int index;

    lactor = &actors[actorNum];
    bodyPtr = lactor->entityDataPtr;

    do
	{

	    var1 = *(bodyPtr++);

	    if (var1 == 0xFF)
		return (-1);

	    bodyPtr2 = bodyPtr + 1;

	    if (var1 == 1)
		{
		    var2 = *(bodyPtr);

		    if (var2 == bodyNum)
			{
			    bodyPtr3 = bodyPtr2 + 1;
				flag = READ_LE_U16(bodyPtr3);

			    if (!(flag & 0x8000))
				{
#ifdef PRELOAD_ALL
					bodyPtrTab[currentPositionInBodyPtrTab] = HQR_GetCopy(HQR_Bodies, flag & 0xFFFF);
#else
				    HQRM_Load("body.hqr", flag & 0xFFFF, &bodyPtrTab[currentPositionInBodyPtrTab]);
#endif
				    if (!bodyPtrTab[currentPositionInBodyPtrTab])
					{
					    printf("Body.HQR in HQ_Mem\n");
					    exit(1);
					}
				    loadGfxSub(bodyPtrTab[currentPositionInBodyPtrTab]);
					WRITE_LE_U16( bodyPtr3, currentPositionInBodyPtrTab + 0x8000); // maintenant, on dit que c'est en memoire HQR
				    index = currentPositionInBodyPtrTab;
				    currentPositionInBodyPtrTab++;
				}
			    else
				{
				    flag &= 0x7FFF;
				    index = flag;
				}

			    bodyPtr3 += 2;
			    loadCostumeVar = -32000;

			    bodyPtr4 = bodyPtr3;
			    bodyPtr3++;

			    if (!*bodyPtr4)
				return (index);

			    bodyPtr4 = bodyPtr3;
			    bodyPtr3++;

			    if (*bodyPtr4 != 14)
				return (index);

			    bodyPtr5 = (short int *) bodyPtr3;

			    loadCostumeVar = READ_LE_U16(bodyPtr3); bodyPtr3+=2;	//X1 bottomLeft
			    loadCostumeVar2 = READ_LE_U16(bodyPtr3); bodyPtr3+=2;	//Z1
			    loadCostumeVar3 = READ_LE_U16(bodyPtr3); bodyPtr3+=2;	//Y1

			    loadCostumeVar4 = READ_LE_U16(bodyPtr3); bodyPtr3+=2;	//X2 topRight
			    loadCostumeVar5 = READ_LE_U16(bodyPtr3); bodyPtr3+=2;	//Z2
			    loadCostumeVar6 = READ_LE_U16(bodyPtr3); bodyPtr3+=2;	//Y2

			    return (index);

			}
		}

	    bodyPtr = *bodyPtr2 + bodyPtr2;

	}
    while (1);

}
