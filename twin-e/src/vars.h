class LBA_engine;

int threadTimer(void);
void startThreadTimer(LBA_engine*);

struct pointTab
{
  short int x;
  short int y;
  short int z;
};

struct lineCoordinates
{
  int data;
  short int x1;
  short int y1;
  short int x2;
  short int y2;
};

struct lineData
{
  int data;
  short int p1;
  short int p2;
};

struct renderTabEntry
{
  short int depth;
  short int renderType;
  unsigned char* dataPtr;
};

struct sceneStruct
{
	short int field_0;
 short int field_2;
 short int field_4;
 short int field_6;
};

struct roomData2Struct
{
	short int field_0;
 short int field_2;
 short int field_4;
 short int field_6;
 short int field_8;
 short int field_A;
 short int field_C;
};

struct reinitAll2DataVar1Struct
{
	short int field_0;
 short int field_2;
 short int field_4;
 short int field_6;
 short int field_8;
 short int field_10;
 short int field_12;
 byte field_14;
 byte field_15;
 short int field_16;
 int field_18;
 short int field_1C;
 short int field_1E;
 short int field_20;
 
};

struct refreshBoxListStruct
{
	unsigned short int field_0;
 unsigned short int field_2;
 unsigned short int field_4;
 unsigned short int field_6; 
};

struct fullRedrawVar6Struct
{
	unsigned short int field_0;
 unsigned short int field_2;
 unsigned short int field_4;
 unsigned short int field_6;
 unsigned short int field_8;
 unsigned short int field_A;
 unsigned short int field_C;
 unsigned short int field_E;
 unsigned short int field_10;
};

struct hqr_entry
{
	char fileName[128];
 int size1;
 int remainingSize;
 short int b;
 short int unk;
 unsigned char *ptr;
};

struct subHqr
{
	unsigned char index;
 unsigned int offFromPtr; 
 unsigned int size;
 int lastAccessedTime;
};

struct pointEntry
{
  short int data1;
  short int data2;
  short int data3;
  short int param;
  int flag1;
  int flag2;
  int field_10;
  int field_14;
  int field_18;
  int field_1C;
  int field_20;
  short int field_24;
};

class LBA_engine
{
  public:
  OSystem* osystem;
  int time;
  short int key;
  uint16	useSound;
  byte	useSB;
  uint32	loadMidiResult;
  uint32	unkPtr;	//recheck
  uint32	cfg_file;

  byte	*videoBuffer2;
  byte	*videoBuffer1;
  byte	*videoBuffer1bis;
 
  byte *videoPtr1;
  byte *videoPtr2;
  byte *videoPtr3;
  byte *videoPtr4;
  byte *videoPtr5;
  byte *videoPtr6;
  byte *videoPtr7;
  byte *videoPtr8;
  byte *videoPtr9;
  byte *videoPtr10;
  byte *videoPtr11;
  byte *videoPtr12;
  byte *videoPtr13;
 
  byte	palette[256*3]; // tempvalue
  byte palette2[256*3]; // tempvalue
  byte paletteRGBA[256*4];



  unsigned char outBuffer[512000];
 
  int cropLeft;

  byte	*bufSpeak;
  byte	*bufMemoSeek;
  char	*bufText;
  char	*bufOrder;
  byte	*bufAni1;
  byte	*bufAni2;
	hqr_entry	*HQRInventory;
	byte	*menuPal;
	byte	menuPalRGBA[1024];
	byte	*HQRess2;
	byte	*HQRess3;
	byte	*lbaFont;
	hqr_entry	*HQRPtrSpriteExtra;
	hqr_entry	*HQRSamples;
	hqr_entry	*HQRanims;

	byte	*bufCube;
	byte	*bufferBrick;
	byte	*bufferBrick2;

	byte	*HQMemory;
	byte	*HQMemory2;
	int		HQMemorySize;
	int		HQMemorySize2;

	byte	*fntFont;

