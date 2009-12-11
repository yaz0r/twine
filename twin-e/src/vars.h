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

#ifndef _VARS_
#define _VARS_

#include <stdlib.h>

#ifdef PCLIKE
# ifdef WIN32
#  include <SDL.h>
# else
#  include <SDL/SDL.h>
# endif
#endif

#define NUM_MAX_FLAGS 200
#define NUM_MAX_ZONES 100

extern short int *tab1;
extern short int *tab2;
extern short int *tab3;

extern int destX;
extern int destY;
extern int destZ;

extern short int polyTab[960];
extern short int polyTab2[960];

extern int isUsingOrhoProjection;

extern short int shadeTable[500];

extern int setSomethingVar1;
extern int setSomethingVar2;
extern short int setSomethingVar3;

extern unsigned char *frontVideoBuffer;

extern int _baseMatrix[3 * 3];

extern int _numOfPoints;
extern int _numOfParts;
extern unsigned char *_pointsPtr;
extern unsigned char *_partsPtr;

extern int _matrixTable[271];
extern unsigned char *_currentMatrixTableEntry;

extern int *_shadePtr;

extern int _shadeMatrix[9];
extern int _lightX;
extern int _lightY;
extern int _lightZ;

extern short int primitiveCounter;

extern int renderBottom;
extern int renderLeft;
extern int renderLoop;
extern int renderRight;
extern int renderTop;

extern short int vertexCoordinates[193];

extern int numOfVertex;

extern short int FillVertic_AType;

extern short int pRenderV3[96];
extern short int *pRenderV1;
extern short int *pRenderV2;
extern short int numOfVertexRemaining;
extern short int polyCropped;

extern short int vleft;
extern short int vtop;
extern short int vright;
extern short int vbottom;

extern unsigned char oldVertexParam;
extern unsigned char vertexParam1;
extern unsigned char vertexParam2;

extern int textWindowTop;
extern int textWindowLeftSave;
extern int textWindowLeft;
extern int textWindowTopSave;
extern int textWindowRight;
extern int textWindowRightSave;
extern int textWindowBottom;
extern int textWindowBottomSave;

extern unsigned char *_partsPtr2;

extern int setSomething3Var12;
extern int setSomething3Var14;
extern int setSomething3Var16;

extern int setSomething2Var1;
extern int setSomething2Var2;
extern int setSomething2Var3;

extern int cameraVar1;
extern int cameraVar2;
extern int cameraVar3;

struct flagDataStruct {
	short int x;
	short int z;
	short int y;
};

typedef struct flagDataStruct flagDataStruct;

struct sceneStruct {
	short int field_0;
	short int field_2;
	short int field_4;
	short int field_6;
};

typedef struct sceneStruct sceneStruct;

struct extraListStruct {
	short int field_0;
	short int X;
	short int Y;
	short int Z;

	short int field_8;
	short int field_A;
	short int field_C;

	time trackActorRotation;

	short int field_E;
	short int field_10;
	short int field_12;
	short int field_14;
	short int field_16;
	int time;
	short int field_1C;
	short int field_1E;
	short int field_20;
};

typedef struct extraListStruct extraListStruct;

extern short int key;
extern uint32 unkPtr;   // recheck
extern uint32 cfg_file;

extern byte *workVideoBuffer;
extern byte *frontVideoBufferbis;

extern byte palette[256 * 3]; // tempvalue
extern byte palette2[256 * 3];  // tempvalue
extern byte paletteRGBA[256 * 4];

extern byte *bufSpeak;
extern byte *bufMemoSeek;
extern char *bufText;
extern char *bufOrder;
extern byte *bufAni1;
extern byte *bufAni2;
extern byte *menuPal;
extern byte menuPalRGBA[1024];
extern byte *shadowSprite;
extern byte *spriteActorData;
extern byte *lbaFont;

extern hqr_entry *HQR_Fic;
extern hqr_entry *HQR_Bodies;
extern hqr_entry *HQR_Scenes;
extern hqr_entry *HQR_Text;
extern hqr_entry *HQR_Grids;
extern hqr_entry *HQR_Bll;
extern hqr_entry *HQR_Midi;
extern hqr_entry *HQR_Inventory;
extern hqr_entry *HQR_Sprites;
extern hqr_entry *HQR_Anims;
extern hqr_entry *HQR_Samples;

