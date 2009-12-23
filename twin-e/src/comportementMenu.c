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
#include "mainMenu.h"
#include "anim.h"
#include "text.h"
#include "mainLoop.h"
#include "font.h"
#include "fullRedraw.h"
#include "room.h"
#include "main.h"
#include "hqr.h"
#include "actors.h"
#include "body.h"
#include "extra.h"
#include "script.h"

#define BEHAVIORBOX_SZX 110
#define BEHAVIOR_SZX 99
#define BEHAVIOR_SZY 119
#define BEHAVIOR_POSX 110
#define BEHAVIOR_POSY 110

animTimerData animDataTab[4];

short int winTab[4];

unsigned char *menuCostumeIndex;

short int comportementHero;

char dataString[256];

int newAngle;

void DrawComportement(int lcomportement) {
	unsigned char *currentAnim;
	int currentAnimState;

	int box_left;
	int box_right;
	int box_top;
	int box_bottom;

	box_left = lcomportement * BEHAVIORBOX_SZX + BEHAVIOR_POSX;
	box_right = box_left + BEHAVIOR_SZX;
	box_top = BEHAVIOR_POSY;
	box_bottom = box_top + BEHAVIOR_SZY;

	currentAnim = HQR_Get(HQR_Anims, TCos[lcomportement]);
	currentAnimState = winTab[lcomportement];

	if (SetInterAnimObjet(currentAnimState, (char *) currentAnim, (char *) menuCostumeIndex, &animDataTab[lcomportement]) && lcomportement == comportementHero) {
		currentAnimState++;
		if (currentAnimState == GetNbFramesAnim((char *) currentAnim)) // max for anim
			currentAnimState = GetBouclageAnim((char *) currentAnim);  // start of anim
		winTab[lcomportement] = currentAnimState;
	}

	DrawCadre(box_left - 1, box_top - 1, box_right + 1, box_bottom + 1);

	saveTextWindow();
	UnSetClip();

	if (lcomportement != comportementHero) {
		Box(box_left, box_top, box_right, box_bottom, 0); // color of unselected
		DrawObj3D(box_left, box_top, box_right, box_bottom, -600, twinsen->angle, menuCostumeIndex);
	} else {
		Box(box_left, box_top, box_right, box_bottom, 68);  // color of selected

		Box(110, 239, 540, 279, 0); // comportement name box
		DrawCadre(110, 239, 540, 279);

		CoulFont(15);

		if (comportementHero == 2 && autoAgressivity == 1) {
			GetMultiText(4, dataString);  // pour afficher le "auto" ou non
		} else {
			GetMultiText(comportementHero, dataString);
		}

		Font((650 - SizeFont(dataString)) / 2, 240, dataString);  // ecrit le nom du comportement dans lequel on se trouve
		DrawObj3D(box_left, box_top, box_right, box_bottom, -600, twinsen->angle - newAngle, menuCostumeIndex);
	}


	os_copyBlockPhys(box_left, box_top, box_right, box_bottom);
	os_copyBlockPhys(110, 239, 540, 279);

	loadSavedTextWindow();
}

