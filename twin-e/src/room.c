/***************************************************************************
                          room.cpp  -  description
                             -------------------
    begin                : Thu Feb 7 2002
    copyright            : (C) 2002 by Yaz0r
    email                : yaz0r@yaz0r.net
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

void LBA_engine::changeRoom(void)
{
    int i;
    int oldRoom;

    if (needChangeRoom == 4 && needChangeRoomVar1[0] != 0)
	needChangeRoom = 118;

    printf("Loading room %d\n", needChangeRoom);

    oldRoom = currentRoom;

    currentRoom = needChangeRoom;
    mainMenu2();

   /*
    * if(isMenuDisplayed!=0) fadeOut((char*)paletteRGBA); else fadeOut((char*)menuPalRGBA); 
    */

    if (drawInGameTransBox == 0)
	{
	   // resetVideoBuffer1();
	   // osystem->drawBufferToScreen(videoBuffer1);
	}

    changeRoom1();
    loadTwinsenCostumes();

    twinsen->field_40 = 1;
    twinsen->zone = -1;
    twinsen->positionInActorScript = 0;
    twinsen->positionInMoveScript = -1;
    twinsen->label = -1;

    loadRoomScene(needChangeRoom);

    if (changeRoomVar1 != -1)
	{
	    mainLoop2(0);
	    resetPalette();
	    changeRoomSub1(changeRoomVar1);
	    changeRoomVar1 = -1;
	}

    if (currentRoom == 116 || currentRoom == 117)
	currentTextBank = 10;

    loadTextBank(currentTextBank + 3);

    if (roomMusic != -1)
	{
	    if (playMusicVar1 != roomMusic)
		{
		    if (playMusicSub())
			playMusicSub2(1);
		}
	}

    loadGridAndBll(needChangeRoom);

    if (reinitVar11 == 1)
	{
	    GV9 = GV9dup;
	    GV10 = GV10dup;
	    GV11 = GV11dup;
	}

    if (reinitVar11 == 2 || reinitVar11 == 0)
	{
	    GV9 = sceneVar16;
	    GV10 = sceneVar17;
	    GV11 = sceneVar18;
	}

    twinsen->X = GV9;
    twinsen->Z = changeRoomVar2 = GV10;
    twinsen->Y = GV11;

    setSomething4(reinitVar1, reinitVar2, 0);

    if (oldRoom != needChangeRoom)
	{
	    reinitVar10 = comportementHero;
	    reinitVar9 = twinsen->angle;
	    saveGame();
	}

    reinitTwinsen();

    for (i = 1; i < numActorInRoom; i++)
	{
	    loadRoomActors(i);
	}

    numKey = 0;
    mainLoopVar10 = 0;
    reinitVar11 = 0;
    changeRoomVar3 = 0;

    newCameraX = actors[reinitVar8].X >> 9;
    newCameraZ = actors[reinitVar8].Z >> 8;
    newCameraY = actors[reinitVar8].Y >> 9;

    changeRoomVar7 = -1;
    changeRoomVar8 = 1;
    changeRoomVar9 = -1;
    currentGrid2 = -1;
    mainLoopVar2 = 1;
    mainLoopVar3 = 1;

    needChangeRoom = -1;
    changeRoomVar10 = 1;
    changeRoomVar11 = 14;

    setSomething4(reinitVar1, reinitVar2, 0);

    if (roomMusic != -1)
	playMusic(roomMusic);

}

void LBA_engine::changeRoom1(void)
{
    int i;

    reinitData();

    for (i = 0; i < 80; i++)
	cubeFlags[i] = 0;

    for (i = 0; i < 10; i++)
	roomData2[i].field_0 = -1;

    changeRoom1Sub1(HQRanims);

    changeRoom1Sub2();

    reinitAll2Var3 = 0;
    isMenuDisplayed = 0;

}

