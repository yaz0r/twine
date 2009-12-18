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

#include "actors.h"
#include "images.h"
#include "renderer.h"
#include "mainMenu.h"
#include "room.h"
#include "fullRedraw.h"
#include "save.h"
#include "text.h"
#include "inventory.h"
#include "anim.h"
#include "comportementMenu.h"
#include "font.h"
#include "extra.h"
#include "moveActor.h"
#include "script.h"
#include "animAction.h"
#include "angle.h"
#include "actorScript.h"
#include "fichePerso.h"
#include "main.h"
#include "hqr.h"
#include "music.h"
#include "cube.h"
#include "body.h"

#include "mainLoop.h"

int cptime = 0;

short int twinsenKey;

short int selectedInventoryObj;

short int usingSword;

short int twinsenZBeforeFall;

time mainLoopVar1;
short int disableScreenRecenter;

actor *processActorVar1;
short int processActorVar2;
short int processActorVar3;
short int processActorVar4;

short int processActorVar5;
short int processActorVar6;

short int processActorVar11;
short int processActorVar12;
short int processActorVar13;

short int useAlternatePalette = 0;

short int fieldCauseDamage;
short int processActorX;
short int processActorY;
short int processActorZ;

short int currentlyProcessedActorNum;

int mainLoopVar17;

int getPosVar1;
int getPosVar2;
int getPosVar3;

short int drawInGameTransBox = 0;
short int twinsenMoved = 0;
short int twinsenWalked = 0;

short int requestBackgroundRedraw = 1;

short int lockPalette = 0;

short int currentTextBank;

int newCameraX;
int newCameraZ;
int newCameraY;

short int currentY;
short int currentX;
short int currentZ;

time timeVar;

int action;

int time1;
int time3;

int mainLoop(void) {
	requestBackgroundRedraw = 1;
	lockPalette = 1;
	setActorAngle(0, -256, 5, &mainLoopVar1);

	os_mainLoop();

	return 0;
}

