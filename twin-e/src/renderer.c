/*
Copyright (C) 2002-2010 The TwinE team

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#include <math.h>

#include "lba.h"

#include "text.h"
#include "main.h"

#include "renderer.h"

#define vertexDebug(id) {}
/*#define vertexDebug(id) if (id[0] == 'U') printf(id "Got polyTab[%d + %d] => %ld\n", temp2, videoHeight, ptr3 - polyTab); \
								   else printf(id "Got polyTab[%d] => %ld\n", temp2, ptr3 - polyTab);*/

pointTab _projectedPointTable[800];
pointTab _flattenPointTable[800];

renderTabEntry *renderTabEntryPtr;

renderTabEntry renderTab[1000];
renderTabEntry renderTabSorted[1000];
unsigned char renderTab7[10000];

static const int videoWidth = 640;
static const int videoHeight = 480;

unsigned char *_pointsPtr;
unsigned char *_partsPtr;

unsigned char *_currentMatrixTableEntry;

int _baseMatrix[3 * 3];

int _matrixTable[271];  // should be matrixes

int _shadeMatrix[9];
int _lightX;
int _lightY;
int _lightZ;

short int *tab1;
short int *tab2;
short int *tab3;

short int polyTab[960];
short int polyTab2[960];

int isUsingOrhoProjection;

short int shadeTable[500];

short int primitiveCounter;

short int vtop;
short int vbottom;

int setSomething3Var12;
int setSomething3Var14;
int setSomething3Var16;

int cameraAngleX;
int cameraAngleZ;
int cameraAngleY;

int cameraVar1;
int cameraVar2;
int cameraVar3;

int destX;
int destY;
int destZ;

int cameraPosX = 320;
int cameraPosZ = 200;

int renderBottom;
int renderLeft;
int renderRight;
int renderTop;

short int vertexCoordinates[193] = { 0x1234, 0 };

int _angleX, _angleY, _angleZ, _X, _Y, _Z, _numOfPrimitives;

int AffObjetIso(int X, int Y, int Z, int angleX, int angleY, int angleZ, unsigned char *costumePtr) {
	unsigned char *ptr;
	short int costumeHeader;

	tab1 = &angleTable[0];
	tab2 = &angleTable[256];
	tab3 = &angleTable[384];

	_angleX = angleX;
	_angleY = angleY;
	_angleZ = angleZ;

	/*
	 * Those 4 vars are used to know the size of the rendered model. They are resetted to their
	 * maximum value at the begining of the renderer
	 */

	renderLeft = 32767;
	renderTop = 32767;

	renderRight = -32767;
	renderBottom = -32767;

	if (isUsingOrhoProjection == 0) {
		setOrthoCameraAngle(X, Y, Z);

		_X = destX - setSomething3Var12;
		_Y = destZ - setSomething3Var14;
		_Z = destY - setSomething3Var16;
	} else {
		_X = X;
		_Y = Y;
		_Z = Z;
	}

	_numOfPrimitives = 0; // we reset the number of primitives in the model

	renderTabEntryPtr = renderTab;  // we restart at the beginning of the renderTable

	costumeHeader = READ_LE_U16(costumePtr);

	ptr = costumePtr + 16 + READ_LE_U16(costumePtr + 14); // we jump after the header

	if (costumeHeader & 2) { // if animated
		return renderAnimatedModel(ptr);  // That's the mostly used renderer code
	} else {
		printf("Unsupported unanimated model render!\n");
		exit(1);
	}

	return 0;
}

void configureOrthoProjection(int x, int z) {
	cameraPosX = x;
	cameraPosZ = z;
	isUsingOrhoProjection = 1;
}

void SetLightVector(int angleX, int angleY, int angleZ) {
	tab1 = &angleTable[0];
	tab2 = &angleTable[256];
	tab3 = &angleTable[384];

	_angleX = angleX;
	_angleY = angleY;
	_angleZ = angleZ;

	RotMatIndex2(_shadeMatrix, _baseMatrix);
	TranslateGroupeS2(0, 0, 59);

	_lightX = destX;
	_lightY = destZ;
	_lightZ = destY;
}

void TranslateGroupeS2(short int ax, short int bx, short int cx) {
	int ebp;
	int ebx;
	int ecx;
	int eax;
	int edi;

	ebp = ax;
	ebx = bx;
	ecx = cx;

	edi = _shadeMatrix[0];
	eax = _shadeMatrix[1];
	edi *= ebp;
	eax *= ebx;
	edi += eax;
	eax = _shadeMatrix[2];
	eax *= ecx;
	eax += edi;
	eax >>= 14;

	destX = eax;

	edi = _shadeMatrix[3];
	eax = _shadeMatrix[4];
	edi *= ebp;
	eax *= ebx;
	edi += eax;
	eax = _shadeMatrix[5];
	eax *= ecx;
	eax += edi;
	eax >>= 14;
	destZ = eax;

	ebp *= _shadeMatrix[6];
	ebx *= _shadeMatrix[7];
	ecx *= _shadeMatrix[8];
	ebx += ebp;
	ebx += ecx;
	ebx >>= 14;
	destY = eax;
}