	int		interCharSpace; // espace inter lettre


	int		spaceLenght; // largeur d'un espace

	int		textColor;
	int		initVar2;
	int		initVar3;
	int		initVar4;
	int		initVar5;

	int		setup_lst;



	int		samplesLoaded;

	int		textVar1;      // current text bank

	byte	textVar2[256];

	byte	textVar3;

	int		language;

	int		languageCD1;

	int		mainMenu3Var1;
	int 	mainMenu3Var2;

	char 	mainMenu3Var3[256];

	FILE*		voxFileHandle;
	int		printTextVar5;

	int		textWindowTop;
	int		textWindowLeftSave;
	int		textWindowLeft;
	int		textWindowTopSave;
	int		textWindowRight;
	int		textWindowRightSave;
	int		textWindowBottom;
	int		textWindowBottomSave;

	int		dialogueBoxLeft;
	int		dialogueBoxTop;
	int		dialogueBoxRight;
	int		dialogueBoxBottom;

	short int	key1;
	short int printTextVar12;
	int		printTextVar13;

	int		largeurEcran;
	int		hauteurEcran;

	int		flagDisplayText;

	short int	skipIntro;
	short int palReseted;

	short int buttonDrawVar1;

	char saveGameFileName[143];
	
	short int currentRoom;
	char mainMenuVar3;
	int mainMenuVar4;
	short int shadowMode;

	char mainMenuVar1[60];

	short int mainMenuData[12];
	short int subMenu2Data[8];
	short int soundMenuData[12];
	short int subMenuData[14];

	short int needChangeRoom;
	short int newGameVar2;
	short int newGameVar4;




	int newGameVar5;

	int screenLockupTable[2000];   // valeur temporaire...

	int initVideoVar1;

//	int reinitVar1;
//	int reinitVar2;
	short int GV9;
	short int GV10;
	short int GV11;
	short int reinitVar3;
	short int numClover;
	short int numCloverBox;
	short int reinitVar4;
	short int GV6;
	short int GV7;
	short int numCoin;
	short int numKey;
	short int GV18;

	short int currentTextBank;
	short int GV15;
	short int reinitVar7;
	short int reinitVar8;
	short int reinitVar9;

	short int comportement;

	short int reinitVar10;

	short int numTextEntry;         // nombre d'entree de text dans la bank actuelle

	int currentTextLength;
	char *currentTextPtr;
	






	int stringLenght;

	char	needChangeRoomVar1[40];
	short int isMenuDisplayed;
	short int drawInGameTransBox;

//	SDL_CD *cdrom;

	int setSomethingVar1;
	int setSomethingVar2;
	
	short int setSomethingVar3;
	short int setSomethingVar4;

//	int setSomething2Var1;
//	int setSomething2Var2;
//	int setSomething2Var3;

	char buf1[256];

	char buf2[256]; // check size

	char *printText8Ptr1;
	char *printText8Ptr2;	

	int printText8Var1;
	int printText8Var2;
	int printText8Var3;

  int printText8Var4;
  int printText8Var5;
  int printText8Var6;
  int printText8Var7;
  char *printText8Var8;

	int printText10Var1;
	int spaceLength;

	int dialogueBoxParam1;
	int dialogueBoxParam2;

	short int playMusciVar1;
	
	int currentlyPlayingMusic;

	int playMusicFlag;

	char playerName[30];

	char enterPlayerNameVar1;

	char allowedCharIndex[71];

	short int currentKey;

	short int enterPlayerNameVar2;
	
	int addLineBreakX;
	int printText8PrepareBufferVar2;

	int wordSizeChar;
	int wordSizePixel;
		
	char spaceChar;

/*	short int backInitVar3;
	short int backDialogueBoxRight;
	short int backDialogueBoxBottom;
	short int back2InitVar3;
	short int back2DialogueBoxRight;
	short int back2DialogueBoxBottom;
	short int backInitVar2;
	
	short int back3DialogueBoxRight;*/
 