int mainLoopInteration(void) {
	int i;
	int currentTime;
	/*int textBank;
	int temp;*/

	currentTime = lba_time;
	for (;;)
	{
		if (needChangeRoom != -1)
			ChangeCube();

		if (lockPalette == 0) {
			/* Starting inputs */

			/* Open the menu */
			if (os_isPressed(KEY_MENU) && twinsen->life > 0 && twinsen->costumeIndex != -1 && !twinsen->staticFlagsBF.noDisplay)
			{
				TestRestoreModeSVGA(1);
				freezeTime();
				if (!makeGiveUpMenu()) {
					unfreezeTime();
					fullRedraw(1);
				}
				else
				{
					unfreezeTime();
					fullRedraw(1);
					freezeTime();
					SaveGame();
					breakMainLoop = 1;
					unfreezeTime();
					return 0;
				}
			}
			/* TODO: fix
			if (os_isPressed(KEY_F6)) { // F6
				temp = languageCD1;
				freezeTime();
				TestRestoreModeSVGA(1);
				soundMenuData[5] = 15;
				languageCD1 = 0;
				InitDial(0);
				optionMenu();
				languageCD1 = temp;
				InitDial(currentTextBank + 3);

				if (currentRoom == 80 && vars[90])
					playMusic(8);
				else
					playMusic(roomMusic);

				unfreezeTime();
				fullRedraw(1);
			}*/
			selectedInventoryObj = -1;
			/* Open the inventory menu */
			if (os_isPressed(KEY_INVENTORY) && twinsen->costumeIndex != -1 && twinsen->comportement == 1)
			{
				freezeTime();
				TestRestoreModeSVGA(1);
				Inventory();

				/* TODO: fix missing objects */
				/* Process selected objects */
				switch (selectedInventoryObj)
				{
				/* holomap */
				case holomap:
					//processHolomap();
					lockPalette = 1;
					break;
				/* magic ball */
				case ball:
					if (usingSword == 1)
						InitBody(0, 0);

					usingSword = 0;

					break;
				/* funfrock's saber */
				case saber:
					if (twinsen->body == 2)
					{
						if (comportementHero == 4)
							SetComportement(0);

						InitBody(2, 0);
						InitAnim(24, 1, 0, 0);

						usingSword = 1;
					}
					break;
				/* bù book */
				case book:
					break;
				/* proto-pack */
				case protopack:
					break;
				/* meca penguin */
				case penguin:
					break;
				/* bonuses list */
				case bonusesList:
					break;
				/* clover leaf */
				case clover:
					if (twinsen->life < 50 && numClover > 0)
					{
						twinsen->life = 50;
						magicPoint = magicLevel * 20;

						numClover--;

						addOverlayObject(3, 27, 0, 0, 0, 0, 3);
					}
					break;
				}

				unfreezeTime();
				fullRedraw(1);
			}

			/* Open behavior menu */
			if (os_isPressed(KEY_CHBEHAVIOR) && twinsen->costumeIndex != -1 && twinsen->comportement == 1)
			{
				freezeTime();
				TestRestoreModeSVGA(1);
				processComportementMenu();
				unfreezeTime();
				fullRedraw(1);
			}

			/* Change behavior */
			/* TODO: fix */
			/*
			if (fkeys >= 1  && fkeys <= 4 && twinsen->costumeIndex != -1 && twinsen->comportement == 1 && !(fkeys == comportementHero + 1))
			{
				TestRestoreModeSVGA(1);

				if (cptime != 0) { // clear if have something already writed in the screen
					blitRectangle(5, 446, 350, 479, (char*)workVideoBuffer, 5, 446, (char*)frontVideoBuffer);
					os_copyBlockPhys(5, 446, 350, 479);
				}
				// Added: Implemented with translations like LBA2 -----
				CoulFont(15);
				textBank = currentTextBank;
				currentTextBank = -1;
				InitDial(0);
				GetMultiText(fkeys - 1, dataString);
				Font(5, 446, dataString);
				os_copyBlockPhys(5, 446, 350, 479);
				currentTextBank = textBank;
				InitDial(currentTextBank + 3);
				cptime = currentTime;
				SetComportement(fkeys - 1);

				fkeys = 0;
			}*/

			/* Enable protopack */
			if (os_isPressed(KEY_ACTION_PROTOPACK) && vars[protopack])
			{
				if (cptime != 0) { // clear if have something already writed in the screen
					blitRectangle(5, 446, 350, 479, (char*)workVideoBuffer, 5, 446, (char*)frontVideoBuffer);
					os_copyBlockPhys(5, 446, 350, 479);
				}

				CoulFont(15);
				Font(5, 446, "Proto-Pack");
				os_copyBlockPhys(5, 446, 200, 479);
				cptime = currentTime;

				if (vars[medaillon])
					twinsen->body = 0;
				else
					twinsen->body = 1;

				if (comportementHero == 4) {
					printf("Stop using Proto-Pack!");
					SetComportement(0);
				} else {
					printf("Now using Proto-Pack!");
					SetComportement(4);
				}
			}

			// Time to display the behaviour text showed in the previous condition.
			if ((lba_time - cptime) > 100 && cptime)
			{
				printf("WTF ?? !!!\n");
				blitRectangle(5, 446, 350, 479, (char*)workVideoBuffer, 5, 446, (char*)frontVideoBuffer);
				os_copyBlockPhys(5, 446, 350, 479);
				cptime = 0;
			}

			/* TODO: fullscreen
			if (fkeys == 12) { // F12 for FullScreen
				osystem_fullScreen();
			} */


			/* Recenter the screen */
			if (os_isPressed(KEY_RECENTER) && disableScreenRecenter == 0)
			{
				newCameraX = actors[currentlyFollowedActor].X >> 9;
				newCameraZ = actors[currentlyFollowedActor].Y >> 8;
				newCameraY = actors[currentlyFollowedActor].Z >> 9;
				requestBackgroundRedraw = 1;
			}

			/* Draw the holomap */
			if (os_isPressed(KEY_ACTION_HOLOMAP) && vars[holomap] == 1 && vars[70] == 0) // draw holomap
			{
				freezeTime();
				TestRestoreModeSVGA(1);
				//processHolomap();
				lockPalette = 1;
				unfreezeTime();
				fullRedraw(1);
			}

			if (os_isPressed(KEY_PAUSE))
			{
				//pauseSound();
				freezeTime();
				if (!drawInGameTransBox)
				{
					printf("Game in Pause...");
					CoulFont(15);
					Font(5, 446, "Pause"); // Don't have an entry in the Text Bank
					os_copyBlockPhys(5, 446, 100, 479);
				}

				while (!os_isPressed(KEY_PAUSE)) ;

				if (!drawInGameTransBox) {
					blitRectangle(5, 446, 100, 479, (char *) workVideoBuffer, 5, 446, (char *) frontVideoBuffer);
					os_copyBlockPhys(5, 446, 100, 479);
				}
				printf("Game Resumed!");
				unfreezeTime();
				//resumeSound();
			}

			/* TODO: F5 input process ? */

			/* Finished input handling */
		}

		/* TODO: better name for this */
		mainLoopVar17 = getRealValue(&mainLoopVar1);
		if (!mainLoopVar17)
			mainLoopVar17 = 1;

		setActorAngle(0, -256, 5, &mainLoopVar1);
		disableScreenRecenter = 0;

		for (i = 0; i < numActorInRoom; i++)
			actors[i].hitBy = -1;

		GereExtras();

		assert(frontVideoBufferbis == frontVideoBuffer);

		/* Actors process wheel */

		for (i = 0; i < numActorInRoom; i++)
		{
			if (!actors[i].dynamicFlagsBF.isDead)
			{
				/* Just killed */
				if (actors[i].life == 0)
				{
					/* If it's twinsen */
					if (i == 0) {
						InitAnim(ANIM_landDeath, 4, 0, 0); /* Play twinsen death anim */
						actors[i].comportement = 0;
					}
					else
					{
						HQ_3D_MixSample(37, rand() % 2000 + 3096, 1, actors[i].X, actors[i].Y, actors[i].Z);

						if (i == currentPingouin) {
							printf("Pinguoin exploded ! Implement\n");
							exit(1);
						}
					}

					/* Give an extra */
					if (actors[i].canGiveBonus && !actors[i].gaveBonus)
						GiveExtraBonus(&actors[i]);
				}

				/* Manage actors' move */
				DoDir(i);

				actors[i].field_20 = actors[i].X;
				actors[i].field_22 = actors[i].Y;
				actors[i].field_24 = actors[i].Z;

				if (actors[i].positionInMoveScript != -1)
					DoTrack(i);

				DoAnim(i);

				if (actors[i].staticFlagsBF.isZonable)
					CheckZoneSce(&actors[i], i);

				if (actors[i].positionInActorScript != -1)
					runActorScript(i);

				/* Already dead */
				if (actors[i].life <= 0)
				{
					/* If it's twinsen and he has finished the die animation */
					if (i == 0 && actors[i].dynamicFlagsBF.animEnded)
					{
						/* If twinsen has clovers, use one of them */
						if (numClover > 0)
						{
							twinsen->X = newTwinsenX;
							twinsen->Y = newTwinsenZ;
							twinsen->Z = newTwinsenY;

							needChangeRoom = currentRoom;
							magicPoint = magicLevel * 20;

							newCameraX = (twinsen->X >> 9);
							newCameraZ = (twinsen->Y >> 8);
							newCameraY = (twinsen->Z >> 9);

							twinsenPositionModeInNewCube = 3;

							twinsen->life = 50;
							requestBackgroundRedraw = 1;
							lockPalette = 1;

							numClover--;
						}
						/* Else, GAME OVER */
						else
						{
							/* TODO: play Game Over anim. Model 20 in Ress file */
							breakMainLoop = 1;
							printf("Game over...\n");
						}
					}
					else
					{
						checkCarrier(i);
						actors[i].dynamicFlagsBF.isDead = 1;
						actors[i].costumeIndex = -1;
						actors[i].zone = -1;
					}
				}
			}
		}

		assert(frontVideoBufferbis == frontVideoBuffer);

		/* Recenter the screen */
		if (!disableScreenRecenter)
		{
			projectPositionOnScreen(actors[currentlyFollowedActor].X - (newCameraX << 9), 
									actors[currentlyFollowedActor].Y - (newCameraZ << 8),
									actors[currentlyFollowedActor].Z - (newCameraY << 9));

			if (projectedPositionX < 80 || projectedPositionX > 539 || projectedPositionY < 80 || projectedPositionY > 429) {
				newCameraX = ((actors[currentlyFollowedActor].X + 0x100) >> 9) + (((actors[currentlyFollowedActor].X + 0x100) >> 9) - newCameraX) / 2;
				newCameraZ = (actors[currentlyFollowedActor].Y) >> 8;
				newCameraY = ((actors[currentlyFollowedActor].Z + 0x100) >> 9) + (((actors[currentlyFollowedActor].Z + 0x100) >> 9) - newCameraY) / 2;

				if (newCameraX >= 64)
					newCameraX = 63;

				if (newCameraY >= 64)
					newCameraY = 63;

				requestBackgroundRedraw = 1;
			}
		}

		/* Redraw the map */
		fullRedraw(requestBackgroundRedraw);
		requestBackgroundRedraw = 0;

#ifndef PCLIKE
		lba_time += 2;
#endif
		break;
	}

	return 0;
}

void reinitAll(int save)
{
	UnSetClip();
	reinitVar1 = 896;
	reinitVar2 = 950;
	reinitAll1();
	reinitVars();
	reinitAll3();
	newTwinsenX = 0x2000;
	newTwinsenZ = 0x1800;
	newTwinsenY = 0x2000;
	currentRoom = -1;
	numClover = 0;
	numCloverBox = 2;
	currentPingouin = -1;
	needChangeRoom = 0;
	magicLevel = 0;
	magicPoint = 0;
	numCoin = 0;
	numKey = 0;
	chapter = 0;
	usingSword = 0;
	currentTextBank = 0;
	fuel = 0;
	currentlyFollowedActor = 0;
	startupAngleInCube = 0;
	comportementHero = 0;
	startupComportementHeroInCube = 0;

	if (save == -1)
	{
		LoadGame();
		if (newTwinsenX == -1)
			twinsenPositionModeInNewCube = 0;
	}

	/*
	 * FadeToBlack((char*)menuPalRGBA); Cls(); os_flip(frontVideoBuffer);
	 */
}

void reinitAll1(void)
{
	configureOrthoProjection(311, 240);
	setSomething2(0, 0, 0);
	setSomething3(0, 0, 0);
	SetLightVector(reinitVar1, reinitVar2, 0);
}

