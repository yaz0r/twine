/***************************************************************************
                          3Dmodels.cpp  -  description
                             -------------------
    begin                : Tue Jun 4 2002
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

/*
 * That's the entry point of the 3D renderer 
 */

int AffObjetIso(int arg_0, int arg_4, int arg_8, int arg_C, int arg_10, int arg_14,
			      unsigned char *costumePtr)
{
    unsigned char *ptr;

    textWindowBottom = textWindowBottom;
    textWindowTop = textWindowTop;
    textWindowLeft = textWindowLeft;
    textWindowRight = textWindowRight;

   
        unsigned char* data;
        data = HQR_Get(InventoryObj,5);
   

//    int temp = renderer.AffObjetIso(arg_0, arg_4, arg_8, 0, 500, 0, data);
    int temp = AffObjetIso(arg_0, arg_4, arg_8, arg_C, arg_10, arg_14, costumePtr);

    renderLeft = renderer._renderLeft;
    renderRight = renderer._renderRight;
    renderTop = renderer._renderTop;
    renderBottom = renderer._renderBottom;

    return (temp);

    renderV1 = arg_C;
    renderV2 = arg_10;
    renderV3 = arg_14;

   /*
    * Those 4 vars are used to know the size of the rendered model. They are resetted to their
    * maximum value at the begining of the renderer 
    */

    renderLeft = 32767;
    renderTop = 32767;

    renderRight = -32767;
    renderBottom = -32767;

   /*
    * the use of isUsingOrhoProjection is still unknown 
    */

    if (renderer.isUsingOrhoProjection == 0)
	{
	    printf("Unimplemented AffObjetIso\n");
	    exit(1);
	}
    else
	{
	    renderV9 = arg_0;
	    renderV11 = arg_4;
	    renderV10 = arg_8;
	}

    numOfPrimitives = 0;	// we reset the number of primitives in the model

    renderTabEntryPtr = renderTab;	// we restart at the beginning of the renderTable

    costumeHeader = *(short int *) costumePtr;	// costumeHeader seems to contains important flags
   // for the rendering

    ptr = costumePtr + 16 + *(short int *) (costumePtr + 14);	// we jump after the header

    if (costumeHeader & 2)	// if animated
	return (AnimNuage(ptr));	// That's the mostly used renderer code
    else
	return (RotateNuage(ptr));	// unanimated models

}

