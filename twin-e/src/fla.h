typedef struct FLAheader
{
	char version[6];
	int numOfFrames;
	char speed;
	char var1;
	short int var2;
	short int var3;
} FLAheader;

typedef struct
{
	short int sampleNum;
	short int freq;
	short int repeat;
	char dummy;
	unsigned char x;
	unsigned char y;
} flaSample;

typedef struct
{
	char videoSize;
	char dummy;
	int frameVar0;
} frameData;

void playFla(char *flaName);
int initFla(char* file);
void drawNextFrameFla();
void drawFrame(char* ptr, int width, int height);
void updateFrame(char* ptr, int width);

extern char flaBuffer[320*200];