void TestRestoreModeSVGA(int arg_0)
{
	if (!drawInGameTransBox)
		return;

	if (useAlternatePalette)
		os_setPalette(menuPalRGBA);
	else
		os_setPalette(paletteRGBA);

	drawInGameTransBox = 0;

	if (!arg_0)
		fullRedraw(1);
}

void waitRetrace(void)
{
#ifdef WIN32
	int temp = os_getTicks();

	while (os_getTicks() - temp < 15) ;
#endif
}

void freezeTime(void)
{
	if (!time1)
		time3 = lba_time;

	time1++;
}

void unfreezeTime(void)
{
	--time1;

	if (time1 == 0)
		lba_time = time3;
}

int makeGiveUpMenu(void)
{
	int saveLangue;
	int temp;

	CopyScreen(frontVideoBuffer, workVideoBuffer);
	HQ_StopSample();

	do {
		saveLangue = languageCD1;
		languageCD1 = 0;
		InitDial(0);
		temp = processMenu(subMenu2Data);
		languageCD1 = saveLangue;
		InitDial(currentTextBank + 3);
	} while (temp != 27 && temp != 28);

	if (temp == 27)
		return (1);
	if (temp == 28)
		return (0);

	return (0);
}

void reinitVars(void)
{
	int i;

	for (i = 0; i < 10; i++)
		overlayObjectList[i].field_0 = -1;

	for (i = 0; i < 80; i++)
		cubeFlags[i] = 0;

	for (i = 0; i < 255; i++)
		vars[i] = 0;

	for (i = 0; i < 28; i++)
		itemUsed[i] = 0;

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

	numActorInRoom = 0;
	currentPositionInBodyPtrTab = 0;
	numOfZones = 0;

	numFlags = 0;
}

void reinitAll3(void) {
	resetActor(0);

	magicBallIdx = -1;
	numCloverBox = 2;
	numClover = 2;
	numCoin = 0;
	numKey = 0;
	magicPoint = 0;
	usingSword = 0;
	twinsen->body = 0;
	twinsen->life = 50;
	twinsen->talkColor = 4;
}

void DrawObj3D(short int arg_0, short int arg_4, short int arg_8, short int arg_C,
			   short int arg_10, short int arg_14, unsigned char *costumePtr) {
	int var_4;
	int temp1;
	int temp2;
	short int temp;

	var_4 = arg_8;

	temp1 = arg_C + arg_4;
	temp1 >>= 1;

	temp2 = arg_8 + arg_0;
	temp2 >>= 1;

	configureOrthoProjection(temp2, temp1);
	SetClip(arg_0, arg_4, var_4, arg_C);

	if (arg_14 == -1)
	{
		temp = getRealAngle(&timeVar);
		if (timeVar.numOfStep == 0)
			setActorAngleSafe(temp, temp - 256, 50, &timeVar);
		AffObjetIso(0, arg_10, 0, 0, temp, 0, costumePtr);
	} else
		AffObjetIso(0, arg_10, 0, 0, arg_14, 0, costumePtr);

}

void SetClip(int left, int top, int right, int bottom) {
	if (left < 0)
		left = 0;

	textWindowLeft = left;

	if (top < 0)
		top = 0;
	textWindowTop = top;

	if (right >= WINDOW_X)
		right = WINDOW_X - 1;
	textWindowRight = right;

	if (bottom >= WINDOW_Y)
		bottom = WINDOW_Y - 1;
	textWindowBottom = bottom;
}

void Box(int left, int top, int right, int bottom, unsigned char e) {

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

	offset = -((right - left) - WINDOW_X);

	ptr = frontVideoBuffer + screenLockupTable[top] + left;

	for (x = top; x < bottom; x++) {
		for (y = left; y < right; y++) {

			*(ptr++) = e;
		}
		ptr += offset;
	}

}

