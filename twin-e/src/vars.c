/*
Copyright (C) 2002-2004 The TwinE team

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

#include "lba.h"

int setSomething3Var12;
int setSomething3Var14;
int setSomething3Var16;

int setSomething2Var1;
int setSomething2Var2;
int setSomething2Var3;

int destX;
int destY;
int destZ;

int cameraVar1;
int cameraVar2;
int cameraVar3;


short int *tab1;
short int *tab2;
short int *tab3;

int renderLeft;
int renderRight;
int renderTop;
int renderBottom;

short int polyTab[960];
short int polyTab2[960];

int isUsingOrhoProjection;

short int shadeTable[500];

int setSomethingVar1;
int setSomethingVar2;
short int setSomethingVar3;

unsigned char *frontVideoBuffer;

int _baseMatrix[3 * 3];

int _numOfPoints;
int _numOfParts;
unsigned char *_pointsPtr;
unsigned char *_partsPtr;

int _matrixTable[271];  // should be matrixes
unsigned char *_currentMatrixTableEntry;

int *_shadePtr;

int _shadeMatrix[9];
int _lightX;
int _lightY;
int _lightZ;

short int primitiveCounter;
int renderLoop;

short int vertexCoordinates[193];

int numOfVertex;

short int FillVertic_AType;

short int pRenderV3[96];
short int *pRenderV1;
short int *pRenderV2;
short int numOfVertexRemaining;
short int polyCropped;

short int vleft;
short int vtop;
short int vright;
short int vbottom;

unsigned char oldVertexParam;
unsigned char vertexParam1;
unsigned char vertexParam2;

int textWindowTop;
int textWindowLeftSave;
int textWindowLeft;
int textWindowTopSave;
int textWindowRight;
int textWindowRightSave;
int textWindowBottom;
int textWindowBottomSave;

unsigned char *_partsPtr2;

short int key;
hqr_entry* HQR_Midi;
uint32 unkPtr;    // recheck
uint32 cfg_file;

byte *workVideoBuffer;
byte *frontVideoBufferbis;

byte palette[256 * 3];  // tempvalue
byte palette2[256 * 3]; // tempvalue
byte paletteRGBA[256 * 4];

byte *bufSpeak;
byte *bufMemoSeek;
char *bufText;
char *bufOrder;
byte *bufAni1;
byte *bufAni2;
byte *menuPal;
byte menuPalRGBA[1024];
byte *shadowSprite;
byte *spriteActorData;
byte *lbaFont;

#ifdef PRELOAD_ALL
hqr_entry *HQR_Fic;
hqr_entry *HQR_Bodies;
hqr_entry *HQR_Scenes;
hqr_entry *HQR_Text;
hqr_entry *HQR_Grids;
hqr_entry *HQR_Bll;
#endif
hqr_entry *HQR_Inventory;
hqr_entry *HQR_Sprites;
hqr_entry *HQR_Anims;
hqr_entry *HQR_Samples;

byte *bufCube;
byte *bufferBrick;
byte *bufferBrick2;

byte *fntFont;

int interCharSpace;   // espace inter lettre

int spaceLenght;    // largeur d'un espace

int textColor;
int progressiveTextStartColor;
int progressiveTextStopColor;
int progressiveTextStepSize;
int progressiveTextBufferSize;

int setup_lst;

int samplesLoaded;

int textVar1;   // current text bank

byte textVar2[256];

byte textVar3;

int language;

int languageCD1;

FILE *voxFileHandle;
int printTextVar5;

int dialogueBoxLeft;
int dialogueBoxTop;
int dialogueBoxRight;
int dialogueBoxBottom;

short int key1;
short int printTextVar12;
int printTextVar13;

int largeurEcran;
int hauteurEcran;

int flagDisplayText;

short int skipIntro;
short int palReseted;

short int buttonDrawVar1;

char saveGameFileName[143];

char currentRoom;
int gameStaffIsDisplayed;
short int shadowMode;

char needChangeRoom;
short int chapter;
short int newGameVar4;

int newGameVar5;

int screenLockupTable[2000];  /* TODO: temporary ? */

