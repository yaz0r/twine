#include "lba.h"
#include "rendererAngleTab.h"

int LBA_renderer::startRenderer(int X, int Y, int Z, int angleX, int angleY, int angleZ,
				unsigned char *costumePtr)
{
    unsigned char *ptr;
    short int costumeHeader;

    tab1 = &rendererTab[0];
    tab2 = &rendererTab[256];
    tab3 = &rendererTab[384];

    _angleX = angleX;
    _angleY = angleY;
    _angleZ = angleZ;

   /*
    * Those 4 vars are used to know the size of the rendered model. They are resetted to their
    * maximum value at the begining of the renderer 
    */

    _renderLeft = 32767;
    _renderTop = 32767;

    _renderRight = -32767;
    _renderBottom = -32767;

   /*
    * the use of setSomethingVar4 is still unknown 
    */

    if (setSomethingVar4 == 0)
	{
	    printf("Unimplemented startRenderer\n");
	    exit(1);
	}
    else
	{
	    _X = X;
	    _Y = Y;
	    _Z = Z;
	}

    _numOfPrimitives = 0;	// we reset the number of primitives in the model

    renderTabEntryPtr = renderTab;	// we restart at the beginning of the renderTable

    costumeHeader = *(short int *) costumePtr;	// costumeHeader seems to contains important flags
   // for the rendering

    ptr = costumePtr + 16 + *(short int *) (costumePtr + 14);	// we jump after the header

    if (costumeHeader & 2)	// if animated
	return (renderAnimatedModel(ptr));	// That's the mostly used renderer code
    else
	{
	    printf("Unsupported unanimated model render!\n");
	    exit(1);
	}

    return (0);

}

void LBA_renderer::setSomething4(int angleX, int angleY, int angleZ)
{
    _cameraAngleX = angleX;
    _cameraAngleY = angleY;
    _cameraAngleZ = angleZ;

    _angleX = angleX;
    _angleY = angleY;
    _angleZ = angleZ;

   /*
    * renderS1S1 (bufRotate0, &setSomething3Var2); renderS2S2 (0, 0, 59);
    * 
    * renderV22 = destX; renderV23 = destZ; renderV24 = destY;
    */
}