void DoDir(int actorNum) {
	actor *lactor;

	lactor = &actors[actorNum];

	if (lactor->costumeIndex == -1)
		return;

	/* If actor is falling, it can just rotate */
	if (lactor->dynamicFlagsBF.isFalling)
	{
		short int tempAngle = 0;

		if (lactor->comportement != 1)
			return;

		if (os_isPressed(KEY_CHAR_LEFT))
			tempAngle = 0x100;

		if (os_isPressed(KEY_CHAR_RIGHT))
			tempAngle = -0x100;

		ManualRealAngle(lactor->angle, lactor->angle + tempAngle, lactor->speed, &lactor->time);
	}
	else
	{
		/* If it's not a sprite */
		if (!lactor->staticFlagsBF.isSpriteActor)
			if (lactor->comportement != athletic)
				lactor->angle = getRealAngle(&lactor->time);

		switch (lactor->comportement)
		{
		case 0: // NO_MOVE
			break;
		case 1: // MOVE_MANUAL
			/* If it's twinsen */
			if (actorNum == 0)
			{
				action = 0;

				/* Use action (even if behavior is not normal) */
				if (os_isPressed(KEY_CHAR_ACTION_2)) {
					action = 1;
					break;
				}

				/* If action key is pressed */
				if (os_isPressed(KEY_CHAR_ACTION))
				{
					switch (comportementHero)
					{
					case normal:
						/* Just do the action */
						action = 1;
						break;
					case athletic:
						/* Change the animation to jump */
						InitAnim(ANIM_jump, 1, 0, actorNum);
						break;
					case agressive:
						/* If agressivity is set automatical */
						if (autoAgressivity)
						{
							twinsenMoved = 1;

							lactor->angle = getRealAngle(&lactor->time);

							/* TODO: figure what it is */
							if (lactor->anim == 0)
							{
								/* Choose a random knock */
								switch (rand() % 3)
								{
								case 0:
										InitAnim(ANIM_kick, 1, 0, actorNum);
										break;
								case 1:
										InitAnim(ANIM_rightPunch, 1, 0, actorNum);
										break;
								case 2:
										InitAnim(ANIM_leftPunch, 1, 0, actorNum);
										break;
								default:
										printf("Bad random in agressivity !\n");
										break;
								}
							}
						}
						else
						{
							if (os_isPressed(KEY_CHAR_ATTACK_RPUNCH))
								InitAnim(ANIM_rightPunch, 1, 0, actorNum);

							if (os_isPressed(KEY_CHAR_ATTACK_LPUNCH))
								InitAnim(ANIM_leftPunch, 1, 0, actorNum);

							if (os_isPressed(KEY_CHAR_ATTACK_KICK))
								InitAnim(ANIM_kick, 1, 0, actorNum);
						}
						break;
					case discrete:
						/* Change the animation to hide */
						InitAnim(ANIM_hide, 0, 255, 0);
						break;
					}
				}

				/* TODO: understand ALL this paragraph */
				/* If the weapon key is pressed and inventory is not open */
				if (os_isPressed(KEY_CHAR_USEWEAPON) && !vars[70])
				{
					/* If twinsen is using the saber and he has it */
					if (usingSword && vars[saber])
					{
						if (lactor->body != 2)
							InitBody(2, 0);

						InitAnim(ANIM_swordAttack, 1, 0, actorNum);

						twinsenMoved = 1;
						lactor->angle = getRealAngle(&lactor->time);
					}
					/* Else, if he has the magic ball */
					else if (vars[ball])
					{
						if (magicBallIdx == -1)
							InitAnim(ANIM_throwBall, 1, 0, actorNum);

						twinsenMoved = 1;
						lactor->angle = getRealAngle(&lactor->time);
					}
				}

			}

			/* If no action key is pressed, or no action is made */
			if ((!os_isPressed(KEY_CHAR_ACTION) && !os_isPressed(KEY_CHAR_ACTION_2)
						&& !os_isPressed(KEY_CHAR_USEWEAPON)) || action != 0)
			{
				short int tempAngle;

				/* If previous key is not pressed, forward is not pressed and twinsen moved, then stay static */
				if ((!os_isPressed(twinsenKey) && twinsenMoved && !os_isPressed(KEY_CHAR_FORWARD) && !os_isPressed(KEY_CHAR_BACKWARD))
						|| (!os_isPressed(KEY_CHAR_FORWARD) && !os_isPressed(KEY_CHAR_BACKWARD)
							&& !os_isPressed(KEY_CHAR_LEFT) && !os_isPressed(KEY_CHAR_RIGHT)))
					InitAnim(ANIM_static, 0, 255, actorNum);

				twinsenMoved = 0;
				twinsenWalked = 0;

				/* Walk forward */
				if (os_isPressed(KEY_CHAR_FORWARD))
				{
					/* TODO: figure out what it is */
					if (currentActorInZoneProcess == 0)
						InitAnim(ANIM_walk, 0, 255, actorNum);
					twinsenMoved = 1;
					twinsenWalked = 1;
					twinsenKey = KEY_CHAR_FORWARD;
				}

				/* Walk backward */
				if (os_isPressed(KEY_CHAR_BACKWARD))
				{
					InitAnim(ANIM_walkBackward, 0, 255, actorNum);
					twinsenMoved = 1;
					twinsenWalked = 1;
					twinsenKey = KEY_CHAR_BACKWARD;
				}

				/* Turn left (counterclockwise) */
				if (os_isPressed(KEY_CHAR_LEFT) && !os_isPressed(KEY_CHAR_RIGHT))
				{
					if (!twinsenWalked)
						InitAnim(ANIM_turnLeft, 0, 255, actorNum);
					else if (!lactor->dynamicFlagsBF.isRotationByAnim)
						lactor->angle =	getRealAngle(&lactor->time);
					tempAngle = 0x100;
					twinsenMoved = 1;
					twinsenKey = KEY_CHAR_LEFT;
				}

				/* Turn right (clockwise) */
				if (os_isPressed(KEY_CHAR_RIGHT) && !os_isPressed(KEY_CHAR_LEFT))
				{
					if (!twinsenWalked)
						InitAnim(ANIM_turnRight, 0, 255, actorNum);
					else if (!lactor->dynamicFlagsBF.isRotationByAnim)
							lactor->angle = getRealAngle(&lactor->time);
					tempAngle = -0x100;
					twinsenMoved = 1;
					twinsenKey = KEY_CHAR_RIGHT;
				}

				ManualRealAngle(lactor->angle, lactor->angle + tempAngle,
								lactor->speed, &lactor->time);
			}
			break;
		/* TODO: understand the other movements */
		case 2: { //MOVE_FOLLOW
				int tempAngle;

				assert(lactor->followedActor != -1);

				tempAngle = GetAngle(lactor->X, lactor->Z, actors[lactor->followedActor].X, actors[lactor->followedActor].Z);

				if (lactor->staticFlagsBF.isSpriteActor)
					lactor->angle = tempAngle;
				else
					ManualRealAngle(lactor->angle, tempAngle, lactor->speed, &lactor->time);
				break;
			}

		case 3: //MOVE_TRACK
			if (lactor->positionInMoveScript == -1)
				lactor->positionInMoveScript = 0;
			break;
		case 4: // MOVE_FOLLOW_2
			break;
		case 5: // MOVE_TRACK_ATTACK
			break;
		case 6: // MOVE_SAME_XZ
			lactor->X = actors[lactor->followedActor].X;
			lactor->Z = actors[lactor->followedActor].Z;
			break;
		case 7: // MOVE_RANDOM
			printf("-- Moving randomly\n");
			if (!lactor->dynamicFlagsBF.isRotationByAnim)
			{
				if (lactor->field_3 & 0x80)
				{
					ManualRealAngle(lactor->angle, (((rand() & 0x100) + (lactor->angle - 0x100)) & 0x3FF), lactor ->speed, &lactor->time);

					lactor->cropLeft = rand() % 300 + lba_time + 300;

					InitAnim(0, 0, 0xFF, actorNum);
				}

				if (lactor->time.numOfStep == 0) {
					InitAnim(1, 0, 0xFF, actorNum);

					if (lba_time > lactor->cropLeft) {
						ManualRealAngle(lactor->angle, (((rand() & 0x100) + (lactor->angle - 0x100)) & 0x3FF) , lactor->speed, &lactor->time);

						lactor->cropLeft = rand() % 300 + lba_time + 300;
					}
				}
			}
			break;
		default:
			printf("Unhandled comportement %d in update actors\n", lactor->comportement);
			exit(1);
		}
	}
}

