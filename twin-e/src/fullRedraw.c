/***************************************************************************
                          fullRedraw.cpp  -  description
                             -------------------
    begin                : Sun Apr 28 2002
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

void
  LBA_engine::fullRedraw(int param)
{
    short int temp1;
    short int temp2;

   // short int temp4;
    int counter;
    int counter2;

   // int i;

    int arg_1A;
    int a12;
    int arg_46;
    int arg_42;
    int a0E;
    int arg_1E;
    actor *lactor;
    int temp3;
    int spriteWidth;
    int spriteHeight;

   // debug:

   // param=1;

   // char stringTemp[10];

    temp1 = fullRedrawVar1;
    temp2 = fullRedrawVar2;

    maximizeTextWindow();

    if (!param) {
	fullRedrawSub1();	// blit background on the actors
    } else			// redraw the background as well
    {
	freezeTime();
	resetVideoBuffer1();
	redrawCube();
	fullRedrawSub3(temp1, temp2, fullRedrawVar1, fullRedrawVar2);
       // fullRedrawSub4();
	copyToBuffer(videoBuffer1, videoBuffer2);
    }

    arg_1A = 0;
    a12 = 0;
    arg_46 = arg_1A + 0x1000;
    arg_42 = arg_1A + 0x0C00;

   // loop1

    for (arg_1A = 0; arg_1A < numActorInRoom; arg_1A++, arg_46++, arg_42++)	// process actors
       // (and draw shadow 
       // if aplicable)
    {
	lactor = &actors[arg_1A];
	*(byte *) & lactor->field_62 &= 0xFFEF;	// recheck -> reinit the draw flags for the current 
						// 
       // 
       // objects

	if ((changeRoomVar9 == -1)
	    || (lactor->Z <= *(short int *) ((currentGrid2) * 24 + sceneVarPtr + 8))) {
	    if (lactor->field_60 & 0x2000 && param == 0) {
	       // calculate the actor position on screen
		fullRedrawS2S1(lactor->X - cameraX, lactor->Z - cameraZ, lactor->Y - cameraY);

	       // is the actor in the viewable screen ?
		if (fullRedrawVar3 > -50 && fullRedrawVar3 < 680
		    && fullRedrawVar4 > -30 && fullRedrawVar4 < 580) {
		    *(byte *) & lactor->field_62 |= 0x10;
		}
	    } else {
		if (lactor->costumeIndex != -1 && !(lactor->field_60 & 0x200))	// 0x200 -> nulos
		{
		   // calculate the actor position on screen
		    fullRedrawS2S1(lactor->X - cameraX, lactor->Z - cameraZ, lactor->Y - cameraY);

		    if (((lactor->field_60 & 8) && fullRedrawVar3 > -112
			 && fullRedrawVar3 < 752 && fullRedrawVar4 > -50
			 && fullRedrawVar4 < 651) || ((!(lactor->field_60 & 8))
						      && fullRedrawVar3 > -50
						      && fullRedrawVar3 < 680
						      && fullRedrawVar4 > -30
						      && fullRedrawVar4 < 580)) {
			temp3 = lactor->Z + lactor->X - cameraX - cameraY;

			if (lactor->standOn != -1)	// if actor is on another actor
			{
			    temp3 =
				actors[lactor->standOn].X - cameraX +
				actors[lactor->standOn].Y - cameraY + 2;
			}

			if (lactor->field_60 & 0x400) {
			    fullRedrawVar6[a12].field_2 = arg_46;	// 0x1000
			    if (lactor->field_60 & 8) {
				temp3 = lactor->lastX - cameraX + lactor->lastY - cameraY;
			    }
			} else {
			    fullRedrawVar6[a12].field_2 = arg_1A;
			}

			fullRedrawVar6[a12].field_0 = temp3;
			a12++;

			if (shadowMode != 0 && !(lactor->field_60 & 0x1000))	// 0x1000 -> no
			   // shadow cast
			{
			   // if(lactor->field_58!=-1) // quick shadow calc if on another actor
			    {
				shadowX = lactor->X;
				shadowZ = lactor->Z - 1;
				shadowY = lactor->Y;
			    }
			   // else
			    {
			       // addShadowActorToCube(lactor->X,lactor->Z,lactor->Y);
			    }

			    temp3--;
			    fullRedrawVar6[a12].field_0 = temp3;	// save the shadow entry in 
									// 
			   // 
			   // the fullRedrawVar6
			    fullRedrawVar6[a12].field_2 = arg_42;	// 0xC00
			    fullRedrawVar6[a12].X = shadowX;
			    fullRedrawVar6[a12].Z = shadowZ;
			    fullRedrawVar6[a12].Y = shadowY;
			    fullRedrawVar6[a12].field_A = 2;
			    a12++;

			}

			if (drawInGameTransBox) {
			    if (reinitVar8 != arg_1A) {
			       /*
			        * arg_16=fullRedrawVar3; arg_12=fullRedrawVar4;
			        */
			    }
			}
		    }
		}
	    }
	}
    }

    arg_1A = 0;
    counter = arg_1A + 0x1800;
    counter2 = 0;

   // loop2

    do				// process "active" elements (like key, hearts,...)
    {
	if (reinitAll2SubVar1[counter2].field_0 != -1) {
	    if (reinitAll2SubVar1[counter2].field_15 & 4) {
		if (time - reinitAll2SubVar1[counter2].field_18 > 35) {
		    reinitAll2SubVar1[counter2].field_18 = time;
		    reinitAll2SubVar1[counter2].field_15 &= 0xFB;
		    fullRedrawS3(11, 0x1000, 1,
				 reinitAll2SubVar1[counter2].field_2,
				 reinitAll2SubVar1[counter2].field_4,
				 reinitAll2SubVar1[counter2].field_6);
		}
	    } else {
		if ((reinitAll2SubVar1[counter2].field_14 & 1) ||
		    (reinitAll2SubVar1[counter2].field_14 & 0x40) ||
		    (reinitAll2SubVar1[counter2].field_1E +
		     reinitAll2SubVar1[counter2].field_18 - 0x96 > time)
		    || (!(time + reinitAll2SubVar1[counter2].field_18 & 8))) {
		    fullRedrawS2S1(reinitAll2SubVar1[counter2].field_2 -
				   cameraX,
				   reinitAll2SubVar1[counter2].field_4 -
				   cameraZ, reinitAll2SubVar1[counter2].field_6 - cameraY);

		    if (fullRedrawVar3 > -50 && fullRedrawVar3 < 680
			&& fullRedrawVar4 > -30 && fullRedrawVar4 < 580) {
			fullRedrawVar6[a12].field_0 =
			    reinitAll2SubVar1[counter2].field_2 - cameraX +
			    reinitAll2SubVar1[counter2].field_6 - cameraY;
			fullRedrawVar6[a12].field_2 = counter;
			a12++;

			if (shadowMode == 2 && reinitAll2SubVar1[counter2].field_0 & 0x8000)	// cast 
												// 
			   // 
			   // shadow
			{
			   // addShadowActorToCube(reinitAll2SubVar1[counter2].field_2,reinitAll2SubVar1[counter2].field_4,reinitAll2SubVar1[counter2].field_6);
			    fullRedrawVar6[a12].field_0 =
				reinitAll2SubVar1[counter2].field_2 - cameraX +
				reinitAll2SubVar1[counter2].field_6 - cameraY - 1;
			    fullRedrawVar6[a12].field_2 = 0xC00;
			    fullRedrawVar6[a12].X = shadowX;
			    fullRedrawVar6[a12].Z = shadowZ;
			    fullRedrawVar6[a12].Y = shadowY;

			    a12++;
			}
		    }
		}
	    }
	}
	arg_1A++;
	counter++;
	counter2++;
    }
    while (arg_1A < 50);

   // sortRenderList(&fullRedrawVar6,a12,12);

   /*
    * if(twinsen->costumeIndex != -1 && !(twinsen->field_60 & 0x200)) {
    * arg_46=twinsen->X+twinsen->field_26; arg_36=twinsen->Z+twinsen->field_2E;
    * arg_52=twinsen->X+twinsen->field_28; arg_4A=twinsen->Z+twinsen->field_30; arg_1A=0;
    * result=-1;
    * 
    * //loop3 while(arg_1A<a12) { if(fullRedrawVar6[arg_1A].field_2 == 0) { result=arg_1A;
    * result2=fullRedrawVar6[arg_1A].field_0; break; } arg_1A++; }
    * 
    * if(result!=-1) { printf("Special draw in fullRedraw!\n"); exit(1); arg_1A=0; // loop 4
    * while(arg_1A<a12) { } }
    * 
    * }
    */

    a0E = 0;
    fullRedrawVar8 = 0;
    arg_1A = 0;
    arg_1E = 0;

   // loop6 -> final drawing code
    if (a12 > 0) {
	unsigned int flags;
	int actorNumber;
	int positionInDebugBox = 0;

	do {
	    actorNumber = fullRedrawVar6[arg_1E].field_2 & 0x3FF;
	    lactor = &actors[actorNumber];
	    flags = ((unsigned int) fullRedrawVar6[arg_1E].field_2) & 0xFC00;

	    if (flags < 0xC00)	// actor
	    {
		if (!flags) {
		    if (!actorNumber) {
		    }
		   // printf("Draw poly actor %d\n",actorNumber);

		   // applyAnim(lactor->animPosition,(char*)getHqrdataPtr(HQRanims,lactor->currentAnim),(char*)bodyPtrTab[lactor->costumeIndex]);

		    if (!startRenderer
			(lactor->X - cameraX, lactor->Z - cameraZ,
			 lactor->Y - cameraY, 0, lactor->angle, 0,
			 bodyPtrTab[lactor->costumeIndex])) {
			setTextWindowSize(renderLeft, renderTop, renderRight, renderBottom);

			if (textWindowLeft <= textWindowRight && textWindowTop <= textWindowBottom) {
			    int tempX;
			    int tempZ;
			    int tempY;

			    lactor->field_62 |= 0x10;

			    tempX = lactor->X >> 9;
			    tempZ = lactor->Z >> 8;
			    if (lactor->field_3 & 0x7F)
				tempZ++;
			    tempY = lactor->Y >> 9;

			    refreshUpperBricks3d(tempX, tempZ, tempY);

			    addToRedrawBoxMain(textWindowLeft, textWindowTop,
					       renderRight + 1, renderBottom + 1);

			    if (lactor->field_60 & 0x2000 && param != 1) {
				drawBoxTrans(textWindowLeft, textWindowTop,
					     textWindowRight, textWindowBottom,
					     (char *) videoBuffer1,
					     textWindowLeft, textWindowTop, (char *) videoBuffer2);
			    }
			}
		    }

		    _debugger.actorBox[positionInDebugBox].actorNum = actorNumber;
		    _debugger.actorBox[positionInDebugBox].left = renderLeft;
		    _debugger.actorBox[positionInDebugBox].right = renderRight;
		    _debugger.actorBox[positionInDebugBox].top = renderTop;
		    _debugger.actorBox[positionInDebugBox].bottom = renderBottom;

		    positionInDebugBox++;

		   /*
		    * sprintf (stringTemp, "%d", actorNumber); setTextColor (57);
		    * 
		    * fullRedrawS2S1 (lactor->X - cameraX, lactor->Z - cameraZ, lactor->Y -
		    * cameraY);
		    * 
		    * if (fullRedrawVar3 > 40 && fullRedrawVar3 < 600 && fullRedrawVar4 > 40 &&
		    * fullRedrawVar4 < 440) printStringSimple (fullRedrawVar3, fullRedrawVar4,
		    * stringTemp);
		    */

		}
	    } else if (flags == 0xC00)	// shadows
	    {
	       /*
	        * if(!(fullRedrawVar6[arg_1E].field_2&0x3FF)) { // arg_0E=1; }
	        * 
	        * fullRedrawS2S1(fullRedrawVar6[arg_1E].X-cameraX,fullRedrawVar6[arg_1E].Z-cameraZ,fullRedrawVar6[arg_1E].Y-cameraY);
	        * getSpriteSize(0,&spriteWidth,&spriteHeight,(char*)shadowSprite);
	        * 
	        * renderLeft=fullRedrawVar3-spriteWidth; // calculate center renderRight=fullRedrawVar3+spriteWidth;
	        * 
	        * renderTop=fullRedrawVar4-spriteHeight; renderBottom=fullRedrawVar4+spriteHeight;
	        * 
	        * setTextWindowSize(renderLeft,renderTop,renderRight,renderBottom);
	        * 
	        * if(textWindowLeft<=textWindowRight && textWindowTop<=textWindowBottom) {
	        * drawSprite(fullRedrawVar6[arg_1E].field_A,renderLeft,renderTop,shadowSprite); }
	        */
	    } else if (flags < 0x1000) {
		printf("Draw actor <0x1000 %d\n", fullRedrawVar6[arg_1E].field_2 & 0x3FF);
	    } else if (flags == 0x1000)	// sprite actor
	    {

	       // printf("Draw sprite actor %d\n",actorNumber);

		fullRedrawS2S1(lactor->X - cameraX, lactor->Z - cameraZ, lactor->Y - cameraY);
		getSpriteSize(0, &spriteWidth, &spriteHeight,
			      (char *) getHqrdataPtr(HQRPtrSpriteExtra, lactor->costumeIndex));

		renderLeft = fullRedrawVar3 + *(short int *) (HQRess3 + lactor->costumeIndex * 16);	// calculate 
													// 
	       // 
	       // center
		renderTop =
		    fullRedrawVar4 + *(short int *) (HQRess3 + lactor->costumeIndex * 16 + 2);

		renderRight = renderLeft + spriteWidth;
		renderBottom = renderTop + spriteHeight;

		if (lactor->field_60 & 8) {
		    setTextWindowSize(fullRedrawVar1 + lactor->field_4E,
				      fullRedrawVar2 + lactor->field_50,
				      fullRedrawVar1 + lactor->field_52,
				      fullRedrawVar2 + lactor->field_54);
		} else {
		    setTextWindowSize(renderLeft, renderTop, renderRight, renderBottom);
		}

		if (textWindowLeft <= textWindowRight && textWindowTop <= textWindowBottom) {
		    drawSprite(0, renderLeft, renderTop,
			       getHqrdataPtr(HQRPtrSpriteExtra, lactor->costumeIndex));

		    lactor->field_62 |= 0x10;

		    if (lactor->field_60 & 8) {
			refreshUpperBricks(lactor->lastX >> 9,
					   lactor->lastZ >> 8, lactor->lastY >> 9);
		    } else {
			int tempX;
			int tempZ;
			int tempY;

			tempX = (lactor->X + lactor->field_28) >> 9;
			tempZ = lactor->Z >> 8;
			if (lactor->field_3 & 0x7F)
			    tempZ++;
			tempY = (lactor->Y + lactor->field_30) >> 9;

			refreshUpperBricks(tempX, tempZ, tempY);
		    }

		    addToRedrawBoxMain(textWindowLeft, textWindowTop,
				       textWindowRight, textWindowBottom);

		    if (lactor->field_60 & 0x2000 && param != 1) {
			drawBoxTrans(textWindowLeft, textWindowTop,
				     textWindowRight, textWindowBottom,
				     (char *) videoBuffer1, textWindowLeft,
				     textWindowTop, (char *) videoBuffer2);
		    }
		}

		_debugger.actorBox[positionInDebugBox].actorNum = actorNumber;
		_debugger.actorBox[positionInDebugBox].left = textWindowLeft;
		_debugger.actorBox[positionInDebugBox].right = textWindowRight;
		_debugger.actorBox[positionInDebugBox].top = textWindowTop;
		_debugger.actorBox[positionInDebugBox].bottom = textWindowBottom;

		positionInDebugBox++;

	       /*
	        * sprintf (stringTemp, "%d", actorNumber); setTextColor (157);
	        * 
	        * fullRedrawS2S1 (lactor->X - cameraX, lactor->Z - cameraZ, lactor->Y - cameraY);
	        * 
	        * if (fullRedrawVar3 > 40 && fullRedrawVar3 < 600 && fullRedrawVar4 > 40 &&
	        * fullRedrawVar4 < 440) printStringSimple (fullRedrawVar3, fullRedrawVar4,
	        * stringTemp); 
	        */

	    } else if (flags == 0x1800) {
		printf("Draw actor ==0x1800 %d\n", fullRedrawVar6[arg_1E].field_2 & 0x3FF);
	    }

	    arg_1A++;
	    maximizeTextWindow();
	    arg_1E++;
	}
	while (arg_1A < a12);

	_debugger.numOfActorOnScreen = positionInDebugBox;

       // unknownLoop

       // loop5
    }

   /*
    * for (arg_1A = 0; arg_1A < numFlags; arg_1A++) //affichage des flags { char stringTemp[256];
    * startRenderer (flagData[arg_1A].x - cameraX, flagData[arg_1A].z - cameraZ, flagData[arg_1A].y 
    * - cameraY, 0, 0, 0, flagModelPtr);
    * 
    * sprintf (stringTemp, "%d", arg_1A); setTextColor (255);
    * 
    * fullRedrawS2S1 (flagData[arg_1A].x - cameraX, flagData[arg_1A].z - cameraZ,
    * flagData[arg_1A].y - cameraY);
    * 
    * if (fullRedrawVar3 > 40 && fullRedrawVar3 < 600 && fullRedrawVar4 > 40 && fullRedrawVar4 <
    * 440) printStringSimple (fullRedrawVar3, fullRedrawVar4, stringTemp); } 
    */

    counter2 = 0;

   // loop7
    do {
	if (roomData2[counter2].field_0 != -1) {
	   // another process
	}
	arg_1A++;
	counter2++;
    }
    while (arg_1A < 10);

    maximizeTextWindow();

   // if(fullRedrawVar9!=0)
   // waitRetrace();

    if (drawInGameTransBox != 0) {
       // fullRedrawSub10(arg_16,arg_12,arg_E,arg_A);
	fullRedrawSub5();
	if (param != 0)
	    unfreezeTime();
    } else {
	if (param == 0) {
	    fullRedrawSub11();	// draw all to screen
	} else {
	    if (!mainLoopVar3)
		osystem->drawBufferToScreen(videoBuffer1);
	    fullRedrawSub5();	// 
	    unfreezeTime();
	}
    }

    if (!mainLoopVar3)
	return;

    loadImageToPtr("ress.hqr", (byte *) & palette, 0);
    convertPalToRGBA(palette, paletteRGBA);

   /*
    * if(isMenuDisplayed) fadeIn2((char*)menuPalRGBA); else fadeIn2((char*)paletteRGBA); 
    */

    if (isMenuDisplayed) {
	osystem->crossFade((char *) videoBuffer1, (char *) menuPalRGBA);
    } else {
	osystem->crossFade((char *) videoBuffer1, (char *) paletteRGBA);
    }

    osystem->setPalette((byte *) & paletteRGBA);

    readKeyboard();

    mainLoopVar3 = 0;

    return;
}

