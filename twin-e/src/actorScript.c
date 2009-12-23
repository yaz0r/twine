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

#include <math.h>

#include "images.h"
#include "lba.h"
#include "renderer.h"
#include "mainMenu.h"
#include "vox.h"
#include "text.h"
#include "mainLoop.h"
#include "fullRedraw.h"
#include "main.h"
#include "fichePerso.h"
#include "anim.h"
#include "samples.h"
#include "hqr.h"
#include "music.h"
#include "actors.h"
#include "body.h"

#include "actorScript.h"

short int choiceTab[18];

int numOfOptionsInChoice;
short int inGameMenuData[10];

int inGameMenuAnswer;

short int objectRotation[255];

int DoTrackVar1;

int GetAngle(int X1, int Y1, int X2, int Y2) {
	int newX;
	int newY;
	int ebp;
	int edi;
	int tempExchange;
	int flag;
	int difX;
	int difY;

	int destVal;
	int startAngle;
	int stopAngle;
	int finalAngle;

	difY = edi = Y2 - Y1;
	newY = edi * edi;

	difX = ebp = X2 - X1;
	newX = ebp * ebp;

	if (newX < newY) {	// exchange X and Y
		tempExchange = difX;
		difX = difY;
		difY = tempExchange;

		flag = 1;
	} else {
		flag = 0;
	}

	DoTrackVar1 = (int) sqrt(newX + newY);

	if (!DoTrackVar1)
		return (0);

	destVal = (difY << 14) / DoTrackVar1;

	startAngle = 0;
	stopAngle = 0x100;

	while (tab3[startAngle] > destVal) {
		startAngle++;
		/*
		* if (startAngle > 256) { printf ("Can't find angle %d...\n", destVal); exit (1); }
		*/
	}

	if (tab3[startAngle] != destVal)
		if ((tab3[startAngle - 1] + tab3[startAngle]) / 2 <= destVal) {
			startAngle--;
		}

	finalAngle = 128 + startAngle;

	if (difX <= 0) {	// if we are going left
		finalAngle = -finalAngle;
	}

	if (flag & 1) {	// X and Y are exchanged -> 90 degree rotation needed
		finalAngle = -finalAngle + 0x100;
	}

	return (finalAngle & 0x3FF);
}

void processInGameMenu(int index) {
	int i;

	printf("Ask choice %d\n", index);
	CopyScreen(frontVideoBuffer, workVideoBuffer);

	choiceTab[1] = numOfOptionsInChoice;
	choiceTab[0] = 0;
	choiceTab[2] = 0;
	choiceTab[3] = currentTextBank + 3;

	for (i = 0;i < numOfOptionsInChoice;i++) {
		choiceTab[i*2+4] = 0;
		choiceTab[i*2+5] = inGameMenuData[i];
	}

	MyDial(index);

	processMenu(choiceTab);

	inGameMenuAnswer = inGameMenuData[choiceTab[0]];

	//TODO: missing giving answer vox
}

void MyDial(int index) {
	int textStatus = 1;
	//int digitStatus;

	if (languageCD1 != 0) {
		setVoxFileAtDigit(index);
	}

	initText(index);
	InitDialWindow();

	while (textStatus) {
		textStatus = printText10();
		printText4(voxFileHandle);

		if (textStatus == 2)
		{
			printf("ahem\n");
			while (os_isPressed(KEY_CONTTEXT))
				printText4(voxFileHandle);

			while (!os_isPressed(KEY_CONTTEXT))
				printText4(voxFileHandle);
		}

		waitRetrace();
	}

	while (printText4(voxFileHandle))
		;

	printTextVar5 = 0;

	if (languageCD1 && voxFileHandle) {
		if (printText6(4660))
			printText7(4660);
	}

	printTextVar13 = 0;
}

