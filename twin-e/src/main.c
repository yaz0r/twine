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
#include "fla.h"
#include "mainMenu.h"
#include "font.h"
#include "text.h"
#include "time.h"
#include "input.h"
#include "hqr.h"
#include "music.h"
#include "samples.h"
#include "room.h"
#include "script.h"

#include "main.h"

short int newGameVar4 = 1;
int newGameVar5 = 0;

unsigned char *frontVideoBuffer;
byte *workVideoBuffer;
byte *frontVideoBufferbis;

byte *bufSpeak;
byte *bufMemoSeek;
char *bufText;
char *bufOrder;
byte *bufAni1;
byte *bufAni2;

byte *menuPal;
byte menuPalRGBA[1024];
byte *shadowSprite;
byte *spriteActorData;
byte *lbaFont;

byte *bufCube;

int screenLockupTable[2000]; /* TODO: temporary ? */

int setup_lst;

void initVideoStuff(void) {
	int i, j, k;

	frontVideoBuffer = frontVideoBufferbis = malloc(sizeof(byte) * 307200);

	osystem_initBuffer((char *) frontVideoBuffer, 640, 480);
	osystem_initVideoBuffer(flaBuffer, 320, 200);

	j = 0;
	k = 0;
	for (i = WINDOW_Y; i > 0; i--) {
		screenLockupTable[j] = k;
		j++;
		k += WINDOW_X;
	}
}

static void init(void)
{
	openCD();
	soundInit();

	initVideoStuff();

#ifdef USE_SDL_MIXER
    printf("Sound activated\n");
	HQR_Midi = HQR_Init_Ressource("midi_mi_win.hqr",32000,2);
#endif

	workVideoBuffer = malloc(307700 * sizeof(byte));

#ifndef FASTDEBUG
	AdelineLogo();
#endif

	bufSpeak = malloc(BUF_SPEAK_SIZE);
	bufMemoSeek = malloc(BUF_MEMOSEEK_SIZE);
	bufText = malloc(BUF_TEXT_SIZE);
	bufOrder = malloc(BUF_ORDER_SIZE);
	bufAni1 = bufAni2 = malloc(BUF_ANIM_SIZE);

    bufCube = malloc(204800);  // 204800 = 64*64*25*2

    HQRM_Load("ress.hqr", 0, (unsigned char**)&menuPal);
    HQRM_Load("ress.hqr", 1, (unsigned char**)&lbaFont);
    HQRM_Load("ress.hqr", 3, (unsigned char**)&spriteActorData);
    HQRM_Load("ress.hqr", 4, (unsigned char**)&shadowSprite);

	convertPalToRGBA(menuPal, menuPalRGBA);

	SetFont(lbaFont, 2, 8);
	CoulFont(14);
	CoulDial(136, 143, 2);

	HQR_Inventory = HQR_Init_Ressource("invobj.hqr", BUF_INVENTORY_SIZE, 30);
	HQR_Sprites = HQR_Init_Ressource("sprites.hqr", 300000, 120); // enough to hold all the sprites in mem
	HQR_Samples = HQR_Init_Ressource("samples.hqr", 4500000, 4500000 / 5000);
	HQR_Anims = HQR_Init_Ressource("anim.hqr", 450000, 600); // should be able to hold all the anims of the game

	FadeToBlack((char *) paletteRGBA);

#ifndef FASTDEBUG
	if (setup_lst == 0)   // switch pour les 2 version de l'ecran titre de LBA
		RessPict(49);
	else
		RessPict(12);

	WaitTime(3000);

	loadImageCrossFade(52);

	WaitTime(1000);

	FadeToBlack((char *) paletteRGBA);

	playFla("DRAGON3");
#endif

	Load_HQR("ress.hqr", workVideoBuffer, 14);

	CopyScreen(workVideoBuffer, frontVideoBuffer);

	osystem_flip(frontVideoBuffer);

	FadeToPal((char *) menuPalRGBA);

	MainGameMenu();
}


int main(int argc, char *argv[])
{
	osystem_init(argc, argv);

	init();

	closeCD();

	return 0;
}

void newGame(void)
{
	int flagDisplayTextSave;
#ifndef FASTDEBUG
	int screen;
#endif

	stopMusic();
	if (needChangeRoom)
		return;
	if (chapter)
		return;

	flagDisplayTextSave = flagDisplayText;

	flagDisplayText = 1;

#ifndef FASTDEBUG
	newGameVar4 = 0;
	newGameVar5 = 1;
	InitDial(2);
	newGame2();
	TestCoulDial(15);

	for (screen = 0; screen < 3 && !skipIntro; screen++)
	{
		Load_HQR("ress.hqr", workVideoBuffer, 15 + screen*2);
		CopyScreen(workVideoBuffer, frontVideoBuffer);
		Load_HQR("ress.hqr", palette, 16 + screen*2);
		convertPalToRGBA(palette, paletteRGBA);

		osystem_crossFade((char *) frontVideoBuffer, (char *) paletteRGBA);
		osystem_setPalette(paletteRGBA);

		printTextFullScreen(150 + screen);
		readKeyboard();
	}

	newGameVar5 = 0;
	newGame4();
	newGameVar4 = 1;
	FadeToBlack((char *) paletteRGBA);
	Cls();
	osystem_flip(frontVideoBuffer);
	playMidi(1);

	playFla("INTROD");
#endif
	SetBackPal();
	Cls();
	osystem_flip(frontVideoBuffer);

	flagDisplayText = flagDisplayTextSave;  // on remet le flag comme il était au debut
}

void newGame2(void) {      /* ok */
	dialogueBoxLeft = 8;
	dialogueBoxTop = 8;
	dialogueBoxRight = 631;

	dialogueBoxBottom = 471;
	dialogueBoxParam1 = 11;
	dialogueBoxParam2 = 607;
}

void newGame4(void) {      /* ok */
	dialogueBoxLeft = 16;
	dialogueBoxTop = 334;
	dialogueBoxRight = 623;
	dialogueBoxBottom = 463;
	dialogueBoxParam1 = 3;
	dialogueBoxParam2 = 591;
}

void TestCoulDial(short int param) {
	progressiveTextStepSize = -1;
	progressiveTextBufferSize = 14;
	progressiveTextStartColor = param << 4;
	progressiveTextStopColor = (param << 4) + 12;
}

void CoulFont(int i) {
	textColor = i;
}

void CoulDial(int a, int b, int c) {
	progressiveTextStartColor = b;
	progressiveTextStopColor = a;
	progressiveTextStepSize = c;

	progressiveTextBufferSize = ((b - a) + 1) / c;

}