void LBA_engine::fullRedrawSub3(short int arg_0, short int arg_4, short int arg_8, short int arg_C)
{
    int i;
    short int var1;
    short int var2;

    var1 = arg_8 - arg_0;
    var2 = arg_C - arg_4;

    for (i = 0; i < 10; i++) {
	if (roomData2[i].field_6 == 1) {
	    roomData2[i].field_2 = var1;
	    roomData2[i].field_4 = var2;
	}
    }
}

void LBA_engine::fullRedrawSub1(void)
{
    int i;

    for (i = 0; i < numOfRedrawBox; i++) {
	drawBoxTrans(refreshBoxList[i].field_0, refreshBoxList[i].field_2,
		     refreshBoxList[i].field_4, refreshBoxList[i].field_6,
		     (char *) videoBuffer2, refreshBoxList[i].field_0,
		     refreshBoxList[i].field_2, (char *) videoBuffer1);
    }
}

void LBA_engine::fullRedrawSub5(void)
{
    int i;

    numOfRedrawBox = 0;

    for (i = 0; i < fullRedrawVar8; i++) {
	addToRedrawBox(refreshBoxList2[i].field_0, refreshBoxList2[i].field_2,
		       refreshBoxList2[i].field_4, refreshBoxList2[i].field_6);
    }
}

