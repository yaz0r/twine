/***************************************************************************
                          mainLoop.cpp  -  description
                             -------------------
    begin                : Sun Feb 3 2002
    copyright            : (C) 2002 by 
    email                : 
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "lba.h"

int
  LBA_engine::mainLoop(void)
{
    int temp;
    int i;

    mainLoopVar2 = 1;
    mainLoopVar3 = 1;
    setActorAngle(0, -256, 5, &mainLoopVar1);

    do {
	readKeyboard();
	if (mainLoopVar4 > 500)
	    waitRetrace();

	if (needChangeRoom != -1)
	    changeRoom();

	mainLoopVar6 = mainLoopVar5;
	key = printTextVar12;
	mainLoopVar5 = key1;
	mainLoopVar7 = skipIntro;

	if (mainMenuVar4 != 0) {
	    if (getCurrentlyPlayingMusic() != 8)
		playCDtrack(8);
	    if (skipIntro != 0)

		return (0);
	    if (printTextVar12 != 0)
		return (0);
	    if (key1 != 0)
		return (0);
	} else {
	    if (mainLoopVar3 == 0) {
	       // debut des inputs

		if (skipIntro == 1 && twinsen->life > 0 && twinsen->costumeIndex != -1 && (twinsen->field_60 & 0x200))	// recheck
		{
		    mainLoop2(1);
		    freezeTime();
		    if (!mainLoop4()) {
			unfreezeTime();
			fullRedraw(1);
		    } else {
			unfreezeTime();
			fullRedraw(1);
			freezeTime();
			saveGame();
			unfreezeTime();
			return (0);
		    }
		}
		if (mainLoopVar7 == 64)	// F6
		{
		    temp = languageCD1;
		    freezeTime();
		    mainLoop2(1);
		    soundMenuData[5] = 15;
		    languageCD1 = 0;
		    loadTextBank(0);
		    optionMenu();
		    languageCD1 = temp;
		    loadTextBank(currentTextBank + 3);

		    if (currentRoom == 80) {
		       /*
		        * if(mainLoopVar8==1) playCDtrack(8); else playMusic(roomMusic); 
		        */
		    } else {
			playMusic(roomMusic);
		    }
		    unfreezeTime();
		    fullRedraw(1);
		}
		mainLoopVar9 = -1;
		if ((byte) mainLoopVar5 & 0x20 && twinsen->costumeIndex != -1 && twinsen->field_40 == 1)	// inventory 
														// 
		   // 
		   // menu
		{
		    freezeTime();
		    mainLoop2(1);
		   // processInput();
		   // todo: implementer ce cas
		}
		if ((byte) mainLoopVar5 & 4 && twinsen->costumeIndex != -1 && twinsen->field_40 == 1)	// comportement 
													// 
		   // 
		   // menu
		{
		    freezeTime();
		    mainLoop2(1);
		    processComportementMenu();
		    unfreezeTime();
		    fullRedraw(1);
		}
		if (mainLoopVar7 >= 59 && mainLoopVar7 <= 62 && twinsen->costumeIndex != -1 && twinsen->field_40 == 1)	// F1-F4
		{
		    freezeTime();
		    mainLoop2(1);
		    changeTwinsenComp(mainLoopVar7 - 59);
		    processComportementMenu();
		    unfreezeTime();
		    fullRedraw(1);
		}
		if ((byte) mainLoopVar5 & 2 && mainLoopVar10 == 0)	// recenter screen
		{
		    newCameraX = actors[reinitVar8].X >> 9;
		    newCameraZ = actors[reinitVar8].Z >> 8;
		    changeRoomVar6 = actors[reinitVar8].Y >> 9;
		    mainLoopVar2 = 1;
		    needChangeRoom = currentRoom + 1;
		}

/***********************************************/
	       /*
	        * camera debugger 
	        */
/***********************************************/

		if (printTextVar12 & 2)	// x-- -> bas
		{
		    changeRoomVar6++;
		    mainLoopVar2 = 1;
		}

		if (printTextVar12 & 1)	// x++ -> haut
		{
		    changeRoomVar6--;
		    mainLoopVar2 = 1;
		}

		if (printTextVar12 & 4)	// y-- -> gauche
		{
		    newCameraX--;
		    mainLoopVar2 = 1;
		}

		if (printTextVar12 & 8)	// y++ -> droite
		{
		    newCameraX++;
		    mainLoopVar2 = 1;
		}

/**********************************************/
	       // angle debug

	       // printf("from %d to %X\n",twinsen->time.from,twinsen->time.to);