void LBA_engine::loadRoomScene(int sceneNumber)
{
    unsigned char *temp;
    short int temp3;

   // short int temp4;
    unsigned short int *temp2;
    short int currentActor;

   // short int wordLocalScenePtr;
   // short int costume;
   // short int ptr1;
   // unsigned char* ptr2;
    int modelNumber;
    int size;

    size = loadDataFileToPtr("scene.hqr", sceneNumber, &scenePtr);
    localScenePtr = scenePtr;

   // todo:faire la gestion d'erreur de chargement

    temp = scenePtr;

    currentTextBank = *(temp++);
    needChangeRoom = sceneNumber;
    sceneRoomNumber = *(temp++);
    temp += 2;
    temp += 2;

    temp2 = (unsigned short int *) temp;
    reinitVar1 = *(temp2++);
    reinitVar2 = *(temp2++);	// ok

    sceneVar2.field_0 = *(temp2++);
    sceneVar3.field_0 = *(temp2++);
    sceneVar4.field_0 = *(temp2++);
    sceneVar2.field_2 = *(temp2++);
    sceneVar3.field_2 = *(temp2++);
    sceneVar4.field_2 = *(temp2++);
    sceneVar2.field_4 = *(temp2++);
    sceneVar3.field_4 = *(temp2++);
    sceneVar4.field_4 = *(temp2++);
    sceneVar2.field_6 = *(temp2++);
    sceneVar3.field_6 = *(temp2++);
    sceneVar4.field_6 = *(temp2++);

    sceneVar14 = *(temp2++);
    sceneVar15 = *(temp2++);
    temp = (unsigned char *) temp2;
    roomMusic = *(temp++);
    temp2 = (unsigned short int *) temp;
    sceneVar16 = *(temp2++);
    sceneVar17 = *(temp2++);
    sceneVar18 = *(temp2++);

    temp3 = *(temp2++);

    twinsen->moveScript = (unsigned char *) temp2;

    temp = (unsigned char *) temp2;
    temp2 = (unsigned short int *) (temp + temp3);

    currentActor = 1;

    temp3 = *(temp2++);

    twinsen->actorScript = (unsigned char *) temp2;

    temp = (unsigned char *) temp2;
    temp2 = (unsigned short int *) (temp + temp3);

    numActorInRoom = *(temp2++);

    while (currentActor < numActorInRoom)
	{
	    localScenePtr = (unsigned char *) temp2;
	    resetActor(currentActor);
	    temp2 = (unsigned short int *) localScenePtr;
	    actors[currentActor].field_60 = *(temp2++);
	    modelNumber = *(temp2++);
	    localScenePtr = (unsigned char *) temp2;

	    if (!(actors[currentActor].field_60 & 0x400))	// if not sprite actor
		{
		    loadDataFileToPtr("file3d.hqr", modelNumber, &actors[currentActor].bodyPtr);
		}

	    actors[currentActor].body = *(localScenePtr++);
	    actors[currentActor].anim = *(localScenePtr++);

		if(currentActor==5 && actors[currentActor].anim==70)
			printf("Anim = %d\n",actors[currentActor].anim);

	    temp2 = (unsigned short int *) localScenePtr;

	    actors[currentActor].field_8 = *(temp2++);
	    actors[currentActor].field_20 = actors[currentActor].X = *(temp2++);
	    actors[currentActor].field_22 = actors[currentActor].Z = *(temp2++);
	    actors[currentActor].field_24 = actors[currentActor].Y = *(temp2++);

	    localScenePtr = (unsigned char *) temp2;

	    actors[currentActor].field_66 = *(localScenePtr++);

	    temp2 = (unsigned short int *) localScenePtr;

	    actors[currentActor].field_10 = *(temp2++) & 0xFE;
	    actors[currentActor].angle = *(temp2++);
	    actors[currentActor].field_34 = *(temp2++);
	    actors[currentActor].field_40 = *(temp2++);
	    actors[currentActor].cropLeft = *(temp2++);
	    actors[currentActor].cropTop = *(temp2++);
	    actors[currentActor].cropRight = *(temp2++);
	    actors[currentActor].cropBottom = *(temp2++);

	    localScenePtr = (unsigned char *) temp2;

	    actors[currentActor].field_12 = *(localScenePtr++);
	    actors[currentActor].talkColor = *(localScenePtr++);
	    actors[currentActor].field_14 = *(localScenePtr++);
	    actors[currentActor].life = *(localScenePtr++);

	    temp2 = (unsigned short int *) localScenePtr;

	    temp3 = *(temp2++);

	    actors[currentActor].moveScript = (unsigned char *) temp2;

	    temp = (unsigned char *) temp2;
	    temp2 = (unsigned short int *) (temp + temp3);

	    temp3 = *(temp2++);

	    actors[currentActor].actorScript = (unsigned char *) temp2;

	    temp = (unsigned char *) temp2;
	    temp2 = (unsigned short int *) (temp + temp3);
	    localScenePtr = (unsigned char *) temp2;

	    currentActor++;
	};

    reinitAll2Var4 = *(temp2++);

    sceneVarPtr = (unsigned char *) temp2;
    localScenePtr = (unsigned char *) temp2 + 2 * (reinitAll2Var4 * 12);
    temp2 = (unsigned short int *) localScenePtr;

    numFlags = *(temp2++);

    localScenePtr = (unsigned char *) temp2;
    flagData = (flagDataStruct *) temp2;
}