void DoAnim(int actorNum) {
	actor *lactor;
	char *animPtr;
	short int var_10;

	short int var_C;
	short int var_4;
	short int var_8;

	lactor = &actors[actorNum];

	currentlyProcessedActorNum = actorNum;
	processActorVar1 = lactor;

	if (lactor->costumeIndex == -1)
		return;

	processActorVar2 = lactor->field_20;
	processActorVar3 = lactor->field_22;
	processActorVar4 = lactor->field_24;

	if (lactor->staticFlagsBF.isSpriteActor) { // is sprite actor
		if (lactor->field_66) {
			lactor->dynamicFlagsBF.bUnk0002 = 1;
		}

		processActorX = lactor->X;
		processActorZ = lactor->Y;
		processActorY = lactor->Z;

		if (!(lactor->dynamicFlagsBF.isFalling)) { // actor freeze movement
			if (lactor->speed) {
				int dx;

				dx = getRealValue(&lactor->time);

				if (!dx) {
					if (lactor->time.to > 0) {
						dx = 1;
					} else {
						dx = -1;
					}
				}

				Rotate(dx, 0, lactor->field_78);

				processActorZ = lactor->Y - destZ;

				Rotate(0, destX, lactor->angle);

				processActorX = lactor->X + destX;
				processActorY = lactor->Z + destZ;

				setActorAngle(0, lactor->speed, 50, &lactor->time);

				if (lactor->dynamicFlagsBF.isMoving) {
					if (lactor->doorStatus) { // is oppening
						var_10 = lactor->doorStatus;
						if (distance2d(processActorX, processActorY, lactor->lastX, lactor->lastY) >= var_10) {
							if (lactor->angle == 0) {
								processActorY = lactor->lastY + lactor->doorStatus;
							} else if (lactor->angle == 0x100) {
								processActorX = lactor->lastX + lactor->doorStatus;
							} else if (lactor->angle == 0x200) {
								processActorY = lactor->lastY - lactor->doorStatus;
							} else if (lactor->angle == 0x300) {
								processActorX = lactor->lastX - lactor->doorStatus;
							}

							lactor->dynamicFlagsBF.isMoving = 0;
							lactor->speed = 0;
						}
					} else { // is closing
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

							lactor->dynamicFlagsBF.isMoving = 0;
							lactor->speed = 0;
						}
					}
				}
			}

			if (lactor->staticFlagsBF.isPushable) {
				processActorX += lactor->lastX;
				processActorZ += lactor->lastZ;
				processActorY += lactor->lastY;

				if (lactor->staticFlagsBF.isUsingMiniZv) {
					processActorX = ((processActorX / 128) * 128);
					processActorY = ((processActorY / 128) * 128);
				}

				lactor->lastX = 0;
				lactor->lastZ = 0;
				lactor->lastY = 0;

			}
		}

	} else {   // not sprite actor
		if (lactor->previousAnimIndex != -1) {
			int keyFramePassed;
			animPtr = (char *) HQR_Get(HQR_Anims, lactor->previousAnimIndex);

			keyFramePassed = SetInterDepObjet(lactor->animPosition, animPtr, (char*)bodyPtrTab[lactor->costumeIndex], &lactor->animTimerData);

			if (processActorVar5)
				lactor->dynamicFlagsBF.isRotationByAnim = 1;
			else
				lactor->dynamicFlagsBF.isRotationByAnim = 0;

			lactor->angle = (lactor->angle + processActorVar6 - lactor->lastRotationSpeed) & 0x3FF;
			lactor->lastRotationSpeed = processActorVar6;

			Rotate(currentX, currentY, lactor->angle);

			currentX = destX; // dest
			currentY = destZ;

			processActorX = lactor->X + currentX - lactor->lastX;
			processActorZ = lactor->Y + currentZ - lactor->lastZ;
			processActorY = lactor->Z + currentY - lactor->lastY;

			lactor->lastX = currentX;
			lactor->lastZ = currentZ;
			lactor->lastY = currentY;

			lactor->dynamicFlagsBF.bUnk0004 = 0;
			lactor->dynamicFlagsBF.bUnk0008 = 0;

			if (keyFramePassed) { // if keyFrame
				lactor->animPosition++;
				if (lactor->animExtraData) { // if actor play sound attached to animation
					GereAnimAction(lactor, actorNum);
				}

				var_10 = lactor->animPosition;
				if (var_10 == GetNbFramesAnim(animPtr)) { // end of animation ?
					lactor->dynamicFlagsBF.bUnk0002 = 0;

					if (lactor->field_78 == 0) {
						lactor->animPosition = GetBouclageAnim(animPtr);
					} else {
						var_C = actorNum;
						lactor->anim = lactor->field_2;
						lactor->previousAnimIndex = getAnimIndexForBody(lactor->anim, actorNum);

						if (lactor->previousAnimIndex == -1) {
							lactor->previousAnimIndex = getAnimIndexForBody(0, var_C);
							lactor->anim = 0;
						}

						lactor->animExtraData = loadTwinsenCostumesVar1;

						lactor->field_78 = 0;
						lactor->animPosition = 0;
						lactor->field_66 = 0;
					}

					if (lactor->animExtraData) {
						GereAnimAction(lactor, actorNum);
					}

					lactor->dynamicFlagsBF.bUnk0004 = 1;
				}

				lactor->lastRotationSpeed = 0;

				lactor->lastX = 0;
				lactor->lastZ = 0;
				lactor->lastY = 0;
			}
		}
	}

	if (lactor->standOn != -1) { // standing on object
		processActorX -= actors[lactor->standOn].field_20;  // apply under object movement
		processActorZ -= actors[lactor->standOn].field_22;
		processActorY -= actors[lactor->standOn].field_24;

		processActorX += actors[lactor->standOn].X;
		processActorZ += actors[lactor->standOn].Y;
		processActorY += actors[lactor->standOn].Z;

		if (!CheckZvOnZv(actorNum, lactor->standOn))  // is actor still standing on another actor ?
			lactor->standOn = -1; // actor fall from the object
	}

	if (lactor->dynamicFlagsBF.isFalling) { // if falling, then no modification...
		processActorX = processActorVar2;
		processActorZ = processActorVar3 + mainLoopVar17; // apply fall speed
		processActorY = processActorVar4;
	}

	if (lactor->staticFlagsBF.computeCollisionWithBricks) { // if wall collision is enabled
		int position;

		getPosVar2 = 0;
		position = WorldColBrick(processActorVar2, processActorVar3, processActorVar4);

		if (position) { // process slope
			if (position == 1) { // if this occure, that means the actor is already in the wall... Not good!
				printf("Warping !!!\n");
				lactor->Y = processActorZ = processActorZ / 256 * 256 + 256;  // warp upper...
			} else {
				ReajustPos(position); // handle slope
			}
		}

		if (lactor->staticFlagsBF.computeCollisionWithObj) // if we check collision with other objects
			CheckObjCol(actorNum);  //check collision and see if actor fall on an object

		if ((lactor->standOn != -1) && (lactor->dynamicFlagsBF.isFalling)) // if actor felt on another an object
			ReceptionObj(); // stop falling

		fieldCauseDamage = 0;

		processActorVar11 = processActorX;
		processActorVar12 = processActorZ;
		processActorVar13 = processActorY;

		if (!actorNum && !(lactor->staticFlagsBF.isDead)) { // check for wall collision
			DoCornerReadjustTwinkel(lactor->boudingBox.X.bottomLeft, lactor->boudingBox.Y.bottomLeft, lactor->boudingBox.Z.bottomLeft, 1);  // twinsen wall collision code
			DoCornerReadjustTwinkel(lactor->boudingBox.X.topRight, lactor->boudingBox.Y.bottomLeft, lactor->boudingBox.Z.bottomLeft, 2);
			DoCornerReadjustTwinkel(lactor->boudingBox.X.topRight, lactor->boudingBox.Y.bottomLeft, lactor->boudingBox.Z.topRight, 4);
			DoCornerReadjustTwinkel(lactor->boudingBox.X.bottomLeft, lactor->boudingBox.Y.bottomLeft, lactor->boudingBox.Z.topRight, 8);
		} else {
			DoCornerReajust(lactor->boudingBox.X.bottomLeft, lactor->boudingBox.Y.bottomLeft, lactor->boudingBox.Z.bottomLeft, 1);  // other objects wall collision code
			DoCornerReajust(lactor->boudingBox.X.topRight, lactor->boudingBox.Y.bottomLeft, lactor->boudingBox.Z.bottomLeft, 2);
			DoCornerReajust(lactor->boudingBox.X.topRight, lactor->boudingBox.Y.bottomLeft, lactor->boudingBox.Z.topRight, 4);
			DoCornerReajust(lactor->boudingBox.X.bottomLeft, lactor->boudingBox.Y.bottomLeft, lactor->boudingBox.Z.topRight, 8);
		}

		if (fieldCauseDamage && !(lactor->dynamicFlagsBF.isFalling) && !currentlyProcessedActorNum && (comportementHero == 1) && (lactor->anim == 1)) { // wall hit while running
			Rotate(lactor->boudingBox.X.bottomLeft, lactor->boudingBox.Z.bottomLeft, lactor->angle + 0x580);

			destX += processActorX;
			destZ += processActorY;

			if (destX >= 0 && destZ >= 0 && destX <= 0x7E00 && destZ <= 0x7E00) {
				if (WorldColBrick(destX, processActorZ + 0x100, destY)) {
					InitSpecial(lactor->X, lactor->Y + 1000, lactor->Z, 0);
					InitAnim(ANIM_hitBig, 2, 0, currentlyProcessedActorNum);

					printf("Wall hit 2\n");

					if (currentlyProcessedActorNum == 0) {
						twinsenMoved = 1; // twinsenHit
					}

					lactor->life--; // damage caused by field
				}
			}
		}

		position = WorldColBrick(processActorX, processActorZ, processActorY);
		var_4 = position;
		lactor->field_3 = position;

		if (position) { // if standing on floor
			if (position == 1) { // if next step is in wall...
				if (lactor->dynamicFlagsBF.isFalling) { // if was falling
					ReceptionObj();
					processActorZ = (getPosVar2 << 8) + 0x100;
				} else {
					if (!actorNum && comportementHero == 1 && lactor->anim == var_4) {
						InitSpecial(lactor->X, lactor->Y + 1000, lactor->Z, 0);
						InitAnim(ANIM_hitBig, 2, 0, currentlyProcessedActorNum);

						printf("Wall hit !\n");

						if (!actorNum) {
							twinsenMoved = 1;
						}

						lactor->life--;
					}

					if (!WorldColBrick(processActorX, processActorZ, processActorVar4)) { // no Y problem
						processActorY = processActorVar4;
					}

					if (!WorldColBrick(processActorVar2, processActorZ, processActorY)) { // no X problem
						processActorX = processActorVar2; // no X Collision
					}

					if (WorldColBrick(processActorX, processActorZ, processActorVar4) && WorldColBrick(processActorVar2, processActorZ, processActorY)) {
						return; // both X/Y problem -> can't move !
					}
				}
			} else { // not standing on flat floor
				if (lactor->dynamicFlagsBF.isFalling)
					ReceptionObj();

				ReajustPos(var_4);
			}

			lactor->dynamicFlagsBF.isFalling = 0;
		} else { // not standing on floor
			if (lactor->staticFlagsBF.isFallable && lactor->standOn == -1) { // if fallable and actor not standing on another actor
				var_8 = WorldColBrick(processActorX, processActorZ - 1, processActorY); // what is 1 step under ?

				if (var_8) { // under is the floor
					if (lactor->dynamicFlagsBF.isFalling) { // if was falling...
						ReceptionObj();
					}

					ReajustPos(var_8);
				} else { // start falling
					if (!lactor->dynamicFlagsBF.isRotationByAnim == 1) {
						lactor->dynamicFlagsBF.isFalling = 1;

						if ((!actorNum) && twinsenZBeforeFall == 0) {
							twinsenZBeforeFall = processActorZ;
						}

						InitAnim(ANIM_fall, 0, 255, actorNum);

					}
				}
			}
		}

		if (getPosVar2 == -1) { // under the map...
			lactor->life = 0; // die...
		}
	} else {   // no wall collision
		if (lactor->staticFlagsBF.computeCollisionWithObj) //if actor collision
			CheckObjCol(actorNum);
	}

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
	lactor->Y = processActorZ;
	lactor->Z = processActorY;
}