/**********************************************/

		if (mainLoopVar7 == 'h' && vars[0] == 1 && vars[28] == 0)	// draw holomap
		   // (vars[0] = have
		   // holomap)
		   // (vars[28]=holomapMode)
		{
		    freezeTime();
		    mainLoop2(1);
		   // processHolomap();
		    mainLoopVar3 = 1;
		    unfreezeTime();
		    fullRedraw(1);
		}

		if (mainLoopVar7 == 'p') {
		   // pauseSound();
		    freezeTime();
		    if (!drawInGameTransBox) {
			setTextColor(15);
			printStringSimple(5, 446, "Pause");
			osystem->refresh(videoBuffer1, 5, 446, 100, 479);
		    }
		    readKeyboard();
		    while (!skipIntro) {
			readKeyboard();
		    };
		    do {
			readKeyboard();
		    }
		    while (!skipIntro && !printTextVar12 && key1);
		    if (!drawInGameTransBox) {
			drawBoxTrans(5, 446, 100, 479, (char *) videoBuffer2,
				     5, 446, (char *) videoBuffer1);
			osystem->refresh(videoBuffer1, 5, 446, 100, 479);
		    }
		    unfreezeTime();
		   // resumeSound();
		}
	       // il manque un process des input l� pour F5

	       // fin des inputs
	    }
	}
	mainLoopVar17 = mainLoopSub17(&mainLoopVar1);
	if (!mainLoopVar17)
	    mainLoopVar17 = 1;

	setActorAngle(0, -256, 5, &mainLoopVar1);
	mainLoopVar10 = 0;
       // mainLoopSub18();

	for (i = 0; i < numActorInRoom; i++) {
	    actors[i].field_64 = -1;
	}

       // mainLoopSub19(); //process all the objects in the room

       // 2nd boucle de process des acteurs

	for (i = 0; i < numActorInRoom; i++) {
	    if (!(actors[i].field_62 & 0x20)) {
	       // printf("Processing actor %d...\n",i);
		if (actors[i].life == 0) {
		    if (i == 0) {
			playAnim(10, 4, 0, 0);	// play twinsen death anim
			actors[i].field_40 = 0;
		    } else {
			printf("Actor %d is dead !\n", i);
		    }

		    if (actors[i].field_10 & 0x1F0 && !(actors[i].field_10 & 1)) {
			printf("Walk actor %d\n", i);
		    }
		}
		updateActors(i);

		actors[i].field_20 = actors[i].X;
		actors[i].field_22 = actors[i].Z;
		actors[i].field_24 = actors[i].Y;

	       // todo: should not be here:
		destX = actors[i].X;
		destZ = actors[i].Z;
		destY = actors[i].Y;

		if (actors[i].positionInMoveScript != -1) {
		    moveActor(i);
		}

		processActor(i);

		if (actors[i].field_60 & 4) {
		   checkZones(&actors[i],i);
		}
		if (actors[i].positionInActorScript != -1) {
		    runActorScript(i);
		}
	       // implementer la suite...

	    }
	}

	if (_debugger.processDebug())
	    mainLoopVar2 = 1;

	if (mainLoopVar10) {
	   // implement this
	}
	fullRedraw(mainLoopVar2);
	mainLoopVar2 = 0;
	counter++;
    }
    while (1);
}

void LBA_engine::reinitAll(int save)
{
    maximizeTextWindow();
    reinitVar1 = 896;
    reinitVar2 = 950;
    reinitAll1();
    reinitVars();
    reinitAll3();
    GV9 = 0x2000;
    GV10 = 0x1800;
    GV11 = 0x2000;
    currentRoom = -1;
    reinitVar3 = -1;
    numClover = 2;
    numCloverBox = 2;
    reinitVar4 = -1;
    needChangeRoom = 0;
    GV6 = 0;
    GV7 = 0;
    numCoin = 0;
    numKey = 0;
    newGameVar2 = 0;
    GV18 = 0;
    currentTextBank = 0;
    GV15 = 0;
    reinitVar7 = 0;
    reinitVar8 = 0;
    reinitVar9 = 0;
    comportement = 0;
    reinitVar10 = 0;
   /*
    * if(save==-1) { loadSaveGame(); if(GV9==-1) { reinitVar11=0; } } 
    */
   /*
    * fadeOut((char*)menuPalRGBA); resetVideoBuffer1(); osystem->drawBufferToScreen(videoBuffer1); 
    */
}

void LBA_engine::reinitAll1(void)
{
    setSomething(311, 240, 512);
    setSomething2(0, 0, 0);
    setSomething3(0, 0, 0);
    setSomething4(reinitVar2, reinitVar1, 0);
}

void LBA_engine::setSomething(int a, int b, int c)
{
    setSomethingVar1 = a;
    setSomethingVar2 = b;
    setSomethingVar3 = c;
    setSomethingVar4 = 1;

    renderer.setSomethingVar1 = a;
    renderer.setSomethingVar2 = b;
    renderer.setSomethingVar3 = c;
    renderer.setSomethingVar4 = 1;
}

void LBA_engine::setSomething2(int a, int b, int c)
{
    setSomething2Var1 = a;
    setSomething2Var2 = b;
    setSomething2Var3 = c;
}

void LBA_engine::setSomething3(int a, int b, int c)	// setupBaseMatrix
{

    int var1;
    int var2;
    int var3;
    int var4;
    int var5;
    int var6;
    int var7;

    setSomething3Var1 = a & 0x3FF;

    var1 = tab1[setSomething3Var1];

    var2 = tab1[(setSomething3Var1 + 256) & 0x3FF];

    setSomething3Var17 = c & 0x3FF;

    var3 = tab1[setSomething3Var17];

    setSomething3Var2 = tab1[(setSomething3Var17 + 256) & 0x3FF];
    setSomething3Var3 = -var3;
    setSomething3Var4 = (var3 * var2) >> 14;
    setSomething3Var5 = (setSomething3Var2 * var2) >> 14;
    setSomething3Var6 = (var3 * var1) >> 14;
    setSomething3Var7 = (setSomething3Var2 * var1) >> 14;
    setSomething3Var8 = b & 0x3FF;

    var4 = tab1[setSomething3Var8];

    var5 = tab1[(setSomething3Var8 + 256) & 0x3FF];

    setSomething3Var2 = (setSomething3Var2 * var5) >> 14;
    setSomething3Var18 = (var4 * setSomething3Var2) >> 14;

    var6 = setSomething3Var4;

    setSomething3Var4 = ((var6 * var5) + (var4 * var1)) >> 14;
    setSomething3Var9 = ((var5 * var1) + (var6 * var4)) >> 14;

    var7 = setSomething3Var6;

    setSomething3Var6 = ((var2 * var4) + (var5 * var7)) >> 14;
    setSomething3Var10 = ((var2 * var5) + (var4 * var7)) >> 14;

   // setSomething3Sub(setSomething2Var3,setSomething2Var2,setSomething2Var1);

    setSomething2Var3 = destX;
    setSomething3Var12 = destZ;
    setSomething3Var14 = destY;

    renderer._baseMatrix[0] = setSomething3Var2;
    renderer._baseMatrix[1] = setSomething3Var3;
    renderer._baseMatrix[2] = setSomething3Var18;
    renderer._baseMatrix[3] = setSomething3Var4;
    renderer._baseMatrix[4] = setSomething3Var5;
    renderer._baseMatrix[5] = setSomething3Var9;
    renderer._baseMatrix[6] = setSomething3Var6;
    renderer._baseMatrix[7] = setSomething3Var7;
    renderer._baseMatrix[8] = setSomething3Var10;

}