void LBA_engine::addToRedrawBox(short int arg_0, short int arg_4, short int arg_8, short int arg_C)
{
    int var1;
    int i = 0;
    int var_C;
    int var_8;
    int var_10;
    int var_4;

    var1 = (arg_8 - arg_0) * (arg_C - arg_4);

    while (i < numOfRedrawBox) {
	if (refreshBoxList[i].field_0 >= arg_0)
	    var_C = arg_0;
	else
	    var_C = refreshBoxList[i].field_0;

	if (refreshBoxList[i].field_4 <= arg_8)
	    var_8 = arg_8;
	else
	    var_8 = refreshBoxList[i].field_4;

	if (refreshBoxList[i].field_2 >= arg_4)
	    var_10 = arg_4;
	else
	    var_10 = refreshBoxList[i].field_2;

	if (refreshBoxList[i].field_6 <= arg_C)
	    var_4 = arg_C;
	else
	    var_4 = refreshBoxList[i].field_6;

	if ((var_8 - var_C) * (var_4 - var_10) <
	    ((refreshBoxList[i].field_6 -
	      refreshBoxList[i].field_2) * (refreshBoxList[i].field_4 -
					    refreshBoxList[i].field_0) + var1)) {
	    refreshBoxList[i].field_0 = var_C;
	    refreshBoxList[i].field_2 = var_10;
	    refreshBoxList[i].field_4 = var_8;
	    refreshBoxList[i].field_6 = var_4;
	    return;
	}

	i++;
    };

    refreshBoxList[i].field_0 = arg_0;
    refreshBoxList[i].field_2 = arg_4;
    refreshBoxList[i].field_4 = arg_8;
    refreshBoxList[i].field_6 = arg_C;

    numOfRedrawBox++;
}