void LBA_engine::changeRoomSub1(int arg_0)
{
    int reinitVar1Copy;
    int reinitVar2Copy;

   // int i;
    short int var_14;
    byte *localChangeRoomSub1Var;
    int j;
    unsigned short int var;

    freezeTime();

    var_14 = 1;
    reinitVar1Copy = reinitVar1;
    reinitVar2Copy = reinitVar2;

   /*
    * if(!isMenuDisplayed) fadeOut((char*)menuPalRGBA); else fadeOut((char*)paletteRGBA);
    */

    maximizeTextWindow();
    resetVideoBuffer1();
    osystem->drawBufferToScreen(videoBuffer1);
    loadVariousGFX();

    localChangeRoomSub1Var = videoPtr12;

    j = 0;

    while (j < arg_0)
	{			/* implementer la suite */
	    localChangeRoomSub1Var += 12;
	    var = *(unsigned short int *) localChangeRoomSub1Var;
	    var += var;
	    localChangeRoomSub1Var += 2;
	    j++;
	    localChangeRoomSub1Var += var;
	}

   /*
    * implementer la suite 
    */

}

void LBA_engine::loadVariousGFX(void)
{

    int i;
    int j;

    videoPtr1 = videoBuffer2;
    videoPtr2 = videoBuffer2 + 4488;
    videoPtr3 = videoBuffer2 + 7854;
    videoPtr4 = videoBuffer2 + 8398;

    videoPtr5 = videoBuffer2 + 73934;

    loadImageToPtr("ress.hqr", videoPtr3, 6);
    loadImageToPtr("ress.hqr", videoPtr4, 7);
    videoPtr6 = videoPtr5 + loadImageToPtr("ress.hqr", videoPtr5, 9);
    videoPtr7 = videoPtr6 + loadImageToPtr("ress.hqr", videoPtr6, 10);
    videoPtr8 = videoPtr7 + loadImageToPtr("ress.hqr", videoPtr7, 11);
    videoPtr11 = videoPtr8 + loadImageToPtr("ress.hqr", videoPtr8, 29);

   // loadGfxSub(videoPtr5);
   // loadGfxSub(videoPtr6);
   // loadGfxSub(videoPtr7);

   // loadGfxSub(videoPtr8);

    videoPtr10 = videoPtr11 + 4488;
    videoPtr12 = videoPtr10 + loadImageToPtr("ress.hqr", videoPtr10, 8);
    videoPtr13 = videoPtr12 + loadImageToPtr("ress.hqr", videoPtr12, 30);

    loadImageToPtr("ress.hqr", (byte *) & palette, 5);

    j = 576;
    for (i = 0; i < 96; i += 3, j += 3)
	{
	    palette2[i] = palette[j];
	    palette2[i + 1] = palette[j + 1];
	    palette2[i + 2] = palette[j + 2];
	}

    j = 576;
    for (i = 96; i < 189; i += 3, j += 3)
	{
	    palette2[i] = palette[j];
	    palette2[i + 1] = palette[j + 1];
	    palette2[i + 2] = palette[j + 2];
	}

   // loadGFXSub1();
   // loadGFXSub2();

   // needToLoadVariousGFX=0;
}

int LBA_engine::loadGridAndBll(short int roomNumber)
{
    int gridSize;
    int bllSize;
    int brickDataSize;
    int size;

    gridSize = prepareResource("lba_gri.hqr", roomNumber);	// pour recuperer la taille

    allocHQRMemory(gridSize, &currentGrid);
    if (currentGrid == NULL)
	return (0);
    loadImageToPtr("lba_gri.hqr", currentGrid, roomNumber);

    bllSize = prepareResource("lba_bll.hqr", roomNumber);	// pour recuperer la taille
    allocHQRMemory(bllSize, &currentBll);
    if (currentBll == NULL)
	return (0);

    loadImageToPtr("lba_bll.hqr", currentBll, roomNumber);

    brickDataSize = loadBrk(gridSize);

    if (!brickDataSize)
	return (0);

    allocHQRMemory(brickDataSize, &bufferBrick2);

    size = processBuffer2Buffer((unsigned int *) bufferBrick, (unsigned int *) bufferBrick2);

    memoryBufferProcess(bufferBrick2, size);

    numberOfBll = (*(unsigned int *) (currentBll)) >> 2;

    createCube();

   /*
    * dumpFile("cube",(char*)bufCube,204800); dumpFile("grid",(char*)currentGrid,10000);
    * dumpFile("brick",(char*)bufferBrick,361472); 
    */

    return (1);
}