void LBA_engine::renderS2S2(short int ax, short int bx, short int cx)
{
    int ebp;
    int ebx;
    int ecx;
    int eax;
    int edi;

    ebp = ax;
    ebx = bx;
    ecx = cx;

    edi = bufRotate0[0];
    eax = bufRotate0[1];
    edi *= ebp;
    eax *= ebx;
    edi += eax;
    eax = bufRotate0[2];
    eax *= ecx;
    eax += edi;
    eax >>= 14;

    destX = eax;

    edi = bufRotate0[3];
    eax = bufRotate0[4];
    edi *= ebp;
    eax *= ebx;
    edi += eax;
    eax = bufRotate0[5];
    eax *= ecx;
    eax += edi;
    eax >>= 14;
    destZ = eax;

    ebp *= bufRotate0[6];
    ebx *= bufRotate0[7];
    ecx *= bufRotate0[8];
    ebx += ebp;
    ebx += ecx;
    ebx >>= 14;
    destY = eax;

}

void LBA_engine::mainLoop2(int arg_0)
{
    if (!drawInGameTransBox)
	return;

    if (useSamples) {
       // code de son non g�r�...
    }

    mainLoop2sub1();

    if (isMenuDisplayed)
	osystem->setPalette(menuPalRGBA);
    else
	osystem->setPalette(paletteRGBA);

    drawInGameTransBox = 0;

   // code son non g�g�...

    if (!arg_0)
	fullRedraw(1);
}

void LBA_engine::mainLoop2sub1(void)
{
   // code dpmi non g�r�.

    initVideoVar1 = -1;
}

void LBA_engine::waitRetrace(void)
{
}

void LBA_engine::freezeTime(void)
{
    if (!time1)

	time3 = time;

    time1++;

}

void LBA_engine::unfreezeTime(void)
{

    if (--time <= 0)
	time = time3;
}

int LBA_engine::mainLoop4(void)	// process le menu "continuer ou abandonner"
{
    int saveLangue;
    int temp;

    copyToBuffer(videoBuffer1, videoBuffer2);
    mainMenu2();

    do {
	saveLangue = languageCD1;
	languageCD1 = 0;
	loadTextBank(0);
	temp = processMenu(subMenu2Data);
	languageCD1 = saveLangue;
	loadTextBank(currentTextBank + 3);
    }
    while (temp != 27 && temp != 28);

    if (temp == 27)
	return (1);
    if (temp == 28)
	return (0);

    return (0);
}

void LBA_engine::reinitVars(void)
{
    int i;

   // reinitData();

    for (i = 0; i < 10; i++)
	roomData2[i].field_0 = -1;

    for (i = 0; i < 80; i++)
	roomData1[i] = 0;

    for (i = 0; i < 255; i++)
	vars[i] = 0;

    for (i = 0; i < 28; i++)
	GV16[i] = 0;

    sceneVar2.field_0 = -1;
    sceneVar2.field_2 = -1;
    sceneVar2.field_4 = -1;
    sceneVar2.field_6 = -1;

    sceneVar3.field_0 = 1;
    sceneVar3.field_2 = 1;
    sceneVar3.field_4 = 1;
    sceneVar3.field_6 = 1;

    sceneVar4.field_0 = 1;
    sceneVar4.field_2 = 1;
    sceneVar4.field_4 = 1;
    sceneVar4.field_6 = 1;

    for (i = 0; i < 150; i++)
	GV14[i] = 0;

    numActorInRoom = 0;
    reinitAll2Var3 = 0;
    reinitAll2Var4 = 0;

    numFlags = 0;

}

void LBA_engine::reinitAll3(void)
{
    resetActor(0);
    newCameraY = -1;
    numCloverBox = 2;
    numClover = 2;
    numCoin = 0;
    numKey = 0;
    GV7 = 0;
    GV18 = 0;
    twinsen->field_0 = 0;
    twinsen->life = 50;
    twinsen->talkColor = 4;

}