int initVideoVar1;

short int newTwinsenX;
short int newTwinsenZ;
short int newTwinsenY;
short int brutalExit;
short int numClover;
short int numCloverBox;
short int currentPingouin;
short int magicLevel;
short int magicPoint;
short int numCoin;
short int numKey;
short int usingSword;

short int currentTextBank;
short int fuel;
short int cropBottomScreen;
short int currentlyFollowedActor;
short int startupAngleInCube;

short int comportementHero;

short int startupComportementHeroInCube;

short int numTextEntry; // nombre d'entree de text dans la bank actuelle

int currentTextLength;
char *currentTextPtr;

int stringLenght;

short int useAlternatePalette;
short int drawInGameTransBox;

char buf1[256];
char buf2[256]; /* TODO: check size */

char *printText8Ptr1;
char *printText8Ptr2;

int printText8Var1;
int printText8Var2;
int printText8Var3;

int TEXT_CurrentLetterX;
int printText8Var5;
int printText8Var6;
int TEXT_CurrentLetterY;
char *printText8Var8;

int printText10Var1;
int spaceLength;

int dialogueBoxParam1;
int dialogueBoxParam2;

short int playMusciVar1;

int currentlyPlayingCDTrack;

char playerName[30];

char enterPlayerNameVar1;

char allowedCharIndex[71];

short int enterPlayerNameVar2;

int addLineBreakX;
int printText8PrepareBufferVar2;

int wordSizeChar;
int wordSizePixel;

char spaceChar;
short int talkingActor;

actor actors[100];
actor *twinsen;

short int holomapTraj;

short int roomMusic;
short int currentlyPlayingMidi;
short int twinsenPositionModeInNewCube;

short int newTwinsenXByZone;
short int newTwinsenZByZone;

short int newTwinsenYByZone;

short int twinsenZBeforeFall;

time mainLoopVar1;
short int mainLoopVar4;
short int disableScreenRecenter;

int timeToNextRoomSample;
int newCameraX;
int newCameraZ;
int newCameraY;

short int magicBallIdx;
short int twinsenMoved;
short int useAnotherGrm;

short int currentGrid2;
short int requestBackgroundRedraw;
short int lockPalette;
short int changeRoomVar10;
short int changeRoomVar11;

int numActorInRoom;

extraListStruct extraList[50];

byte cubeFlags[80];
byte itemUsed[28];

int currentPositionInBodyPtrTab;
short int numOfZones;
short int numFlags;

byte *file3D[5];

short int TCos[5];

unsigned char *loadTwinsenCostumesVar1;

unsigned char *fireEffectVar1;

unsigned char *fireEffectVar2;

unsigned char *scenePtr;
unsigned char *localScenePtr;

short int sceneRoomNumber;
sceneStruct sceneVar2;
sceneStruct sceneVar3;
sceneStruct sceneVar4;
short int sceneVar14;
short int sceneVar15;
short int newTwinsenXByScene;
short int newTwinsenYByScene;
short int newTwinsenZByScene;
flagDataStruct flagData[NUM_MAX_FLAGS];
ZONE_Box zoneData[NUM_MAX_ZONES];

unsigned char *currentGrid;
unsigned char *currentBll = NULL;

int numberOfBll;
short int autoAgressivity;

short int mainLoopVar5;
short int mainLoopVar6;
short int mainLoopVar7;

int musicPosition;
int musicLength;

int useSamples;

int time1;
int time3;

unsigned char vars[256];
byte GV14[150];
int counter;

short int fullRedrawVar1;
short int fullRedrawVar2;

int numOfRedrawBox;

short int fullRedrawVar8;

unsigned char *actorScriptPtr;

int cameraX;
int cameraZ;
int cameraY;

