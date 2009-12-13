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
void FillVertic_A(int ecx, int edi);
int ComputePoly_A(void);
void configureOrthoProjection(int x, int y);
void setSomething3sub(int eax, int ebx, int ecx);
void setCameraAngleSub(int eax, int ebx, int ecx);
void setCameraAngle(int param0, int param1, int param2, int param3, int param4, int param5, int param6);
void setCameraPosition(int X, int Y, int param2, int param3, int param4);
void setSomething2(int a, int b, int c);
void setSomething3(int a, int b, int c);
void drawLine(int a, int b, int c, int d, int e);

extern short int *tab1;
extern short int *tab2;
extern short int *tab3;

extern int isUsingOrhoProjection;

extern short int *pRenderV1;

extern int setSomething3Var12;
extern int setSomething3Var14;
extern int setSomething3Var16;

extern int numOfVertex;

extern int cameraVar1;
extern int cameraVar2;
extern int cameraVar3;

extern int destX;
extern int destY;
extern int destZ;

extern int setSomethingVar1;
extern int setSomethingVar2;

extern int renderBottom;
extern int renderLeft;
extern int renderRight;
extern int renderTop;

extern short int vertexCoordinates[193];

extern short int FillVertic_AType;