void LBA_engine::draw3D4(short int arg_0, short int arg_4, short int arg_8,
			 short int arg_C, short int arg_10, short int arg_14,
			 unsigned char *costumePtr)
{
    int var_4;
    int temp1;
    int temp2;
    short int temp;

    var_4 = arg_8;

    temp1 = arg_C + arg_4;
    temp1 >>= 1;

    temp2 = arg_8 + arg_0;
    temp2 >>= 1;

    setSomething(temp2, temp1, 0);
    setTextWindowSize(arg_0, arg_4, var_4, arg_C);

    if (arg_14 == -1) {
	temp = processActorAngle(&timeVar);
	if (timeVar.numOfStep == 0) {
	    setActorAngleSafe(temp, temp - 256, 50, &timeVar);
	}
	startRenderer(0, arg_10, 0, 0, temp, 0, costumePtr);
    } else
	startRenderer(0, arg_10, 0, 0, arg_14, 0, costumePtr);

}

void LBA_engine::setTextWindowSize(int left, int top, int right, int bottom)
{
    if (left < 0)
	left = 0;

    textWindowLeft = left;

    if (top < 0)
	top = 0;
    textWindowTop = top;

    if (right > largeurEcran)
	right = largeurEcran - 1;
    textWindowRight = right;

    if (bottom > hauteurEcran)
	bottom = hauteurEcran - 1;
    textWindowBottom = bottom;
}

int LBA_engine::getAnimMaxIndex(char *ptr)
{
    return (*(short int *) ptr);
}

int LBA_engine::getAnimStartIndex(char *ptr)
{
    return (*(short int *) (ptr + 4));
}

void LBA_engine::drawBlackBox(int left, int top, int right, int bottom, unsigned char e)
{

    unsigned char *ptr;

    int offset;

    int x;
    int y;

    if (left > textWindowRight)
	return;
    if (right < textWindowLeft)

	return;
    if (top > textWindowBottom)
	return;
    if (bottom < textWindowTop)
	return;

   // cropping

    offset = -((right - left) - largeurEcran);

    ptr = videoBuffer1 + screenLockupTable[top] + left;

    for (x = top; x < bottom; x++) {
	for (y = left; y < right; y++) {

	    *(ptr++) = e;
	}
	ptr += offset;
    }

}

unsigned char *LBA_engine::getHqrdataPtr(hqr_entry * hqrPtr, short int arg_4)
{

    FILE *fin;
    int headerSize;
    int offToData;
    int dataSize;
    int compressedSize;
    short int mode;

    short int var_4;
    int ltime;
    int temp2;
    unsigned char *ptr;

    int i;

    int dataSize2;
    subHqr *hqrdata;
    subHqr *hqrdataPtr;

    if (arg_4 < 0)
	return (0);

    hqrdata = (subHqr *) ((unsigned char *) hqrPtr + sizeof(hqr_entry));

    hqrdataPtr = findSubHqr(arg_4, hqrPtr->unk, hqrdata);

    if (hqrdataPtr) {
	hqrdataPtr->lastAccessedTime = time;
	drawInventoryVar = 0;
	return (hqrdataPtr->offFromPtr + hqrPtr->ptr);
    }
   // printf ("Need to load: %s-%d\n", hqrPtr->fileName, arg_4);

    prepareResource(hqrPtr->fileName, arg_4);

    fin = openResource(hqrPtr->fileName);

    if (!fin)
	return (0);

    readResourceData(fin, (char *) &headerSize, 4);

    if (arg_4 >= headerSize / 4) {
	closeResource(fin);
	return (0);
    }

    fseek(fin, arg_4 * 4, SEEK_SET);
    readResourceData(fin, (char *) &offToData, 4);

    fseek(fin, offToData, SEEK_SET);
    readResourceData(fin, (char *) &dataSize, 4);
    readResourceData(fin, (char *) &compressedSize, 4);
    readResourceData(fin, (char *) &mode, 2);

    dataSize2 = dataSize;

   // ici, test sur la taille de dataSize

    ltime = time;

    while (dataSize2 >= hqrPtr->remainingSize || hqrPtr->unk >= hqrPtr->b)	// pour retirer les 
										// 
       // 
       // elements les
       // plus vieux
       // jusqu'a ce qu'on 
       // ai de la place
    {
	var_4 = 0;
	temp2 = 0;

	for (i = 0; i < hqrPtr->unk; i++) {
	    if (temp2 <= ltime - hqrdata[i].lastAccessedTime) {
		temp2 = ltime - hqrdata[var_4].lastAccessedTime;
		var_4 = i;
	    }
	}

	drawInventory2(hqrPtr, var_4);

    }

    ptr = hqrPtr->ptr + hqrPtr->size1 - hqrPtr->remainingSize;

    if (mode <= 0)		// uncompressed
    {
	readResourceData(fin, (char *) ptr, dataSize);
    } else {
	if (mode == 1)		// compressed
	{
	    readResourceData(fin, (char *) (ptr + dataSize - compressedSize + 500), compressedSize);
	    decompress(dataSize, ptr, (ptr + dataSize - compressedSize + 500));
	}

	else {
	    closeResource(fin);
	    return (0);
	}
    }

    closeResource(fin);

    hqrdataPtr = &hqrdata[hqrPtr->unk];

    hqrdataPtr->index = arg_4;
    drawInventoryVar = 1;
    hqrdataPtr->lastAccessedTime = time;
    hqrdataPtr->offFromPtr = hqrPtr->size1 - hqrPtr->remainingSize;
    hqrdataPtr->size = dataSize2;

    hqrPtr->unk++;
    hqrPtr->remainingSize -= dataSize2;

    return (ptr);

}

subHqr *LBA_engine::findSubHqr(int arg_0, int arg_4, subHqr * arg_8)
{
    subHqr *temp;
    int i;

    if (arg_4 == 0)
	return (0);

    temp = arg_8;

    for (i = 0; i < arg_4; i++) {
	if (temp->index == arg_0)
	    return (temp);

	temp++;
    }

    return (0);

}