void LBA_engine::redrawCube(void)
{

    int i;
    int x, y, z;
    unsigned char *localBufCube = bufCube;
    unsigned char val;

    cameraX = newCameraX << 9;
    cameraZ = newCameraZ << 8;
    cameraY = changeRoomVar6 << 9;

    fullRedrawS2S1(-cameraX, -cameraZ, -cameraY);

    fullRedrawVar1 = fullRedrawVar3;
    fullRedrawVar2 = fullRedrawVar4;

    for (i = 0; i < 28; i++) {
	zbufferTab[i] = 0;
    }

    if (changeRoomVar10 == 0)
	return;

    for (y = 0; y < 64; y++) {
	for (x = 0; x < 64; x++) {
	    for (z = 0; z < 25; z++) {
		val = *(localBufCube++);
		if (val) {
		    zbuffer(--val, *(localBufCube), x, z, y);
		}
		localBufCube++;
	    }
	}
    }
}

void LBA_engine::zbuffer(int var1, int var2, int x, int z, int y)
{
    unsigned char *ptr;
    unsigned short int bx;
    int zbufferIndex;
    zbufferDataStruct *currentZbufferData;

    ptr = zbufferSub1(var1) + 3 + var2 * 4 + 2;

    bx = *(unsigned short int *) ptr;

    if (!bx)
	return;

    zbufferSub2(x - newCameraX, z - newCameraZ, y - changeRoomVar6);

    if (zbufferVar1 < -24)
	return;
    if (zbufferVar1 >= 640)
	return;
    if (zbufferVar2 < -38)
	return;
    if (zbufferVar2 >= 480)
	return;

    drawSprite(bx - 1, zbufferVar1, zbufferVar2, bufferBrick);

    zbufferIndex = (zbufferVar1 + 24) / 24;

    if (zbufferTab[zbufferIndex] >= 150) {
	printf("Arg MAX_BRICK Z BUFFER atteint\n");
	exit(1);
    }

    currentZbufferData = &zbufferData[zbufferIndex][zbufferTab[zbufferIndex]];

    currentZbufferData->y = y;
    currentZbufferData->z = z;
    currentZbufferData->x = x;
    currentZbufferData->drawY = zbufferVar2;
    currentZbufferData->spriteNum = bx - 1;

    zbufferTab[zbufferIndex]++;
}