void DoCornerReadjustTwinkel(int X, int Y, int Z, int mask) { // twinsen wall colision
	int pos;

	pos = WorldColBrick(processActorX, processActorZ, processActorY);

	processActorX += X;
	processActorZ += Y;
	processActorY += Z;

	if (processActorX >= 0 && processActorY >= 0 && processActorX <= 0x7E00 && processActorY <= 0x7E00) {
		ReajustPos(pos); //handle slope
		pos = WorldColBrickFull(processActorX, processActorZ, processActorY, processActorVar1->boudingBox.Y.topRight);

		if (pos == 1) { // next position is a wall
			fieldCauseDamage |= mask;
			pos = WorldColBrickFull(processActorX, processActorZ, processActorVar4 + Z, processActorVar1->boudingBox.Y.topRight);
			if (pos == 1) {
				pos = WorldColBrickFull(X + processActorVar2, processActorZ, processActorY, processActorVar1->boudingBox.Y.topRight);

				if (pos != 1) {
					processActorVar11 = processActorVar2;
				}
			} else {
				processActorVar13 = processActorVar4;
			}
		}
	}

	processActorX = processActorVar11;
	processActorZ = processActorVar12;
	processActorY = processActorVar13;
}

void DoCornerReajust(int var0, int var1, int var2, int var3) {
	int pos;

	pos = WorldColBrick(processActorX, processActorZ, processActorY);

	processActorX += var0;
	processActorZ += var1;
	processActorY += var2;

	if (processActorX >= 0 && processActorY >= 0 && processActorX <= 0x7E00 && processActorY <= 0x7E00) {
		ReajustPos(pos);
		pos = WorldColBrick(processActorX, processActorZ, processActorY);

		if (pos == 1) { // next position is a wall
			fieldCauseDamage |= var3;
			pos = WorldColBrick(processActorX, processActorZ, processActorVar4 + var2);
			if (pos == 1) {
				pos = WorldColBrick(var0 + processActorVar2, processActorZ, processActorY);

				if (pos != 1) {
					processActorVar11 = processActorVar2;
				}
			} else {
				processActorVar13 = processActorVar4;
			}
		}
	}

	processActorX = processActorVar11;
	processActorZ = processActorVar12;
	processActorY = processActorVar13;
}

void ReajustPos(int param) {
	int localGetPosVar1;
	int localGetPosVar2;
	int localGetPosVar3;

	if (!param)
		return;

	localGetPosVar1 = (getPosVar1 << 9) - 0x100;
	localGetPosVar2 = (getPosVar2 << 8);
	localGetPosVar3 = (getPosVar3 << 9) - 0x100;

	//****************** special collisions ******************//
	if (param >= 6 && param <= 13) {
		switch (param) {
		case 6: {
				if ((processActorY - getPosVar3) <= (processActorX - getPosVar1)) {
					param = 2;
				} else {
					param = 3;
				}
				break;
			}
		case 7: {
				if ((processActorY - getPosVar3) <= (processActorX - getPosVar1)) {
					param = 4;
				} else {
					param = 5;
				}
				break;
			}
		case 8: {
				if ((512 - (processActorX - getPosVar1)) <= (processActorY - getPosVar3)) {
					param = 2;
				} else {
					param = 4;
				}
				break;
			}
		case 9: {
				if ((512 - (processActorX - getPosVar1)) <= (processActorY - getPosVar3)) {
					param = 3;
				} else {
					param = 5;
				}
				break;
			}
		case 10: {
				if ((processActorX - getPosVar1) >= (processActorY - getPosVar3)) {
					param = 3;
				} else {
					param = 2;
				}
				break;
			}
		case 11: {
				if ((processActorY - getPosVar3) <= (processActorX - getPosVar1)) {
					param = 5;
				} else {
					param = 4;
				}
				break;
			}
		case 12: {
				if ((512 - processActorX - getPosVar1) <= (processActorY - getPosVar3)) {
					param = 4;
				} else {
					param = 2;
				}
				break;
			}
		case 13: {
				if ((512 - processActorX - getPosVar1) <= (processActorY - getPosVar3)) {
					param = 5;
				} else {
					param = 3;
				}
				break;
			}
		default: {
				printf("collision %d\n", param);
				//      exit(1);
			}
		}
	}

	if (param >= 2 && param <= 5) {
		switch (param) {
			//****************** slope collisions *****************//
		case 2: {
				processActorZ =
					localGetPosVar2 + BoundRegleTrois(0, 0x100, 0x200,
													  processActorX - localGetPosVar1);
				break;
			}
		case 3: {
				processActorZ =
					localGetPosVar2 + BoundRegleTrois(0, 0x100, 0x200,
													  processActorY - localGetPosVar3);
				break;
			}
		case 4: {
				processActorZ =
					localGetPosVar2 + BoundRegleTrois(0x100, 0, 0x200,
													  processActorY - localGetPosVar3);
				break;
			}
		case 5: {
				processActorZ =
					localGetPosVar2 + BoundRegleTrois(0x100, 0, 0x200,
													  processActorX - localGetPosVar1);
				return;
			}
		}
	}

}