int LBA_engine::drawInventory2(hqr_entry * hqrPtr, int var)
{
    subHqr *subPtr;

    int lvar;

    unsigned char *source;
    unsigned char *dest;
    int size;
    int retVal;

    lvar = var;

    subPtr = (subHqr *) (hqrPtr + sizeof(hqr_entry) + var * sizeof(subHqr));

    retVal = subPtr->size;

    if (hqrPtr->unk - 1 > lvar) {
	source = hqrPtr->ptr + subPtr->offFromPtr;

	dest = source + retVal;
	size = hqrPtr->ptr + hqrPtr->size1 - dest;

	memmove(source, dest, size);

	printf("Unsupported drawInventory2\n");
	exit(1);
       // implementer la suite
    }

    hqrPtr->unk--;

    hqrPtr->remainingSize = retVal;

    return (retVal);
}

void LBA_engine::updateActors(int actorNum)
{
    actor *lactor;

    lactor = &actors[actorNum];

    if (lactor->costumeIndex == -1)
	return;

    if (lactor->field_62 & 0x100) {
    } else {
	if (!(lactor->field_60 & 0x400)) {
	    if (lactor->field_40 != 1) {
		lactor->angle = processActorAngle(&lactor->time);
	    }
	}

	if (lactor->field_40 > 6)
	    return;

	switch (lactor->field_40) {
	case 0:
	    break;
	case 1:		// comp_normal
	    if (!actorNum)	// if it's twinsen
	    {
		int angleModif;

		updateActorScript = 0;
		switch (comportement) {
		case 0:	// normal
		    if (mainLoopVar5 & 1) {
			updateActorScript = 1;
		    }
		    break;
		case 3:	// discret
		    if (mainLoopVar5 & 1) {
			playAnim(16, 0, 255, 0);
		    }
		    break;
		}

		angleModif = 0;

		if (key & 4) {
		    angleModif = 0x100;
		}

		if (key & 8) {
		    angleModif = -0x100;
		}

		updateActorAngle(lactor->angle, lactor->angle + angleModif,
				 lactor->field_34, &lactor->time);
	    } else {
	    }
	    break;
	case 2:
	    break;
	case 3:
	    break;
	case 4:
	    break;
	case 5:
	    break;
	case 6:
	    break;
	default:
	    printf("Unhandled comportement %d in update actors\n", lactor->field_40);
	    exit(1);
	}
    }
}