void foundObject(int objectNumber) {
	int objectX;
	int objectY;
	int objectZ;

	int object2X;
	int object2Y;
	int object2Z;

	int boxTopLeftX;
	int boxTopLeftY;
	int boxBottomRightX;
	int boxBottomRightY;

	char voxNotFound;

	int textDisplayState;
	char closeWindow;

	unsigned char* twinsenAnim;

	unsigned char* objectPtr;

	int twinsenAnimKeyframe;

	newCameraX = (twinsen->X + 0x100) >> 9;
	newCameraZ = (twinsen->Y + 0x100) >> 8;
	newCameraY = (twinsen->Z + 0x100) >> 9;

	twinsen->staticFlagsBF.noDisplay = 1;
	fullRedraw(1);
	twinsen->staticFlagsBF.noDisplay = 0;

	CopyScreen(frontVideoBuffer, workVideoBuffer);

	objectX = newCameraX << 9;
	objectZ = newCameraZ << 8;
	objectY = newCameraY << 9;

	AffObjetIso(twinsen->X - objectX, twinsen->Y - objectZ, twinsen->Z - objectY, 0, 0x80, 0, bodyPtrTab[twinsen->costumeIndex]);

	SetClip(renderLeft, renderTop, renderRight, renderBottom);

	object2X = (twinsen->X + 0x100) >> 9;
	object2Z = (twinsen->Y) >> 8;

	if (twinsen->field_3 & 0x7F)
		object2Z++;

	object2Y = (twinsen->Z + 0x100) >> 9;

	DrawOverBrick(object2X, object2Z, object2Y);

	os_flip(frontVideoBuffer);

	projectPositionOnScreen(twinsen->X - objectX, twinsen->Y - objectZ, twinsen->Z - objectY);

	projectedPositionY -= 150;

	boxTopLeftX = projectedPositionX - 65;
	boxTopLeftY = projectedPositionY - 65;

	boxBottomRightX = projectedPositionX + 65;
	boxBottomRightY = projectedPositionY + 65;

	playSample(41, /*0x1000, */1/*, 0x80, 0x80*/);

	/*    if(getFoundVox(2))
	{
	voxNotFound = 0;
	InitDial(2);
	}
	else*/
	{
		int temp;

		stopMusic();
		voxNotFound = 1;

		temp = languageCD1;
		languageCD1 = 0;
		InitDial(2);
		languageCD1 = temp;
	}

	UnSetClip();
	OpenDial(objectNumber);

	textDisplayState = 1;
	closeWindow = 0;

	TestCoulDial(4);

	if (!voxNotFound) {
		//startFoundVox(2, objectNumber);
	} else {
		setVoxFileAtDigit(objectNumber);
	}

	twinsenAnim = HQR_Get(HQR_Anims, getAnimIndexForBody(ANIM_findObject, 0));

	bufAni2 += StockInterAnim((char*)bufAni2, (char*)bodyPtrTab[twinsen->costumeIndex], &twinsen->animTimerData);
	if (bufAni1 + 4488 > bufAni2)
		bufAni2 = bufAni1;

	twinsenAnimKeyframe = 0;

	objectPtr = HQR_Get(HQR_Inventory, objectNumber);

	if (HQR_Flag)
		loadGfxSub(objectPtr);

	numOfRedrawBox = 0;

	while (!closeWindow) {
		UnSetClip();
		fullRedrawVar8 = 0;
		blitBackgroundOnDirtyBoxes();
		drawBoxInsideTrans(boxTopLeftX, boxTopLeftY, boxBottomRightX, boxBottomRightY, 4);

		SetClip(boxTopLeftX, boxTopLeftY, boxBottomRightX, boxBottomRightY);

		objectRotation[objectNumber] += 8;

		Draw3dObject(projectedPositionX, projectedPositionY, (char*)objectPtr, objectRotation[objectNumber], 10000);

		DrawCadre(boxTopLeftX, boxTopLeftY, boxBottomRightX, boxBottomRightY);
		AddPhysBox(boxTopLeftX, boxTopLeftY, boxBottomRightX, boxBottomRightY);
		UnSetClip();
		reinitAll1();

		if (SetInterAnimObjet(twinsenAnimKeyframe, (char*)twinsenAnim, (char*)bodyPtrTab[twinsen->costumeIndex], &twinsen->animTimerData)) {
			twinsenAnimKeyframe++;
			if (twinsenAnimKeyframe == GetNbFramesAnim((char*)twinsenAnim)) {
				twinsenAnimKeyframe = GetBouclageAnim((char*)twinsenAnim);
			}
		}

		AffObjetIso(twinsen->X - objectX, twinsen->Y - objectZ, twinsen->Z - objectY, 0, 0x80, 0, bodyPtrTab[twinsen->costumeIndex]);
		SetClip(renderLeft, renderTop, renderRight, renderBottom);
		DrawOverBrick(object2X, object2Z, object2Y);
		AddPhysBox(renderLeft, renderTop, renderRight, renderBottom);

		if (textDisplayState) {
			UnSetClip();
			textDisplayState = printText10();
		}

		if (textDisplayState) {
			waitRetrace();
		}

		FlipBoxes();

		if (os_isPressed(KEY_CONTTEXT)) {
			if (!textDisplayState) {
				closeWindow = 1;
			}

			if (textDisplayState == 2)
				textDisplayState = 1;
		}

		/* TODO: cleanup */
		os_delay(15);
		lba_time++;
	}

	/*
	if (!voxNotFound)
		while (closeFoundVox())
			if (os_isPressed(KEY_SKIP))
				break;
	else*/
	{
		while (printText11())
			if (os_isPressed(KEY_SKIP))
				break;
	}

	//stopVox();
	reinitAll1();
	InitDial(currentTextBank + 3);

	while (os_isPressed(KEY_SKIP))
		;
}

void OpenDial(int textNumber) {
	initText(textNumber);
	InitDialWindow();
}

void Draw3dObject(int X, int Y, char* objectPtr, int rotation, int param) {
	setCameraPosition(X, Y, 128, 200, 200);
	setCameraAngle(0, 0, 0, 60, 0, 0, param);

	AffObjetIso(0, 0, 0, 0, rotation, 0, (unsigned char*)objectPtr);
}
