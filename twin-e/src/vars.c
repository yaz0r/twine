#include "lba.h"
#include "tab.h"

void LBA_engine::initVars(void)
{
    useSound = 0;
    useSB = 1;
    loadMidiResult = 0;
    unkPtr = 0;			// recheck

    videoBuffer2 = 0;
    videoBuffer1 = 0;
    videoBuffer1bis = 0;

    bufMemoSeek = 0;
    bufText = 0;
    bufOrder = 0;
    bufAni1 = 0;
    bufAni2 = 0;
    HQRInventory = 0;
    menuPal = 0;
    shadowSprite = 0;
    HQRess3 = 0;
    lbaFont = 0;

    bufCube = 0;

    HQMemory = 0;
    HQMemory2 = 0;
    HQMemorySize = 0;
    HQMemorySize2 = 0;

    fntFont = 0;
    interCharSpace = 1;
    spaceLenght = 10;

    setup_lst = 0;

    textVar1 = -1;
    textVar3 = 0;

    voxFileHandle = 0;
    printTextVar5 = 0;
    key1 = 0;
    printTextVar12 = 0;
    printTextVar13 = 0;

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

    skipIntro = 0;
    palReseted = 0;

    needChangeRoom = -1;
    chapter = 0;
    newGameVar4 = 1;
    newGameVar5 = 0;

    currentRoom = 0;
    mainMenuVar3 = 'S';
    mainMenuVar4 = 0;
    shadowMode = 2;
    time = 0;
    key = 0;

    mainMenuData[0] = 0;	// 0
    mainMenuData[1] = 4;	// nombre de boutons //2
    mainMenuData[2] = 200;	// 4
    mainMenuData[3] = 0;	// 6
    mainMenuData[4] = 0;	// 8
    mainMenuData[5] = 20;	// Exit point 1 //10 -> debut des points de sortie // 20->newGame
    mainMenuData[6] = 0;
    mainMenuData[7] = 21;	// Exit point 2 // 21->continueGame
    mainMenuData[8] = 0;
    mainMenuData[9] = 23;	// Exit point 3 // 23->options
    mainMenuData[10] = 0;
    mainMenuData[11] = 22;	// Exit point 4 // 22->quit

    subMenu2Data[0] = 0;
    subMenu2Data[1] = 2;
    subMenu2Data[2] = 240;
    subMenu2Data[3] = 0;
    subMenu2Data[4] = 0;
    subMenu2Data[5] = 28;
    subMenu2Data[6] = 0;
    subMenu2Data[7] = 27;

    soundMenuData[0] = 0;
    soundMenuData[1] = 4;
    soundMenuData[2] = 0;
    soundMenuData[3] = 0;
    soundMenuData[4] = 0;
    soundMenuData[5] = 24;
    soundMenuData[6] = 0;
    soundMenuData[7] = 30;
    soundMenuData[8] = 0;
    soundMenuData[9] = 46;
    soundMenuData[10] = 0;
    soundMenuData[11] = 47;

    subMenuData[0] = 0;
    subMenuData[1] = 5;
    subMenuData[2] = 0;
    subMenuData[3] = 0;
    subMenuData[4] = 0;
    subMenuData[5] = 26;
    subMenuData[6] = 0;
    subMenuData[7] = 4;
    subMenuData[8] = 6;
    subMenuData[9] = 31;
    subMenuData[10] = 7;
    subMenuData[11] = 32;
    subMenuData[12] = 8;
    subMenuData[13] = 33;

    buttonDrawVar1 = 550;

#ifdef ENGLISH_LANG
    language = 0;
#else
    language = 1;		// langue ecrite
#endif

    languageCD1 = 0;

    mainMenu3Var1 = -1;
    mainMenu3Var2 = -1;

    needChangeRoomVar1[0] = 0;
    isMenuDisplayed = 0;
    drawInGameTransBox = 0;

    setSomethingVar1 = 320;
    setSomethingVar2 = 200;
    setSomethingVar3 = 500;
    renderer.setSomethingVar4 = 0;
    setSomethingVar4 = 0;

   // setSomething2Var1=0;
   // setSomething2Var2=0;
   // setSomething2Var3=0;

    dialogueBoxParam1 = 3;
    dialogueBoxParam2 = 591;

    playMusciVar1 = -2;

    currentlyPlayingMusic = -1;

    playMusicFlag = 0;

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

    enterPlayerNameVar2 = 0;

    spaceChar = ' ';

    vertexCoordinates[0] = 0x1234;
   /*
    * backDialogueBoxRight=0; backDialogueBoxBottom=0; back2InitVar3=0; back2DialogueBoxRight=0;
    * back2DialogueBoxBottom=0; backInitVar2=0; backUnk1=0; back3DialogueBoxRight=0; backUnk2=0;
    * back3DialogueBoxBottom=0; 
    */

    pt8s4var2 = pt8s4var1 + 1;
    pt8s4var3 = pt8s4var1 + 2;

    pt8s4var4 = pt8s4var1 + 93;
    pt8s4var5 = pt8s4var1 + 94;
    pt8s4var6 = pt8s4var1 + 95;

    tab1 = &tab[0];
    tab2 = &tab[256];
    tab3 = &tab[384];

    twinsen = &actors[0];

    changeRoomVar1 = -1;
    roomMusic = -1;
    playMusicVar1 = -2;

    reinitVar11 = 0;

    GV9dup = 0;
    GV10dup = 0;
    GV11dup = 0;

    sceneVar16 = 0;
    sceneVar17 = 0;
    sceneVar18 = 0;

    changeRoomVar2 = 0;

    numActorInRoom = 0;

    mainLoopVar10 = 0;

    changeRoomVar3 = 0;
    newCameraX = 0;
    newCameraZ = 0;
    newCameraY = 0;

    changeRoomVar7= -1;
    changeRoomVar8 = 0;
    changeRoomVar9 = -1;
    currentGrid2 = -1;
    mainLoopVar2 = 1;
    mainLoopVar3 = 0;

    changeRoomVar10 = 1;
    changeRoomVar11 = 0;

    reinitAll2Var3 = 0;
    reinitAll2Var4 = 0;
    numFlags = 0;

    file3D0 = NULL;
    file3D1 = NULL;

    file3D2 = NULL;
    file3D3 = NULL;
    file3D4 = NULL;

    TCos0Init = 0;
    TCos1Init = 0;
    TCos2Init = 0;
    TCos3Init = 0;
    TCos4Init = 0;

    fireEffectVar1 = NULL;
    fireEffectVar2 = NULL;

    sceneRoomNumber = 0;

    sceneVar2.field_0 = 0;
    sceneVar2.field_2 = 0;
    sceneVar2.field_4 = 0;
    sceneVar2.field_6 = 0;

    sceneVar3.field_0 = 0;
    sceneVar3.field_2 = 0;
    sceneVar3.field_4 = 0;
    sceneVar3.field_6 = 0;

    sceneVar4.field_0 = 0;
    sceneVar4.field_2 = 0;
    sceneVar4.field_4 = 0;
    sceneVar4.field_6 = 0;

    sceneVar14 = 0;
    sceneVar15 = 0;
    sceneVar16 = 0;
    sceneVar17 = 0;
    sceneVar18 = 0;
    sceneVarPtr = 0;
    flagData = 0;

    mainLoopVar4 = 0;
    agressivity = 1;

    mainLoopVar5 = 0;
    mainLoopVar6 = 0;
    mainLoopVar7 = 0;

    musicPosition = 0;
    musicLength = 0;

    useSamples = 0;

    time1 = 0;
    time3 = 0;

    counter = 0;

    numOfRedrawBox = 0;

    fullRedrawVar8 = 0;

    scanCodeTab1[0] = 0x48;
    scanCodeTab1[1] = 0x50;
    scanCodeTab1[2] = 0x4B;
    scanCodeTab1[3] = 0x4D;
    scanCodeTab1[4] = 0x47;
    scanCodeTab1[5] = 0x49;
    scanCodeTab1[6] = 0x51;
    scanCodeTab1[7] = 0x4F;

    scanCodeTab1[8] = 0x39;
    scanCodeTab1[9] = 0x1C;
    scanCodeTab1[10] = 0x1D;
    scanCodeTab1[11] = 0x38;
    scanCodeTab1[12] = 0x53;
    scanCodeTab1[13] = 0x2A;
    scanCodeTab1[14] = 0x36;
    scanCodeTab1[15] = 0x3B;
    scanCodeTab1[16] = 0x3C;
    scanCodeTab1[17] = 0x3D;
    scanCodeTab1[18] = 0x3E;
    scanCodeTab1[19] = 0x3F;
    scanCodeTab1[20] = 0x40;
    scanCodeTab1[21] = 0x41;
    scanCodeTab1[22] = 0x42;
    scanCodeTab1[23] = 0x43;
    scanCodeTab1[24] = 0x44;
    scanCodeTab1[25] = 0x57;
    scanCodeTab1[26] = 0x58;
    scanCodeTab1[27] = 0x2A;
    scanCodeTab1[28] = 0x0;

    scanCodeTab2[0] = 0x0100;	// up
    scanCodeTab2[1] = 0x0200;	// down
    scanCodeTab2[2] = 0x0400;	// left
    scanCodeTab2[3] = 0x0800;	// right
    scanCodeTab2[4] = 0x0500;	// home
    scanCodeTab2[5] = 0x0900;	// pageup
    scanCodeTab2[6] = 0x0A00;	// pagedown
    scanCodeTab2[7] = 0x0600;	// end

    scanCodeTab2[8] = 0x0101;	// space bar
    scanCodeTab2[9] = 0x0201;	// enter
    scanCodeTab2[10] = 0x0401;	// ctrl
    scanCodeTab2[11] = 0x0801;	// alt
    scanCodeTab2[12] = 0x1001;	// del
    scanCodeTab2[13] = 0x2001;	// left shift
    scanCodeTab2[14] = 0x2001;	// right shift

    scanCodeTab2[15] = 0x0102;	// F1
    scanCodeTab2[16] = 0x0202;	// F2
    scanCodeTab2[17] = 0x0402;	// F3
    scanCodeTab2[18] = 0x0802;	// F4
    scanCodeTab2[19] = 0x1002;	// F5
    scanCodeTab2[20] = 0x2002;	// F6
    scanCodeTab2[21] = 0x4002;	// F7
    scanCodeTab2[22] = 0x8002;	// F8

    scanCodeTab2[23] = 0x0103;	// F9
    scanCodeTab2[24] = 0x0203;	// F10
    scanCodeTab2[25] = 0x0403;	// ?
    scanCodeTab2[26] = 0x0803;	// ?
    scanCodeTab2[27] = 0x00FF;	// left shift
    scanCodeTab2[28] = 0x00FF;
    scanCodeTab2[29] = 0x0;
    scanCodeTab2[30] = 0x0;

    loadCostumeVar = -32000;

    drawInventoryVar = 0;

    renderTab3 = &renderTab2[9];

    action = 0;

	useFlaPCX=1;
	flaVar2=1;

	showTalkVar=1;

   /*
    * setSomething2Var1; setSomething2Var2; setSomething2Var3; setSomething3Var12;
    * setSomething3Var14; setSeomthing3Var16; reinitVar1; reinitVar2; reinitVar12;
    * setSomething3Var1; setSomething3Var8; setSomething3Var17; renderV1; renderV2; renderV3;
    * renderV22; renderV23; renderV24; destX; destZ; destY; rmv0; rmv1; rmv2; rmv3; rmv4; rmv5;
    * rmv6; rmv7; rmv8; rmv9; rmv10; rm2v0; m2v1; m2v2; m2v3; m2v4; m2v5; m2v6; m2v7;
    * setSomething3Var2; int setSomething3Var3; int setSomething3Var18; int setSomething3Var4; int
    * setSomething3Var5; int setSomething3Var9; int setSomething3Var6; int setSomething3Var7; int
    * setSomething3Var10; 
    */

}

void LBA_engine::setTextColor(byte i)
{
    int a = i;

    setTextColorSub(a);
}

void LBA_engine::setTextColorSub(int i)
{
    textColor = i;
}

void LBA_engine::setInitVars(int a, int b, int c)
{
    initVar2 = b;
    initVar3 = a;
    initVar4 = c;

    initVar5 = ((b - a) + 1) / c;

}