extern byte *bufCube;
extern byte *bufferBrick;
extern byte *bufferBrick2;

extern byte *fntFont;

extern int interCharSpace;    // espace inter lettre

extern int spaceLenght;   // largeur d'un espace

extern int textColor;
extern int progressiveTextStartColor;
extern int progressiveTextStopColor;
extern int progressiveTextStepSize;
extern int progressiveTextBufferSize;

extern int setup_lst;

extern int samplesLoaded;

extern int textVar1;    // current text bank

extern byte textVar2[256];

extern byte textVar3;

extern int language;

extern int languageCD1;

extern FILE *voxFileHandle;
extern int printTextVar5;

extern int dialogueBoxLeft;
extern int dialogueBoxTop;
extern int dialogueBoxRight;
extern int dialogueBoxBottom;

extern short int key1;
extern short int printTextVar12;
extern int printTextVar13;

extern int largeurEcran;
extern int hauteurEcran;

extern int flagDisplayText;

extern short int skipIntro;
extern short int palReseted;

extern short int buttonDrawVar1;

extern char saveGameFileName[143];

extern char currentRoom;
extern int gameStaffIsDisplayed;
extern short int shadowMode;

extern char needChangeRoom;
extern short int chapter;
extern short int newGameVar4;

extern int newGameVar5;

extern int screenLockupTable[2000]; // valeur temporaire...

extern int initVideoVar1;

extern short int newTwinsenX;
extern short int newTwinsenZ;
extern short int newTwinsenY;
extern short int brutalExit;
extern short int numClover;
extern short int numCloverBox;
extern short int currentPingouin;
extern short int magicLevel;
extern short int magicPoint;
extern short int numCoin;
extern short int numKey;
extern short int usingSword;

extern short int currentTextBank;
extern short int fuel;
extern short int cropBottomScreen;
extern short int currentlyFollowedActor;
extern short int startupAngleInCube;

extern short int comportementHero;

extern short int startupComportementHeroInCube;

extern short int numTextEntry;  // nombre d'entree de text dans la bank actuelle

extern int currentTextLength;
extern char *currentTextPtr;

extern int stringLenght;

extern short int useAlternatePalette;
extern short int drawInGameTransBox;

extern char buf1[256];

extern char buf2[256];    // check size

extern char *printText8Ptr1;
extern char *printText8Ptr2;

extern int printText8Var1;
extern int printText8Var2;
extern int printText8Var3;

extern int TEXT_CurrentLetterX;
extern int printText8Var5;
extern int printText8Var6;
extern int TEXT_CurrentLetterY;
extern char *printText8Var8;

extern int printText10Var1;
extern int spaceLength;

extern int dialogueBoxParam1;
extern int dialogueBoxParam2;

extern short int playMusciVar1;

extern int currentlyPlayingCDTrack;

extern char playerName[30];

extern char enterPlayerNameVar1;

extern char allowedCharIndex[71];

extern short int enterPlayerNameVar2;

extern int addLineBreakX;
extern int printText8PrepareBufferVar2;

extern int wordSizeChar;
extern int wordSizePixel;

extern char spaceChar;
extern short int talkingActor;

extern int printText8PrepareBufferVar3;

extern actor actors[100];   // yeah, can use up to 100 actor !
extern actor *twinsen;

extern short int holomapTraj;

extern short int roomMusic;
extern short int currentlyPlayingMidi;
extern short int twinsenPositionModeInNewCube;

extern short int newTwinsenXByZone;
extern short int newTwinsenZByZone;

extern short int newTwinsenYByZone;

extern short int twinsenZBeforeFall;

extern time mainLoopVar1;
extern short int mainLoopVar4;
extern short int disableScreenRecenter;

extern int timeToNextRoomSample;
extern int newCameraX;
extern int newCameraZ;
extern int newCameraY;

extern short int magicBallIdx;
extern short int twinsenMoved;
extern short int useAnotherGrm;

extern short int currentGrid2;
extern short int requestBackgroundRedraw;
extern short int lockPalette;
extern short int changeRoomVar10;
extern short int changeRoomVar11;