void LBA_engine::processActor(int actorNum)
{
    actor *lactor;
    char *animPtr;
    int animData;
    short int var_10;

   // short int var_C;
   // short int var_4;
   // short int var_8;

    lactor = &actors[actorNum];

    currentlyProcessedActorNum = actorNum;
    processActorVar1 = lactor;

    if (lactor->costumeIndex == -1)
	return;

    processActorVar2 = lactor->field_20;
    processActorVar3 = lactor->field_22;
    processActorVar4 = lactor->field_24;

    if (lactor->field_60 & 0x400)	// is sprite actor
    {
	if (lactor->field_66) {
	    lactor->field_62 |= 2;
	}

	processActorX = lactor->X;
	processActorZ = lactor->Z;
	processActorY = lactor->Y;

	if (!(lactor->field_62 & 0x100))	// actor freeze movement
	{
	    if (lactor->field_34) {
		int dx;

		dx = mainLoopSub17(&lactor->time);

		if (!dx) {
		    if (lactor->time.to > 0) {
			dx = 1;
		    } else {
			dx = -1;
		    }

		    processActorSub1(dx, 0, lactor->field_78);

		    processActorZ = lactor->Z - destZ;

		    processActorSub1(0, destX, lactor->angle);

		    processActorX = lactor->X + destX;
		    processActorY = lactor->Y + destZ;

		    setActorAngle(0, lactor->field_34, 0, &lactor->time);

		    if (lactor->field_62 & 0x40)	// can rotate ?
		    {
			if (lactor->field_72) {
			    var_10 = lactor->field_72;
			    if (var_10 >=
				getDistanceToward(processActorX, processActorY,
						  lactor->lastX, lactor->lastY)) {
				if (lactor->angle == 0) {
				    processActorY = lactor->lastY + lactor->field_72;
				} else if (lactor->angle == 0x100) {
				    processActorX = lactor->lastX + lactor->field_72;
				} else if (lactor->angle == 0x200) {
				    processActorY = lactor->lastY - lactor->field_72;
				} else if (lactor->angle == 0x300) {
				    processActorX = lactor->lastX - lactor->field_72;
				}

				lactor->field_62 &= 0xFFBF;
				lactor->field_34 = 0;
			    }
			} else {
			    int tempVar = 0;

			    if (lactor->angle == 0) {
				if (processActorY <= lactor->lastY)
				    tempVar = 1;
			    } else if (lactor->angle == 0x100) {
				if (processActorX <= lactor->lastX)
				    tempVar = 1;
			    } else if (lactor->angle == 0x200) {
				if (processActorY >= lactor->lastY)
				    tempVar = 1;
			    } else if (lactor->angle == 0x300) {
				if (processActorX >= lactor->lastX)
				    tempVar = 1;
			    }

			    if (tempVar) {
				processActorX = lactor->lastX;
				processActorZ = lactor->lastZ;
				processActorY = lactor->lastY;

				lactor->field_62 &= 0xFFBF;
				lactor->field_34 = 0;
			    }
			}
		    }
		}
	    }

	    if (lactor->field_60 & 0x10) {
		processActorX = lactor->lastX;
		processActorZ = lactor->lastZ;
		processActorY = lactor->lastY;

		if (lactor->field_60 & 0x8000) {
		    processActorX = abs(processActorX);
		    processActorY = abs(processActorY);
		}

		lactor->lastX = 0;
		lactor->lastZ = 0;
		lactor->lastY = 0;

	    }
	}

    } else			// not sprite actor
    {
	if (lactor->currentAnim != -1) {
	    animPtr = (char *) getHqrdataPtr(HQRanims, lactor->currentAnim);
	   // animData=processActorSub2(lactor->animPosition,animPtr,(char*)bodyPtrTab[lactor->costumeIndex]); 
	   // 
	   // 
	   // // get the current frame anim data (for step length ?)
	    animData = applyAnim(lactor->animPosition, animPtr, (char *) bodyPtrTab[lactor->costumeIndex]);	// get 
														// 
	   // 
	   // the 
	   // current 
	   // frame 
	   // anim 
	   // data 
	   // (for 
	   // step 
	   // length 
	   // ?)

	    if (processActorVar5) {
		lactor->field_62 |= 0x80;
	    } else {
		lactor->field_62 &= 0xFF7F;
	    }

	    lactor->angle = (lactor->angle + processActorVar6 - lactor->field_6A) & 0x3FF;
	    lactor->field_6A = processActorVar6;

	    processActorSub1(currentX, currentY, lactor->angle);

	    currentX = destX;	// dest
	    currentY = destZ;

	    processActorX = lactor->X + currentX - lactor->lastX;
	    processActorZ = lactor->Z + currentZ - lactor->lastZ;
	    processActorY = lactor->Y + currentY - lactor->lastY;

	    lactor->lastX = currentX;
	    lactor->lastZ = currentZ;
	    lactor->lastY = currentY;

	    lactor->field_62 &= 0xFFF3;

	    if (animData)	// if keyFrame
	    {
		lactor->animPosition++;
		if (lactor->field_4)	// if actor play sound attached to animation
		{
		    initNewCSub(lactor, actorNum);
		}

		var_10 = lactor->animPosition;
		if (var_10 == getAnimMaxIndex(animPtr)) {
		    lactor->field_62 &= 0xFFFD;
		   // if(lactor->field_78==0)
		    {
			lactor->animPosition = getAnimStartIndex(animPtr);
		    }
		   /*
		    * else { var_C=actorNum; lactor->costume=lactor->field_2;
		    * lactor->currentAnim=initCostume(lactor->costume,actorNum);
		    * 
		    * if(lactor->currentAnim==-1) { lactor->currentAnim=initCostume(0,var_C);
		    * lactor->costume=0; }
		    * 
		    * lactor->field_4=loadTwinsenCostumesVar1;
		    * 
		    * lactor->field_78=0; lactor->animPosition=0; lactor->field_66=0; } 
		    */

		    if (lactor->field_4) {
			initNewCSub(lactor, actorNum);
		    }

		    lactor->field_62 |= 4;
		}

		lactor->field_6A = 0;

		lactor->lastX = 0;
		lactor->lastZ = 0;
		lactor->lastY = 0;
	    }
	}
    }

    if (lactor->standOn != -1)	// standing on object
    {
	processActorX -= actors[lactor->standOn].field_20;	// apply under object movement
	processActorZ -= actors[lactor->standOn].field_22;
	processActorY -= actors[lactor->standOn].field_24;

	processActorX += actors[lactor->standOn].X;
	processActorZ += actors[lactor->standOn].Z;
	processActorY += actors[lactor->standOn].Y;

	if (processActorSub4(actorNum, lactor->standOn))	// is actor still standing on
	   // another actor ?
	    lactor->standOn = -1;	// actor fall from the object

    }

    if (lactor->field_62 & 0x100)	// movement freeze
    {
	processActorX = processActorVar2;
	processActorZ = processActorVar3 + mainLoopVar17;	// ??!? mainLoopVar17 isn't related 
								// 
       // 
       // to angle ?!
	processActorY = processActorVar4;
    }

   /*
    * if(lactor->field_60&0x2) { int position;
    * 
    * getPosVar2=0; position=getCurPos(processActorVar2,processActorVar3,processActorVar4);
    * 
    * if(position) { if(position==1) { printf("currentpos dans col 1 ?\n");
    * lactor->Z=processActorZ=abs(processActorZ)+0x100; } else { processActorSub5(position); } }
    * 
    * if(lactor->field_60&1) // actor fall processActorSub6(actorNum);
    * 
    * if((lactor->standOn!=-1) && (lactor->field_62&0x100)) //if actor standing on another actor
    * and ? processActorSub7();
    * 
    * fieldCauseDamage=0;
    * 
    * processActorVar11=processActorX; processActorVar12=processActorZ;
    * processActorVar13=processActorY;
    * 
    * if(!actorNum && !(lactor->field_60&0x20)) // all this check if field around the actor can
    * hurt { processActorSub8(lactor->field_26,lactor->field_2A,lactor->field_2E,1);
    * processActorSub8(lactor->field_28,lactor->field_2A,lactor->field_2E,2);
    * processActorSub8(lactor->field_28,lactor->field_2A,lactor->field_30,4);
    * processActorSub8(lactor->field_26,lactor->field_2A,lactor->field_30,8); } else {
    * processActorSub9(lactor->field_26,lactor->field_2A,lactor->field_2E,1);
    * processActorSub9(lactor->field_28,lactor->field_2A,lactor->field_2E,2);
    * processActorSub9(lactor->field_28,lactor->field_2A,lactor->field_30,4);
    * processActorSub9(lactor->field_26,lactor->field_2A,lactor->field_30,8); }
    * 
    * if(fieldCauseDamage && !(lactor->field_62&0x100) && currentlyProcessedActorNum &&
    * (comportement!=1) && (lactor->costume!=1)) //field cause damage {
    * processActorSub1(lactor->field_26,lactor->field_2E,lactor->angle+0x580);
    * 
    * destX+=processActorX; destZ+=processActorY;
    * 
    * if(destX>=0 && destZ>=0 && destX<=0x7E00 && destZ<=0x7E00) {
    * if(getCurPos(destX,processActorZ+0x100,destZ)) {
    * processActorSub10(lactor->X,lactor->Z+1000,lactor->Y,0);
    * playAnim(6,2,0,currentlyProcessedActorNum);
    * 
    * if(currentlyProcessedActorNum==0) { changeRoomVar8=1; //twinsenHit }
    * 
    * lactor->life--; //damage caused by field } } }
    * 
    * position=getCurPos(processActorX,processActorZ,processActorY); var_4=position;
    * lactor->field_3=position;
    * 
    * if(position) //if standing on floor { if(position==1) //if standing on floor ==1 {
    * if((lactor->field_62>>8)!=position) { processActorSub7();
    * processActorZ=(getPosVar2<<8)+0x100; } else { if(!actorNum && comportement==1 &&
    * lactor->costume==var_4) { processActorSub10(lactor->X,lactor->Z+0x1000,lactor->Y,0);
    * playAnim(6,2,0,currentlyProcessedActorNum);
    * 
    * if(!actorNum) { changeRoomVar8=1; }
    * 
    * lactor->life--; }
    * 
    * if(!getCurPos(processActorX,processActorZ,processActorVar4)) // wall collision ? {
    * if(getCurPos(processActorVar2,processActorZ,processActorY)) { return; }
    * 
    * processActorX=processActorVar2; } else { processActorY=processActorVar4; } } } else { }
    * 
    * lactor->field_62&=0xFFFE; } else //standing on floor !=1 { if(lactor->field_60&0x800 &&
    * lactor->standOn!=-1) // if ? and actor standing on another actor {
    * var_8=getCurPos(processActorX,processActorZ-1,processActorY);
    * 
    * if(var_8) { if(lactor->field_62&0x100) { processActorSub7(); }
    * 
    * processActorSub5(var_8); } else { if(!(lactor->field_62&0x80)) { lactor->field_62|=0x100;
    * 
    * if((!actorNum) && changeRoomVar2==0) { changeRoomVar2=processActorZ; }
    * 
    * playAnim(7,0,255,actorNum);
    * 
    * } } } }
    * 
    * if(getPosVar2!=-1) // collision with floor under actor { lactor->life=0; // die because au
    * drown ? } } else // if falling { if(lactor->field_60&0x1) //actor fall
    * processActorSub6(actorNum); } 
    */

    if (fieldCauseDamage)
	lactor->field_3 |= 0x80;

    if (processActorX < 0)
	processActorX = 0;

    if (processActorY < 0)
	processActorY = 0;

    if (processActorZ < 0)
	processActorZ = 0;

    if (processActorX > 0x7E00)
	processActorX = 0x7E00;

    if (processActorY > 0x7E00)
	processActorY = 0x7E00;

    lactor->X = processActorX;
    lactor->Z = processActorZ;
    lactor->Y = processActorY;

}