#ifdef USE_FLOAT
float projectedPositionX;
float projectedPositionY;
float projectedPositionZ;
#else
short int projectedPositionX;
short int projectedPositionY;
short int projectedPositionZ;
#endif

short int manipActorVar1;

short int manipActorResult;

short int mainLoopVar9;

unsigned char *bodyPtrTab[200];

short int loadCostumeVar;
short int loadCostumeVar2;
short int loadCostumeVar3;
short int loadCostumeVar4;
short int loadCostumeVar5;
short int loadCostumeVar6;

unsigned char *menuCostumeIndex;
short int TCOS[4];

time timeVar;

short int winTab[4];

char dataString[256];

short int currentLineColor;

unsigned short int costumeHeader;

int action;

int shadowX;
int shadowZ;
int shadowY;

unsigned char *flagModelPtr;

char *keyFramePtr;
char *animVar1;
char *lastKeyFramePtr;

short int processActorSub2Var0;
short int processActorSub2Var1;

int DoTrackVar1;

int mainLoopVar17;

short int currentlyProcessedActorNum;

actor *processActorVar1;
short int processActorVar2;
short int processActorVar3;
short int processActorVar4;

short int fieldCauseDamage;
short int processActorX;
short int processActorY;
short int processActorZ;

short int processActorVar5;
short int processActorVar6;
short int currentY;
short int currentX;
short int currentZ;

int getPosVar1;
int getPosVar2;
int getPosVar3;

short int processActorVar11;
short int processActorVar12;
short int processActorVar13;

short int animVar4;

int drawVar1;

int currentActorInZoneProcess;

short int moveVar1;

int showTalkVar;

int numOfOptionsInChoice;
short int inGameMenuData[10];

short int choiceTab[18];
int inGameMenuAnswer;

int magicBallNumBounce;
int magicBallParam;

char shadowVar;

short int objectRotation[255];
char inventorySelectedColor;
char currentSelectedObjectInInventory;

// order important !

int reinitVar1;
int reinitVar2;
int reinitVar12;

int bufRotate0[9];
int bufRotate1[9];

byte fkeys;
boolean breakmainLoop;