int AnimNuage(unsigned char *costumePtr)
{
    unsigned int var1;
    unsigned char *ptr3;
    int *ptr4;
    int *ptr5;

    int coX;
    int coY;
    int coZ;

    unsigned char *ptr6;
    int eax;
    int edi;
    int temp;
    pointEntry *ptEntryPtr;
    pointTab *pointPtr;
    pointTab *pointPtr2;

    numOfPri1 = *(short int *) costumePtr;
    costumePtr += 2;

    pri1Ptr = costumePtr;

    var1 = numOfPri1 + numOfPri1 * 2;
    costumePtr = costumePtr + var1 * 2;

    numOfPri2 = *(short int *) costumePtr;	// that's the number of points in the model

    costumePtr += 2;		// we jump to the points data

    pri2Ptr2 = pri2Ptr = costumePtr;	// we save the point data ptr

    renderV19 = (unsigned char *) renderTab2;

    RotateGroupe(renderV3, renderV2, renderV1, (pointEntry *) pri2Ptr2);	// that's the process of
   // the "base point"

    pri2Ptr2 += 38;		// jump to the next point data

    ptEntryPtr = (pointEntry *) pri2Ptr2;	// that's the structure used to load the points

    if (numOfPri2 - 1 != 0)	// if there is points after the base point
	{
	    primitiveCounter = numOfPri2 - 1;	// Since the base point is already processed, we
	   // need to load numOfPri2-1 points
	    renderV19 = (unsigned char *) renderTab3;	// the use of renderTab3 is still unknown.
	   // It's a table with 36byte long element

	    do			// That's the loop that load and convert all the points in the
	       // model
		{
		    if (ptEntryPtr->flag == 0)
			{
			    RotateGroupe(ptEntryPtr->rotate3, ptEntryPtr->rotate2, ptEntryPtr->rotate1, ptEntryPtr);	// RotateGroupe 
			   // 
			   // 
			   // 
			   // 
			   // 
			   // 
			   // 
			   // 
			   // 
			   // 
			   // 
			   // 
			   // load 
			   // the 
			   // points, 
			   // rotate 
			   // them 
			   // and 
			   // store 
			   // them 
			   // in 
			   // renderTab5 
			   // (it 
			   // may 
			   // do 
			   // other 
			   // things 
			   // too..)
			}
		    else
			{
			    if (ptEntryPtr->flag == 1)
				{
				    TranslateGroupe(ptEntryPtr->rotate3, ptEntryPtr->rotate2,
					     ptEntryPtr->rotate1, ptEntryPtr);
				}
			}

		    renderV19 += 36;	// next entry in renderTab3

		    pri2Ptr2 += 38;	// next point data
		    ptEntryPtr = (pointEntry *) pri2Ptr2;	// we use the structure to load the point
		   // data
		}
	    while (--primitiveCounter);
	}

    primitiveCounter = numOfPri1;

    pointPtr2 = (pointTab *) renderTab5;
    pointPtr = renderTab6;

    if (renderer.isUsingOrhoProjection != 0)	// boucle de convertion des points de 3D (de renderTab5) à 2D (de
       // renderTab6)
	{
	    do
		{		// this loop convert the 3D points (renderTab5) into 2D points
		   // (renderTab6)
		    coX = pointPtr2->x + renderV9;
		    coY = pointPtr2->y + renderV11;
		    coZ = -(pointPtr2->z + renderV10);

		    pointPtr->x = (coX - coY) * 24 / 512 + renderer.setSomethingVar1;
		    pointPtr->y = (((coX + coY) * 12) - coZ * 30) / 512 + renderer.setSomethingVar2;
		    pointPtr->z = coZ - coX - coY;

		    if (pointPtr->x < renderLeft)	// this part setup the size of the rendered model
		       // (to be used by the main drawing function)
			renderLeft = pointPtr->x;
		    if (pointPtr->x > renderRight)
			renderRight = pointPtr->x;

		    if (pointPtr->y < renderTop)
			renderTop = pointPtr->y;
		    if (pointPtr->y > renderBottom)
			renderBottom = pointPtr->y;

		    pointPtr2++;	// next 3D point entry
		    pointPtr++;	// next 2D point entry

		}
	    while (--primitiveCounter);

	    ptr5 = (int *) pri2Ptr2;
	    eax = -1;

	}
    else
	{
	    printf("AnimNuage\n");	// another rarely used part.
	    exit(1);
	}

   // #ifdef DRAWPOINTS
   /*
    * This part draw all the points of the model 
    */
    primitiveCounter = numOfPri1;
    pointPtr = renderTab6;
    do
	{
	    if ((pointPtr->x > 0 && pointPtr->x < 640) && (pointPtr->y > 0 && pointPtr->y < 480))
		{
		    *(frontVideoBuffer + screenLockupTable[pointPtr->y] + pointPtr->x) = 255;
		}
	    else
		{
		    printf("Buggy point...\n");
		}

	    pointPtr++;

	}
    while (--primitiveCounter);

   // osystem->Flip (frontVideoBuffer);
   // #endif

    temp = *(short int *) ptr5;

    ptr5 = (int *) (((unsigned char *) ptr5) + 2);

    if (temp)			// process shading table
	{
	    renderV19 = (unsigned char *) shadeTable;
	    renderV21 = renderTab2;
	    primitiveCounter = numOfPri2;
	    ptr3 = pri2Ptr2 = pri2Ptr + 18;

	    do			// pour chaque poly ?
		{
		    temp = *(short int *) ptr3;
		    if (temp)
			{
			    rs1s2v1 = temp;

			    renderV22 = 0;	// light position
			    renderV23 = 40;
			    renderV24 = 40;

			    bufRotate0[0] = (*renderV21) * renderV22;
			    bufRotate0[1] = (*(renderV21 + 1)) * renderV22;
			    bufRotate0[2] = (*(renderV21 + 2)) * renderV22;

			    bufRotate0[3] = (*(renderV21 + 3)) * renderV23;
			    bufRotate0[4] = (*(renderV21 + 4)) * renderV23;
			    bufRotate0[5] = (*(renderV21 + 5)) * renderV23;

			    bufRotate0[6] = (*(renderV21 + 6)) * renderV24;
			    bufRotate0[7] = (*(renderV21 + 7)) * renderV24;
			    bufRotate0[8] = (*(renderV21 + 8)) * renderV24;

			    do	// pour chaque vertex ?
				{
				    short int col1;
				    short int col2;
				    short int col3;

				    short int *colPtr;

				    colPtr = (short int *) ptr5;

				    col1 = *(colPtr++);
				    col2 = *(colPtr++);
				    col3 = *(colPtr++);

				    eax =
					bufRotate0[0] * col1 + bufRotate0[1] * col2 +
					bufRotate0[2] * col3;
				    eax +=
					bufRotate0[3] * col1 + bufRotate0[4] * col2 +
					bufRotate0[5] * col3;
				    eax +=
					bufRotate0[6] * col1 + bufRotate0[7] * col2 +
					bufRotate0[8] * col3;

				    edi = 0;

				    if (eax > 0)
					{
					    eax >>= 14;
					    ptr6 = (unsigned char *) ptr5;
					    eax /= *(unsigned short int *) (ptr6 + 6);
					    edi = (unsigned short int) eax;
					}

				    *(short int *) renderV19 = edi;
				    renderV19 += 2;
				    ptr5 += 2;

				}
			    while (--rs1s2v1);

			}
		    ptr3 = pri2Ptr2 = pri2Ptr2 + 38;
		    ptr4 = renderV21 = renderV21 + 9;
		}

	    while (--primitiveCounter);

	   /*
	    * FILE* fileHandle;
	    * 
	    * fileHandle=fopen("shade1.dmp","wb+");
	    * 
	    * int i;
	    * 
	    * for(i=0;i<500;i++) { fwrite(&shadeTable[i],1,2,fileHandle); }
	    * 
	    * fclose(fileHandle); 
	    */
	}

    return (finishRender((unsigned char *) ptr5));
}