int LBA_renderer::renderAnimatedModel(unsigned char *costumePtr)
{
    pointEntry *ptEntryPtr;
    pointTab *pointPtr;
    pointTab *pointPtrDest;
    int coX;
    int coY;
    int coZ;
    unsigned char *ptr3;
    int *ptr4;
    int *ptr5;
    unsigned char *ptr6;

    _numOfPoints = *(short int *) costumePtr;
    costumePtr += 2;
    _pointsPtr = costumePtr;

    costumePtr += _numOfPoints * 6;

    _numOfParts = *(short int *) costumePtr;
    costumePtr += 2;
    _partsPtr = _partsPtr2 = costumePtr;

    _currentMatrixTableEntry = (unsigned char *) _matrixTable;

    loadPart(_angleX, _angleY, _angleZ, (pointEntry *) _partsPtr);

    _partsPtr += 38;

    ptEntryPtr = (pointEntry *) _partsPtr;

    if (_numOfParts - 1 != 0)
	{
	    _numOfPrimitives = _numOfParts - 1;
	    _currentMatrixTableEntry = (unsigned char *) &_matrixTable[9];

	    do
		{
		    if (ptEntryPtr->flag == 0)
			{
			    loadPart(ptEntryPtr->rotateX, ptEntryPtr->rotateY, ptEntryPtr->rotateZ, ptEntryPtr);	// rotation
			}
		    else
			{
			    if (ptEntryPtr->flag == 1)
				{
				    renderS2(ptEntryPtr->rotateX, ptEntryPtr->rotateY, ptEntryPtr->rotateZ, ptEntryPtr);	// translation
				}
			}

		    _currentMatrixTableEntry += 36;
		    _partsPtr += 38;
		    ptEntryPtr = (pointEntry *) _partsPtr;

		}
	    while (--_numOfPrimitives);
	}

    _numOfPrimitives = _numOfPoints;

    pointPtr = (pointTab *) _projectedPointTable;
    pointPtrDest = (pointTab *) _flattenPointTable;

    if (setSomethingVar4 != 0)	// use standard projection
	{
	    do
		{
		    coX = pointPtr->x + _X;
		    coY = pointPtr->y + _Y;
		    coZ = -(pointPtr->z + _Z);

		    pointPtrDest->x =
			(((coX + coZ) * 8 + (coX + coZ) * 16) >> 9) + setSomethingVar1;
		    pointPtrDest->z = coZ - coX - coY;
		    pointPtrDest->y =
			((((coX - coZ) * 4 + (coX - coZ) * 8) + (2 * coY - (coY << 5))) >> 9) +
			setSomethingVar2;

		    if (pointPtrDest->x < _renderLeft)
			_renderLeft = pointPtrDest->x;
		    if (pointPtrDest->x > _renderRight)
			_renderRight = pointPtrDest->x;

		    if (pointPtrDest->y < _renderTop)
			_renderTop = pointPtrDest->y;
		    if (pointPtrDest->y > _renderBottom)
			_renderBottom = pointPtrDest->y;

		    pointPtr++;
		    pointPtrDest++;

		}
	    while (--_numOfPrimitives);

	    _shadePtr = (int *) _partsPtr;
	}
    else
	{
	    printf("Unsuported render with unstandard camera in renderAnimatedModel !\n");
	    exit(1);
	}

  /*_numOfPrimitives = _numOfPoints;
  pointPtr = _flattenPointTable;
  do
    {
      if ((pointPtr->x > 0 && pointPtr->x < 640)
	  && (pointPtr->y > 0 && pointPtr->y < 480))
	{
	  *(videoBuffer1 + screenLockupTable[pointPtr->y] + pointPtr->x) =
	    255;
	}
      else
	{
	  printf ("Buggy point...\n");
	}

      pointPtr++;

    }
  while (--_numOfPrimitives);*/

    int temp = *(short int *) _shadePtr;

    _shadePtr = (int *) (((unsigned char *) _shadePtr) + 2);
    if (temp)			// process shading table
	{
	    int eax;
	    int edi;

	    unsigned char *_currentShadeDestination = (unsigned char *) shadeTable;
	    int *renderV21 = _matrixTable;

	    _numOfPrimitives = _numOfParts;
	    unsigned char *pri2Ptr2;

	    ptr3 = pri2Ptr2 = _partsPtr2 + 18;

	    do
		{
		    temp = *(short int *) ptr3;
		    if (temp)
			{
			    int rs1s2v1 = temp;

			    _lightX = 0;	// light position
			    _lightY = 40;
			    _lightZ = 40;

			    _shadeMatrix[0] = (*renderV21) * _lightX;
			    _shadeMatrix[1] = (*(renderV21 + 1)) * _lightX;
			    _shadeMatrix[2] = (*(renderV21 + 2)) * _lightX;

			    _shadeMatrix[3] = (*(renderV21 + 3)) * _lightY;
			    _shadeMatrix[4] = (*(renderV21 + 4)) * _lightY;
			    _shadeMatrix[5] = (*(renderV21 + 5)) * _lightY;

			    _shadeMatrix[6] = (*(renderV21 + 6)) * _lightZ;
			    _shadeMatrix[7] = (*(renderV21 + 7)) * _lightZ;
			    _shadeMatrix[8] = (*(renderV21 + 8)) * _lightZ;

			    do	// pour chaque vertex ?
				{
				    short int col1;
				    short int col2;
				    short int col3;

				    short int *colPtr;

				    colPtr = (short int *) _shadePtr;

				    col1 = *(colPtr++);
				    col2 = *(colPtr++);
				    col3 = *(colPtr++);

				    eax =
					_shadeMatrix[0] * col1 + _shadeMatrix[1] * col2 +
					_shadeMatrix[2] * col3;
				    eax +=
					_shadeMatrix[3] * col1 + _shadeMatrix[4] * col2 +
					_shadeMatrix[5] * col3;
				    eax +=
					_shadeMatrix[6] * col1 + _shadeMatrix[7] * col2 +
					_shadeMatrix[8] * col3;

				    edi = 0;

				    if (eax > 0)
					{
					    eax >>= 14;
					    ptr6 = (unsigned char *) _shadePtr;
					    eax /= *(unsigned short int *) (ptr6 + 6);
					    edi = (unsigned short int) eax;
					}

				    *(short int *) _currentShadeDestination = edi;
				    _currentShadeDestination += 2;
				    _shadePtr += 2;

				}
			    while (--rs1s2v1);

			}
		    ptr3 = pri2Ptr2 = pri2Ptr2 + 38;
		    ptr4 = renderV21 = renderV21 + 9;
		}
	    while (--_numOfPrimitives);
	}

   /*
    * FILE* fileHandle;
    * 
    * fileHandle=fopen("shade2.dmp","wb+");
    * 
    * int i;
    * 
    * for(i=0;i<500;i++) { fwrite(&shadeTable[i],1,2,fileHandle); }
    * 
    * fclose(fileHandle); 
    */

    return (finishRender((unsigned char *) _shadePtr));
}