void LBA_engine::processActorSub8(int var0, int var1, int var2, int var3)
{
}

void LBA_engine::processActorSub9(int var0, int var1, int var2, int var3)
{
}

void LBA_engine::processActorSub10(int var0, int var1, int var2, int var3)
{
}

void LBA_engine::processActorSub5(int param)
{
}

void LBA_engine::processActorSub6(int param)	// actor fall
{
}

void LBA_engine::processActorSub7(void)
{
}

int LBA_engine::getCurPos(int var0, int var1, int var2)
{
    char *ptr;
    char *ptr2;
    char temp0;
    char temp3;

    ptr = (char *) bufCube;

    getPosVar1 = (var0 + 0x100) >> 9;
    getPosVar2 = var1 >> 8;
    getPosVar3 = (var2 + 0x100) >> 9;

    if (getPosVar1 < 0 || getPosVar1 >= 64)
	return (0);

    ptr += getPosVar1 * 50;

    if (getPosVar2 <= -1)
	return (1);

    if (getPosVar2 < 0 || getPosVar2 >= 24)
	return (0);

    ptr += getPosVar2 * 2;

    if (getPosVar3 < 0 || getPosVar3 >= 64)
	return (0);

    ptr += (getPosVar2 >> 7) * 50;

    temp0 = *ptr;

    if (temp0) {
	ptr2 = (char *) currentBll;

	ptr2 += *(int *) (ptr2 + temp0 * 4 - 4);
	ptr2 += 3;

	temp3 = *(ptr2 + 1);

	ptr2 = ptr2 + temp3 * 4;

	return (*ptr2);
    } else {
	return (*(ptr + 1));
    }
}

void LBA_engine::processActorSub1(int var0, int var1, int angle)
{
    int angle1;
    int angle2;

    if (!angle) {
	destX = var0;
	destZ = var1;
    } else {
	angle1 = tab1[angle & 0x3FF];
	angle2 = tab1[(angle + 0x100) & 0x3FF];

	destX = (var0 * angle2 + var1 * angle1) >> 14;
	destZ = (var1 * angle2 - var0 * angle1) >> 14;
    }
}