 short int polyRenderType;
 
 short int vertexCoordinates[193];
 
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


	short int numOfVertex;
//	short int back3DialogueBoxBottom;

	short int pt8s4var1[96];
	short int *pt8s4var2;
	short int *pt8s4var3;
	short int *pt8s4var4;
	short int *pt8s4var5;
	short int *pt8s4var6;

	int printText8PrepareBufferVar3;

	short int *tab1;
	short int *tab2;
	short int *tab3;

//	int setSomething3Var1;
//	int setSomething3Var2;
//	int setSomething3Var3;
//	int setSomething3Var4;
//	int setSomething3Var5;
//	int setSomething3Var6;
//	int setSomething3Var7;
//	int setSomething3Var8;
//	int setSomething3Var9;
//	int setSomething3Var10;
//	int setSomething3Var11;
//	int setSomething3Var12;
//	int setSomething3Var13;
//	int setSomething3Var14;
//	int setSomething3Var15;
//	int setSomething3Var16;
//	int setSomething3Var17;
//	int setSomething3Var18;

	actor actors[100]; // yeah, can use up to 100 actor !
	actor *twinsen;

	short int changeRoomVar1;

	short int roomMusic;
	short int playMusicVar1;
	short int reinitVar11;

	short int GV9dup;
	short int GV10dup;

	short int GV11dup;

	short int changeRoomVar2;

	timeStruct mainLoopVar1;
	short int mainLoopVar4;
	short int mainLoopVar10;

	int changeRoomVar3;
	int changeRoomVar4;
	int changeRoomVar5;
	int changeRoomVar6;

	short int changeRoomVar7;
	short int changeRoomVar8;

	short int changeRoomVar9;

	short int currentGrid2;
	short int mainLoopVar2;
	short int mainLoopVar3;
	short int changeRoomVar10;
	short int changeRoomVar11;





	int numActorInRoom;

	reinitAll2DataVar1Struct reinitAll2SubVar1[50];

	byte roomData1[80];
	roomData2Struct roomData2[10];
 byte GV16[28];

	int reinitAll2Var3;
	short int reinitAll2Var4;
	short int reinitAll2Var5;

	byte *file3D0;
	byte *file3D1;
	byte *file3D2;
	byte *file3D3;
	byte *file3D4;

	short int TCos0Init;
	short int TCos1Init;	
	short int TCos2Init;
	short int TCos3Init;
	short int TCos4Init;

	unsigned char* loadTwinsenCostumesVar1;

	unsigned char* fireEffectVar1;


	unsigned char* fireEffectVar2;

	unsigned char* scenePtr;
	unsigned char* localScenePtr;

	short int sceneRoomNumber;
	sceneStruct sceneVar2;
	sceneStruct sceneVar3;
	sceneStruct sceneVar4;
	short int sceneVar14;
	short int sceneVar15;
	short int sceneVar16;
	short int sceneVar17;
	short int sceneVar18;
	unsigned char *sceneVar20;
	unsigned char *sceneVarPtr;


	unsigned char *currentGrid;
	unsigned char *currentBll;

	int numberOfBll;
 short int agressivity;
 
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
 short int  counter;
 
 short int fullRedrawVar1;
 short int fullRedrawVar2;
 
 int numOfRedrawBox;
 
 refreshBoxListStruct refreshBoxList[300];
 refreshBoxListStruct refreshBoxList2[300];
 
 short int fullRedrawVar8;
 
 unsigned char scanCodeTab1[29];
 unsigned short int scanCodeTab2[31];
 
 unsigned char * actorScriptPtr;
 
 int changeRoomVar4Bis;
 int changeRoomVar5Bis;
 int changeRoomVar6Bis;
 
 short int fullRedrawVar3;
 short int fullRedrawVar4;
 
 unsigned char zbufferTab[56];
 
 int zbufferVar1;
 int zbufferVar2;
 
 short int manipActorVar1;
 
 short int manipActorResult;
 