// fait la moyenne pour la hauteur.
// min max ? var
int BoundRegleTrois(int var0, int var1, int var2, int var3) {
	if (var3 <= 0)
		return (var0);

	if (var3 >= var2)
		return (var1);

	return ((((var1 - var0) * var3) / var2) + var0);
}

int CheckObjCol(int actorNum) {
	int X1;
	int X2;
	int Z1;
	int Z2;
	int Y1;
	int Y2;
	int currentlyTestedActor = 0;
	int var_60;

	actor* lactor;
	lactor = &actors[actorNum];

	X1 = processActorX + lactor->boudingBox.X.bottomLeft;
	X2 = processActorX + lactor->boudingBox.X.topRight;

	Z1 = processActorZ + lactor->boudingBox.Y.bottomLeft;
	Z2 = processActorZ + lactor->boudingBox.Y.topRight;

	Y1 = processActorY + lactor->boudingBox.Z.bottomLeft;
	Y2 = processActorY + lactor->boudingBox.Z.topRight;

	lactor->collision = -1;

	while (currentlyTestedActor < numActorInRoom) {
		if (currentlyTestedActor != actorNum && actors[currentlyTestedActor].costumeIndex != -1 && !(lactor->staticFlagsBF.isDead) && actors[currentlyTestedActor].standOn != actorNum) { // is actor valid (not self and defined)
			actor* lactor2;

			int X1_2;
			int X2_2;
			int Z1_2;
			int Z2_2;
			int Y1_2;
			int Y2_2;

			lactor2 = &actors[currentlyTestedActor];

			X1_2 = lactor2->X + lactor2->boudingBox.X.bottomLeft;
			X2_2 = lactor2->X + lactor2->boudingBox.X.topRight;

			Z1_2 = lactor2->Y + lactor2->boudingBox.Y.bottomLeft;
			Z2_2 = lactor2->Y + lactor2->boudingBox.Y.topRight;

			Y1_2 = lactor2->Z + lactor2->boudingBox.Z.bottomLeft;
			Y2_2 = lactor2->Z + lactor2->boudingBox.Z.topRight;

			if (X1<X2_2 && X2>X1_2 && Z1<Z2_2 && Z2>Z1_2 && Y1<Y2_2 && Y2>Y1_2) {

				lactor->collision = currentlyTestedActor;

				if (lactor2->staticFlagsBF.isCarrier) { // if carrier
					if (lactor->dynamicFlagsBF.isFalling) { // if can stand on object
						processActorZ = Z2_2 - lactor->boudingBox.Y.bottomLeft + 1; // new Z

						lactor->standOn = currentlyTestedActor;

						printf("Actor %d fall on %d\n", actorNum, currentlyTestedActor);
					} else {
						if (CheckZvOnZv(actorNum, currentlyTestedActor)) {
							processActorZ = Z2_2 - lactor->boudingBox.Y.bottomLeft + 1; // new Z

							lactor->standOn = currentlyTestedActor;
						} else {
							goto lab12AC5;
						}
					}
				} else {
					int newAngle;
					var_60 = actorNum;
					if (CheckZvOnZv(actorNum, currentlyTestedActor)) {
						HitObj(var_60, currentlyTestedActor, 1, -1);
					}

lab12AC5:
					newAngle = GetAngle(processActorX, processActorY, lactor2->X, lactor2->Z);

					if (lactor2->staticFlagsBF.isPushable && !(lactor->staticFlagsBF.isPushable)) { // should be pushed ?
						lactor2->lastZ = 0;

						if (lactor2->staticFlagsBF.isUsingMiniZv) {
							if (newAngle >= 0x80 && newAngle < 0x180 && lactor->angle > 0x80 && lactor->angle < 0x180)
								lactor2->lastX = 192;

							if (newAngle >= 0x180 && newAngle < 0x280 && lactor->angle > 0x180 && lactor->angle < 0x280)
								lactor2->lastY = -64;

							if (newAngle >= 0x280 && newAngle < 0x380 && lactor->angle > 0x280 && lactor->angle < 0x380)
								lactor2->lastX = -64;

							if ((newAngle >= 0x380 || newAngle < 0x80) && (lactor->angle >= 0x380 || lactor->angle < 0x80))
								lactor2->lastY = 192;

						} else {
							lactor2->lastX = processActorX - lactor->field_20;
							lactor2->lastY = processActorY - lactor->field_24;
						}
					}

					if (((lactor2->boudingBox.X.topRight - lactor2->boudingBox.X.bottomLeft) == (lactor2->boudingBox.Z.topRight - lactor2->boudingBox.Z.bottomLeft)) && ((lactor->boudingBox.X.topRight - lactor->boudingBox.X.bottomLeft) == (lactor->boudingBox.Z.topRight - lactor->boudingBox.Z.bottomLeft))) {
						if (newAngle < 0x180)
							processActorX = X1_2 - lactor->boudingBox.X.topRight;

						if (newAngle >= 0x180 && newAngle < 0x280)
							processActorY = Y2_2 - lactor->boudingBox.Z.bottomLeft;

						if (newAngle >= 0x280 && newAngle < 0x380)
							processActorX = X2_2 - lactor->boudingBox.X.bottomLeft;

						if (newAngle >= 0x380 || (newAngle < 0x380 && newAngle < 0x80))
							processActorY = Y1_2 - lactor->boudingBox.Z.topRight;
					} else {
						if (!(lactor->dynamicFlagsBF.isFalling)) {
							processActorX = processActorVar2;
							processActorZ = processActorVar3;
							processActorY = processActorVar4;
						}
					}
				}
			}
		}

		currentlyTestedActor++;
	}

	if (lactor->dynamicFlagsBF.bUnk0002) { // lactor is attacking
		int i;

		Rotate(0, 200, lactor->angle);

		X1 = destX + processActorX + lactor->boudingBox.X.bottomLeft;
		X2 = destX + processActorX + lactor->boudingBox.X.topRight;

		Z1 = processActorZ + lactor->boudingBox.Y.bottomLeft;
		Z2 = processActorZ + lactor->boudingBox.Y.topRight;

		Y1 = destZ + processActorY + lactor->boudingBox.Z.bottomLeft;
		Y2 = destZ + processActorY + lactor->boudingBox.Z.topRight;

		for (i = 0;i < numActorInRoom;i++) {
			if (i != actorNum && actors[i].costumeIndex != -1 && !(actors[i].staticFlagsBF.noDisplay) && actors[i].standOn != actorNum) { // is actor valid (not self and defined)
				actor* lactor2;

				int X1_2;
				int X2_2;
				int Z1_2;
				int Z2_2;
				int Y1_2;
				int Y2_2;

				lactor2 = &actors[i];

				X1_2 = lactor2->X + lactor2->boudingBox.X.bottomLeft;
				X2_2 = lactor2->X + lactor2->boudingBox.X.topRight;

				Z1_2 = lactor2->Y + lactor2->boudingBox.Y.bottomLeft;
				Z2_2 = lactor2->Y + lactor2->boudingBox.Y.topRight;

				Y1_2 = lactor2->Z + lactor2->boudingBox.Z.bottomLeft;
				Y2_2 = lactor2->Z + lactor2->boudingBox.Z.topRight;

				if (X1<X2_2 && X2>X1_2 && Z1<Z2_2 && Z2>Z1_2 && Y1<Y2_2 && Y2>Y1_2) {
					HitObj(actorNum, i, lactor->field_66, lactor->angle + 0x200);
					lactor->dynamicFlagsBF.bUnk0002 = 0;
				}
			}
		}
	}

	return(lactor->collision);

}