unsigned char *LBA_engine::zbufferSub1(int var)
{
    return (currentBll + *(unsigned int *) (currentBll + 4 * var));
}

void LBA_engine::zbufferSub2(int x, int z, int y)
{
    zbufferVar1 = ((x - y) * 16) + (x - y) * 8 + 288;	// x pos
    zbufferVar2 = (((x + y) * 8 + (x + y) * 4) - ((z * 16) - z)) + 215;	// y pos
}

void LBA_engine::drawSprite(int num, int var1, int var2, unsigned char *localBufferBrick)
{
    unsigned char *ptr;
    int top;
    int bottom;
    int left;
    int right;
    unsigned char *outPtr;
    unsigned char *outPtr2;
    int offset;
    int c1;
    int c2;
    int vc3;

    int temp;
    int iteration;
    int i;

    ptr = localBufferBrick + *(unsigned int *) (localBufferBrick + num * 4);

    left = var1 + *(ptr + 2);
    top = var2 + *(ptr + 3);
    right = *ptr + left - 1;
    bottom = *(ptr + 1) + top - 1;

    ptr += 4;

    if (left >= textWindowLeft && top >= textWindowTop && right <= textWindowRight && bottom <= textWindowBottom) {	// no 
															// 
       // 
       // crop
	right++;
	bottom++;

	outPtr = videoBuffer1 + screenLockupTable[top] + left;

	offset = -((right - left) - largeurEcran);

	for (c1 = 0; c1 < bottom - top; c1++) {
	    vc3 = *(ptr++);
	    for (c2 = 0; c2 < vc3; c2++) {
		temp = *(ptr++);
		iteration = temp & 0x3F;
		if (temp & 0xC0) {
		    iteration++;
		    if (!(temp & 0x40)) {
			temp = *(ptr++);
			for (i = 0; i < iteration; i++)
			    *(outPtr++) = temp;
		    } else {
			for (i = 0; i < iteration; i++)
			    *(outPtr++) = *(ptr++);
		    }
		} else {
		    outPtr += iteration + 1;
		}
	    }
	    outPtr += offset;
	}
    } else {
	if (left <= textWindowRight && top <= textWindowBottom
	    && right >= textWindowLeft && bottom >= textWindowTop) {
	    if (top < textWindowTop)	// si il manque un bout du haut de la brique
	    {

		for (c1 = 0; c1 < (textWindowTop - top); c1++) {
		    vc3 = *(ptr++);
		    for (c2 = 0; c2 < vc3; c2++) {
			temp = *(ptr++);
			if (temp & 0xC0) {
			    iteration = 0;
			    if (temp & 0x40) {
				iteration = temp & 0x3F;
			    }
			    ptr += iteration + 1;
			}
		    }
		}
		top = textWindowTop;
	    }

	    if (bottom > textWindowBottom)
		bottom = textWindowBottom;

	    if (left < textWindowLeft)
		cropLeft = textWindowLeft - left;
	    else
		cropLeft = 0;

	    offset = -(left + cropLeft - 1 - right);

	    right -= textWindowRight;
	    if (right > 0)
		offset -= right;

	    outPtr2 = screenLockupTable[top] + videoBuffer1;
	    if (left >= 0)
		outPtr2 += left;
	    outPtr = outBuffer;

	    for (c1 = 0; c1 <= (bottom - top); c1++) {
		outPtr = outBuffer;

		for (i = 0; i < 512; i++)
		    outBuffer[i] = 0;

		vc3 = *(ptr++);
		for (c2 = 0; c2 < vc3; c2++) {
		    temp = *(ptr++);
		    iteration = temp & 0x3F;
		    if (temp & 0xC0) {
			iteration++;
			if (!(temp & 0x40)) {
			    temp = *(ptr++);
			    for (i = 0; i < iteration; i++)
				*(outPtr++) = temp;
			} else {
			    for (i = 0; i < iteration; i++)
				*(outPtr++) = *(ptr++);
			}
		    } else {
			outPtr += iteration + 1;
		    }
		}

		for (i = 0; i < offset; i++) {
		    if (outBuffer[cropLeft + i] != 0)
			*(outPtr2) = outBuffer[cropLeft + i];
		    outPtr2++;
		}
		outPtr2 += 640 - offset;
	    }
	}
    }
}