// this unpack the grid to the cube buffer
void LBA_engine::createCube(void)
{
    int var2 = 0;
    int ptr1;
    int ptr2;
    int i;
	int j;

    for(j=0;j<64;j++)
	{
	    ptr1 = var2;
	    ptr2 = j << 6;

	    for(i=0;i<64;i++)
		{
		    addCubeEntry(currentGrid + *(unsigned short int *) (currentGrid + 2 * (i + ptr2)), bufCube + ptr1);
		    ptr1 += 50;
		}

	    var2 += 3200;
	}
}

// this unpack a vertical column from the grid to the cube buffer
void LBA_engine::addCubeEntry(unsigned char *gridEntry, unsigned char *dest)
{

    int numIteration;
    int temp1;
    int temp2;

    int temp3;

    int i;
    unsigned short int *source;
    unsigned short int *arrive;

    temp1 = *(gridEntry++);

    do
	{
	    temp2 = *(gridEntry++);

	    numIteration = (temp2 & 0x3F) + 1;

	    source = (unsigned short int *) gridEntry;
	    arrive = (unsigned short int *) dest;

	    if (!(temp2 & 0xC0))
		{
		    for (i = 0; i < numIteration; i++)
			*(arrive++) = 0;
		}
	    else if (temp2 & 0x40)
		{
		    for (i = 0; i < numIteration; i++)
			*(arrive++) = *(source++);
		}
	    else
		{
		    temp3 = *(source++);
		    for (i = 0; i < numIteration; i++)
			*(arrive++) = temp3;
		}

	    gridEntry = (unsigned char *) source;
	    dest = (unsigned char *) arrive;

	}
    while (--temp1);
}

void LBA_engine::reinitTwinsen(void)
{
    twinsen->field_40 = 1;
    twinsen->field_62 = 0;
    twinsen->field_60 = 2119;
    twinsen->field_14 = 1;
    twinsen->positionInMoveScript = -1;
    twinsen->label = -1;
    twinsen->positionInActorScript = 0;
    twinsen->zone = -1;
    twinsen->angle = reinitVar9;
    setActorAngleSafe(twinsen->angle, twinsen->angle, 0, &twinsen->time);
    changeTwinsenComp(reinitVar10);
    reinitVar7 = 0;
}

void LBA_engine::changeTwinsenComp(int newComportement)
{
    int temp;

    switch (newComportement)
	{
	case 0:
	    comportementHero = 0;
	    twinsen->bodyPtr = file3D0;
	    break;
	case 1:
	    comportementHero = 1;
	    twinsen->bodyPtr = file3D1;
	    break;
	case 2:
	    comportementHero = 2;
	    twinsen->bodyPtr = file3D2;
	    break;
	case 3:
	    comportementHero = 3;
	    twinsen->bodyPtr = file3D3;
	    break;
	case 4:
	    comportementHero = 4;
	    twinsen->bodyPtr = file3D4;
	    break;
	};

    temp = twinsen->body;

    twinsen->costumeIndex = -1;
    twinsen->body = -1;

    loadActorCostume(temp, 0);

    twinsen->anim = -1;
    twinsen->field_78 = 0;

    playAnim(ANIM_static, 0, 255, 0);
}

void LBA_engine::memoryBufferProcess(unsigned char *ptr, int size)
{
    int temp;

    if (HQMemory == 0)
	return;

    if (*((int *) (ptr - 12)) != 0x12345678)
	return;

    temp = *(int *) (ptr - 8);
    temp -= size;

    HQMemory2 -= temp;
    HQMemorySize2 += temp;

    *(int *) (ptr - 8) -= temp;
}

/*
 * this function uncompress all the bricks to the destBuffer 
 */