int LBA_engine::processActorSub2(int position, char *anim, char *body)
{
    short int bodyFlags;
    char *edi;
    char *ebx;
    int ebp;
    int eax;
    int keyFrameLength;

   // int numOfPointInBody;
    int numOfPointInAnim;
    char *keyFramePtrOld;

    numOfPointInAnim = *(short int *) (anim + 2);

    keyFramePtr = (numOfPointInAnim * 8 + 8) * position + anim + 8;

    keyFrameLength = *(short int *) keyFramePtr;

    bodyFlags = *(short int *) body;

    if (bodyFlags & 2) {
	edi = body + 16;

	animVar1 = edi;

	ebx = *(char **) edi;
	ebp = *(int *) (edi + 4);

	if (!ebx) {
	    ebx = keyFramePtr;
	    ebp = keyFrameLength;
	}

	lastKeyFramePtr = ebx;

	eax = time - ebp;

	printf("delta=%d / %d  -> time=%d -> ebp=%d\n", eax, keyFrameLength, time, ebp);

	if (eax >= keyFrameLength) {
	    *(char **) animVar1 = keyFramePtr;
	    *(int *) (animVar1 + 4) = time;

	    currentX = *(short int *) (keyFramePtr + 2);
	    currentZ = *(short int *) (keyFramePtr + 4);
	    currentY = *(short int *) (keyFramePtr + 6);

	    processActorVar5 = *(short int *) (keyFramePtr + 8);
	    processActorSub2Var0 = *(short int *) (keyFramePtr + 10);
	    processActorVar6 = *(short int *) (keyFramePtr + 12);
	    processActorSub2Var1 = *(short int *) (keyFramePtr + 14);

	    return (1);
	} else {
	    keyFramePtrOld = keyFramePtr;

	    lastKeyFramePtr += 8;
	    keyFramePtr += 8;

	    processActorVar5 = *(short int *) keyFramePtr;
	    processActorSub2Var0 = (*(short int *) (keyFramePtr + 2) * eax) / keyFrameLength;
	    processActorVar6 = (*(short int *) (keyFramePtr + 4) * eax) / keyFrameLength;
	    processActorSub2Var1 = (*(short int *) (keyFramePtr + 6) * eax) / keyFrameLength;

	    lastKeyFramePtr += 8;
	    keyFramePtr += 8;

	    currentX = (*(short int *) (keyFramePtrOld + 2) * eax) / keyFrameLength;
	    currentZ = (*(short int *) (keyFramePtrOld + 4) * eax) / keyFrameLength;
	    currentY = (*(short int *) (keyFramePtrOld + 6) * eax) / keyFrameLength;

	    return (0);
	}
    }

    return (0);
}

int LBA_engine::processActorSub4(int var0, int var1)	// is actor still standing on object ?
{
    actor *lactor1;
    actor *lactor2;

    int bx;
    int si;
    int di;
    int var_1C;
    int var_4;
    int var_20;
    int var_18;
    int cx;
    int var_8;
    int var_10;
    int var_C;
    int var_14;

    lactor1 = &actors[var0];
    lactor2 = &actors[var1];

   // ------------- actor1

    bx = processActorX + lactor1->field_26;
    si = processActorX + lactor1->field_28;

    di = processActorZ + lactor1->field_2A;
    var_1C = processActorZ + lactor1->field_2C;

    var_4 = processActorY + lactor1->field_2E;
    var_20 = processActorY + lactor1->field_30;

   // ------------- actor2

    var_18 = lactor2->X + lactor2->field_26;
    cx = lactor2->X + lactor2->field_28;

    var_8 = lactor2->Z + lactor2->field_2A;
    var_10 = lactor2->Z + lactor2->field_2C;

    var_C = lactor2->Y + lactor2->field_2E;
    var_14 = lactor2->Y + lactor2->field_30;

    if (bx >= cx)		// X1
	return (0);

    if (si <= var_18)		// X2
	return (0);

    if (di > (var_10 + 1))	// Z1a
	return (0);

    if (di <= (var_10 - 0x100))	// Z1b
	return (0);

    if (var_1C <= var_8)	// Z2
	return (0);

    if (var_4 >= var_14)	// Y1
	return (0);

    if (var_20 <= var_C)	// Y2
	return (0);

    return (1);
}

void LBA_engine::checkZones(actor* lactor, int actorNumber)
{
	int currentX=lactor->X;
	int currentY=lactor->Y;
	int currentZ=lactor->Z;

	int var_C=0;
	int i;
	unsigned char* localPtr=sceneVarPtr;
	short int opcode;

	if(actorNumber)
	{
		currentActorInZoneProcess=actorNumber;
	}

	for(i=0;i<reinitAll2Var4;i++)
	{
		if(currentX>*(short int*)localPtr && currentX<*(short int*)(localPtr+6))
			if(currentZ>*(short int*)(localPtr+2) && currentZ<*(short int*)(localPtr+8))
				if(currentY>*(short int*)(localPtr+4) && currentY<*(short int*)(localPtr+10)) // if actor in zone
				{
					opcode=*(short int*)(localPtr+12);
					switch(opcode)
					{
					case 2:
						{
							lactor->field_5A=*(short int*)(localPtr+14);
							break;
						}
					default:
						{
							printf("Unsupported checkZones opcode %d for actor %d!\n",opcode,actorNumber);
							exit(1);
						}
					}
				}
		localPtr+=24;
	}

	//if(var_C


}