void LBA_engine::drawSprite2(int num, int var1, int var2, unsigned char *localBufferBrick)
{
    unsigned char *ptr;
    int top;
    int bottom;
    int left;
    int right;
    unsigned char *outPtr;
    unsigned char *outPtr2;
    int offset;
    int c1;
    int c2;
    int vc3;

    int temp;
    int iteration;
    int i;

    ptr = localBufferBrick + *(unsigned int *) (localBufferBrick + num * 4);

    left = var1 + *(ptr + 2);
    top = var2 + *(ptr + 3);
    right = *ptr + left - 1;
    bottom = *(ptr + 1) + top - 1;

    ptr += 4;

   // check des bords

    if (left >= textWindowLeft && top >= textWindowTop
	&& right <= textWindowRight && bottom <= textWindowBottom) {
	right++;
	bottom++;

	outPtr = videoBuffer2 + screenLockupTable[top] + left;

	offset = -((right - left) - largeurEcran);

	for (c1 = 0; c1 < bottom - top; c1++) {
	    vc3 = *(ptr++);
	    for (c2 = 0; c2 < vc3; c2++) {
		temp = *(ptr++);
		iteration = temp & 0x3F;
		if (temp & 0xC0) {
		    iteration++;
		    if (!(temp & 0x40)) {
			temp = *(ptr++);
			for (i = 0; i < iteration; i++)
			    *(outPtr++) = temp;
		    } else {
			for (i = 0; i < iteration; i++)
			    *(outPtr++) = *(ptr++);
		    }
		} else {
		    outPtr += iteration + 1;
		}
	    }
	    outPtr += offset;
	}
    } else {
	if (left <= textWindowRight && top <= textWindowBottom
	    && right >= textWindowLeft && bottom >= textWindowTop) {
	    if (top < textWindowTop)	// si il manque un bout du haut de la brique
	    {

		for (c1 = 0; c1 < (textWindowTop - top); c1++) {
		    vc3 = *(ptr++);
		    for (c2 = 0; c2 < vc3; c2++) {
			temp = *(ptr++);
			if (temp & 0xC0) {
			    iteration = 0;
			    if (temp & 0x40) {
				iteration = temp & 0x3F;
			    }
			    ptr += iteration + 1;
			}
		    }
		}
		top = textWindowTop;
	    }

	    if (bottom > textWindowBottom)
		bottom = textWindowBottom;

	    if (left < textWindowLeft)
		cropLeft = textWindowLeft - left;
	    else
		cropLeft = 0;

	    offset = -(left + cropLeft - 1 - right);

	    right -= textWindowRight;
	    if (right > 0)
		offset -= right;

	    outPtr2 = screenLockupTable[top] + videoBuffer2;
	    if (left >= 0)
		outPtr2 += left;
	    outPtr = outBuffer;

	    for (c1 = 0; c1 <= bottom - top; c1++) {
		outPtr = outBuffer;

		for (i = 0; i < 512; i++)
		    outBuffer[i] = 0;

		vc3 = *(ptr++);
		for (c2 = 0; c2 < vc3; c2++) {
		    temp = *(ptr++);
		    iteration = temp & 0x3F;
		    if (temp & 0xC0) {
			iteration++;
			if (!(temp & 0x40)) {
			    temp = *(ptr++);
			    for (i = 0; i < iteration; i++)
				*(outPtr++) = temp;
			} else {
			    for (i = 0; i < iteration; i++)
				*(outPtr++) = *(ptr++);
			}
		    } else {
			outPtr += iteration + 1;
		    }
		}

		for (i = 0; i < offset; i++) {
		    if (outBuffer[cropLeft + i] != 0)
			*(outPtr2) = outBuffer[cropLeft + i];
		    outPtr2++;
		}
		outPtr2 += 640 - offset;
	    }

	}
    }
}