void initVars(void)
{
	textVar1 = -1;

	textWindowTop = 0;
	textWindowLeftSave = 0;
	textWindowLeft = 0;
	textWindowTopSave = 0;
	textWindowRight = 639;
	textWindowRightSave = 639;
	textWindowBottom = 479;
	textWindowBottomSave = 479;

	dialogueBoxLeft = 16;
	dialogueBoxTop = 334;
	dialogueBoxRight = 623;
	dialogueBoxBottom = 463;

	largeurEcran = 640;
	hauteurEcran = 480;

	languageCD1 = 0;
	flagDisplayText = 1;

	needChangeRoom = -1;
	chapter = 0;
	newGameVar4 = 1;
	newGameVar5 = 0;

	currentRoom = 0;
	gameStaffIsDisplayed = 0;
	shadowMode = 2;
	lba_time = 0;
	key = 0;

	buttonDrawVar1 = 550;

#ifdef LANG_FRENCH
	language = 1;
#elif defined LANG_GERMAN
	language = 2;
#elif defined LANG_ESPANOL
	language = 3;
#elif defined LANG_ITALIANO
	language = 4;
#elif defined LANG_PORTUGUESE
	language = 5;	// Portuguese - especial Text/Ress.hqr to use it (not an original game translation)
#else
	language = 0;	// English
#endif

	languageCD1 = 0;

	useAlternatePalette = 0;
	drawInGameTransBox = 0;

	setSomethingVar1 = 320;
	setSomethingVar2 = 200;
	setSomethingVar3 = 500;
	isUsingOrhoProjection = 0;

	dialogueBoxParam1 = 3;
	dialogueBoxParam2 = 591;

	playMusciVar1 = -2;

	currentlyPlayingCDTrack = -1;

	enterPlayerNameVar1 = 0;

	allowedCharIndex[0] = ' ';
	allowedCharIndex[1] = 'A';
	allowedCharIndex[2] = 'B';
	allowedCharIndex[3] = 'C';
	allowedCharIndex[4] = 'D';
	allowedCharIndex[5] = 'E';
	allowedCharIndex[6] = 'F';
	allowedCharIndex[7] = 'G';
	allowedCharIndex[8] = 'H';
	allowedCharIndex[9] = 'I';
	allowedCharIndex[10] = 'J';
	allowedCharIndex[11] = 'K';
	allowedCharIndex[12] = 'L';
	allowedCharIndex[13] = 'M';
	allowedCharIndex[14] = 0x2E;
	allowedCharIndex[15] = 'N';
	allowedCharIndex[16] = 'O';
	allowedCharIndex[17] = 'P';
	allowedCharIndex[18] = 'Q';
	allowedCharIndex[19] = 'R';
	allowedCharIndex[20] = 'S';
	allowedCharIndex[21] = 'T';
	allowedCharIndex[22] = 'U';
	allowedCharIndex[23] = 'V';
	allowedCharIndex[24] = 'W';
	allowedCharIndex[25] = 'X';
	allowedCharIndex[26] = 'Y';
	allowedCharIndex[27] = 'Z';
	allowedCharIndex[28] = 0x2D;
	allowedCharIndex[29] = 'a';
	allowedCharIndex[30] = 'b';
	allowedCharIndex[31] = 'c';
	allowedCharIndex[32] = 'd';
	allowedCharIndex[33] = 'e';
	allowedCharIndex[34] = 'f';
	allowedCharIndex[35] = 'g';
	allowedCharIndex[36] = 'h';
	allowedCharIndex[37] = 'i';
	allowedCharIndex[38] = 'j';
	allowedCharIndex[39] = 'k';
	allowedCharIndex[40] = 'l';
	allowedCharIndex[41] = 'm';
	allowedCharIndex[42] = 0x3F;
	allowedCharIndex[43] = 'n';
	allowedCharIndex[44] = 'o';
	allowedCharIndex[45] = 'p';
	allowedCharIndex[46] = 'q';
	allowedCharIndex[47] = 'r';
	allowedCharIndex[48] = 's';
	allowedCharIndex[49] = 't';
	allowedCharIndex[50] = 'u';
	allowedCharIndex[51] = 'v';
	allowedCharIndex[52] = 'w';
	allowedCharIndex[53] = 'x';
	allowedCharIndex[54] = 'y';
	allowedCharIndex[55] = 'z';
	allowedCharIndex[56] = 0x21;
	allowedCharIndex[57] = '0';
	allowedCharIndex[58] = '1';
	allowedCharIndex[59] = '2';
	allowedCharIndex[60] = '3';
	allowedCharIndex[61] = '4';
	allowedCharIndex[62] = '5';
	allowedCharIndex[63] = '6';
	allowedCharIndex[64] = '7';
	allowedCharIndex[65] = '8';
	allowedCharIndex[66] = '9';
	allowedCharIndex[67] = 0x20;
	allowedCharIndex[68] = 0x08;
	allowedCharIndex[69] = 0x0D;
	allowedCharIndex[70] = 0x00;

	spaceChar = ' ';

	vertexCoordinates[0] = 0x1234;

	tab1 = &angleTable[0];
	tab2 = &angleTable[256];
	tab3 = &angleTable[384];

	twinsen = &actors[0];

	holomapTraj = -1;
	roomMusic = -1;
	currentlyPlayingMidi = -2;

	magicBallIdx = -1;
	twinsenMoved = 0;
	useAnotherGrm = -1;
	currentGrid2 = -1;
	requestBackgroundRedraw = 1;
	lockPalette = 0;

	changeRoomVar10 = 1;
	changeRoomVar11 = 0;

	loadCostumeVar = -32000;

	showTalkVar = 1;

	magicBallNumBounce = 1;
	magicBallParam = 1;

	fkeys = 0;
	breakmainLoop = false;
}