int finishRender(unsigned char *esi)
{
    unsigned char *edi;
    short int temp;
    int eax, ecx;

   // unsigned char temp2;
    short int counter;
    short int type;
    short int color;

   // float tempFloat;
   // short int ebx;
   // short int edx;
    lineData *lineDataPtr;
    lineCoordinates *lineCoordinatesPtr;

    int point1;

    int point2;

    int depth;
    int bestDepth;
    int currentDepth;
    short int bestZ;
    int j;
    int bestPoly = 0;
    short int shadeEntry;
    short int shadeValue;

    short int ax, bx, cx;

   // short int borrom;

    unsigned char *destPtr;
    int i;

    struct polyHeader
	{
	    unsigned char FillVertic_AType;
	    unsigned char numOfVertex;
	    short int colorIndex;
	};

    struct polyVertexHeader
	{
	    short int shadeEntry;
	    short int dataOffset;
	};

    struct computedVertex
	{
	    short int shadeValue;
	    short int x;
	    short int y;
	};

    polyVertexHeader *currentPolyVertex;
    polyHeader *currentPolyHeader;
    polyHeader *destinationHeader;
    computedVertex *currentComputedVertex;
    pointTab *currentVertex;
    pointTab *destinationVertex;

    edi = renderTab7;		// renderTab7 c'est le buffer de coordonnées
    temp = *(short int *) esi;	// we read the number of polygones
    esi += 2;

    if (temp)			// if there is polygones
	{
	    primitiveCounter = temp;	// the number of primitives = the number of polygones

	    do			// loop that load all the polygones
		{
		    render23 = edi;
		    currentPolyHeader = (polyHeader *) esi;
		    ecx = *(int *) esi;
		    esi += 2;
		    FillVertic_AType = currentPolyHeader->FillVertic_AType;

		    if (FillVertic_AType >= 9)
			{
			    destinationHeader = (polyHeader *) edi;

			    destinationHeader->FillVertic_AType =
				currentPolyHeader->FillVertic_AType - 2;
			    destinationHeader->numOfVertex = currentPolyHeader->numOfVertex;
			    destinationHeader->colorIndex = currentPolyHeader->colorIndex;

			    esi += 2;
			    edi += 4;

			    counter = destinationHeader->numOfVertex;

			    bestDepth = -32000;
			    renderV19 = edi;

			    do
				{
				    currentPolyVertex = (polyVertexHeader *) esi;

				    shadeValue =
					currentPolyHeader->colorIndex +
					shadeTable[currentPolyVertex->shadeEntry];

				    currentComputedVertex = (computedVertex *) edi;

				    currentComputedVertex->shadeValue = shadeValue;

				    currentVertex = &renderTab6[currentPolyVertex->dataOffset / 6];
				    destinationVertex = (pointTab *) (edi + 2);

				    destinationVertex->x = currentVertex->x;
				    destinationVertex->y = currentVertex->y;

				    edi += 6;
				    esi += 4;

				    currentDepth = currentVertex->z;

				    if (currentDepth > bestDepth)
					bestDepth = currentDepth;
				}
			    while (--counter);
			}
		    else if (FillVertic_AType >= 7)	// only 1 shade value is used
			{
			    destinationHeader = (polyHeader *) edi;

			    destinationHeader->FillVertic_AType =
				currentPolyHeader->FillVertic_AType - 7;
			    destinationHeader->numOfVertex = currentPolyHeader->numOfVertex;

			    color = currentPolyHeader->colorIndex;

			    shadeEntry = *(short int *) (esi + 2);

			    esi += 4;

			    *(short int *) (edi + 2) = color + shadeTable[shadeEntry];

			    edi += 4;
			    renderV19 = edi;
			    bestDepth = -32000;
			    counter = destinationHeader->numOfVertex;

			    do
				{
				    eax = *(short int *) esi;
				    esi += 2;

				    currentVertex = &renderTab6[eax / 6];

				    destinationVertex = (pointTab *) (edi + 2);

				    destinationVertex->x = currentVertex->x;
				    destinationVertex->y = currentVertex->y;

				    edi += 6;

				    currentDepth = currentVertex->z;

				    if (currentDepth > bestDepth)
					bestDepth = currentDepth;
				}
			    while (--counter);
			}
		    else	// no shade is used
			{
			    destinationHeader = (polyHeader *) edi;

			    destinationHeader->FillVertic_AType =
				currentPolyHeader->FillVertic_AType - 2;
			    destinationHeader->numOfVertex = currentPolyHeader->numOfVertex;
			    destinationHeader->colorIndex = currentPolyHeader->colorIndex;

			    esi += 2;
			    edi += 4;

			    bestDepth = -32000;
			    renderV19 = edi;
			    eax = 0;
			    counter = currentPolyHeader->numOfVertex;

			    do
				{
				    eax = *(short int *) esi;
				    esi += 2;

				    currentVertex = &renderTab6[eax / 6];

				    destinationVertex = (pointTab *) (edi + 2);

				    destinationVertex->x = currentVertex->x;
				    destinationVertex->y = currentVertex->y;

				    edi += 6;

				    currentDepth = currentVertex->z;

				    if (currentDepth > bestDepth)
					bestDepth = currentDepth;
				}
			    while (--(counter));
			}

		    render24 = edi;
		    edi = renderV19;

		    render25 = bestDepth;

		    ax = *(short int *) (edi + 4);
		    bx = *(short int *) (edi + 8);

		    ax -= *(short int *) (edi + 16);
		    bx -= *(short int *) (edi + 2);

		    ax *= bx;

		    bestDepth = ax;
		    bx = currentDepth;

		    ax = *(short int *) (edi + 2);
		    cx = *(short int *) (edi + 10);

		    ax -= *(short int *) (edi + 14);
		    cx -= *(short int *) (edi + 4);

		    ax *= cx;

		    ax -= bestDepth;

			// recheck next line:
			currentDepth -= (bx) - 1;

		    if (currentDepth < 0)
			{
			    edi = render23;
			}
		    else
			{
			    numOfPrimitives++;

			    renderTabEntryPtr->depth = render25;
			    renderTabEntryPtr->renderType = 1;
			    renderTabEntryPtr->dataPtr = render23;
			    renderTabEntryPtr++;

			    edi = render24;
			}

		}
	    while (--primitiveCounter);
	}

    temp = *(short int *) esi;
    esi += 2;
    if (temp)			// pour les lignes (0)
	{
	    numOfPrimitives += temp;
	    do
		{
		    lineDataPtr = (lineData *) esi;
		    lineCoordinatesPtr = (lineCoordinates *) edi;

		    if (lineDataPtr->p1 % 6 != 0 || lineDataPtr->p2 % 6 != 0)
			{
			    printf("lineDataPtr reference is malformed !\n");
			    exit(1);
			}

		    point1 = lineDataPtr->p1 / 6;
		    point2 = lineDataPtr->p2 / 6;
		    lineCoordinatesPtr->data = lineDataPtr->data;
		    lineCoordinatesPtr->x1 = renderTab6[point1].x;
		    lineCoordinatesPtr->y1 = renderTab6[point1].y;
		    lineCoordinatesPtr->x2 = renderTab6[point2].x;
		    lineCoordinatesPtr->y2 = renderTab6[point2].y;
		    bestDepth = renderTab6[point1].z;
		    depth = renderTab6[point2].z;

		    if (depth >= bestDepth)
			bestDepth = depth;

		    renderTabEntryPtr->depth = bestDepth;
		    renderTabEntryPtr->renderType = 0;
		    renderTabEntryPtr->dataPtr = edi;
		    renderTabEntryPtr++;

		    esi += 8;
		    edi += 12;
		}
	    while (--temp);
	}

    temp = *(short int *) esi;
    esi += 2;
    if (temp)			// pour les cercles (2)
	{
	   // numOfPrimitives+=temp;
	    do
		{
		    esi += 8;
		}
	    while (--temp);
	}

    renderTabEntryPtr2 = renderTab;

    renderTabSortedPtr = renderTabSorted;
    for (i = 0; i < numOfPrimitives; i++)	// then we sort the polygones (FIXME: very slow !)
	{
	    renderTabEntryPtr2 = renderTab;
	    bestZ = -0x7FFF;
	    for (j = 0; j < numOfPrimitives; j++)
		{
		    if (renderTabEntryPtr2->depth > bestZ)
			{
			    bestZ = renderTabEntryPtr2->depth;
			    bestPoly = j;
			}
		    renderTabEntryPtr2++;
		}
	    renderTabSortedPtr->depth = renderTab[bestPoly].depth;
	    renderTabSortedPtr->renderType = renderTab[bestPoly].renderType;
	    renderTabSortedPtr->dataPtr = renderTab[bestPoly].dataPtr;
	    renderTabSortedPtr++;
	    renderTab[bestPoly].depth = -0x7FFF;
	}
    renderTabEntryPtr2 = renderTabSorted;

    if (numOfPrimitives)
	{
	    primitiveCounter = numOfPrimitives;
	    renderV19 = esi;

	    do
		{
		    type = renderTabEntryPtr2->renderType;
		    esi = renderTabEntryPtr2->dataPtr;
		    renderV19 += 8;
		    switch (type)
			{
			case 0:	// draw a line
			   {
			       lineCoordinatesPtr = (lineCoordinates *) esi;
			       color = (lineCoordinatesPtr->data & 0xFF00) >> 8;
			       //drawLine(lineCoordinatesPtr->x1,lineCoordinatesPtr->y1,lineCoordinatesPtr->x2,lineCoordinatesPtr->y2,color);
			       break;
			   }
			case 1:	// draw a polygon
			   {
			       eax = *(int *) esi;
			       esi += 4;

			       FillVertic_AType = eax & 0xFF;
			       numOfVertex = (eax & 0xFF00) >> 8;
			       color = (eax & 0xFF0000) >> 16;

			       destPtr = (unsigned char *) vertexCoordinates;

			       for (i = 0; i < (numOfVertex * 3); i++)
				   {
				       *(short int *) destPtr = *(short int *) esi;
				       destPtr += 2;
				       esi += 2;
				   }

			       if (ComputePoly_A() != 2)
				   FillVertic_A(FillVertic_AType, color);

			       break;
			   }

			default:
			   {
			       break;
			   }
			};
		    esi = renderV19;
		    renderTabEntryPtr2++;
		}
	    //while (--primitiveCounter);
	    while(0);
	}
    else
	{
	    renderRight = -1;
	    renderBottom = -1;
	    renderLeft = -1;
	    renderTop = -1;
	    return (-1);
	}

    return (0);
}