void LBA_renderer::loadPart(int edx, int ecx, int ebx, pointEntry * ptr)
{
    int *ebp;
    short int var;

   // int* ptr1;

    _angleX = ebx;
    _angleZ = ecx;
    _angleY = edx;

    int rs1v1 = ptr->data1;

    int rs1v2 = ptr->data2;

    if (rs1v1 % 6)
	{
	    printf("Error: rs1V1\n");
	    exit(1);
	}

    var = ptr->param;

    if (var == -1)		// si c'est le premier point
	{
	    ebp = _baseMatrix;

	    _destX = 0;
	    _destY = 0;
	    _destZ = 0;
	}
    else
	{
	    ebp = (int *) ((unsigned char *) _matrixTable + var);

	    _destX = _projectedPointTable[ptr->data3 / 6].x;
	    _destY = _projectedPointTable[ptr->data3 / 6].y;
	    _destZ = _projectedPointTable[ptr->data3 / 6].z;
	}

   // renderV19= dest
   // ebp= source
    renderS1S1((int *) _currentMatrixTableEntry, ebp);	// copie dans renderTab2 + application de
   // la rotation

   // ? , numOfPoint , destination, rotation data
    renderS1S2(_pointsPtr + rs1v1, rs1v2, &_projectedPointTable[rs1v1 / 6], (int *) _currentMatrixTableEntry);	// rotation 
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
   // du 
   // model
}

void LBA_renderer::renderS1S2(unsigned char *esi, int ecx, pointTab * dest, int *eax)
{
    short int param1;
    short int param2;
    short int param3;

    short int *tempPtr;

    int rs1s2v1 = ecx;
    unsigned char *rs1s2v2;

    do
	{
	    rs1s2v2 = esi;
	    tempPtr = (short int *) (esi);

	    param1 = tempPtr[0];
	    param2 = tempPtr[1];
	    param3 = tempPtr[2];

	    dest->x = ((eax[0] * param1 + eax[1] * param2 + eax[2] * param3) >> 14) + _destX;
	    dest->y = ((eax[3] * param1 + eax[4] * param2 + eax[5] * param3) >> 14) + _destY;
	    dest->z = ((eax[6] * param1 + eax[7] * param2 + eax[8] * param3) >> 14) + _destZ;

	    dest++;
	    esi = rs1s2v2 + 6;
	}
    while (--rs1s2v1);
}