int LBA_engine::fullRedrawS2S1(int coX, int coZ, int coY)
{
    if (setSomethingVar4 != 1) {
	printf("Unimplemented fullRedrawS2S1\n");
	exit(1);
    } else {
	fullRedrawVar3 = (((coX - coY) * 8 + (coX - coY) * 16) >> 9) + setSomethingVar1;
	fullRedrawVar4 =
	    ((((coX + coY) * 4 + (coX + coY) * 8) +
	      (2 * coZ - (coZ << 5))) >> 9) + setSomethingVar2;
	return (-1);
    }
    return (-1);
}

int LBA_engine::fullRedrawS3(int param0, int param1, int param2, int param3, int param4, int param5)
{
    if (samplesLoaded != 0) {
	printf("sampleLoaded != 0 in fullRedrawS3!\n");
       // exit(1);
    }
    return (0);
}

void LBA_engine::getSpriteSize(int arg_0, int *arg_4, int *arg_8, char *ptr)
{
    ptr += *(int *) (ptr + arg_0 * 4);

    *arg_4 = *ptr;
    *arg_8 = *(ptr + 1);
}

void LBA_engine::refreshUpperBricks3d(int X, int Z, int Y)
{
    int refreshLeft;
    int refreshRight;
    int i;
    zbufferDataStruct *currentZbufferData;

    refreshLeft = (textWindowLeft + 24) / 24 - 1;
    refreshRight = (textWindowRight + 24) / 24;

    if (refreshLeft <= refreshRight) {
	do {
	    currentZbufferData = zbufferData[refreshLeft];

	    for (i = 0; i < zbufferTab[refreshLeft]; i++) {
		if (currentZbufferData->drawY + 38 > textWindowTop
		    && currentZbufferData->drawY <= textWindowBottom
		    && currentZbufferData->z >= Z) {
		    if (currentZbufferData->x + currentZbufferData->y > Y + X) {
			redrawBrick(currentZbufferData->spriteNum,
				    (refreshLeft * 24) - 24,
				    currentZbufferData->drawY, bufferBrick2, videoBuffer2);
		    }
		}
		currentZbufferData++;
	    }

	}
	while (++refreshLeft <= refreshRight);
    }
}