int LBA_engine::processBuffer2Buffer(unsigned int *buffer1, unsigned int *destBuffer)
{
    unsigned char *destPtr;
    int numOfBricks;
    int i;
    int sizeOfCurrentBrick;
    int startOffset;
    unsigned int firstBrickOffset;

    firstBrickOffset = *buffer1;
    destPtr = (unsigned char *) (destBuffer + firstBrickOffset);
    numOfBricks = (*buffer1) >> 2;

    *(destBuffer++) = startOffset = *buffer1;

    for (i = 0; i < numOfBricks - 1; i++)
	{
	    sizeOfCurrentBrick = processBuffer2BufferSub(i, buffer1, (unsigned int *) destPtr);

	    destPtr += sizeOfCurrentBrick;
	    counter += sizeOfCurrentBrick;

	    startOffset += sizeOfCurrentBrick;

	    *(destBuffer++) = startOffset;
	}

    return (firstBrickOffset);
}

int LBA_engine::processBuffer2BufferSub(int var, unsigned int *buffer, unsigned int *ptr)
{
    unsigned int *ptrSave = ptr;
    unsigned char *ptr2;
    unsigned char *esi;
    unsigned char *edi;
    byte cl, ch, dl, ah, bl, al, bh;
    int ebx;

    buffer = (unsigned int *) (*(buffer + var) + (unsigned char *) buffer);

    ebx = *ptr++ = *buffer++;	// on ecrit le flag de la brique

    bh = (ebx & 0x0000FF00) >> 8;

    esi = (unsigned char *) buffer;
    edi = (unsigned char *) ptr;

    cl = 0;
    ch = 0;

    do
	{
	    dl = 0;
	    ah = 0;
	    ptr2 = edi;

	    edi++;

	    bl = *(esi++);

	    if (*(esi) & 0xC0)
		{
		    *edi++ = 0;
		    dl++;
		}

	    do
		{
		    al = *esi++;
		    cl = al;
		    cl &= 0x3F;
		    cl++;

		    if (al & 0x80)
			{
			    ah += cl;
			    esi++;
			}
		    else if (al & 0x40)
			{
			    ah += cl;
			    esi += cl + 256 * ch;
			}
		    else
			{
			    if (ah)
				{
				    *edi++ = ah;
				    dl++;
				    ah = 0;
				}
			    *edi += cl;
			    dl++;
			}

		}
	    while (--bl > 0);

	    if (ah)
		{
		    *edi++ = ah;
		    dl++;

		    ah = 0;
		}
	}
    while (--bh > 0);

    return ((int) ((unsigned char *) edi - (unsigned char *) ptrSave));
}

void LBA_engine::reinitData(void)
{
    int counter;

    for (counter = 0; counter < 50; counter++)
	{
	    reinitAll2SubVar1[counter].field_0 = -1;
	    reinitAll2SubVar1[counter].field_20 = 1;
	}

    for (counter = 0; counter < 10; counter++)
	{
	    roomData2[counter].field_0 = -1;
	}

    changeRoom1Sub1(HQRanims);
    changeRoom1Sub2();

    reinitAll2Var3 = 0;
    isMenuDisplayed = 0;

}

void LBA_engine::changeRoom1Sub1(hqr_entry * ptr)
{
    ptr->remainingSize = ptr->size1;
    ptr->unk = 0;
}

void LBA_engine::changeRoom1Sub2(void)
{
    byte *temp;

    temp = HQMemory;

    if (!temp)
	{
	    HQMemory2 = temp;
	    HQMemorySize2 = HQMemorySize;
	}

    HQMemory = temp;
}