int renderAnimatedModel(unsigned char *costumePtr) {
	pointEntry *ptEntryPtr;
	pointTab *pointPtr;
	pointTab *pointPtrDest;
	int coX;
	int coY;
	int coZ;
	unsigned char *ptr3;
	int *ptr4;
	unsigned char *ptr6;
	int processNormal, numNormal;
	unsigned char *_partsPtr2;

	int *_shadePtr;

	int _numOfParts;
	int _numOfPoints = READ_LE_U16(costumePtr);
	costumePtr += 2;
	_pointsPtr = costumePtr;

	costumePtr += _numOfPoints * 6;

	_numOfParts = READ_LE_U16(costumePtr);
	costumePtr += 2;
	_partsPtr = _partsPtr2 = costumePtr;

	_currentMatrixTableEntry = (unsigned char *) _matrixTable;

	loadPart(_angleX, _angleY, _angleZ, (unsigned char *) _partsPtr);

	_partsPtr += 38;

	ptEntryPtr = (pointEntry *) _partsPtr;

	if (_numOfParts != 1) {
		_numOfPrimitives = _numOfParts - 1;
		_currentMatrixTableEntry = (unsigned char *) & _matrixTable[9];

		do {
			int boneType = READ_LE_U16(((unsigned char*)ptEntryPtr) + 8);
			if (boneType == 0) {
				loadPart(ptEntryPtr->rotateX, ptEntryPtr->rotateY, ptEntryPtr->rotateZ, (unsigned char *) ptEntryPtr);  // rotation
			} else
				if (boneType == 1) {
					TranslateGroupe(ptEntryPtr->rotateX, ptEntryPtr->rotateY, ptEntryPtr->rotateZ, ptEntryPtr); // translation
				}

			_currentMatrixTableEntry += 36;
			_partsPtr += 38;
			ptEntryPtr = (pointEntry *) _partsPtr;

		} while (--_numOfPrimitives);
	}

	_numOfPrimitives = _numOfPoints;

	/* Loading position */
	pointPtr = (pointTab *) _projectedPointTable;
	pointPtrDest = (pointTab *) _flattenPointTable;

	if (isUsingOrhoProjection != 0) { // use standard projection
		do {
			coX = pointPtr->x + _X;
			coY = pointPtr->y + _Y;
			coZ = -(pointPtr->z + _Z);

#ifdef USE_FLOAT
			pointPtrDest->x = (coX + coZ) * 24 / 512.f + cameraPosX;
			pointPtrDest->y = (((coX - coZ) * 12) - coY * 30) / 512.f + cameraPosZ;
			pointPtrDest->z = coZ - coX - coY;
#else
			pointPtrDest->x = (coX + coZ) * 24 / 512 + cameraPosX;
			pointPtrDest->y = (((coX - coZ) * 12) - coY * 30) / 512 + cameraPosZ;
			pointPtrDest->z = coZ - coX - coY;
#endif

			if (pointPtrDest->x < renderLeft)
				renderLeft = pointPtrDest->x;
			if (pointPtrDest->x > renderRight)
				renderRight = pointPtrDest->x;

			if (pointPtrDest->y < renderTop)
				renderTop = pointPtrDest->y;
			if (pointPtrDest->y > renderBottom)
				renderBottom = pointPtrDest->y;

			pointPtr++;
			pointPtrDest++;
		} while (--_numOfPrimitives);
	} else {
		do {
			coX = pointPtr->x + _X;
			coY = pointPtr->y + _Y;
			coZ = -(pointPtr->z + _Z);

			coZ += cameraVar1;

			if (coZ <= 0)
				coZ = 0x7FFFFFFF;

			/* X */
			coX = (coX * cameraVar2) / coZ + cameraPosX;

			if (coX > 0xFFFF)
				coX = 0x7FFF;

			pointPtrDest->x = coX;

			if (pointPtrDest->x < renderLeft)
				renderLeft = pointPtrDest->x;

			if (pointPtrDest->x > renderRight)
				renderRight = pointPtrDest->x;
			

			/* Y */
			coY = (-coY * cameraVar3) / coZ + cameraPosZ;

			if (coY > 0xFFFF)
				coY = 0x7FFF;

			pointPtrDest->y = coY;

			if (pointPtrDest->y < renderTop)
				renderTop = pointPtrDest->y;
			if (pointPtrDest->y > renderBottom)
				renderBottom = pointPtrDest->y;

			/* Z */
			if (coZ > 0xFFFF)
				coZ = 0x7FFF;

			pointPtrDest->z = coZ;


			pointPtr++;
			pointPtrDest++;

		} while (--_numOfPrimitives);
	}

	_shadePtr = (int *) _partsPtr;

	processNormal = READ_LE_U16(_shadePtr);

	_shadePtr = (int *)(((unsigned char *) _shadePtr) + 2);

	if (processNormal) {   // process normal data
		int eax;
		int edi;

		unsigned char *_currentShadeDestination = (unsigned char *) shadeTable;
		int *renderV21 = _matrixTable;
		unsigned char *pri2Ptr2;

		_numOfPrimitives = _numOfParts;

		ptr3 = pri2Ptr2 = _partsPtr2 + 18;

		while (_numOfPrimitives--) {
			numNormal = READ_LE_U16(ptr3);

			_shadeMatrix[0] = renderV21[0] * _lightX;
			_shadeMatrix[1] = renderV21[1] * _lightX;
			_shadeMatrix[2] = renderV21[2] * _lightX;

			_shadeMatrix[3] = renderV21[3] * _lightY;
			_shadeMatrix[4] = renderV21[4] * _lightY;
			_shadeMatrix[5] = renderV21[5] * _lightY;

			_shadeMatrix[6] = renderV21[6] * _lightZ;
			_shadeMatrix[7] = renderV21[7] * _lightZ;
			_shadeMatrix[8] = renderV21[8] * _lightZ;

			while (numNormal--) { // for each normal
				short int col1;
				short int col2;
				short int col3;

				short int *colPtr;

				colPtr = (short int *) _shadePtr;

				col1 = READ_LE_S16(colPtr++);
				col2 = READ_LE_S16(colPtr++);
				col3 = READ_LE_S16(colPtr++);

				eax = _shadeMatrix[0] * col1 + _shadeMatrix[1] * col2 + _shadeMatrix[2] * col3;
				eax += _shadeMatrix[3] * col1 + _shadeMatrix[4] * col2 + _shadeMatrix[5] * col3;
				eax += _shadeMatrix[6] * col1 + _shadeMatrix[7] * col2 + _shadeMatrix[8] * col3;

				edi = 0;

				if (eax > 0) {
					eax >>= 14;
					ptr6 = (unsigned char *) _shadePtr;
					eax /= READ_LE_U16(ptr6 + 6);
					edi = (unsigned short int) eax;
				}

				WRITE_LE_U16(_currentShadeDestination, edi);
				_currentShadeDestination += 2;
				_shadePtr += 2;
			}
			ptr3 = pri2Ptr2 = pri2Ptr2 + 38;
			ptr4 = renderV21 = renderV21 + 9;
		}
	}

	return (finishRender((unsigned char *) _shadePtr));
}