 short int mainLoopVar9;
 
 unsigned char* bodyPtrTab[200];

 

 short int loadCostumeVar;
 short int loadCostumeVar2;
 short int loadCostumeVar3;
 short int loadCostumeVar4;
 short int loadCostumeVar5;
 short int loadCostumeVar6;
 
 fullRedrawVar6Struct fullRedrawVar6[150];
 
 unsigned char* currentCostume;
 short int TCOS[4];
 
 timeStruct timeVar;
 
 short int drawInventoryVar; 
 
 short int winTab[4];
 
 char dataString[256];
 
 short int stringProcessVar;
 
// int renderV1;
// int renderV2;
// int renderV3;
 
 short int renderLeft;
 short int renderTop;
 short int renderRight;
 short int renderBottom;
 
 unsigned char *pri2Ptr2;
 
 int renderV9;
 int renderV11;
 int renderV10;
 
 short int numOfPrimitives;
 renderTabEntry *renderTabEntryPtr;
 renderTabEntry *renderTabEntryPtr2;
 renderTabEntry *renderTabSortedPtr;
 unsigned short int costumeHeader;
 
 renderTabEntry renderTab[1000];
 renderTabEntry renderTabSorted[1000];
 short int polyTab[960];
 short int polyTab2[960];
 
 short int numOfPri1;
 unsigned char *pri1Ptr;
 
 short int numOfPri2;
 unsigned char *pri2Ptr;

 int mainTab[57]; // attention, variables liees
 int renderTab2[271];
 int *renderTab3;

 unsigned char *renderV19;
 
 pointTab renderTab5[800];
 pointTab renderTab6[800];
 short int shadeTable[500];
 
 int *renderV21;

 
 int rs1v1;
 int rs1v2;
 
 short int rs1s2v1;
 unsigned char* rs1s2v2;
 
 short int primitiveCounter;
 

 
// int rmv1;
// int rmv2;
// int rmv3;
// int rmv4;
// int rmv5;
// int rmv6;
// int rmv7;
// int rmv8;
// int rmv9;
// int rmv10;
 
// int renderV22;
// int renderV23;
// int renderV24;
 
 unsigned char renderTab7[10000];
 
 unsigned char* render23;
 
 short int render25;
 
 unsigned char* render24;

 int renderLoop;

 int updateActorScript;

 int shadowVar1;
 int shadowVar2;
 int shadowVar3;

//----------------------------------------------------------------------------------------

 void loadImageCrossFade(int imageNumber);

 void convertPalToRGBA(byte* palSource,byte* palDest);

	int fullRedrawS3(int param0,int param1,int param2,int param3,int param4,int param5);

  int draw3D4sub1(timeStruct *arg_0);

  void setTextWindowSize(int left, int top, int right, int bottom);

  void renderS2S2(short int eax, short int ebx, short int ecx);


	int finishRender(unsigned char *esi);

	void renderS1S2(unsigned char * esi, int ecx, pointTab *edi, int* eax);
 
 void renderS1S1(int* eax, int* ebp);

	void renderS1(int edx, int ecx, int ebx, pointEntry* esi);

	int prepareRender(void);

	int printText10SubSub2(void);

 void printText10SubSub(int arg0, int arg_4);
	void polyRender(int ecx, int edi);

	int renderM1(unsigned char *ptr);
 
  int renderM2(unsigned char *ptr);


	int startRenderer(int arg_0,int arg_4,int arg_8,int arg_C,int arg_10,int arg_14,unsigned char* arg_18);


	void draw3D4(short int arg_0, short int arg_4, short int arg_8, short int arg_C, short int arg_10, short int arg_14, unsigned char * arg_18);

	void loadActorCostumeSub(unsigned char *arg_0, unsigned char* arg_4);

	void drawInGameMenuEntry(int anim, int arg, int costume);

	void drawBlackBox(int a,int b,int c,int d, unsigned char e);