int LBA_engine::loadBrk(int gridSize)
{
    int firstBrick;
    int lastBrick;
    int currentBrick;

   // int tempSize2;
    int counter;
    int counter2;
    int counter3;
    int counter6;
    int counter7;
    int offset;
    int offset2;

    unsigned char *endOfGridPtr;
    unsigned char *endOfGridPtr2;
    unsigned char *endOfGridPtr3;
    unsigned char *outPtr;
    unsigned short int *outPtr2;
    unsigned short int *outPtr3;
    unsigned short int *outPtr4;
    unsigned char *destPtr;
    unsigned char *compressedPtr;
    unsigned int *ptrUnk;

    byte temp;
    byte temp2;
    int temp3;
    int temp4;
    unsigned char *ptr1;
    unsigned char *ptr2;
    int val1;
    int val2;
    int val3;
    unsigned short int val4;
    int finalSize;
    unsigned int *localBufferBrick;

    FILE *file;
    int headerSize;
    int dataSize;
    int compressedSize;

    short int mode;

    firstBrick = 60000;
    lastBrick = 0;
    counter = 1;

    outPtr = videoBuffer2 + 153800;
    outPtr2 = (unsigned short int *) outPtr;

    loadBrkSub1(outPtr, 20000);

    offset = 4;

    endOfGridPtr2 = endOfGridPtr = currentGrid + (gridSize - 32);

    do
	{
	    temp = *(endOfGridPtr2 + (counter >> 3));
	    temp2 = 7 - (counter & 7);

	    temp3 = 1 << temp2;

	    if (temp & temp3)
		{
		    temp4 = *(int *) (currentBll + offset - 4);
		    ptr1 = currentBll + temp4;

		    val1 = *ptr1;
		    val2 = *(ptr1 + 1);

		    val2 *= val1;

		    val3 = *(char *) (ptr1 + 2);

		    val3 *= val2;

		    ptr2 = ptr1 + 5;

		    counter2 = 0;

		    while (counter2++ < val3)
			{
			    val4 = *(unsigned short int *) ptr2;
			    if (val4 != 0)
				{
				    val4--;

				    if (val4 <= firstBrick)
					firstBrick = val4;

				    if (val4 > lastBrick)
					lastBrick = val4;

				    *(outPtr2 + val4) = 1;
				}

			    ptr2 += 4;
			}

		}

	    offset += 4;
	}
    while (++counter < 256);

    outPtr3 = outPtr2 + firstBrick;

    currentBrick = firstBrick;

    counter3 = 0;

    while (currentBrick <= lastBrick)
	{
	    if (*outPtr3 != 0)
		counter3++;
	    outPtr3++;
	    currentBrick++;
	}

    printf("Need to load %d bricks\n", counter3);

    file = openResource("lba_brk.hqr");

    if (!file)
	return (0);

    readResourceData(file, (char *) &headerSize, 4);

    fseek(file, 0, 0);

    readResourceData(file, (char *) videoBuffer2, headerSize);

    counter3 *= 4;
    counter3 += 4;

    finalSize = counter3;	// car on doit au moin avoir 1 ptr par brique

    headerSize = headerSize >> 2;

    outPtr4 = outPtr2 + firstBrick;

    localBufferBrick = (unsigned int *) bufferBrick;

    destPtr = bufferBrick + counter3;

    *(int *) localBufferBrick = counter3;

    localBufferBrick++;

    currentBrick = firstBrick;

    ptrUnk = (unsigned int *) (firstBrick * 4 + videoBuffer2);

    counter6 = 0;

    while (currentBrick <= lastBrick)
	{
	    if (*outPtr4)
		{
		    counter6++;
		    *outPtr4 = counter6;
		    fseek(file, *ptrUnk, 0);

		    readResourceData(file, (char *) &dataSize, 4);
		    readResourceData(file, (char *) &compressedSize, 4);

		    readResourceData(file, (char *) &mode, 2);

		    if (mode == 0)
			{
			    readResourceData(file, (char *) destPtr, dataSize);
			}
		    else if (mode == 1)
			{
			    compressedPtr = dataSize + destPtr - compressedSize + 500;
			    readResourceData(file, (char *) compressedPtr, compressedSize);
			    decompress(dataSize, destPtr, compressedPtr);
			}
		    finalSize += dataSize;
		    destPtr += dataSize;
		    *localBufferBrick = finalSize;
		    localBufferBrick++;
		}

	    outPtr4++;
	    ptrUnk++;
	    currentBrick++;
	}

    counter6 = 1;

    offset2 = 4;

    endOfGridPtr3 = endOfGridPtr = currentGrid + (gridSize - 32);

    do
	{
	    temp = *(endOfGridPtr3 + (counter6 >> 3));
	    temp2 = 7 - (counter6 & 7);

	    temp3 = 1 << temp2;

	    if (temp & temp3)
		{
		    temp4 = *(int *) (currentBll + offset2 - 4);
		    ptr1 = currentBll + temp4;

		    val1 = *ptr1;
		    val2 = *(ptr1 + 1);

		    val2 *= val1;

		    val3 = *(char *) (ptr1 + 2);

		    val3 *= val2;

		    ptr2 = ptr1 + 5;

		    counter7 = 0;

		    while (counter7++ < val3)
			{
			    val4 = *(unsigned short int *) ptr2;
			    if (val4 != 0)
				{
				    *(unsigned short int *) ptr2 =
					*(unsigned short int *) (outPtr + val4 * 2 - 2);

				}
			    ptr2 += 4;
			}

		}

	    offset2 += 4;
	}
    while (++counter6 < 256);

    closeResource(file);

    return (finalSize);
}

void LBA_engine::loadBrkSub1(unsigned char *ptr, int size)
{
    int i;

    for (i = 0; i < size; i++)
	{
	    *(ptr++) = 0;
	}
}
