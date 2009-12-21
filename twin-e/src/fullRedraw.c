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

#include "images.h"
#include "renderer.h"
#include "mainMenu.h"
#include "text.h"
#include "mainLoop.h"
#include "shadow.h"
#include "anim.h"
#include "extra.h"
#include "font.h"
#include "main.h"
#include "room.h"
#include "hqr.h"
#include "samples.h"
#include "actors.h"
#include "cube.h"
#include "body.h"
#include "script.h"

#include "fullRedraw.h"

extern unsigned char* brickTable[];
extern unsigned char* brickMaskTable[];

int zbufferVar1;
int zbufferVar2;

zbufferDataStruct zbufferData[28][150];
short int zbufferTab[28];

currentDirtyBoxListStruct currentDirtyBoxList[300];
currentDirtyBoxListStruct nextDirtyBoxList[300];
drawListStruct drawList[150];

unsigned char outBuffer[512000];

int cropLeft;

short int overlay3dObect = 0;

short int fullRedrawVar1;
short int fullRedrawVar2;
short int fullRedrawVar8;

#ifdef USE_FLOAT
float projectedPositionX;
float projectedPositionY;
float projectedPositionZ;
#else
short int projectedPositionX;
short int projectedPositionY;
short int projectedPositionZ;
#endif

int numOfRedrawBox;

int cameraX;
int cameraZ;
int cameraY;