void RotateGroupe(int edx, int ecx, int ebx, pointEntry * ptr)
{
    int *ebp;
    short int var;

   // int* ptr1;

    renderV1 = ebx;
    renderV2 = ecx;
    renderV3 = edx;

    rs1v1 = ptr->data1;

    rs1v2 = ptr->data2;

    if (rs1v1 % 6)
	{
	    printf("Error: rs1V1\n");
	    exit(1);
	}

    var = ptr->param;

    if (var == -1)		// si c'est le premier point
	{
	//    ebp = &setSomething3Var2;

	    destX = 0;
	    destZ = 0;
	    destY = 0;
	}
    else
	{

	    ebp = (int *) (((unsigned char *) renderTab2) + var);

	    destX = renderTab5[ptr->data3 / 6].x;
	    destZ = renderTab5[ptr->data3 / 6].y;
	    destY = renderTab5[ptr->data3 / 6].z;
	}

   // renderV19= dest
   // ebp= source
    RotMatIndex2((int *) renderV19, ebp);	// copie dans renderTab2 + application de la rotation

   // ? , numOfPoint , destination, rotation data
    RotList(pri1Ptr + rs1v1, rs1v2, &renderTab5[rs1v1 / 6], (int *) renderV19);	// rotation 
   // 
   // 
   // 
   // 
   // 
   // 
   // 
   // 
   // 
   // 
   // 
   // 
   // des
   // elements 
   // du model
}