extern int numActorInRoom;

extern extraListStruct extraList[50];

extern byte cubeFlags[80];
extern byte itemUsed[28];

extern int currentPositionInBodyPtrTab;
extern short int numOfZones;
extern short int numFlags;

extern byte *file3D[];

extern short int TCos[];

extern unsigned char *loadTwinsenCostumesVar1;

extern unsigned char *fireEffectVar1;

extern unsigned char *fireEffectVar2;

extern unsigned char *scenePtr;
extern unsigned char *localScenePtr;

extern short int sceneRoomNumber;
extern sceneStruct sceneVar2;
extern sceneStruct sceneVar3;
extern sceneStruct sceneVar4;
extern short int sceneVar14;
extern short int sceneVar15;
extern short int newTwinsenXByScene;
extern short int newTwinsenYByScene;
extern short int newTwinsenZByScene;
extern flagDataStruct flagData[NUM_MAX_FLAGS];
extern ZONE_Box zoneData[NUM_MAX_ZONES];

extern unsigned char *currentGrid;
extern unsigned char *currentBll;

extern int numberOfBll;
extern short int autoAgressivity;

extern short int mainLoopVar5;
extern short int mainLoopVar6;
extern short int mainLoopVar7;

extern int musicPosition;
extern int musicLength;

extern int useSamples;

extern int time1;
extern int time3;

extern unsigned char vars[256];
extern byte GV14[150];
extern int counter;

extern short int fullRedrawVar1;
extern short int fullRedrawVar2;

extern int numOfRedrawBox;

extern short int fullRedrawVar8;

extern unsigned char *actorScriptPtr;

extern int cameraX;
extern int cameraZ;
extern int cameraY;

#ifdef USE_FLOAT
extern float projectedPositionX;
extern float projectedPositionY;
extern float projectedPositionZ;
#else
extern short int projectedPositionX;
extern short int projectedPositionY;
extern short int projectedPositionZ;
#endif

extern short int manipActorVar1;

extern short int manipActorResult;

extern short int mainLoopVar9;

extern unsigned char *bodyPtrTab[200];

extern short int loadCostumeVar;
extern short int loadCostumeVar2;
extern short int loadCostumeVar3;
extern short int loadCostumeVar4;
extern short int loadCostumeVar5;
extern short int loadCostumeVar6;

extern unsigned char *menuCostumeIndex;

extern time timeVar;

extern short int winTab[4];

extern char dataString[256];

extern short int currentLineColor;

extern unsigned short int costumeHeader;

extern int action;

extern int shadowX;
extern int shadowZ;
extern int shadowY;

extern unsigned char *flagModelPtr;

extern char *keyFramePtr;
extern char *animVar1;
extern char *lastKeyFramePtr;

extern short int processActorSub2Var0;
extern short int processActorSub2Var1;

extern int DoTrackVar1;

extern int mainLoopVar17;

extern short int currentlyProcessedActorNum;

extern actor *processActorVar1;
extern short int processActorVar2;
extern short int processActorVar3;
extern short int processActorVar4;

extern short int fieldCauseDamage;
extern short int processActorX;
extern short int processActorY;
extern short int processActorZ;

extern short int processActorVar5;
extern short int processActorVar6;
extern short int currentY;
extern short int currentX;
extern short int currentZ;

extern int getPosVar1;
extern int getPosVar2;
extern int getPosVar3;

extern short int processActorVar11;
extern short int processActorVar12;
extern short int processActorVar13;

extern short int animVar4;

extern int drawVar1;
extern int currentActorInZoneProcess;

extern short int moveVar1;

extern int showTalkVar;

extern int numOfOptionsInChoice;
extern short int inGameMenuData[10];

extern short int choiceTab[18];
extern int inGameMenuAnswer;

extern int magicBallNumBounce;
extern int magicBallParam;

extern char shadowVar;

extern short int objectRotation[255];
extern char inventorySelectedColor;
extern char currentSelectedObjectInInventory;

extern int reinitVar1;
extern int reinitVar2;
extern int reinitVar12;

extern int bufRotate0[9];
extern int bufRotate1[9];

extern byte fkeys;
extern boolean breakmainLoop;

void initVars(void);

#endif