void LBA_renderer::renderS1S1(int *eax, int *ebp)
{
    int angle;
    int angleVar1;		// esi
    int angleVar2;		// ecx

    int matrix1[9];
    int matrix2[9];

    if (_angleX)		// rotation par vers l'avant
	{
	    angle = _angleX;
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

    if (_angleY)
	{
	    angle = _angleY;
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

    if (_angleZ)		// rotation de coté (la plus courante)
	{

	    angle = _angleZ;
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

void LBA_renderer::renderS2(int edx, int ecx, int ebx, pointEntry * esi)
{
    int *dest;
    int *source;

    _angleZ = ebx;
    _angleY = ecx;
    _angleX = edx;

    if (esi->param == -1)	// base point
	{
	    _destX = 0;
	    _destZ = 0;
	    _destY = 0;

	    dest = (int *) _currentMatrixTableEntry;

	    dest[0] = _baseMatrix[0];
	    dest[1] = _baseMatrix[1];
	    dest[2] = _baseMatrix[2];
	    dest[3] = _baseMatrix[3];
	    dest[4] = _baseMatrix[4];
	    dest[5] = _baseMatrix[5];
	    dest[6] = _baseMatrix[6];
	    dest[7] = _baseMatrix[7];
	    dest[8] = _baseMatrix[8];
	}
    else			// dependent
	{
	    _destX = _projectedPointTable[(esi->data3) / 6].x;	// todo:
	    _destZ = _projectedPointTable[(esi->data3) / 6].z;	// inverse because pointab and dest 
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
	   // entry are not in the same
	   // order....
	    _destY = _projectedPointTable[(esi->data3) / 6].y;

	    source = (int *) ((unsigned char *) _matrixTable + esi->param);
	    dest = (int *) _currentMatrixTableEntry;

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

    renderS2Sub(_pointsPtr + esi->data1, esi->data2, &_projectedPointTable[esi->data1 / 6],
		(int *) _currentMatrixTableEntry);
}

void LBA_renderer::renderS2Sub(unsigned char *esi, int ecx, pointTab * dest, int *eax)
{
    short int param1;
    short int param2;
    short int param3;
    short int *tempPtr;

    int rs1s2v1 = ecx;

    do
	{
	    unsigned char *rs1s2v2 = esi;

	    tempPtr = (short int *) (esi);

	    param1 = tempPtr[0] + _angleZ;
	    param2 = tempPtr[1] + _angleY;
	    param3 = tempPtr[2] + _angleX;

	    dest->x = ((eax[0] * param1 + eax[1] * param2 + eax[2] * param3) >> 14) + _destX;
	    dest->y = ((eax[3] * param1 + eax[4] * param2 + eax[5] * param3) >> 14) + _destY;
	    dest->z = ((eax[6] * param1 + eax[7] * param2 + eax[8] * param3) >> 14) + _destZ;

	    dest++;
	    esi = rs1s2v2 + 6;

	}
    while (--rs1s2v1);
}

int LBA_renderer::finishRender(unsigned char *esi)
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

    unsigned char *render23;
    unsigned char *render24;
    int render25;

    struct polyHeader
	{
	    unsigned char polyRenderType;
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
		    polyRenderType = currentPolyHeader->polyRenderType;

		    if (polyRenderType >= 9)
			{
			    destinationHeader = (polyHeader *) edi;

			    destinationHeader->polyRenderType =
				currentPolyHeader->polyRenderType - 2;
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

				    currentVertex =
					&_flattenPointTable[currentPolyVertex->dataOffset / 6];
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
		    else if (polyRenderType >= 7)	// only 1 shade value is used
			{
			    destinationHeader = (polyHeader *) edi;

			    destinationHeader->polyRenderType =
				currentPolyHeader->polyRenderType - 7;
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

				    currentVertex = &_flattenPointTable[eax / 6];

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

			    destinationHeader->polyRenderType =
				currentPolyHeader->polyRenderType - 2;
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

				    currentVertex = &_flattenPointTable[eax / 6];

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
		    currentDepth -= (bx) - 1;	// peut-etre une erreur là

		    if (currentDepth < 0)
			{
			    edi = render23;
			}
		    else
			{
			    _numOfPrimitives++;

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
	    _numOfPrimitives += temp;
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
		    lineCoordinatesPtr->x1 = _flattenPointTable[point1].x;
		    lineCoordinatesPtr->y1 = _flattenPointTable[point1].y;
		    lineCoordinatesPtr->x2 = _flattenPointTable[point2].x;
		    lineCoordinatesPtr->y2 = _flattenPointTable[point2].y;
		    bestDepth = _flattenPointTable[point1].z;
		    depth = _flattenPointTable[point2].z;

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
    for (i = 0; i < _numOfPrimitives; i++)	// then we sort the polygones (FIXME: very slow !)
	{
	    renderTabEntryPtr2 = renderTab;
	    bestZ = -0x7FFF;
	    for (j = 0; j < _numOfPrimitives; j++)
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

    if (_numOfPrimitives)
	{
	    primitiveCounter = _numOfPrimitives;
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
			      // drawLine(lineCoordinatesPtr->x1,lineCoordinatesPtr->y1,lineCoordinatesPtr->x2,lineCoordinatesPtr->y2,color);
			       break;
			   }
			case 1:	// draw a polygon
			   {
			       eax = *(int *) esi;
			       esi += 4;

			       polyRenderType = eax & 0xFF;
			       numOfVertex = (eax & 0xFF00) >> 8;
			       color = (eax & 0xFF0000) >> 16;

			       destPtr = (unsigned char *) vertexCoordinates;

			       for (i = 0; i < (numOfVertex * 3); i++)
				   {
				       *(short int *) destPtr = *(short int *) esi;
				       destPtr += 2;
				       esi += 2;
				   }

			       if (prepareRender() != 2)
				   polyRender(polyRenderType, color);

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
	    while (--primitiveCounter);
	   // while(0);
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

void LBA_renderer::polyRender(int ecx, int edi)
{
    unsigned char *out, *out2;
    short int *ptr1, *ptr2;
    int vsize, hsize;
    int color;
    int eax;
    int i, j;
    short int var2, var3;

   // char borrow;
    short int start, stop;
    short int startColor, stopColor;

    float varf2;
    float varf3;
    float varf4;

    if (vtop <= 0 || vbottom <= 0)
	return;
    if (vleft <= 0 || vright <= 0)
	return;
    if (vleft >= 640)
	return;
   // if(vright>=640)
   // return;
    if (vtop >= 480 || vbottom >= 480)
	return;

    out = videoBuffer1 + screenLockupTable[vtop];

    ptr1 = &polyTab[vtop];
    ptr2 = &polyTab2[vtop];

    vsize = vbottom - vtop;
    vsize++;

    color = edi;

   // osystem->drawBufferToScreen(videoBuffer1);

    switch (ecx)
	{
	case 0:		// flat polygone
	   {
	       eax =
		   (color & 0xFF) | ((color & 0xFF) << 8) | ((color & 0xFF) << 16) | ((color & 0xFF)
										      << 24);
	       for (i = 0; i < vsize; i++)
		   {
		       stop = ptr1[480];
		       start = ptr1[0];

		       ptr1++;
		       hsize = stop - start;

		       if (hsize > 0)
			   {
			       hsize++;
			       out2 = start + out;
			       for (j = 0; j < hsize; j++)
				   {
				       *(out2++) = color;
				   }
			   }
		       out += 640;
		   }
	       break;
	   }
	case 7:		// main codec
	case 8:
	   {
	       renderLoop = vsize;
	       do
		   {
		       stop = ptr1[480];	// stop
		       start = ptr1[0];	// start

		       ptr1++;
		       out2 = start + out;
		       hsize = stop - start;

		       var2 = stopColor = ptr2[480];
		       var3 = startColor = ptr2[0];

		       ptr2++;

		       varf2 = var2;
		       varf3 = var3;

		       varf4 = varf2 - varf3;

		       if (hsize == 0)
			   {
			       *out2 = (unsigned char) ((varf3 + varf2) / 2);	// moyenne des 2 couleurs
			   }
		       else if (hsize > 0)
			   {
			       if (hsize == 1)
				   {
				       *(out2 + 1) = (char) varf2;
				       *(out2) = (char) varf3;
				   }
			       else if (hsize == 2)
				   {
				       *(out2 + 2) = (char) varf2;
				       *(out2 + 1) = (char) ((varf2 + varf3) / 2);
				       *(out2) = (char) varf3;
				   }
			       else
				   {
				       varf4 /= hsize;

				       do
					   {
					       *(out2++) = (char) varf3;
					       varf3 += varf4;
					   }
				       while (--hsize);
				   }
			   }

		      /*
		       * if(hsize==0) // ok { stopColor+=startColor; ptr2++; stopColor>>=1;
		       * *out2=((stopColor&0xFF00)>>8); } else if(hsize>0) // face dans le bon sens {
		       * var2=stopColor-=startColor; //largeur de la bande varf2=var2; if(stopColor<0) //
		       * ok { if(hsize<=2) // ok { varf3=ptr2[480]; varf2=ptr2[0]; ptr2++;
		       * *(out2+hsize)=(char)varf3; if(--hsize) { varf3+=varf2; varf3/=2;
		       * *(out2+1)=(char)varf3; } *out2=(char)varf2; } else // ok { varf2=-varf2; varf3=0;
		       * varf2/=hsize; varf3=ptr2[0]; ptr2++; hsize++; do { (*out2++)=(char)varf3;
		       * varf3-=varf2; }while(--hsize); } } else { if(hsize<=2) { var2=ptr2[480]; ptr2++;
		       * *(out2+hsize)=var2; if(--hsize) { var3+=var2; var3>>=1; *(out2+1)=var3; }
		       * *(out2)=var2; } else // le plus interessant, face visible + couleurs normales {
		       * var3=0; //var2/=hsize; //largeur de la bande
		       * 
		       * varf2=var2; varf2/=(hsize);
		       * 
		       * varf3=ptr2[0];
		       * 
		       * ptr2++; hsize++; do { (*out2++)=(char)varf3; varf3+=varf2; }while(--hsize); } } } 
		       */
		       out += 640;
		   }
	       while (--renderLoop);

	       break;
	   }
	default:
	   {
	      // printf("Unsuported render type %d\n",polyRenderType);
	       break;
	   }
	};
}

int LBA_renderer::prepareRender(void)
{
    short int vertexX, vertexY;
    short int *ptr1, *ptr3;
    int i;
    short int psh1, psh2;
    char direction = 1;
    int echange;
    short int oldVertexX, oldVertexY;
    short int currentVertexX, currentVertexY;
    short int size;
    int temp2, temp4, temp5, temp6;
    int step;
    float vfloat, vfloat2;
    float vcfloat, vcfloat2;

    pRenderV1 = vertexCoordinates;
    pRenderV2 = pRenderV3;
    numOfVertexRemaining = numOfVertex;
    polyCropped = 0;

    vleft = vtop = 32767;
    vright = vbottom = -32768;

    ptr1 = vertexCoordinates;

    for (i = 0; i < numOfVertex; i++)
	{
	    ptr1++;		// discarding the 1st parameter

	    vertexX = *(ptr1++);

	    if (vertexX < vleft)
		vleft = vertexX;
	    if (vertexX > vright)
		vright = vertexX;

	    vertexY = *(ptr1++);

	    if (vertexY < vtop)
		vtop = vertexY;
	    if (vertexY > vbottom)
		vbottom = vertexY;
	}

    ptr1[0] = pRenderV1[0];
    ptr1[1] = pRenderV1[1];
    ptr1[2] = pRenderV1[2];

    if (vbottom < vtop)
	return (0);

    if (vleft < textWindowLeft)
	{
	    if (vright < textWindowLeft)
		return (0);
	   // pRenderSub();
	   // printf("3Dcropping...\n");
	   // exit(1);
	   // if(vright==textWindowLeft)
	    return (2);
	}

    if (vright > textWindowRight)
	{
	    if (vleft > textWindowRight)
		return (0);
	   // pRenderSub2();
	   // printf("3Dcropping...\n");
	   // exit(1);
	   // if(vleft==textWindowRight)
	    return (2);
	}

    if (vtop < textWindowTop)
	{
	    if (vbottom < textWindowTop)
		return (0);
	   // pRenderSub3();
	   // printf("3Dcropping...\n");
	   // exit(1);
	   // if(vbottom==textWindowBottom)
	    return (2);
	}

    if (vbottom > textWindowBottom)
	{
	    if (vtop > textWindowBottom)
		return (0);
	   // pRenderSub4();
	   // printf("3Dcropping...\n");
	   // exit(1);
	   // if(vtop==textWindowBottom)
	    return (2);
	}

    if (polyCropped)
	{
	    printf("prepareRender-> cropped poly !\n");
	    exit(1);
	}

    ptr1 = pRenderV1;		// on retourne au debut de la liste

    vertexParam1 = vertexParam2 = (*(ptr1++)) & 0xFF;
    oldVertexX = *(ptr1++);
    oldVertexY = *(ptr1++);

    do
	{
	    oldVertexParam = vertexParam1;

	    vertexParam1 = vertexParam2 = (*(ptr1++)) & 0xFF;
	    currentVertexX = *(ptr1++);
	    currentVertexY = *(ptr1++);

	   // drawLine(oldVertexX,oldVertexY,currentVertexX,currentVertexY,255);

	    if (currentVertexY == oldVertexY)	// since it's scanline based, we don't care when we 
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
	       // are only moving along X
		{
		    oldVertexX = size = currentVertexX;
		}
	    else
		{
		    psh1 = currentVertexX;	// let's save the current coordinates since we are going to 
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
		   // modify the values
		    psh2 = currentVertexY;

		    if (currentVertexY < oldVertexY)	// if we are going up
			{
			    size = oldVertexY - currentVertexY;
			    direction = -1;

			    if (oldVertexX < currentVertexX)	// if we are going up right
				{
				    echange = oldVertexX;	// we invert the vertex to draw from new to old
				    oldVertexX = currentVertexX;
				    currentVertexX = echange;

				    echange = currentVertexY;
				    currentVertexY = oldVertexY;

				    oldVertexY = echange;

				    echange = oldVertexParam;
				    oldVertexParam = vertexParam2;
				    vertexParam2 = echange;

				    direction = 1;	// we will draw by going down the tab
				}

			    temp2 = oldVertexY;	// temp2 is the starting Y position
			    oldVertexY = size;	// oldVertexY now become the number of pixel
			    size = temp2 * 2;

			    ptr3 = &polyTab[temp2 + 480];	// ptr3 is the output ptr in the renderTab

			    temp4 = ((oldVertexX - currentVertexX) << 16);	// temp4 = size in X << 16

			    temp5 = temp4 / oldVertexY;	// temp5 is the size of a step << 16
			    temp6 = temp4 % oldVertexY;	// temp6 is the remaining << 16

			    vfloat = ((float) (oldVertexX - currentVertexX)) / ((float) oldVertexY);

			    temp6 >>= 1;
			    temp6 += 0x7FFF;

			    step = (unsigned short) temp5;	// retrieve the size of a step

			   // temp7 = (((unsigned short)temp6) | ((oldVertexX & 0xFFFF)<<16));
			    vfloat2 = oldVertexX;

			    oldVertexX = oldVertexY;	// oldVertexX is now the number of vertical pixels

			    oldVertexY += 2;

			    for (i = 0; i < oldVertexY; i++)
				{
				   // *(ptr3)=((temp7&0xFFFF0000)>>16);
				    *(ptr3) = (short int) vfloat2;
				    ptr3 += direction;
				   // temp7+=step;
				    vfloat2 -= vfloat;
				}

			    if (polyRenderType >= 7)
				{
				    ptr3 = &polyTab2[temp2 + 480];

				    temp4 = ((vertexParam2 - oldVertexParam));

				    if (temp4 >= 0)
					{
					   /*
					    * temp5=temp4/oldVertexX; temp6=temp4%oldVertexX;
					    */

					    vcfloat = ((float) (temp4)) / ((float) oldVertexX);

					   /*
					    * (*(unsigned char*)&temp6)>>=1; (*(unsigned char*)&temp6)+=0x7F;
					    */

					   // temp6=(temp6&0xFF) | (oldVertexParam)<<8;

					    vcfloat2 = oldVertexParam;

					   // temp6=oldVertexParam;

					    oldVertexX += 2;

					    for (i = 0; i <= oldVertexX; i++)
						{
						    *(ptr3) = (short int) vcfloat2;
						    ptr3 += direction;
						    vcfloat2 += vcfloat;
						}
					}
				    else
					{
					    temp5 = temp4 / oldVertexX;
					    temp6 = temp4 % oldVertexX;

					    vcfloat = -((float) (temp4)) / ((float) oldVertexX);

					    (*(unsigned char *) &temp6) >>= 1;
					    (*(unsigned char *) &temp6) += 0x7F;

					    temp6 = (temp6 & 0xFF) | (oldVertexParam & 0xFF) << 8;

					    vcfloat2 = oldVertexParam;

					    temp6 = oldVertexParam;

					    oldVertexX += 2;

					    for (i = 0; i <= oldVertexX; i++)
						{
						    *(ptr3) = (short int) vcfloat2;
						    ptr3 += direction;
						    vcfloat2 -= vcfloat;
						}
					}
				}
			    direction = 1;
			    oldVertexY = psh2;
			    oldVertexX = psh1;
			}

		    else	// if we are going down
			{

			    size = currentVertexY - oldVertexY;	// size is the number of pixel we must go
			   // verticaly

			    if (oldVertexX > currentVertexX)	// if we are going down and to the left
				{
				    echange = oldVertexX;	// in that case, we will draw the line the other
				   // side (from new point to old point)
				    oldVertexX = currentVertexX;
				    currentVertexX = echange;

				    echange = currentVertexY;
				    currentVertexY = oldVertexY;
				    oldVertexY = echange;

				    echange = oldVertexParam;
				    oldVertexParam = vertexParam2;
				    vertexParam2 = echange;

				    direction = -1;	// since we are going backward in the screen
				}

			    temp2 = oldVertexY;	// temp2 is the starting Y position
			    oldVertexY = size;	// oldVertexY now become the number of pixel
			    size = temp2 * 2;

			    ptr3 = &polyTab[temp2];	// ptr3 is the output ptr in the renderTab

			    temp4 = ((currentVertexX - oldVertexX) << 16);	// temp4 = size in X << 16

			    temp5 = temp4 / oldVertexY;	// temp5 is the size of a step << 16
			    temp6 = temp4 % oldVertexY;	// temp6 is the remaining << 16

			    vfloat = ((float) (currentVertexX - oldVertexX)) / ((float) oldVertexY);

			    temp6 >>= 1;
			    temp6 += 0x7FFF;

			    step = (unsigned short) temp5;	// retrieve the size of a step

			   // temp7 = (((unsigned short)temp6) | ((oldVertexX & 0xFFFF)<<16));
			    vfloat2 = oldVertexX;

			    oldVertexX = oldVertexY;	// oldVertexX is now the number of vertical pixels

			    oldVertexY += 2;

			    for (i = 0; i < oldVertexY; i++)
				{
				   // *(ptr3)=((temp7&0xFFFF0000)>>16);
				    *(ptr3) = (short int) vfloat2;
				    ptr3 += direction;
				   // temp7+=step;
				    vfloat2 += vfloat;
				}

			    if (polyRenderType >= 7)
				{
				    ptr3 = &polyTab2[temp2];

				    temp4 = ((vertexParam2 - oldVertexParam));

				    if (temp4 >= 0)
					{
					    temp5 = temp4 / oldVertexX;
					    temp6 = temp4 % oldVertexX;

					    vcfloat =
						((float) (vertexParam2 - oldVertexParam)) /
						((float) oldVertexX);

					    (*(unsigned char *) &temp6) >>= 1;
					    (*(unsigned char *) &temp6) += 0x7F;

					    temp6 = (temp6 & 0xFF) | (oldVertexParam) << 8;

					    vcfloat2 = oldVertexParam;

					    temp6 = oldVertexParam;

					    oldVertexX += 2;

					    for (i = 0; i <= oldVertexX; i++)
						{
						    *(ptr3) = (short int) vcfloat2;
						    ptr3 += direction;
						    vcfloat2 += vcfloat;
						}
					}
				    else
					{
					    temp5 = temp4 / oldVertexX;
					    temp6 = temp4 % oldVertexX;

					    vcfloat =
						-((float) (vertexParam2 - oldVertexParam)) /
						((float) oldVertexX);

					    (*(unsigned char *) &temp6) >>= 1;
					    (*(unsigned char *) &temp6) += 0x7F;

					    temp6 = (temp6 & 0xFF) | (oldVertexParam & 0xFF) << 8;

					    vcfloat2 = oldVertexParam;

					    temp6 = oldVertexParam;

					   // oldVertexX+=2;

					    for (i = 0; i <= oldVertexX; i++)
						{
						    *(ptr3) = (short int) vcfloat2;
						    ptr3 += direction;
						    vcfloat2 -= vcfloat;
						}
					}
				}

			    direction = 1;
			    oldVertexY = psh2;
			    oldVertexX = psh1;
			}
		}
	}
    while (--numOfVertexRemaining);

    return (1);
}

void LBA_renderer::drawLine(int a, int b, int c, int d, int e)
{
    int temp;
    short int flag;
    int flag2;
    unsigned char *out;
    short int color;
    short int var2;
    short int xchg;
    int stringProcessVar = e;

    if (a > c)			// pour toujours dessiner de gauche à droite
	{
	    temp = c;
	    c = a;
	    a = temp;

	    temp = d;
	    d = b;
	    b = temp;

	}

    flag = 0;

    if (a < textWindowLeft)
	{
	    flag |= 1;
	}
    else
	{
	    if (a > textWindowRight)
		return;
	}
    if (b < textWindowTop)
	{
	    flag |= 8;
	}
    else
	{
	    if (b > textWindowBottom)
		flag |= 4;
	}

    flag <<= 8;

    if (c < textWindowLeft)
	return;
    if (c <= textWindowLeft)
	flag |= 2;

    if (d < textWindowTop)
	{
	    flag |= 8;
	}
    else
	{
	    if (d > textWindowBottom)
		flag |= 4;
	}

    flag2 = flag;

    if (flag)
	return;

   // implementer la suite

    flag2 = 640;		// esi
    c -= a;
    d -= b;
    if (d < 0)
	{
	    flag2 = -flag2;
	    d = -d;
	}

    out = videoBuffer1 + screenLockupTable[b] + a;

    color = stringProcessVar;
    if (c < d)			// pente importante
	{
	    xchg = c;
	    c = d;
	    d = xchg;
	    var2 = c;
	    var2 <<= 1;
	    b = c;
	    d <<= 1;
	    c++;
	    do
		{
		    *out = (unsigned char) color;
		    b -= d;
		    if (b > 0)
			{
			    out += flag2;
			}
		    else
			{
			    b += var2;
			    out += flag2 + 1;
			}
		}
	    while (--c);
	}
    else			// pente reduite
	{
	    var2 = c;
	    var2 <<= 1;
	    b = c;
	    d <<= 1;
	    c++;
	    do
		{
		    *out = (unsigned char) color;
		    out++;
		    b -= d;
		    if (b < 0)
			{
			    b += var2;
			    out += flag2;
			}
		}
	    while (--c);
	}
}