void RotList(unsigned char *esi, int ecx, pointTab * dest, int *eax)
{
    short int param1;
    short int param2;
    short int param3;

    short int *tempPtr;

    rs1s2v1 = ecx;

    do
	{
	    rs1s2v2 = esi;
	    tempPtr = (short int *) (esi);

	    param1 = tempPtr[0];
	    param2 = tempPtr[1];
	    param3 = tempPtr[2];

	    dest->x = ((eax[0] * param1 + eax[1] * param2 + eax[2] * param3) >> 14) + destX;
	    dest->y = ((eax[3] * param1 + eax[4] * param2 + eax[5] * param3) >> 14) + destZ;
	    dest->z = ((eax[6] * param1 + eax[7] * param2 + eax[8] * param3) >> 14) + destY;

	    dest++;
	    esi = rs1s2v2 + 6;
	}
    while (--rs1s2v1);
}

void RotMatIndex2(int *eax, int *ebp)
{
    int angle;
    int angleVar1;		// esi
    int angleVar2;		// ecx

    int matrix1[9];
    int matrix2[9];

    if (renderV1)		// rotation par vers l'avant
	{
	    angle = renderV1;
	    angleVar2 = tab1[angle & 0x3FF];
	    angle += 0x100;
	    angleVar1 = tab1[angle & 0x3FF];

	    matrix1[0] = ebp[0];
	    matrix1[3] = ebp[3];
	    matrix1[6] = ebp[6];

	    matrix1[1] = (ebp[2] * angleVar2 + ebp[1] * angleVar1) >> 14;
	    matrix1[2] = (ebp[2] * angleVar1 - ebp[1] * angleVar2) >> 14;
	    matrix1[4] = (ebp[5] * angleVar2 + ebp[4] * angleVar1) >> 14;
	    matrix1[5] = (ebp[5] * angleVar1 - ebp[4] * angleVar2) >> 14;
	    matrix1[7] = (ebp[8] * angleVar2 + ebp[7] * angleVar1) >> 14;
	    matrix1[8] = (ebp[8] * angleVar1 - ebp[7] * angleVar2) >> 14;

	}
    else
	{
	    matrix1[0] = ebp[0];
	    matrix1[1] = ebp[1];
	    matrix1[2] = ebp[2];
	    matrix1[3] = ebp[3];
	    matrix1[4] = ebp[4];
	    matrix1[5] = ebp[5];
	    matrix1[6] = ebp[6];
	    matrix1[7] = ebp[7];
	    matrix1[8] = ebp[8];
	}

    if (renderV3)
	{
	    angle = renderV3;
	    angleVar2 = tab1[angle & 0x3FF];
	    angle += 0x100;
	    angleVar1 = tab1[angle & 0x3FF];

	    matrix2[2] = matrix1[2];
	    matrix2[5] = matrix1[5];
	    matrix2[8] = matrix1[8];

	    matrix2[0] = (matrix1[1] * angleVar2 + matrix1[0] * angleVar1) >> 14;
	    matrix2[1] = (matrix1[1] * angleVar1 - matrix1[0] * angleVar2) >> 14;
	    matrix2[3] = (matrix1[4] * angleVar2 + matrix1[3] * angleVar1) >> 14;
	    matrix2[4] = (matrix1[4] * angleVar1 - matrix1[3] * angleVar2) >> 14;
	    matrix2[6] = (matrix1[7] * angleVar2 + matrix1[6] * angleVar1) >> 14;
	    matrix2[7] = (matrix1[7] * angleVar1 - matrix1[6] * angleVar2) >> 14;

	}
    else
	{
	    matrix2[0] = matrix1[0];
	    matrix2[1] = matrix1[1];
	    matrix2[2] = matrix1[2];
	    matrix2[3] = matrix1[3];
	    matrix2[4] = matrix1[4];
	    matrix2[5] = matrix1[5];
	    matrix2[6] = matrix1[6];
	    matrix2[7] = matrix1[7];
	    matrix2[8] = matrix1[8];

	}

    if (renderV2)		// rotation de coté (la plus courante)
	{

	    angle = renderV2;
	    angleVar2 = tab1[angle & 0x3FF];	// esi
	    angle += 0x100;
	    angleVar1 = tab1[angle & 0x3FF];	// ecx

	    eax[1] = matrix2[1];
	    eax[4] = matrix2[4];
	    eax[7] = matrix2[7];

	    eax[0] = (matrix2[0] * angleVar1 - matrix2[2] * angleVar2) >> 14;
	    eax[2] = (matrix2[0] * angleVar2 + matrix2[2] * angleVar1) >> 14;
	    eax[3] = (matrix2[3] * angleVar1 - matrix2[5] * angleVar2) >> 14;
	    eax[5] = (matrix2[3] * angleVar2 + matrix2[5] * angleVar1) >> 14;

	    eax[6] = (matrix2[6] * angleVar1 - matrix2[8] * angleVar2) >> 14;
	    eax[8] = (matrix2[6] * angleVar2 + matrix2[8] * angleVar1) >> 14;
	}
    else
	{
	    eax[0] = matrix2[0];
	    eax[1] = matrix2[1];
	    eax[2] = matrix2[2];
	    eax[3] = matrix2[3];
	    eax[4] = matrix2[4];
	    eax[5] = matrix2[5];
	    eax[6] = matrix2[6];
	    eax[7] = matrix2[7];
	    eax[8] = matrix2[8];
	}

}