	void drawMenuWin2(short int arg_0, short int arg_4);

	int drawMenuWin1(int arg_0, unsigned char * ptr, unsigned char* arg_8);


	int drawInventory2(hqr_entry *hqrPtr, int var);


	subHqr* findSubHqr(int arg_0, int arg_4, subHqr * arg_8);

	unsigned char* getHqrdataPtr(hqr_entry* arg_0, short int arg_4);

	void drawMenuWin(short int var);

	void drawInGameMenu(void);

	int fullRedrawS2S1(int arg_0, int arg_4, int arg_8);

	void loadGfxSub(unsigned char * ptr);

	int loadBody(int costumeNum, int actorNum);

	int doCalc(void);

	void manipActor(actor* lactor);

	int initNewCostume(char costume, short int arg_4, unsigned char arg_8, short int actorNum);

	void loadActorCostume(char arg_0, short int arg_4);
 
 void changeTwinsenComp(int newCostume);

	void loadVariousGFX(void);

	void drawSprite(int num, int var1, int var2, unsigned char* localBufferBrick);

	void zbufferSub2(int y, int z, int x);
 
 unsigned char * zbufferSub1(int var);

	void zbuffer(int var1, int var2, int y, int z, int x);

	void fullRedrawSub2(void);

	void runActorScript(short int actorNumber);

	void addToRedrawBox(short int arg_0, short int arg_4, short int arg_8, short int arg_C);

	void fullRedrawSub5(void);

	void fullRedrawSub1(void);

	void fullRedrawSub3(short int arg_0, short int arg_4, short int arg_8, short int arg_C);

	void setActorTime(short int arg_0, short int arg_4, short int arg_8, timeStruct *timePtr);

 void resetActor(int actorNumber);
 
 void reinitAll3(void);
 
 void reinitVars(void);

	void unfreezeTime(void);
	int mainLoop4(void); 

	void freezeTime(void);





	void mainLoop2sub1(void);

	int getCurrentlyPlayingMusic(void);
 void fullRedraw(int param);
	void waitRetrace(void);
	void mainLoop1(short int arg0, short int arg4, short int arg8, timeStruct *ptr);

	void createCube(void);
	void addCubeEntry(unsigned char* gridEntry, unsigned char* dest);

	void memoryBufferProcess(unsigned char * bufferBrick2, int size);

	int processBuffer2BufferSub(int var,unsigned int *ptr1,unsigned int *ptr2);

	int processBuffer2Buffer(unsigned int* buffer1, unsigned int* buffer2);


	void loadBrkSub1(unsigned char * ptr,int size);

	int loadBrk(int gridSize);

	int prepareResource(char * fileName, int index);

	void makeFireEffect(int top,int param);
	void makeFireEffectInit(void);

	int allocHQRMemory(int dataSize, byte** ptr);

	void loadDataFileToPtr(char *fileName, short int arg_4, byte** ptr);


	int initCostume(byte arg_0,short int actorNumber);

	void changeRoom1Sub1(hqr_entry *ptr);

	void changeRoom1Sub2(void);

	void reinitData(void);


	void loadRoomActors(short int arg_0);

	void reinitTwinsen(void);

	void saveGame(void);

	int loadGridAndBll(short int roomNumber);

	void playMusicSub2(short int arg_0);


	int playMusicSub(void);

	void changeRoomSub1(int arg_0);

	void mainLoop2(int arg_0);

	void loadRoomScene(int sceneNumber);

	void loadTwinsenCostumes(void);


	void changeRoom1(void);

	void printText8Sub2(void);

	void drawLetter2(int x, int y, int c);

	void drawDoubleLetter(int a, int b, int c, int d);


	void printText8Sub4(short int a, short int b, short int c);

	void setSomething4(int a, int b, int c);

	void printText10Sub2(void);

	void printText10Sub3(byte c,int *b,int *a,byte *font);


	void getWordSize(char* arg1, char* arg2);	

	short int getKeyboardChar(void);


