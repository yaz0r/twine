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

typedef struct
{
#ifdef USE_FLOAT
	float x;
	float y;
	float z;
#else
	short int x;
	short int y;
	short int z;
#endif
} pointTab;

typedef struct
{
	short int data1;
	short int data2;
	short int data3;
	short int param;
	short int flag;
	short int rotateZ;
	short int rotateY;
	short int rotateX;
	int field_10;
	int field_14;
	int field_18;
	int Y;
	int field_20;
	short int field_24;
} pointEntry;

typedef struct
{
	short int depth;
	short int renderType;
	unsigned char *dataPtr;
} renderTabEntry;

typedef struct
{
	int data;
	short int x1;
	short int y1;
	short int x2;
	short int y2;
} lineCoordinates;

typedef struct
{
	int data;
	short int p1;
	short int p2;
} lineData;

typedef struct {
	unsigned char FillVertic_AType;
	unsigned char numOfVertex;
	short int colorIndex;
} polyHeader;

typedef struct {
	short int shadeEntry;
	short int dataOffset;
} polyVertexHeader;

typedef struct {
	short int shadeValue;
	short int x;
	short int y;
} computedVertex;

void RotList(unsigned char *esi, int ecx, pointTab * dest, int *eax);
void TransRotList(unsigned char *esi, int ecx, pointTab * dest, int *eax);
void TranslateGroupe(int edx, int ecx, int ebx, pointEntry * esi);
void TranslateGroupeS2(short int ax, short int bx, short int cx);
void loadPart(int edx, int ecx, int ebx, unsigned char * ptr);
int AffObjetIso(int X, int Y, int Z, int angleX, int angleY, int angleZ, unsigned char *costumePtr);
void SetLightVector(int a, int b, int c);
int renderAnimatedModel(unsigned char *costumePtr);
void RotMatIndex2(int *eax, int *ebp);
int finishRender(unsigned char *esi);
void fillVertices(int color, short int drawType);
void drawVertices(int numOfVertex, short int drawType);
void configureOrthoProjection(int x, int y);
void setOrthoCameraAngle(int eax, int ebx, int ecx);
void setCameraAngleSub(int eax, int ebx, int ecx);
void setCameraAngle(int param0, int param1, int param2, int param3, int param4, int param5, int param6);
void setCameraPosition(int X, int Y, int param2, int param3, int param4);
void setOnlyCameraAngle(int a, int b, int c);
void setSomething3(int a, int b, int c);
void drawLine(int a, int b, int c, int d, int e);

extern short int *tab1;
extern short int *tab2;
extern short int *tab3;

extern int isUsingOrhoProjection;

extern int setSomething3Var12;
extern int setSomething3Var14;
extern int setSomething3Var16;

extern int cameraVar1;
extern int cameraVar2;
extern int cameraVar3;

extern int destX;
extern int destY;
extern int destZ;

extern int cameraPosX;
extern int cameraPosZ;

extern int renderBottom;
extern int renderLeft;
extern int renderRight;
extern int renderTop;

extern short int vertexCoordinates[193];