void loadPart(int edx, int ecx, int ebx, unsigned char * ptr) {
	int *ebp;
	short int var;

	int rs1v1 = READ_LE_S16(ptr);
	int rs1v2 = READ_LE_S16(ptr + 2);

	_angleX = ebx;
	_angleZ = ecx;
	_angleY = edx;

	if (rs1v1 % 6) {
		printf("Error: rs1V1\n");
		exit(1);
	}

	var = READ_LE_U16(ptr + 6);

	if (var == -1) {  // si c'est le premier point
		ebp = _baseMatrix;

		destX = 0;
		destY = 0;
		destZ = 0;
	} else {
		int pointIdx = (READ_LE_U16(ptr + 4)) / 6;
		ebp = (int *)((unsigned char *) _matrixTable + var);

		destX = _projectedPointTable[pointIdx].x;
		destY = _projectedPointTable[pointIdx].y;
		destZ = _projectedPointTable[pointIdx].z;
	}

	RotMatIndex2((int *) _currentMatrixTableEntry, ebp);

	if (!rs1v2) {
		printf("No points !\n");
	}

	RotList(_pointsPtr + rs1v1, rs1v2, &_projectedPointTable[rs1v1 / 6], (int *) _currentMatrixTableEntry); // rotation des elements du model
}

void RotList(unsigned char *esi, int ecx, pointTab * dest, int *eax) {
	short int param1;
	short int param2;
	short int param3;

	short int *tempPtr;

	while (ecx--) {
		tempPtr = (short int *)(esi);

		param1 = tempPtr[0];
		param2 = tempPtr[1];
		param3 = tempPtr[2];

		dest->x = ((eax[0] * param1 + eax[1] * param2 + eax[2] * param3) >> 14) + destX;
		dest->y = ((eax[3] * param1 + eax[4] * param2 + eax[5] * param3) >> 14) + destY;
		dest->z = ((eax[6] * param1 + eax[7] * param2 + eax[8] * param3) >> 14) + destZ;

		dest++;
		esi += 6;
	}
}