void DrawInfoMenu(short int arg_0, short int arg_4) {
	int var_10;
	int var_4;
	int var_8;
	int var_C;

	int var_14;

	int temp;
	int i;

	UnSetClip();
	DrawCadre(arg_0, arg_4, arg_0 + 450, arg_4 + 80);
	Box(arg_0 + 1, arg_4 + 1, arg_0 + 449, arg_4 + 79, 0);
	var_14 = arg_0 + 9;

	AffGraph(0, var_14, arg_4 + 13, HQR_Get(HQR_Sprites, SPRITE_heart));

	var_10 = arg_0 + 325;
	temp = arg_0 + 25;
	var_4 = RegleTrois32(temp, var_10, 50, twinsen->life);

	var_8 = arg_4 + 10;
	var_C = arg_4 + 25;
	Box(temp, var_8, var_4, var_C, 91);
	Box(var_4, var_8, var_10, var_C, 0);

	DrawCadre(arg_0 + 25, arg_4 + 10, arg_0 + 325, arg_4 + 10 + 15);

	if (!vars[70] && vars[4]) { // ca doit etre le mana
		AffGraph(0, var_14, arg_4 + 36, HQR_Get(HQR_Sprites, SPRITE_manabox));
		if (magicLevel > 0) {
			Box(temp, arg_4 + 35, RegleTrois32(temp, var_10, 80, magicPoint), arg_4 + 50, 75);
		}
	}

	var_4 = arg_0 + 340;

	AffGraph(0, var_4, arg_4 + 15, HQR_Get(HQR_Sprites, SPRITE_coin));  // draw coins
	CoulFont(155);
	Font(arg_0 + 370, arg_4 + 5, my_itoa(numCoin));  // amount of coins

	AffGraph(0, var_4, arg_4 + 55, HQR_Get(HQR_Sprites, SPRITE_key));  // draw key
	CoulFont(155);
	Font(arg_0 + 370, arg_4 + 40, my_itoa(numKey));

	for (i = 0; i < numCloverBox; i++) { // boites à trefles
		AffGraph(0, RegleTrois32(arg_0 + 25, arg_0 + 325, 10, i), arg_4 + 58, HQR_Get(HQR_Sprites, SPRITE_cloverbox));
	}

	for (i = 0; i < numClover; i++) { // trefles
		AffGraph(0, RegleTrois32(arg_0 + 25, arg_0 + 325, 10, i) + 2, arg_4 + 60, HQR_Get(HQR_Sprites, SPRITE_clover));
	}

	os_copyBlockPhys(arg_0, arg_4, arg_0 + 450, arg_4 + 80);
}

void drawMenuWin() {
	unsigned short int i;

	DrawCadre(100, 100, 550, 290);
	drawBoxInsideTrans(101, 101, 549, 289, 2);

	for (i = 0; i < 4; i++)
		DrawComportement(i);

	DrawInfoMenu(100, 300);

	os_copyBlockPhys(100, 100, 550, 290);
}

void processComportementMenu(void) {

	int languageCD1temp;
	int var_4;
	int savedComportement;
	int canPress = 1;

	freezeTime();

	if (comportementHero == 4) {
		HQ_StopSample();
		SetComportement(0);
	}

	menuCostumeIndex = bodyPtrTab[twinsen->costumeIndex];

	setActorAngleSafe(twinsen->angle, twinsen->angle - 256, 50, &timeVar);

	CopyScreen(frontVideoBuffer, workVideoBuffer);

	languageCD1temp = languageCD1;
	languageCD1 = 0;
	var_4 = currentTextBank;
	currentTextBank = -1;

	InitDial(0);

	savedComportement = comportementHero;

	while (os_isPressed(KEY_CHBEHAVIOR))
    {
		drawMenuWin();

		if (os_isPressed(KEY_CHBEHAVIOR_RIGHT) && canPress) {
			comportementHero++;
			canPress = 0;
		}
		else if (os_isPressed(KEY_CHBEHAVIOR_LEFT) && canPress) {
			comportementHero--;
			canPress = 0;
		}

		if (!os_isPressed(KEY_CHBEHAVIOR_LEFT) && !os_isPressed(KEY_CHBEHAVIOR_RIGHT))
			canPress = 1;

		if (comportementHero < 0)
			comportementHero = 3;

		if (comportementHero >= 4)
			comportementHero = 0;

		if (savedComportement != comportementHero) {
			savedComportement = comportementHero;
			newAngle = 0;
		}

		os_delay(15); /* TODO: cleanup */
		lba_time += 1;
		newAngle += 4;
#ifndef PCLIKE
		lba_time += 3;
#endif
	}

	SetComportement(comportementHero);

	reinitAll1();

	currentTextBank = var_4;
	InitDial(currentTextBank + 3);

	languageCD1 = languageCD1temp;

	unfreezeTime();
}