void ReceptionObj(void) { // stop falling
	int fall;

	if (currentlyProcessedActorNum == 0) {
		fall = twinsenZBeforeFall - processActorZ;

		if (fall >= 0x1000) {
			InitSpecial(processActorVar1->X, processActorVar1->Y + 1000, processActorVar1->Z, 0);
			processActorVar1->life = 0;
			InitAnim(ANIM_landHit, 2, 0, currentlyProcessedActorNum);
		} else if (fall >= 0x800) {
			InitSpecial(processActorVar1->X, processActorVar1->Y + 1000, processActorVar1->Z, 0);
			processActorVar1->life--;
			InitAnim(ANIM_landHit, 2, 0, currentlyProcessedActorNum);
		} else if (fall > 10) {
			InitAnim(ANIM_land, 2, 0, currentlyProcessedActorNum);
		} else {
			InitAnim(ANIM_static, 0, 0, currentlyProcessedActorNum);
		}

		twinsenZBeforeFall = 0;
	} else {
		InitAnim(ANIM_land, 2, processActorVar1->field_2, currentlyProcessedActorNum);
	}

	processActorVar1->dynamicFlagsBF.isFalling = 0;
}

int WorldColBrickFull(int var0, int var1, int var2, int var3) {
	unsigned char *ptr;
	char *ptr2;
	unsigned char temp0;
	unsigned char temp3;

	ptr = (unsigned char *) bufCube;

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

	ptr += (getPosVar3 << 7) * 25;

	temp0 = *ptr;

	if (temp0) {
		int i;
		int j;
		char al;
		int edx;

		ptr2 = (char *) currentBll;

		ptr2 += READ_LE_U32(ptr2 + temp0 * 4 - 4);
		ptr2 += 3;

		temp3 = *(ptr + 1);

		ptr2 = ptr2 + temp3 * 4;

		al = *ptr2;

		edx = ((var3 + 255) >> 8);

		j = getPosVar2;

		for (i = 0;i < edx;i++) { // search from bottom to top
			if (j > 24) { // check if we are not out of the cube
				return(al);
			}

			ptr += 2;
			j++;

			if (READ_LE_S16(ptr) != 0) {
				return(1);
			}
		}

		return(al);
	} else {
		int i;
		int j;
		char al;
		int edx;

		al = *(ptr + 1);
		edx = ((var3 + 255) >> 8);

		j = getPosVar2;

		for (i = 0;i < edx;i++) {
			if (j > 24) {
				return(al);
			}

			ptr += 2;
			j++;

			if (READ_LE_S16(ptr) != 0) {
				return(1);
			}
		}
	}

	return(0);
}

int WorldColBrick(int var0, int var1, int var2) {
	char *ptr;
	char *ptr2;
	unsigned char temp0;
	unsigned char temp3;

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

	ptr += (getPosVar3 << 7) * 25;

	temp0 = *(unsigned char *) ptr;

	if (temp0) {
		ptr2 = (char *) currentBll;

		ptr2 += READ_LE_U32(ptr2 + temp0 * 4 - 4);
		ptr2 += 3;

		temp3 = *(ptr + 1);

		ptr2 = ptr2 + temp3 * 4;

		return (*ptr2);
	} else {
		return (*(ptr + 1));
	}
}

void Rotate(int initialX, int initialY, int angle) {
	int angle1;
	int angle2;

	if (!angle) {
		destX = initialX;
		destZ = initialY;
	} else {
		angle1 = tab1[angle & 0x3FF];
		angle2 = tab1[(angle + 0x100) & 0x3FF];

		destX = (initialX * angle2 + initialY * angle1) >> 14;
		destZ = (initialY * angle2 - initialX * angle1) >> 14;
	}
}

int CheckZvOnZv(int var0, int var1) { // is actor still standing on object ?
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

	bx = processActorX + lactor1->boudingBox.X.bottomLeft;
	si = processActorX + lactor1->boudingBox.X.topRight;

	di = processActorZ + lactor1->boudingBox.Y.bottomLeft;
	var_1C = processActorZ + lactor1->boudingBox.Y.topRight;

	var_4 = processActorY + lactor1->boudingBox.Z.bottomLeft;
	var_20 = processActorY + lactor1->boudingBox.Z.topRight;

	// ------------- actor2

	var_18 = lactor2->X + lactor2->boudingBox.X.bottomLeft;
	cx = lactor2->X + lactor2->boudingBox.X.topRight;

	var_8 = lactor2->Y + lactor2->boudingBox.Y.bottomLeft;
	var_10 = lactor2->Y + lactor2->boudingBox.Y.topRight;

	var_C = lactor2->Z + lactor2->boudingBox.Z.bottomLeft;
	var_14 = lactor2->Z + lactor2->boudingBox.Z.topRight;

	if (bx >= cx)   // X1
		return (0);

	if (si <= var_18)   // X2
		return (0);

	if (di > (var_10 + 1))  // Z1a
		return (0);

	if (di <= (var_10 - 0x100)) // Z1b
		return (0);

	if (var_1C <= var_8)  // Z2
		return (0);

	if (var_4 >= var_14)  // Y1
		return (0);

	if (var_20 <= var_C)  // Y2
		return (0);

	return (1);
}

void HitObj(int actorAttacking, int actorAttacked, int param, int angle) {
//    actor* pActorAttacking = &actors[actorAttacking];
	actor* pActorAttacked = &actors[actorAttacked];

	if (pActorAttacked->life <= 0)
		return;

	pActorAttacked->hitBy = actorAttacking;

	if (pActorAttacked->field_14 <= param) { //armure ?
		if (pActorAttacked->anim == ANIM_hitBig || pActorAttacked->anim == ANIM_hit2) {
			int temp;
			temp = pActorAttacked->animPosition;
			pActorAttacked->animPosition = 1;

			if (pActorAttacked->animExtraData) {
				GereAnimAction(pActorAttacked, actorAttacked);
			}

			pActorAttacked->animPosition = temp;
		} else {
			if (angle != -1) {
				setActorAngleSafe(angle, angle, 0, &pActorAttacked->time); // force angle without transition
			}

			if (rand() & 1) {
				InitAnim(ANIM_hit2, 3, 255, actorAttacked);
			} else {
				InitAnim(ANIM_hitBig, 3, 255, actorAttacked);
			}
		}

		InitSpecial(pActorAttacked->X, pActorAttacked->Y + 1000, pActorAttacked->Z, 0);

		if (!actorAttacked) { // if twisen is attacked
			twinsenMoved = 1;
		}

		pActorAttacked->life -= param;

		if (pActorAttacked->life < 0)
			pActorAttacked->life = 0;
	} else {
		InitAnim(ANIM_hit, 3, 255, actorAttacked);
	}
}