	int optionMenu(void);

  void reinitAll1(void);

	void newGame4(void);
	void newGame2(void);
	void newGame3(short int param);

	
	void printText10Sub(void);

  void readBufferSpeak(FILE* fileHandle);


	void stopCD(void);


	int loadVox(int index);

	int checkCD(char* string);

	void changeRoom(void);

	void drawCharacter(int X,int Y,unsigned char caractere);

	void printStringSimple(int X,int Y,char *string);


	int getStringLength(char *string);

  int findString(int index);


	
	void drawLine(int a, int b, int c, int d, int e);

	void launchStringProcess(int a,int b, int c, int d, int e);

	void setInitVars(int a,int b, int c);

	void setTextColor(byte i);
	void setTextColorSub(int i);
	void drawButton(short int* data,int a);


	char* getStringFromNum(int valeur);
	void reinitAll(int save);
	int mainLoop(void);

	void checkHeap();

	void fontInit(byte* font,int param1,int param2);

	byte* loadImageToMemory(char* fileName, short int imageNumber);


	int	processMenu(short int* menuData);
	int enterPlayerName(short int param);
	void init(void);

	void loadTextBank(int index);

	void saveTextWindow(void);
	void maximizeTextWindow(void);
	int setVoxFileAtDigit(int index);
	int printText4(FILE* var1);
	void loadSavedTextWindow(void);
	int printText6(int var);


	int printText7(int var);
	int printText8(int var);
	void drawTextBox(void);
	int printText10(void);
	int printText11(void);

	void drawButtonGFX(int largeur, int posY, int c,int d,int mode);

	void resetPalette(void);
	void resetVideoBuffer1(void);

	void printTextFullScreen(int textIndex);
	
	void copyStringToString(char *a, char *b, int c);

	void mainMenu2(void);
	char* printString(int a,char* b);
	void playCDtrack(int trackNumber);


	void fadeOut(char* palette);
	void loadImageAndPalette(int imageNumber);
	void waitFor(void);
	void fadeIn2(char* ptr);


	void readKeyboard(void);

	void drawBoxTrans(int right, int top,int left, int bottom, char* dest, int right2, int top2, char* source);

	void drawBoxInsideTrans(int left,int top, int right, int bottom, int mode);

	void blackToWhite(void);
	void newGame(void);

	FILE* fileOpen(char* fileName);
	FILE* openResource(char* fileName);
	void closeResource(FILE* resourceFile);

	int readResourceData(FILE* resourceFile,char* ptr, int length);

	void allocBufCube(void);

	byte* allocateBuf(int size);

	void fadeIn(byte* palette);
	void adjustPalette(byte R, byte G, byte B, byte* palette, int a);

	int remapComposante(int modifier, int color, int param, int intensity);
	void setPalette(byte* palette);


	void decompress(int decompressedSize, unsigned char* destination, unsigned char* source);
	void dumpFile(char* name,char* ptr,int size);



	void initVideoStuff(void);

	void copyToBuffer(byte* source, byte* destination);

	void displayAdelineLogo(void);
	void playMidi(int musicNum);
	void playMusic(int musicNum);


	int loadImageToPtr(char* resourceName,byte* Ptr,int imageNumber);
	byte allocHQMemory(int size);

	hqr_entry* load_hqr(char* fileName, int a, int b);

	void loadCFG(void);
	void initAll(char* fileName,int a);

	void initVars(void);

	void mainMenu(void);

	void playFLA(char* flaName);
	
	void drawBoxOutLine(int left, int top, int right, int bottom);

	void setSomething(int a, int b, int c);
	void setSomething2(int a, int b, int c);
	void setSomething3(int a,int b,int c);

	void stopMusic(void);
	void fullStopMusic(void);

	void optionMenu2(void);


	void drawSelectableLetters(void);

	void drawSelectableLetter(int y, int x, int arg);



	int choosePlayerName(int param);

	void processTextLine(void);

};