void LBA_engine::refreshUpperBricks(int X, int Z, int Y)
{
    int refreshLeft;
    int refreshRight;
    int i;
    zbufferDataStruct *currentZbufferData;

    return;

    refreshLeft = (textWindowLeft + 24) / 24 - 1;
    refreshRight = (textWindowRight + 24) / 24;

    if (refreshLeft <= refreshRight) {
	do {
	    currentZbufferData = zbufferData[refreshLeft];

	    for (i = 0; i < zbufferTab[refreshLeft]; i++) {
		if (currentZbufferData->drawY + 38 > textWindowTop
		    && currentZbufferData->drawY <= textWindowBottom
		    && currentZbufferData->z >= Z) {
		    if (currentZbufferData->x == Y && currentZbufferData->y == X) {
			redrawBrick(currentZbufferData->spriteNum,
				    (refreshLeft * 24) - 24,
				    currentZbufferData->drawY, bufferBrick2, videoBuffer2);
		    }

		    if (currentZbufferData->x > Y || currentZbufferData->y > X) {
			redrawBrick(currentZbufferData->spriteNum,
				    (refreshLeft * 24) - 24,
				    currentZbufferData->drawY, bufferBrick2, videoBuffer2);
		    }
		}
		currentZbufferData++;
	    }

	}
	while (++refreshLeft <= refreshRight);
    }
}

void LBA_engine::redrawBrick(int spriteNum, int x, int y, byte * localBufferBrick, byte * buffer)
{
    unsigned char *ptr;
    unsigned char *sourcePtr;

    int top;
    int bottom;
    int left;
    int right;

    ptr = localBufferBrick + *(unsigned int *) (localBufferBrick + spriteNum * 4);

    left = x + *(ptr + 2);
    top = y + *(ptr + 3);
    right = *ptr + left - 1;
    bottom = *(ptr + 1) + top - 1;

    sourcePtr = ptr + 4;

    drawSprite2(spriteNum, x, y, bufferBrick);
}

void LBA_engine::addToRedrawBoxMain(int left, int top, int right, int bottom)
{
    if (left < 0)
	left = 0;
    if (top < 0)
	top = 0;
    if (right >= 640)
	right = 639;
    if (bottom > 480)
	bottom = 479;

    if (left > right || top > bottom)
	return;

   // drawBoxOutLine (left, top, right, bottom);

    refreshBoxList2[fullRedrawVar8].field_0 = left;
    refreshBoxList2[fullRedrawVar8].field_2 = top;
    refreshBoxList2[fullRedrawVar8].field_4 = right;
    refreshBoxList2[fullRedrawVar8].field_6 = bottom;

    fullRedrawVar8++;

    addToRedrawBox(left, top, right, bottom);
}

void LBA_engine::sortRenderList(fullRedrawVar6Struct list, int listSize, int param)
{

}

void LBA_engine::fullRedrawSub11(void)
{
    int i;

    for (i = 0; i < numOfRedrawBox; i++) {
	osystem->refresh(videoBuffer1, refreshBoxList[i].field_0,
			 refreshBoxList[i].field_2, refreshBoxList[i].field_4,
			 refreshBoxList[i].field_6);
    }

    numOfRedrawBox = 0;

    for (i = 0; i < fullRedrawVar8; i++) {
	addToRedrawBox(refreshBoxList2[i].field_0, refreshBoxList2[i].field_2,
		       refreshBoxList2[i].field_4, refreshBoxList2[i].field_6);
    }
}