void RotMatIndex2(int *eax, int *ebp) {
	int angle;
	int angleVar1;	// esi
	int angleVar2;	// ecx

	int matrix1[9];
	int matrix2[9];

	if (_angleX) {  // rotation par vers l'avant
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

	} else {
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

	if (_angleY) {
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
	} else {
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

	if (_angleZ) {  // rotation de coté (la plus courante)
		angle = _angleZ;
		angleVar2 = tab1[angle & 0x3FF];  // esi
		angle += 0x100;
		angleVar1 = tab1[angle & 0x3FF];  // ecx

		eax[1] = matrix2[1];
		eax[4] = matrix2[4];
		eax[7] = matrix2[7];

		eax[0] = (matrix2[0] * angleVar1 - matrix2[2] * angleVar2) >> 14;
		eax[2] = (matrix2[0] * angleVar2 + matrix2[2] * angleVar1) >> 14;
		eax[3] = (matrix2[3] * angleVar1 - matrix2[5] * angleVar2) >> 14;
		eax[5] = (matrix2[3] * angleVar2 + matrix2[5] * angleVar1) >> 14;

		eax[6] = (matrix2[6] * angleVar1 - matrix2[8] * angleVar2) >> 14;
		eax[8] = (matrix2[6] * angleVar2 + matrix2[8] * angleVar1) >> 14;
	} else {
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

void TranslateGroupe(int edx, int ecx, int ebx, pointEntry * esi) {
	int *dest;
	int *source;

	_angleZ = ebx;
	_angleY = ecx;
	_angleX = edx;

	if (esi->param == -1) { // base point
		destX = 0;
		destZ = 0;
		destY = 0;

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
	} else {   // dependent
		destX = _projectedPointTable[(esi->data3) / 6].x; // todo:
		destZ = _projectedPointTable[(esi->data3) / 6].z; // inverse because pointab and dest entry are not in the same order....
		destY = _projectedPointTable[(esi->data3) / 6].y;

		source = (int *)((unsigned char *) _matrixTable + esi->param);
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

	TransRotList(_pointsPtr + esi->data1, esi->data2, &_projectedPointTable[esi->data1 / 6], (int *) _currentMatrixTableEntry);
}

void TransRotList(unsigned char *esi, int ecx, pointTab * dest, int *eax) {
	short int param1;
	short int param2;
	short int param3;
	short int *tempPtr;

	while (ecx--) {
		tempPtr = (short int *)(esi);

		param1 = tempPtr[0] + _angleZ;
		param2 = tempPtr[1] + _angleY;
		param3 = tempPtr[2] + _angleX;

		dest->x = ((eax[0] * param1 + eax[1] * param2 + eax[2] * param3) >> 14) + destX;
		dest->y = ((eax[3] * param1 + eax[4] * param2 + eax[5] * param3) >> 14) + destY;
		dest->z = ((eax[6] * param1 + eax[7] * param2 + eax[8] * param3) >> 14) + destZ;

		dest++;
		esi += 6;
	}
}

void circle_fill(int x, int y, int radius, byte color) {
	int currentLine;

	radius += 1;

	for (currentLine = -radius; currentLine <= radius; currentLine++) {
		double width;

		if (abs(currentLine) != radius) {
			width = sin(acos((float)currentLine / (float)radius));
		} else {
			width = 0;
		}

		width *= radius;

		if (width < 0)
			width = - width;

		drawLine(x - width, currentLine + y, x + width, currentLine + y, color);
	}
}

int finishRender(unsigned char *esi) {
	unsigned char *edi;
	short int temp;
	int eax, ecx;

	short int counter;
	short int type;
	short int color;

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

	unsigned char *destPtr;
	int i;

	unsigned char *render23;
	unsigned char *render24;
	int render25;

	short int drawType;

	unsigned char *renderV19;

	polyVertexHeader *currentPolyVertex;
	polyHeader *currentPolyHeader;
	polyHeader *destinationHeader;
	computedVertex *currentComputedVertex;
	pointTab *currentVertex;
	pointTab *destinationVertex;

	renderTabEntry *renderTabEntryPtr2;
	renderTabEntry *renderTabSortedPtr;

	edi = renderTab7;   // renderTab7 c'est le buffer de coordonnées
	temp = READ_LE_S16(esi);  // we read the number of polygones
	esi += 2;

	if (temp) {   // if there is polygones
		primitiveCounter = temp;  // the number of primitives = the number of polygones

		do {	// loop that load all the polygones
			render23 = edi;
			currentPolyHeader = (polyHeader *) esi;
			ecx = READ_LE_S32(esi);
			esi += 2;
			drawType = currentPolyHeader->FillVertic_AType;

			assert(drawType <= 10);

			if (drawType >= 9) {
				destinationHeader = (polyHeader *) edi;

				destinationHeader->FillVertic_AType = currentPolyHeader->FillVertic_AType - 2;
				destinationHeader->numOfVertex = currentPolyHeader->numOfVertex;
				destinationHeader->colorIndex = currentPolyHeader->colorIndex;

				esi += 2;
				edi += 4;

				counter = destinationHeader->numOfVertex;

				bestDepth = -32000;
				renderV19 = edi;

				do {
					currentPolyVertex = (polyVertexHeader *) esi;

					shadeValue = currentPolyHeader->colorIndex + shadeTable[currentPolyVertex->shadeEntry];

					currentComputedVertex = (computedVertex *) edi;

					currentComputedVertex->shadeValue = shadeValue;

					currentVertex = &_flattenPointTable[currentPolyVertex->dataOffset / 6];
					destinationVertex = (pointTab *)(edi + 2);

					destinationVertex->x = currentVertex->x;
					destinationVertex->y = currentVertex->y;

					edi += 6;
					esi += 4;

					currentDepth = currentVertex->z;

					if (currentDepth > bestDepth)
						bestDepth = currentDepth;
				} while (--counter);

			} else if (drawType >= 7) { // only 1 shade value is used
				destinationHeader = (polyHeader *) edi;

				destinationHeader->FillVertic_AType = currentPolyHeader->FillVertic_AType - 7;
				destinationHeader->numOfVertex = currentPolyHeader->numOfVertex;

				color = currentPolyHeader->colorIndex;

				shadeEntry = READ_LE_S16(esi + 2);

				esi += 4;

				WRITE_LE_S16(edi + 2, color + shadeTable[shadeEntry]);

				edi += 4;
				renderV19 = edi;
				bestDepth = -32000;
				counter = destinationHeader->numOfVertex;


				do {
					eax = READ_LE_S16(esi);
					esi += 2;

					currentVertex = &_flattenPointTable[eax / 6];

					destinationVertex = (pointTab *)(edi + 2);

					destinationVertex->x = currentVertex->x;
					destinationVertex->y = currentVertex->y;

					edi += 6;

					currentDepth = currentVertex->z;

					if (currentDepth > bestDepth)
						bestDepth = currentDepth;
				} while (--counter);

			} else { // no shade is used
				destinationHeader = (polyHeader *) edi;

				destinationHeader->FillVertic_AType = currentPolyHeader->FillVertic_AType;
				destinationHeader->numOfVertex = currentPolyHeader->numOfVertex;
				destinationHeader->colorIndex = currentPolyHeader->colorIndex;

				esi += 2;
				edi += 4;

				bestDepth = -32000;
				renderV19 = edi;
				eax = 0;
				counter = currentPolyHeader->numOfVertex;

				do {
					eax = READ_LE_S16(esi);
					esi += 2;

					currentVertex = &_flattenPointTable[eax / 6];

					destinationVertex = (pointTab *)(edi + 2);

					destinationVertex->x = currentVertex->x;
					destinationVertex->y = currentVertex->y;

					edi += 6;

					currentDepth = currentVertex->z;

					if (currentDepth > bestDepth)
						bestDepth = currentDepth;
				} while (--(counter));
			}

			render24 = edi;
			edi = renderV19;

			render25 = bestDepth;

			ax = READ_LE_S16(edi + 4);
			bx = READ_LE_S16(edi + 8);

			ax -= READ_LE_S16(edi + 16);
			bx -= READ_LE_S16(edi + 2);

			ax *= bx;

			bestDepth = ax;
			bx = currentDepth;

			ax = READ_LE_S16(edi + 2);
			cx = READ_LE_S16(edi + 10);

			ax -= READ_LE_S16(edi + 14);
			cx -= READ_LE_S16(edi + 4);

			ax *= cx;

			ax -= bestDepth;
			currentDepth -= bx - 1; // peut-etre une erreur la

			if (currentDepth < 0) {
				edi = render23;
			} else {
				_numOfPrimitives++;

				renderTabEntryPtr->depth = render25;
				renderTabEntryPtr->renderType = 1;
				renderTabEntryPtr->dataPtr = render23;
				renderTabEntryPtr++;

				edi = render24;
			}
		} while (--primitiveCounter);
	}

	temp = READ_LE_S16(esi);
	esi += 2;
	if (temp) {   // pour les lignes (0)
		_numOfPrimitives += temp;
		do {
			int param;
			lineDataPtr = (lineData *) esi;
			lineCoordinatesPtr = (lineCoordinates *) edi;

			if (READ_LE_S16(&lineDataPtr->p1) % 6 != 0 || READ_LE_S16(&lineDataPtr->p2) % 6 != 0) {
				printf("lineDataPtr reference is malformed !\n");
				exit(1);
			}

			point1 = READ_LE_S16(&lineDataPtr->p1) / 6;
			point2 = READ_LE_S16(&lineDataPtr->p2) / 6;
			param = READ_LE_S32(&lineDataPtr->data);
			WRITE_LE_S32(&lineCoordinatesPtr->data, param);
			WRITE_LE_S16(&lineCoordinatesPtr->x1, _flattenPointTable[point1].x);
			WRITE_LE_S16(&lineCoordinatesPtr->y1, _flattenPointTable[point1].y);
			WRITE_LE_S16(&lineCoordinatesPtr->x2, _flattenPointTable[point2].x);
			WRITE_LE_S16(&lineCoordinatesPtr->y2, _flattenPointTable[point2].y);
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
		} while (--temp);
	}

	temp = READ_LE_S16(esi);
	esi += 2;
	if (temp) {   // pour les cercles (2)
		_numOfPrimitives += temp;
		do {
			unsigned char color = *(esi + 1);
			short int center = READ_LE_U16(esi + 6);
			short int size = READ_LE_U16(esi + 4);

			*(unsigned char*)edi = color;
			WRITE_LE_S16(edi + 1, _flattenPointTable[center/6].x);
			WRITE_LE_S16(edi + 3, _flattenPointTable[center/6].y);
			WRITE_LE_S16(edi + 5, size);

			renderTabEntryPtr->depth = _flattenPointTable[center/6].z;
			renderTabEntryPtr->renderType = 2;
			renderTabEntryPtr->dataPtr = edi;
			renderTabEntryPtr++;

			esi += 8;
			edi += 7;
		} while (--temp);
	}

	renderTabSortedPtr = renderTabSorted;

	for (i = 0; i < _numOfPrimitives; i++) { // then we sort the polygones (FIXME: very slow !)
		bestZ = -0x7FFF;
		for (j = 0; j < _numOfPrimitives; j++) {
			if (renderTab[j].depth > bestZ) {
				bestZ = renderTab[j].depth;
				bestPoly = j;
			}
		}
		renderTabSortedPtr->depth = renderTab[bestPoly].depth;
		renderTabSortedPtr->renderType = renderTab[bestPoly].renderType;
		renderTabSortedPtr->dataPtr = renderTab[bestPoly].dataPtr;
		renderTabSortedPtr++;
		renderTab[bestPoly].depth = -0x7FFF;
	}
	renderTabEntryPtr2 = renderTabSorted;

	if (_numOfPrimitives) {
		primitiveCounter = _numOfPrimitives;
		renderV19 = esi;

		do {
			type = renderTabEntryPtr2->renderType;
			esi = renderTabEntryPtr2->dataPtr;
			renderV19 += 8;

			switch (type) {
				case 0: { // draw a line

							unsigned int x1;
							unsigned int y1;
							unsigned int x2;
							unsigned int y2;

#ifndef PCLIKE
							break;
#endif
							lineCoordinatesPtr = (lineCoordinates *) esi;
							color = (READ_LE_S32(&lineCoordinatesPtr->data) & 0xFF00) >> 8;

							x1 = READ_LE_S16((unsigned short int*) & lineCoordinatesPtr->x1);
							y1 = READ_LE_S16((unsigned short int*) & lineCoordinatesPtr->y1);
							x2 = READ_LE_S16((unsigned short int*) & lineCoordinatesPtr->x2);
							y2 = READ_LE_S16((unsigned short int*) & lineCoordinatesPtr->y2);

							drawLine(x1, y1, x2, y2, color);

							break;
						}
#ifndef MACOSX
				case 1: { // draw a polygon
							short int drawType = *(esi++);
							int numOfVertex = *(esi++);
							color = READ_LE_S16(esi);
							esi += 2;

							assert(drawType <= 10);

							destPtr = (unsigned char *) vertexCoordinates;

							for (i = 0; i < (numOfVertex * 3); i++) {
								WRITE_LE_S16(destPtr, READ_LE_S16(esi));
								destPtr += 2;
								esi += 2;
							}

							drawVertices(numOfVertex, drawType);
							fillVertices(color, drawType); /* TODO: find out why color is limited */

							break;
						}
#endif
				case 2: { // draw a circle
							int circleParam1;
							int circleParam3;
							int circleParam4;
							int circleParam5;

							eax = *(int*) esi;

							circleParam1 = *(unsigned char*)esi;
							circleParam4 = READ_LE_S16(esi + 1);
							circleParam5 = READ_LE_S16(esi + 3);
							circleParam3 = READ_LE_S16(esi + 5);

							if (!isUsingOrhoProjection) {
								circleParam3 = (circleParam3 * cameraVar2) / (cameraVar1 + *(short int*)esi);
							} else {
								circleParam3 = (circleParam3 * 34) >> 9;
							}

							circleParam3 += 3;

							if (circleParam4 + circleParam3 > renderRight)
								renderRight = circleParam4 + circleParam3;

							if (circleParam4 - circleParam3 < renderLeft)
								renderLeft = circleParam4 - circleParam3;

							if (circleParam5 + circleParam3 > renderBottom)
								renderBottom = circleParam5 + circleParam3;

							if (circleParam5 - circleParam3 < renderTop)
								renderTop = circleParam5 - circleParam3;

							circleParam3 -= 3;

							circle_fill(circleParam4, circleParam5, circleParam3, circleParam1);
						}
						break;

				default:
						break;
			}

			esi = renderV19;
			renderTabEntryPtr2++;
		} while (--primitiveCounter);
	} else {
		renderRight = -1;
		renderBottom = -1;
		renderLeft = -1;
		renderTop = -1;
		return (-1);
	}

	return (0);
}

void fillFlat(unsigned char *out, short int *posPtr, short int *colorPtr, int color) {
	int i;
	for (i = posPtr[0]; i < posPtr[videoHeight]+1; i++)
		if (i >= 0 && i < videoWidth)
			out[i] = color;
}

void fillCopper(unsigned char *out, short int *posPtr, short int *colorPtr, int color) {
	int i;
	unsigned short int mask = 0x43DB;
	unsigned short int dx;
	short int start = posPtr[0];
	dx = (unsigned char)color;
	dx |= 0x300;

	for (i = posPtr[0]; i < posPtr[videoHeight]+1; i++) {
		start += mask;
		start = (start & 0xFF00) | ((start & 0xFF) & (unsigned char)(dx >> 8)) ;
		start = (start & 0xFF00) | ((start & 0xFF) + (dx & 0xFF));
		if (i >= 0 && i < videoWidth) {
			assert(out + i < frontVideoBuffer + videoWidth * videoHeight);
			out[i] = start & 0xFF;
		}
		mask = (mask << 2) | (mask >> 14);
		mask++;
	}
}

/* TODO: buggy ? */
void fillBopper(unsigned char *out, short int *posPtr, short int *colorPtr, int color) {
	int i;
	for (i = posPtr[0]; i < posPtr[videoHeight]+1; i++) {
		if (i >= 0 && i < videoWidth && i % 2) {
			assert(out + i < frontVideoBuffer + videoWidth * videoHeight);
			out[i] = color;
		}
	}
}

/* TODO */
void fillMarble(unsigned char *out, short int *posPtr, short int *colorPtr, int color) {

}

/* TODO */
void fillTele(unsigned char *out, short int *posPtr, short int *colorPtr, int color) {

}

/* TODO */
void fillTras(unsigned char *out, short int *posPtr, short int *colorPtr, int color) {

}

/* TODO: buggy */
void fillTrame(unsigned char *out, short int *posPtr, short int *colorPtr, int color) {
	int i;
	unsigned char bh = 0;
	unsigned char *out2 = out + posPtr[0];
	short int hsize = posPtr[videoHeight] - posPtr[0] + 1;
	hsize /= 2;
	if (hsize > 1) {
		unsigned short int ax;
		bh ^= 1;
		ax = (unsigned short int)*(int*) out2;
		ax &= 1;
		if (ax ^ bh) {
			assert(out2 < frontVideoBuffer + videoWidth * videoHeight);
			out2++;
		}

		for (i = 0; i < hsize; i++) {
			assert(out2 < frontVideoBuffer + videoWidth * videoHeight);
			*(out2) = (unsigned char)color;
			out2 += 2;
		}
	}
}

/* TODO: color bug ? */
void fillGouraud(unsigned char *out, short int *posPtr, short int *colorPtr, int color) {
	short int start = posPtr[0];
	short int hsize = posPtr[videoHeight] - start;
	unsigned char *out2 = out + start;
	unsigned short int startColor = colorPtr[0];
	unsigned short int stopColor = colorPtr[videoHeight];
	short int colorSize = stopColor - startColor;

	if (hsize == 0) {
		if (start >= 0 && start < videoWidth)
			*out2 = ((startColor + stopColor) / 2) >> 8;
	} else {
		colorSize /= hsize;
		hsize++;

		while (hsize--) {
			if (start >= 0 && start < videoWidth)
				*out2 = startColor >> 8;

			start++;
			startColor += colorSize;
			out2++;
		}
	}
}

/* TODO: color bug ? */
void fillDithering(unsigned char *out, short int *posPtr, short int *colorPtr, int color) {
	short int startColor = colorPtr[0];
	short int stopColor = colorPtr[videoHeight];
	short int colorSize = stopColor - startColor;
	short int start = posPtr[0];
	short int hsize = posPtr[videoHeight] - start;
	unsigned short int currentColor = startColor;
	unsigned char *out2 = out + start;

	if (hsize == 0) {
		if (start >= 0 && start < videoWidth)
			*out2 = (unsigned char)(((startColor + stopColor) / 2) >> 8);
	} else {
		colorSize /= hsize;

		while (hsize--) {
			currentColor &= 0xFF;
			if (!(hsize % 2)) {
				startColor += colorSize;
				currentColor = ((currentColor & 0xFF00) | (((currentColor & 0xFF) << (hsize & 0xFF)) & 0xFF));
			}
			currentColor += startColor;
			if (start >= 0 && start < videoWidth)
				*out2 = currentColor >> 8;

			start++;
			out2++;
			startColor += colorSize;
		}
	}
}

void (*fillFuncArray[])(unsigned char *out, short int *ptr1, short int *ptr2, int color) = {
	&fillFlat,
	&fillCopper,
	&fillBopper,
	&fillMarble,
	&fillTele,
	&fillTras,
	&fillTrame,
	&fillGouraud,
	&fillDithering
};

void fillVertices(int color, short int drawType) {
	unsigned char *out;
	short int *ptr1;
	short int *ptr2;
	int vsize;
	int currentLine;

	if (vtop < 0 || vbottom >= videoHeight - 1)
		return;

	out = frontVideoBuffer + videoWidth * vtop;

	ptr1 = &polyTab[vtop];
	ptr2 = &polyTab2[vtop];

	vsize = vbottom - vtop + 1;

	currentLine = vtop;

	while (vsize--) {
		if (currentLine >= 0 && currentLine < videoHeight) {
			if ((ptr1[videoHeight] - ptr1[0]) >= 0)
				fillFuncArray[drawType](out, ptr1, ptr2, color);

			ptr1++;
			ptr2++;
		}
		out += videoWidth;
		currentLine++;
	}
}

void drawVertices(int numOfVertex, short int drawType) {
	short int vertexY;
	short int *vertexPosPtr, *polyPtr;
	int i;
	char dir = 1;

	short int oldVertexX, oldVertexY;
	short int currentVertexX, currentVertexY;

	unsigned char oldColor;
	unsigned char currentColor;

	float startX;
	int startY;
	float step;

	short int startColor, stepColor;

	/* Set vtop & vbottom to their maximal/minimal size */
	vtop = 32767;
	vbottom = -32768;

	vertexPosPtr = vertexCoordinates;

	for (i = 0; i < numOfVertex; i++) {
		vertexPosPtr += 2;

		vertexY = *(vertexPosPtr++);

		if (vertexY < vtop)
			vtop = vertexY;
		if (vertexY > vbottom)
			vbottom = vertexY;
	}

	/* To end the loop */
	vertexPosPtr[0] = vertexCoordinates[0];
	vertexPosPtr[1] = vertexCoordinates[1];
	vertexPosPtr[2] = vertexCoordinates[2];
	vertexPosPtr = vertexCoordinates;

	oldColor = *(vertexPosPtr++);
	oldVertexX = *(vertexPosPtr++);
	oldVertexY = *(vertexPosPtr++);

	while (numOfVertex--) {
		currentColor = *(vertexPosPtr++);
		currentVertexX = *(vertexPosPtr++);
		currentVertexY = *(vertexPosPtr++);

		/* Since it's scanline based, we don't care when we are only moving along X */
		if (currentVertexY == oldVertexY) {
			oldVertexX = currentVertexX;
		} else {
			/* Move up */
			if (currentVertexY < oldVertexY)
				dir = -1;
			/* Move down */
			else
				dir = 1;

			/* Starting Y position */
			startY = oldVertexY;
			/* oldVertexY becomes the number of pixel */
			oldVertexY = abs(currentVertexY - oldVertexY);

			/* polyPtr is the pointer to the actual polygons cell */
			if (dir == -1)
				polyPtr = &polyTab[startY + videoHeight];
			else
				polyPtr = &polyTab[startY];
			/* 'step' is the step for every point to the current vertex */
			step = (float)(currentVertexX - oldVertexX) / oldVertexY;

			/* Starting X position */
			startX = oldVertexX;

			/* oldVertexX is now the number of vertical pixels */
			oldVertexX = oldVertexY;

			/* Add all the points */
			for (i = 0; i < oldVertexY+1; i++) { /* TODO: why +1 ? */
				if ((polyPtr - polyTab) < 960 && (polyPtr - polyTab) >= 0)
					*polyPtr = (short int) startX;
				polyPtr += dir;
				startX += step;
			}

			/* Compute the color progression */
			if (drawType >= 7) {
				if (dir == -1)
					polyPtr = &polyTab2[startY + videoHeight];
				else
					polyPtr = &polyTab2[startY];

				/* Color progression per step */
				stepColor = ((currentColor - oldColor) << 8) / oldVertexX;
				/* Starting color */
				startColor = (oldColor << 8) | 0xFF; /* starts with the old vertex */

				/* Add all the colors */
				for (i = 0; i < oldVertexX+1; i++) {
					if ((polyPtr - polyTab2) < 960 && (polyPtr - polyTab2) >= 0)
						*polyPtr = startColor;
					polyPtr += dir;
					startColor += stepColor;
				}
			}
			/* Set the old vertex position to the current one */
			oldVertexX = currentVertexX;
			oldVertexY = currentVertexY;
		}
		oldColor = currentColor;
	}
}

void drawLine(int a, int b, int c, int d, int e) {
	int temp;
	short int flag;
	int flag2;
	unsigned char *out;
	short int color;
	short int var2;
	short int xchg;
	int currentLineColor = e;

	if (a > c) {	// pour toujours dessiner de gauche à droite
		temp = c;
		c = a;
		a = temp;

		temp = d;
		d = b;
		b = temp;
	}

	flag = 0;

	if (a < textWindowLeft) {
		flag |= 1;
	} else {
		if (a > textWindowRight)
			return;
	}
	if (b < textWindowTop) {
		flag |= 8;
	} else {
		if (b > textWindowBottom)
			flag |= 4;
	}

	flag <<= 8;

	if (c < textWindowLeft)
		return;
	if (c <= textWindowLeft)
		flag |= 2;

	if (d < textWindowTop) {
		flag |= 8;
	} else {
		if (d > textWindowBottom)
			flag |= 4;
	}

	flag2 = flag;

	if (flag)
		return;

// implementer la suite

	flag2 = videoWidth;	// esi
	c -= a;
	d -= b;
	if (d < 0) {
		flag2 = -flag2;
		d = -d;
	}

	out = frontVideoBuffer + WINDOW_X*b + a;

	color = currentLineColor;
	if (c < d) {	// pente importante
		xchg = c;
		c = d;
		d = xchg;
		var2 = c;
		var2 <<= 1;
		b = c;
		d <<= 1;
		c++;
		do {
			*out = (unsigned char) color;
			b -= d;
			if (b > 0) {
				out += flag2;
			} else {
				b += var2;
				out += flag2 + 1;
			}
		} while (--c);
	} else {   // pente reduite
		var2 = c;
		var2 <<= 1;
		b = c;
		d <<= 1;
		c++;
		do {
			*out = (unsigned char) color;
			out++;
			b -= d;
			if (b < 0) {
				b += var2;
				out += flag2;
			}
		} while (--c);
	}
}

void setCameraPosition(int X, int Z, int param2, int param3, int param4) {
	cameraPosX = X;
	cameraPosZ = Z;

	cameraVar1 = param2;
	cameraVar2 = param3;
	cameraVar3 = param4;

	isUsingOrhoProjection = 0;
}

void setCameraAngle(int x, int z, int y, int param3, int param4, int param5, int param6) {
	cameraAngleX = x;
	cameraAngleZ = z;
	cameraAngleY = y;

	setSomething3(param3, param4, param5);

	setSomething3Var16 += param6;

	setCameraAngleSub(setSomething3Var12, setSomething3Var14, setSomething3Var16);

	cameraAngleX = destX;
	cameraAngleZ = destZ;
	cameraAngleY = destY;
}

void setCameraAngleSub(int eax, int ebx, int ecx) {
	destX = (_baseMatrix[0] * eax + _baseMatrix[3] * ebx + _baseMatrix[6] * ecx) >> 14;
	destZ = (_baseMatrix[1] * eax + _baseMatrix[4] * ebx + _baseMatrix[7] * ecx) >> 14;
	destY = (_baseMatrix[2] * eax + _baseMatrix[5] * ebx + _baseMatrix[8] * ecx) >> 14;
}

void setOrthoCameraAngle(int eax, int ebx, int ecx) {
	destX = (_baseMatrix[0] * eax + _baseMatrix[1] * ebx + _baseMatrix[2] * ecx) >> 14;
	destZ = (_baseMatrix[3] * eax + _baseMatrix[4] * ebx + _baseMatrix[5] * ecx) >> 14;
	destY = (_baseMatrix[6] * eax + _baseMatrix[7] * ebx + _baseMatrix[8] * ecx) >> 14;
}

void setOnlyCameraAngle(int x, int z, int y) {
	cameraAngleX = x;
	cameraAngleZ = z;
	cameraAngleY = y;
}

void setSomething3(int a, int b, int c) { // setupBaseMatrix
	int angleXCos;
	int angleXSin;

	int angleYCos;
	int angleYSin;

	int angleZCos;
	int angleZSin;

	int temp;

	int baseMatrixRotationX;
	int baseMatrixRotationY;
	int baseMatrixRotationZ;

	tab1 = &angleTable[0];
	tab2 = &angleTable[256];
	tab3 = &angleTable[384];

	baseMatrixRotationX = a & 0x3FF;
	baseMatrixRotationY = b & 0x3FF;
	baseMatrixRotationZ = c & 0x3FF;

	angleXCos = tab1[baseMatrixRotationX];
	angleXSin = tab1[(baseMatrixRotationX + 256) & 0x3FF];

	angleYCos = tab1[baseMatrixRotationY];
	angleYSin = tab1[(baseMatrixRotationY + 256) & 0x3FF];

	angleZCos = tab1[baseMatrixRotationZ];
	angleZSin = tab1[(baseMatrixRotationZ + 256) & 0x3FF];

	_baseMatrix[0] = angleZSin;
	_baseMatrix[1] = -angleZCos;
	_baseMatrix[3] = (angleZCos * angleXSin) >> 14;
	_baseMatrix[4] = (angleZSin * angleXSin) >> 14;
	_baseMatrix[6] = (angleZCos * angleXCos) >> 14;
	_baseMatrix[7] = (angleZSin * angleXCos) >> 14;

	_baseMatrix[0] = (angleZSin * angleYSin) >> 14;
	_baseMatrix[2] = (angleZSin * angleYCos) >> 14;

	temp = _baseMatrix[3];

	_baseMatrix[3] = ((angleYSin * temp) + (angleYCos * angleXCos)) >> 14;
	_baseMatrix[5] = ((angleYCos * temp) - (angleYSin * angleXCos)) >> 14;

	temp = _baseMatrix[6];

	_baseMatrix[6] = ((angleYSin * temp) - (angleXSin * angleYCos)) >> 14;
	_baseMatrix[8] = ((angleYCos * temp) + (angleXSin * angleYSin)) >> 14;

	setOrthoCameraAngle(cameraAngleX, cameraAngleZ, cameraAngleY);

	setSomething3Var12 = destX;
	setSomething3Var14 = destZ;
	setSomething3Var16 = destY;
}
