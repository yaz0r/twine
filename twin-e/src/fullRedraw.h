typedef struct
{
	short int y;
	short int z;
	short int x;
	short int drawX;
	short int drawY;
	short int spriteNum;
} zbufferDataStruct;

typedef struct
{
	unsigned short int left;
	unsigned short int top;
	unsigned short int right;
	unsigned short int bottom;
} currentDirtyBoxListStruct;

typedef struct
{
	short int field_0;
	unsigned short int field_2;
	unsigned short int X;
	unsigned short int Z;
	unsigned short int Y;
	unsigned short int field_A;
	unsigned short int field_C;
	unsigned short int field_E;
	unsigned short int field_10;
} drawListStruct;

void SmallSort(drawListStruct *list, int listSize/*, int param*/);
void DrawOverBrick3(int X, int Z, int Y);
void DrawOverBrick(int X, int Z, int Y);
void CopyMask(int spriteNum, int x, int y, /*byte * localBufferBrick, */byte * buffer);
void fullRedraw(int param);
void blitBackgroundOnDirtyBoxes(void);
void fullRedrawSub5(void);
void addToRedrawBox(short int arg_0, short int arg_4, short int arg_8, short int arg_C);
void zbuffer(int var1, int var2, int y, int z, int x);
void redrawCube(void);
void zbufferSub2(int y, int z, int x);
unsigned char *zbufferSub1(int var);
void AffGraph(int num, int var1, int var2, unsigned char *localBufferBrick);
int projectPositionOnScreen(int arg_0, int arg_4, int arg_8);
int HQ_3D_MixSample(/* int param0, int param1, int param2, int param3, int param4, int param5 */);
void GetDxDyGraph(int arg_0, int *arg_4, int *arg_8, unsigned char *ptr);
void AddPhysBox(int left, int top, int right, int bottom);
void FlipBoxes(void);

extern short int fullRedrawVar8;

#ifdef USE_FLOAT
extern float projectedPositionX;
extern float projectedPositionY;
extern float projectedPositionZ;
#else
extern short int projectedPositionX;
extern short int projectedPositionY;
extern short int projectedPositionZ;
#endif

extern int numOfRedrawBox;

extern int cameraX;
extern int cameraZ;
extern int cameraY;