void fullRedraw(int param) {
	short int temp1;
	short int temp2;

	int counter;
	int counter2;

	int i;

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

	temp1 = fullRedrawVar1;
	temp2 = fullRedrawVar2;

	UnSetClip();

	if (!param) {
		blitBackgroundOnDirtyBoxes(); // blit background on dirty boxes
	} else {   // fully redraw the background
		freezeTime();
		Cls();
		redrawCube(); // draw cube background to frontVideoBuffer
		updateOverlayObjectsPosition(temp1, temp2, fullRedrawVar1, fullRedrawVar2);
		CopyScreen(frontVideoBuffer, workVideoBuffer);
	}

	arg_1A = 0;
	a12 = 0;
	arg_46 = arg_1A + 0x1000;
	arg_42 = arg_1A + 0x0C00;

	// loop1

	for (arg_1A = 0; arg_1A < numActorInRoom; arg_1A++, arg_46++, arg_42++) { // process actors (and draw shadow if applicable)
		lactor = &actors[arg_1A];
		lactor->dynamicFlagsBF.wasDrawn = 0;  // recheck -> reinit the draw flags for the current objects

		if ((useAnotherGrm == -1) || (lactor->Y <= READ_LE_S16((currentGrid2) * 24 + (char*)zoneData + 8))) { // eject characters hidden when using another GRM

			if (lactor->staticFlagsBF.isBackgrounded && param == 0) {  // background actor, no redraw required
				// calculate the actor position on screen
				projectPositionOnScreen(lactor->X - cameraX, lactor->Y - cameraZ, lactor->Z - cameraY);

				// is the actor in the viewable screen ?
				if (projectedPositionX > -50 && projectedPositionX < 680 && projectedPositionY > -30 && projectedPositionY < 580) {
					lactor->dynamicFlagsBF.wasDrawn = 1; // flag ?
				}
			} else
			{
				if (lactor->costumeIndex != -1  && !(lactor->staticFlagsBF.noDisplay)) { // 0x200 -> visible
					// calculate the actor position on screen
					projectPositionOnScreen(lactor->X - cameraX, lactor->Y - cameraZ, lactor->Z - cameraY);

					if (((lactor->staticFlagsBF.isUsingClipping) && projectedPositionX > -112 && projectedPositionX < 752 && projectedPositionY > -50 && projectedPositionY < 651)
							|| ((!(lactor->staticFlagsBF.isUsingClipping)) && projectedPositionX > -50 && projectedPositionX < 680 && projectedPositionY > -30 && projectedPositionY < 580)) {
						temp3 = lactor->Z + lactor->X - cameraX - cameraY;

						if (lactor->standOn != -1) { // if actor is on another actor
							temp3 = actors[lactor->standOn].X - cameraX + actors[lactor->standOn].Z - cameraY + 2;
						}

						if (lactor->staticFlagsBF.isSpriteActor) {
							drawList[a12].field_2 = arg_46; // 0x1000
							if (lactor->staticFlagsBF.isUsingClipping) {
								temp3 = lactor->lastX - cameraX + lactor->lastY - cameraY;
							}
						} else {
							drawList[a12].field_2 = arg_1A;
						}

						drawList[a12].field_0 = temp3;

						a12++;

						if (shadowMode != 0 && !(lactor->staticFlagsBF.doesntCastShadow)) { // 0x1000 -> no shadow cast
							if (lactor->standOn != -1) { // quick shadow calc if on another actor
								shadowX = lactor->X;
								shadowZ = lactor->Y - 1;
								shadowY = lactor->Z;
							} else {
								GetShadow(lactor->X, lactor->Y, lactor->Z);
							}

							temp3--;
							drawList[a12].field_0 = temp3;  // save the shadow entry in the drawList
							//drawList[a12].field_2 = arg_42; // 0xC00
							drawList[a12].field_2 = 0xC00; // 0xC00
							drawList[a12].X = shadowX;
							drawList[a12].Z = shadowZ;
							drawList[a12].Y = shadowY;
							drawList[a12].field_A = 2;
							a12++;
						}

						if (drawInGameTransBox) {
							if (currentlyFollowedActor != arg_1A) {
								/*
								 * arg_16=projectedPositionX; arg_12=projectedPositionY;
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


	do { // process extras (like key, hearts,...)
		if (extraList[counter2].field_0 != -1) {
			if (extraList[counter2].field_14 & 0x400) {
				if (lba_time - extraList[counter2].time > 35) {
					extraList[counter2].time = lba_time;
					extraList[counter2].field_14 &= 0xFBFF;
					HQ_3D_MixSample(11, 0x1000, 1, extraList[counter2].X, extraList[counter2].Z, extraList[counter2].Y);
				}
			} else {
				if ((extraList[counter2].field_14 & 1) || (extraList[counter2].field_14 & 0x40) || (extraList[counter2].field_1C + extraList[counter2].time - 150 < lba_time) || (!((lba_time + extraList[counter2].time) & 8))) {
					projectPositionOnScreen(extraList[counter2].X - cameraX, extraList[counter2].Z - cameraZ, extraList[counter2].Y - cameraY);

					if (projectedPositionX > -50 && projectedPositionX < 680 && projectedPositionY > -30 && projectedPositionY < 580) {
						int extraType;

						drawList[a12].field_0 = extraList[counter2].X - cameraX + extraList[counter2].Y - cameraY;
						drawList[a12].field_2 = counter;
						a12++;

						// to not show shadows for stars and explode clouds
						extraType = extraList[counter2].field_0 & 0x7FFF; // 0 - damage star | 1 - explode cloud

						if (shadowMode == 2 && (extraList[counter2].field_0 & 0x8000) && (extraType != 0) && (extraType != 1)) { // cast shadow
							GetShadow(extraList[counter2].X, extraList[counter2].Z, extraList[counter2].Y);
							drawList[a12].field_0 = extraList[counter2].X - cameraX + extraList[counter2].Y - cameraY - 1;
							drawList[a12].field_2 = 0xC00;
							drawList[a12].X = shadowX;
							drawList[a12].Z = shadowZ;
							drawList[a12].Y = shadowY;

							a12++;
						}
					}
				}
			}
		}
		arg_1A++;
		counter++;
		counter2++;
	} while (arg_1A < 50);

	SmallSort(drawList, a12/*, 12*/);

	a0E = 0;
	fullRedrawVar8 = 0;
	arg_1A = 0;
	arg_1E = 0;

	if (a12 > 0) {
		unsigned int flags;
		int actorNumber;

		do {
			actorNumber = drawList[arg_1E].field_2 & 0x3FF;
			lactor = &actors[actorNumber];
			flags = ((unsigned int) drawList[arg_1E].field_2) & 0xFC00;

			if (flags < 0xC00) { // actor
				if (!flags) {
					if (lactor->previousAnimIndex != -1) // TODO: this shouldn't be required but crash when drawing the soldier getting out of the yellow moto. To investigate...
						SetInterAnimObjet2(lactor->animPosition, (char*)HQR_Get(HQR_Anims, lactor->previousAnimIndex), (char*)bodyPtrTab[lactor->costumeIndex], &lactor->animTimerData);

					if (!AffObjetIso(lactor->X - cameraX, lactor->Y - cameraZ, lactor->Z - cameraY, 0, lactor->angle, 0, bodyPtrTab[lactor->costumeIndex])) {
						if (renderLeft < 0)
							renderLeft = 0;

						if (renderTop < 0)
							renderTop = 0;

						if (renderRight >= 640)
							renderRight = 639;

						if (renderBottom >= 480)
							renderBottom = 479;

						SetClip(renderLeft, renderTop, renderRight, renderBottom);

						if (textWindowLeft <= textWindowRight && textWindowTop <= textWindowBottom) {
							int tempX;
							int tempZ;
							int tempY;

							lactor->dynamicFlagsBF.wasDrawn = 1;

							tempX = (lactor->X + 0x100) >> 9;
							tempZ = lactor->Y >> 8;

							if (lactor->field_3 & 0x7F)
								tempZ++;

							tempY = (lactor->Z + 0x100) >> 9;

							DrawOverBrick(tempX, tempZ, tempY);

							AddPhysBox(textWindowLeft, textWindowTop, renderRight, renderBottom);
						}
					}
				}
			} else if (flags == 0xC00) { // shadows
				projectPositionOnScreen(drawList[arg_1E].X - cameraX, drawList[arg_1E].Z - cameraZ, drawList[arg_1E].Y - cameraY);
				GetDxDyGraph(drawList[arg_1E].field_A, &spriteWidth, &spriteHeight, shadowSprite);

				renderLeft = projectedPositionX - (spriteWidth - (drawList[arg_1E].field_2 & 0x3FF)) / 2;
				renderTop = projectedPositionY - (spriteHeight - (drawList[arg_1E].field_2 & 0x3FF)) / 2;

				renderBottom = projectedPositionY + (spriteHeight - (drawList[arg_1E].field_2 & 0x3FF)) / 2;
				renderRight = projectedPositionX + (spriteWidth - (drawList[arg_1E].field_2 & 0x3FF)) / 2;

				SetClip(renderLeft, renderTop, renderRight, renderBottom);

				if (textWindowLeft <= textWindowRight && textWindowTop <= textWindowBottom) {
					AffGraph(drawList[arg_1E].field_A, renderLeft, renderTop, shadowSprite);
				}

				DrawOverBrick((drawList[arg_1E].X + 0x100) >> 9, drawList[arg_1E].Z >> 8 , (drawList[arg_1E].Y + 0x100) >> 9);

				AddPhysBox(textWindowLeft, textWindowTop, renderRight, renderBottom);
			} else if (flags < 0x1000) {
				printf("Draw actor <0x1000 %d\n",
					   drawList[arg_1E].field_2 & 0x3FF);
			} else if (flags == 0x1000) { // sprite actor
				projectPositionOnScreen(lactor->X - cameraX, lactor->Y - cameraZ, lactor->Z - cameraY);
				GetDxDyGraph(0, &spriteWidth, &spriteHeight, HQR_Get(HQR_Sprites, lactor->costumeIndex));

				renderLeft = projectedPositionX + *(short int *)(spriteActorData + lactor->costumeIndex * 16);  // calculate center
				renderTop = projectedPositionY + *(short int *)(spriteActorData + lactor->costumeIndex * 16 + 2);

				renderRight = renderLeft + spriteWidth;
				renderBottom = renderTop + spriteHeight;

				if (lactor->staticFlagsBF.isUsingClipping) {
					// if sprite actor use croping
					SetClip(fullRedrawVar1 + lactor->cropLeft, fullRedrawVar2 + lactor->cropTop, fullRedrawVar1 + lactor->cropRight, fullRedrawVar2 + lactor->cropBottom);
				} else {
					SetClip(renderLeft, renderTop, renderRight, renderBottom);
				}

				if (textWindowLeft <= textWindowRight && textWindowTop <= textWindowBottom) {
					AffGraph(0, renderLeft, renderTop, HQR_Get(HQR_Sprites, lactor->costumeIndex));
					lactor->dynamicFlagsBF.wasDrawn = 1;

					if (lactor->staticFlagsBF.isUsingClipping) {
						DrawOverBrick3((lactor->lastX + 0x100) >> 9, lactor->lastZ >> 8, (lactor->lastY + 0x100) >> 9);
					} else {
						int tempX;
						int tempZ;
						int tempY;

						tempX = (lactor->X + lactor->boudingBox.X.topRight + 0x100) >> 9;
						tempZ = lactor->Y >> 8;
						if (lactor->field_3 & 0x7F)
							tempZ++;
						tempY = (lactor->Z + lactor->boudingBox.Z.topRight + 0x100) >> 9;

						DrawOverBrick3(tempX, tempZ, tempY);
					}

					AddPhysBox(textWindowLeft, textWindowTop, textWindowRight, textWindowBottom);
				}
			} else if (flags == 0x1800) { // extras
				int extraFlag;
				extraListStruct* extraEntry = &extraList[actorNumber];

				projectPositionOnScreen(extraEntry->X - cameraX, extraEntry->Z - cameraZ, extraEntry->Y - cameraY);

				extraFlag = extraEntry->field_0;

				if (extraFlag & 0x8000) {
					AffSpecial(actorNumber, projectedPositionX, projectedPositionY);
				} else {
					GetDxDyGraph(0, &spriteWidth, &spriteHeight, HQR_Get(HQR_Sprites, extraEntry->field_0));

					renderLeft = (*(short int*)(((extraEntry->field_0 * 8) * 2) + spriteActorData)) + projectedPositionX;
					renderTop = (*(short int*)(((extraEntry->field_0 * 8) * 2) + spriteActorData) + 2) + projectedPositionY;

					renderRight = renderLeft + spriteWidth;
					renderBottom = renderTop + spriteHeight;

					AffGraph(0, renderLeft, renderTop, HQR_Get(HQR_Sprites, extraEntry->field_0));
				}

				SetClip(renderLeft, renderTop, renderRight, renderBottom);

				if ((textWindowLeft <= textWindowRight) && (textWindowTop <= textWindowBottom)) {
					DrawOverBrick((drawList[arg_1E].X + 0x100) >> 9, drawList[arg_1E].Z >> 8 , (drawList[arg_1E].Y + 0x100) >> 9);
					AddPhysBox(textWindowLeft, textWindowTop, renderRight, renderBottom);
				}
			}
			arg_1A++;
			UnSetClip();
			arg_1E++;
		} while (arg_1A < a12);
	}

	counter2 = 0;

	for (i = 0;i < 10;i++) {
		if (overlayObjectList[i].field_0 != -1) {
			switch (overlayObjectList[i].positionType) {
			case 0: { // normal
					if (lba_time >= overlayObjectList[i].timeToDie) {
						overlayObjectList[i].field_0 = -1;
						continue;
					}
					break;
				}
			case 1: { // follow actor
					projectPositionOnScreen(actors[overlayObjectList[i].followedActor].X - cameraX, actors[overlayObjectList[i].followedActor].boudingBox.Y.topRight - cameraZ, actors[overlayObjectList[i].followedActor].Z - cameraY);

					overlayObjectList[i].X = projectedPositionX;
					overlayObjectList[i].Y = projectedPositionY;

					if (lba_time >= overlayObjectList[i].timeToDie) {
						overlayObjectList[i].field_0 = -1;
						continue;
					}

					break;
				}
			}

			switch (overlayObjectList[i].type) {
			case 0: { // sprite
					unsigned char* pSprite;

					pSprite = HQR_Get(HQR_Sprites, overlayObjectList[i].field_0);

					GetDxDyGraph(0, &spriteWidth, &spriteHeight, pSprite);

					renderLeft = (*(short int*)(((overlayObjectList[i].field_0 * 8) * 2) + spriteActorData)) + overlayObjectList[i].X;
					renderTop = (*(short int*)(((overlayObjectList[i].field_0 * 8) * 2) + spriteActorData) + 2) + overlayObjectList[i].Y;

					renderRight = renderLeft + spriteWidth;
					renderBottom = renderTop + spriteHeight;

					AffGraph(0, renderLeft, renderTop, pSprite);

					if ((textWindowLeft <= textWindowRight) && (textWindowTop <= textWindowBottom))
						AddPhysBox(textWindowLeft, textWindowTop, renderRight, renderBottom);

					break;
				}
			case 1: { // number
					char tempString[10];

					sprintf(tempString, "%d", overlayObjectList[i].field_0);

					spriteWidth = SizeFont(tempString);
					spriteHeight = 48;

					renderLeft = overlayObjectList[i].X - (spriteWidth / 2);
					renderRight = overlayObjectList[i].X + (spriteWidth / 2);
					renderTop = overlayObjectList[i].Y - 24;
					renderBottom = overlayObjectList[i].Y + spriteHeight;

					SetClip(renderLeft, renderTop, renderRight, renderBottom);

					CoulFont(overlayObjectList[i].followedActor);

					Font(renderLeft, renderTop, tempString);

					if ((textWindowLeft <= textWindowRight) && (textWindowTop <= textWindowBottom))
						AddPhysBox(textWindowLeft, textWindowTop, renderRight, renderBottom);

					break;
				}
			case 2: { // number range
					char tempString[10];

					sprintf(tempString, "%d", BoundRegleTrois(overlayObjectList[i].followedActor, overlayObjectList[i].field_0, 100, (overlayObjectList[i].timeToDie - lba_time - 50)));

					spriteWidth = SizeFont(tempString);
					spriteHeight = 48;

					renderLeft = overlayObjectList[i].X - (spriteWidth / 2);
					renderRight = overlayObjectList[i].X + (spriteWidth / 2);
					renderTop = overlayObjectList[i].Y - 24;
					renderBottom = overlayObjectList[i].Y + spriteHeight;

					SetClip(renderLeft, renderTop, renderRight, renderBottom);

					CoulFont(155);

					Font(renderLeft, renderTop, tempString);

					if ((textWindowLeft <= textWindowRight) && (textWindowTop <= textWindowBottom))
						AddPhysBox(textWindowLeft, textWindowTop, renderRight, renderBottom);

					break;
				}
			case 3: { // 3d object
					unsigned char* pObject;

					Box(10, 10, 69, 69, 0);
					SetClip(10, 10, 69, 69);

					pObject = HQR_Get(HQR_Inventory, overlayObjectList[i].field_0);

					if (HQR_Flag) {
						loadGfxSub(pObject);
					}

					setCameraPosition(40, 40, 128, 200, 200);
					setCameraAngle(0, 0, 0, 60, 0, 0, 16000);

					overlay3dObect += 8;

					AffObjetIso(0, 0, 0, 0, overlay3dObect, 0, pObject);

					if (renderLeft < 0)
						renderLeft = 0;

					if (renderTop < 0)
						renderTop = 0;

					if (renderRight >= 640)
						renderRight = 639;

					if (renderBottom >= 480)
						renderBottom = 479;

					DrawCadre(10, 10, 69, 69);
					AddPhysBox(10, 10, 69, 69);

					reinitAll1();
					break;
				}
			case 4: { // text
					char tempString[256];
					GetMultiText(overlayObjectList[i].field_0, tempString);

					spriteWidth = SizeFont(tempString);
					spriteHeight = 48;

					renderLeft = overlayObjectList[i].X - (spriteWidth / 2);
					renderRight = overlayObjectList[i].X + (spriteWidth / 2);
					renderTop = overlayObjectList[i].Y - 24;
					renderBottom = overlayObjectList[i].Y + spriteHeight;

					if (renderLeft < 0)
						renderLeft = 0;

					if (renderTop < 0)
						renderTop = 0;

					if (renderRight > 639)
						renderRight = 639;

					if (renderBottom > 479)
						renderBottom = 479;

					SetClip(renderLeft, renderTop, renderRight, renderBottom);

					CoulFont(actors[overlayObjectList[i].followedActor].talkColor);

					Font(renderLeft, renderTop, tempString);

					if ((textWindowLeft <= textWindowRight) && (textWindowTop <= textWindowBottom))
						AddPhysBox(textWindowLeft, textWindowTop, renderRight, renderBottom);

					break;
				}
			}
		}
	}

	UnSetClip();

	if (drawInGameTransBox != 0) {
		fullRedrawSub5();
		if (param != 0)
			unfreezeTime();
	} else {
		if (param == 0) {
			FlipBoxes();  // update dirtyBox to screen and move all the nextDirtyBox to dirtyBox
		} else {
			if (!lockPalette)
				os_flip(frontVideoBuffer);
			fullRedrawSub5(); // move all the nextDirtyBox to dirtyBox
			unfreezeTime();
		}
	}

	if (!lockPalette)
		return;

	Load_HQR("ress.hqr", (byte *) & palette, 0);
	convertPalToRGBA(palette, paletteRGBA);

	if (useAlternatePalette)
		os_crossFade((char *) frontVideoBuffer, (char *) menuPalRGBA);
	else
		os_crossFade((char *) frontVideoBuffer, (char *) paletteRGBA);

	os_setPalette((byte *) & paletteRGBA);

	lockPalette = 0;

	return;
}

void blitBackgroundOnDirtyBoxes(void) {
	int i;
	currentDirtyBoxListStruct* currentBox;

	currentBox = currentDirtyBoxList;

	for (i = 0; i < numOfRedrawBox; i++) {
		blitRectangle(currentBox->left, currentBox->top, currentBox->right, currentBox->bottom, (char *) workVideoBuffer, currentBox->left, currentBox->top, (char *) frontVideoBuffer);
		currentBox++;
	}
}

void fullRedrawSub5(void) {
	int i;

	numOfRedrawBox = 0;

	for (i = 0; i < fullRedrawVar8; i++) {
		addToRedrawBox(nextDirtyBoxList[i].left, nextDirtyBoxList[i].top, nextDirtyBoxList[i].right, nextDirtyBoxList[i].bottom);
	}
}

void addToRedrawBox(short int arg_0, short int arg_4, short int arg_8, short int arg_C) {
	int var1;
	int i = 0;
	int var_C;
	int var_8;
	int var_10;
	int var_4;

	var1 = (arg_8 - arg_0) * (arg_C - arg_4);

	while (i < numOfRedrawBox) {
		if (currentDirtyBoxList[i].left >= arg_0)
			var_C = arg_0;
		else
			var_C = currentDirtyBoxList[i].left;

		if (currentDirtyBoxList[i].right <= arg_8)
			var_8 = arg_8;
		else
			var_8 = currentDirtyBoxList[i].right;

		if (currentDirtyBoxList[i].top >= arg_4)
			var_10 = arg_4;
		else
			var_10 = currentDirtyBoxList[i].top;

		if (currentDirtyBoxList[i].bottom <= arg_C)
			var_4 = arg_C;
		else
			var_4 = currentDirtyBoxList[i].bottom;

		if ((var_8 - var_C) * (var_4 - var_10) < ((currentDirtyBoxList[i].bottom - currentDirtyBoxList[i].top) * (currentDirtyBoxList[i].right - currentDirtyBoxList[i].left) + var1)) {
			currentDirtyBoxList[i].left = var_C;
			currentDirtyBoxList[i].top = var_10;
			currentDirtyBoxList[i].right = var_8;
			currentDirtyBoxList[i].bottom = var_4;

			if (currentDirtyBoxList[i].bottom >= 480)
				currentDirtyBoxList[i].bottom = 479;
			return;
		}

		i++;
	};

	currentDirtyBoxList[i].left = arg_0;
	currentDirtyBoxList[i].top = arg_4;
	currentDirtyBoxList[i].right = arg_8;
	currentDirtyBoxList[i].bottom = arg_C;

	if (currentDirtyBoxList[i].bottom >= 480)
		currentDirtyBoxList[i].bottom = 479;

	numOfRedrawBox++;
}

struct cubeEntry {
	unsigned char brickType1;
	unsigned char brickType2;
};
typedef struct cubeEntry cubeType[64][64][25];

void redrawCube(void) {

	int i;
	int x, y, z;
	unsigned char val;
	cubeType* cube = (cubeType*)bufCube;

	cameraX = newCameraX << 9;
	cameraZ = newCameraZ << 8;
	cameraY = newCameraY << 9;

	projectPositionOnScreen(-cameraX, -cameraZ, -cameraY);

	fullRedrawVar1 = projectedPositionX;
	fullRedrawVar2 = projectedPositionY;

	for (i = 0; i < 28; i++)
		zbufferTab[i] = 0;

	if (changeRoomVar10 == 0)
		return;

	for (z = 0; z < 64; z++) {
		for (x = 0; x < 64; x++) {
			for (y = 0; y < 25; y++) {
				val = (*cube)[z][x][y].brickType1;
				if (val) {
					zbuffer(val - 1, (*cube)[z][x][y].brickType2, x, y, z);
				}
			}
		}
	}
}

/** Draw cubes using zbuffer */
void zbuffer(int var1, int var2, int x, int z, int y) {
	unsigned char *ptr;
	unsigned short int bx;
	int zbufferIndex;
	zbufferDataStruct *currentZbufferData;

	/* Inits zbuffer */
	ptr = zbufferSub1(var1) + 3 + var2 * 4 + 2;

	bx = READ_LE_U16(ptr);

	if (!bx)
		return;

	/* Sets dest of the cubes to camera's position */
	zbufferSub2(x - newCameraX, z - newCameraZ, y - newCameraY);

	if (zbufferVar1 < -24)
		return;
	if (zbufferVar1 >= 640)
		return;
	if (zbufferVar2 < -38)
		return;
	if (zbufferVar2 >= 480)
		return;

	/* Draws the cubes */
	AffGraph(bx - 1, zbufferVar1, zbufferVar2, (unsigned char*)brickTable);

	zbufferIndex = (zbufferVar1 + 24) / 24;

	if (zbufferTab[zbufferIndex] >= 150) {
		printf("Arg MAX_BRICK Z BUFFER atteint\n");
		exit(1);
	}

	currentZbufferData = &zbufferData[zbufferIndex][zbufferTab[zbufferIndex]];

	currentZbufferData->y = y;
	currentZbufferData->z = z;
	currentZbufferData->x = x;
	currentZbufferData->drawX = zbufferVar1;
	currentZbufferData->drawY = zbufferVar2;
	currentZbufferData->spriteNum = bx - 1;

	zbufferTab[zbufferIndex]++;
}

unsigned char *zbufferSub1(int var) {
	return (currentBll + READ_LE_U32(currentBll + 4 * var));
}

void zbufferSub2(int x, int y, int z) {
	zbufferVar1 = (x - z) * 24 + 288; // x pos
	zbufferVar2 = ((x + z) * 12) - (y * 15) + 215;  // y pos
}

/* Draw a sprite */
void AffGraph(int num, int var1, int var2, unsigned char *localBufferBrick) {
	unsigned char *ptr;
	int top;
	int bottom;
	int left;
	int right;
	unsigned char *outPtr;
	int offset;
	int c1;
	int c2;
	int vc3;

	int temp;
	int iteration;
	int i;

	int x;
	int y;

	assert(textWindowLeft >= 0);
	assert(textWindowRight <= 639);
	assert(textWindowTop >= 0);
	assert(textWindowBottom <= 479);

	if (localBufferBrick != (unsigned char*)brickTable) {
		ptr = localBufferBrick + READ_LE_U32(localBufferBrick + num * 4);
	} else {
		ptr = brickTable[num];
	}

	left = var1 + *(ptr + 2);
	top = var2 + *(ptr + 3);
	right = *ptr + left - 1;
	bottom = *(ptr + 1) + top - 1;

	ptr += 4;

	x = left;
	y = top;

	right++;
	bottom++;

	outPtr = frontVideoBuffer + screenLockupTable[top] + left;

	offset = -((right - left) - WINDOW_X);

	/* All the Y cubes */
	for (c1 = 0; c1 < bottom - top; c1++) {
		vc3 = *(ptr++);
		/* All the X cubes */
		for (c2 = 0; c2 < vc3; c2++) {
			/* Get cube data */
			temp = *(ptr++);
			iteration = temp & 0x3F;
			if (temp & 0xC0) {
				iteration++;
				/* Same color */
				if (!(temp & 0x40)) {
					temp = *(ptr++);
					for (i = 0; i < iteration; i++) {
						if (x >= textWindowLeft && x<textWindowRight && y >= textWindowTop && y < textWindowBottom)
							frontVideoBuffer[y*640+x] = temp;

						x++;
						outPtr++;
					}
				/* Not the same color */
				} else {
					for (i = 0; i < iteration; i++) {
						if (x >= textWindowLeft && x<textWindowRight && y >= textWindowTop && y < textWindowBottom)
							frontVideoBuffer[y*640+x] = *ptr;

						x++;
						ptr++;
						outPtr++;
					}
				}
			} else {
				outPtr += iteration + 1;
				x += iteration + 1;
			}
		}
		outPtr += offset;
		x = left;
		y++;
	}
}

int projectPositionOnScreen(int coX, int coZ, int coY) {
	if (!isUsingOrhoProjection) {
		coX -= setSomething3Var12;
		coZ -= setSomething3Var14;
		coY -= setSomething3Var16;

		if (coY >= 0) {
			int bp = coY + cameraVar1;

			if (bp < 0)
				bp = 0x7FFF;

			projectedPositionX = (coX * cameraVar2) / bp + setSomethingVar1;
			projectedPositionY = (-coZ * cameraVar3) / bp + setSomethingVar2;
			projectedPositionZ = bp;

			return(-1);
		} else {
			projectedPositionX = 0;
			projectedPositionY = 0;
			projectedPositionZ = 0;
			return(0);
		}
	} else {
#ifdef USE_FLOAT
		projectedPositionX = (coX - coY) * 24 / 512.f + setSomethingVar1;
		projectedPositionY = (((coX + coY) * 12) - coZ * 30) / 512.f + setSomethingVar2;
		projectedPositionZ = coZ - coX - coY;
#else
		projectedPositionX = (coX - coY) * 24 / 512 + setSomethingVar1;
		projectedPositionY = (((coX + coY) * 12) - coZ * 30) / 512 + setSomethingVar2;
		projectedPositionZ = coZ - coX - coY;
#endif
		return (-1);
	}
	return (-1);
}

int HQ_3D_MixSample(int param0/*, int param1, int param2, int param3, int param4, int param5 */) {
	playSample(param0, /*1, */1/*, 10, 10*/);
	return (0);
}

void GetDxDyGraph(int arg_0, int *arg_4, int *arg_8, unsigned char *ptr) {
	ptr += READ_LE_S32(ptr + arg_0 * 4);

	*arg_4 = *ptr;
	*arg_8 = *(ptr + 1);
}

void DrawOverBrick(int X, int Z, int Y) {
	int CopyBlockPhysLeft;
	int CopyBlockPhysRight;
	int i;
	int j;
	zbufferDataStruct *currentZbufferData;

	CopyBlockPhysLeft = ((textWindowLeft + 24) / 24) - 1;
	CopyBlockPhysRight = ((textWindowRight + 24) / 24);

	for (j = CopyBlockPhysLeft; j <= CopyBlockPhysRight; j++)
	{
		for (i = 0; i < zbufferTab[j]; i++)
		{
			currentZbufferData = &zbufferData[j][i];

			if (currentZbufferData->drawY + 38 > textWindowTop && currentZbufferData->drawY <= textWindowBottom && currentZbufferData->z >= Z)
				if (currentZbufferData->x + currentZbufferData->y > Y + X)
					CopyMask(currentZbufferData->spriteNum, (j * 24) - 24, currentZbufferData->drawY, workVideoBuffer);
		}
	}
}

/*static */void DrawOverBrick3(int X, int Z, int Y) {
	int CopyBlockPhysLeft;
	int CopyBlockPhysRight;
	int i;
	int j;
	zbufferDataStruct *currentZbufferData;

	CopyBlockPhysLeft = ((textWindowLeft + 24) / 24) - 1;
	CopyBlockPhysRight = (textWindowRight + 24) / 24;

	for (j = CopyBlockPhysLeft; j <= CopyBlockPhysRight; j++) {
		for (i = 0; i < zbufferTab[j]; i++) {
			currentZbufferData = &zbufferData[j][i];

			if (currentZbufferData->drawY + 38 > textWindowTop && currentZbufferData->drawY <= textWindowBottom && currentZbufferData->z >= Z) {
				if ((currentZbufferData->x == X) && (currentZbufferData->y == Y)) {
					CopyMask(currentZbufferData->spriteNum, (j * 24) - 24, currentZbufferData->drawY, workVideoBuffer);
				}

				if ((currentZbufferData->x > X) || (currentZbufferData->y > Y)) {
					CopyMask(currentZbufferData->spriteNum, (j * 24) - 24,  currentZbufferData->drawY, workVideoBuffer);
				}
			}
		}
	}
}

void CopyMask(int spriteNum, int x, int y, /*byte * localBufferBrick, */byte * buffer)
{
	unsigned char *ptr;
	int top;
	int bottom;
	int left;
	int right;
	unsigned char *outPtr;
	unsigned char *inPtr;
	int offset;
	int vc3;

	int temp;
	int j;

	int absX;
	int absY;

	int vSize;

	assert(textWindowLeft >= 0);
	assert(textWindowRight <= 639);
	assert(textWindowTop >= 0);
	assert(textWindowBottom <= 479);

	//ptr = localBufferBrick + *(unsigned int *) (localBufferBrick + spriteNum * 4);

	ptr = brickMaskTable[spriteNum];

	assert(ptr);

	left = x + *(ptr + 2);
	top = y + *(ptr + 3);
	right = *ptr + left - 1;
	bottom = *(ptr + 1) + top - 1;

	if (left > textWindowRight || right < textWindowLeft || bottom < textWindowTop || top > textWindowBottom)
		return;

	ptr += 4;

	absX = left;
	absY = top;

	vSize = (bottom - top) + 1;

	if (vSize <= 0)
		return;

	offset = -((right - left) - WINDOW_X) - 1;

	right++;
	bottom++;

	// if line on top aren't in the blitting area...
	if (absY < textWindowTop) {
		int numOfLineToRemove;

		numOfLineToRemove = textWindowTop - absY;

		vSize -= numOfLineToRemove;
		if (vSize <= 0)
			return;

		absY += numOfLineToRemove;

		do {
			int lineDataSize;

			lineDataSize = *(ptr++);
			ptr += lineDataSize;
		} while (--numOfLineToRemove);
	}

	// reduce the vSize to remove lines on bottom
	if (absY + vSize - 1 > textWindowBottom) {
		vSize = textWindowBottom - absY + 1;
		if (vSize <= 0)
			return;
	}

	outPtr = frontVideoBuffer + screenLockupTable[absY] + left;
	inPtr = buffer + screenLockupTable[absY] + left;

	do {
		vc3 = *(ptr++);

		do {
			temp = *(ptr++); // skip size
			outPtr += temp;
			inPtr += temp;

			absX += temp;

			vc3--;
			if (!vc3)
				break;

			temp = *(ptr++); // copy size

			for (j = 0;j < temp;j++) {
				if (absX >= textWindowLeft && absX <= textWindowRight)
					*outPtr = *inPtr;

				absX++;
				outPtr++;
				inPtr++;
			}
		} while (--vc3);

		absX = left;

		outPtr += offset;
		inPtr += offset;
	} while (--vSize);
}

void AddPhysBox(int left, int top, int right, int bottom) {
	if (left < 0)
		left = 0;
	if (top < 0)
		top = 0;
	if (right >= 640)
		right = 639;
	if (bottom >= 480)
		bottom = 479;

	if (left > right || top > bottom)
		return;

	// DrawCadre (left, top, right, bottom);

	nextDirtyBoxList[fullRedrawVar8].left = left;
	nextDirtyBoxList[fullRedrawVar8].top = top;
	nextDirtyBoxList[fullRedrawVar8].right = right;
	nextDirtyBoxList[fullRedrawVar8].bottom = bottom;

	fullRedrawVar8++;

	addToRedrawBox(left, top, right, bottom);
}

//sort character list using bubble-sort
//TODO: make real use of stepSize to tell the scructure size
void SmallSort(drawListStruct *list, int listSize/* , int stepSize*/)
{
	int i;
	int j;

	drawListStruct tempStruct;

	for (i = 0;i < listSize - 1;i++) {
		for (j = 0;j < listSize - 1 - i;j++) {
			if (list[j+1].field_0 < list[j].field_0) {
				memcpy(&tempStruct, &list[j+1], sizeof(drawListStruct));
				memcpy(&list[j+1], &list[j], sizeof(drawListStruct));
				memcpy(&list[j], &tempStruct, sizeof(drawListStruct));
			}
		}
	}
}

void FlipBoxes(void) {
	int i;

	for (i = 0; i < numOfRedrawBox; i++) { //draw the dirty box on screen
		os_copyBlockPhys(currentDirtyBoxList[i].left, currentDirtyBoxList[i].top, currentDirtyBoxList[i].right, currentDirtyBoxList[i].bottom);
	}

	numOfRedrawBox = 0;

	for (i = 0; i < fullRedrawVar8; i++) { //setup the dirty box for next display
		addToRedrawBox(nextDirtyBoxList[i].left, nextDirtyBoxList[i].top, nextDirtyBoxList[i].right, nextDirtyBoxList[i].bottom);
	}
}