int RotateNuage(unsigned char *ptr)
{
    printf("RotateNuage\n");
    exit(1);
    return (-1);
}

void TranslateGroupe(int edx, int ecx, int ebx, pointEntry * esi)
{
    int *dest;
    int *source;

    renderV1 = ebx;
    renderV2 = ecx;
    renderV3 = edx;

    if (esi->param == -1)	// base point
	{
	    destX = 0;
	    destZ = 0;
	    destY = 0;

	    dest = (int *) renderV19;

/*	    dest[0] = setSomething3Var2;
	    dest[1] = setSomething3Var3;
	    dest[2] = setSomething3Var18;
	    dest[3] = setSomething3Var4;
	    dest[4] = setSomething3Var5;
	    dest[5] = setSomething3Var9;
	    dest[6] = setSomething3Var6;
	    dest[7] = setSomething3Var7;
	    dest[8] = setSomething3Var10;*/
	}
    else			// dependent
	{
	    destX = renderTab5[(esi->data3) / 6].x;	// todo:
	    destZ = renderTab5[(esi->data3) / 6].y;	// inverse because pointab and dest entry
	   // are not
	   // in the same order....
	    destY = renderTab5[(esi->data3) / 6].z;

	    source = (int *) (renderTab2 + esi->param);
	    dest = (int *) renderV19;

	    dest[0] = source[0];
	    dest[1] = source[1];
	    dest[2] = source[2];
	    dest[3] = source[3];
	    dest[4] = source[4];
	    dest[5] = source[5];
	    dest[6] = source[6];
	    dest[7] = source[7];
	    dest[8] = source[8];
	}

    TransRotList(pri1Ptr + esi->data1, esi->data2, &renderTab5[esi->data1 / 6], (int *) renderV19);
}

void TransRotList(unsigned char *esi, int ecx, pointTab * dest, int *eax)
{
    short int param1;
    short int param2;
    short int param3;
    short int *tempPtr;

    rs1s2v1 = ecx;

    do
	{
	    rs1s2v2 = esi;
	    tempPtr = (short int *) (esi);

	    param1 = tempPtr[0] + renderV1;
	    param2 = tempPtr[1] + renderV2;
	    param3 = tempPtr[2] + renderV3;

	    dest->x = ((eax[0] * param1 + eax[1] * param2 + eax[2] * param3) >> 14) + destX;
	    dest->y = ((eax[3] * param1 + eax[4] * param2 + eax[5] * param3) >> 14) + destZ;
	    dest->z = ((eax[6] * param1 + eax[7] * param2 + eax[8] * param3) >> 14) + destY;

	    dest++;
	    esi = rs1s2v2 + 6;

	}
    while (--rs1s2v1);
}
