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
#include "main.h"
#include "mainLoop.h"
#include "text.h"
#include "font.h"
#include "fireEffect.h"
#include "renderer.h"
#include "hqr.h"
#include "music.h"
#include "room.h"

#include "mainMenu.h"

char mainMenuVar1[60];
int mainMenu3Var1;
int mainMenu3Var2;
char mainMenu3Var3[256];

///////////////////////////////////////////////////////////
// MENU DATA

short int mainMenuData[] = {
	0,          // unk
	4,          // num of buttons
	200,        // unk
	0,          // unk
	0,          // unk
//---------------------------- exit points
	20,         // new game
	0,
	21,         // continue game
	0,
	23,         // options
	0,
	22,         // quit
};

short int subMenu2Data[] = {
	0,
	2,
	240,
	0,
	0,
	28,
	0,
	27,
};

short int soundMenuData[] = {
	0,
	4,
	0,
	0,
	0,
	24,
	0,
	30,
	0,
	46,
	0,
	47,
};

short int subMenuData[] = {
	0,
	5,
	0,
	0,
	0,
	26,
	0,
	4,
	6,
	31,
	7,
	32,
	8,
	33,
};
////////////////////////////////////////

char allowedCharIndex[] = " ABCDEFGHIJKLM.NOPQRSTUVWXYZ-abcdefghijklm?nopqrstuvwxyz!0123456789\040\b\r\0";

short int buttonDrawVar1 = 550;

short int shadowMode = 2;
short int autoAgressivity = 1;

void rungame(void)
{
    int shadowtemp, langtemp;
    shadowtemp = shadowMode;
    langtemp = languageCD1;
    languageCD1 = 0;
    shadowMode = 0;
    reinitAll(1);
    currentRoom = 119;
    needChangeRoom = 119;
    mainLoop();
                   
    languageCD1 = langtemp;
    shadowMode = shadowtemp;

    Cls();
    os_flip(frontVideoBuffer);
    playFla("The_end");
    Cls();
    os_flip(frontVideoBuffer);
    os_setPalette(menuPalRGBA);
}

void MainGameMenu(void) {
#ifndef FASTDEBUG
	int nmenu;
    int cont = 1;

	HQ_StopSample();
	CopyScreen(frontVideoBuffer, workVideoBuffer);

	while (cont)
    {
		InitDial(0);
		playMusic(9);
		HQ_StopSample();
		GetMultiText(49, mainMenuVar1);
		nmenu = processMenu(mainMenuData);
        os_delay(100);

        switch(nmenu)
        {
        case 20: /* new game */
			if (enterPlayerName(42))
            {
#endif
				reinitAll(1);
				newGame();

				if (mainLoop())
                    rungame();

#ifndef FASTDEBUG

				CopyScreen(frontVideoBuffer, workVideoBuffer);
			}
            break;

        case 21: /* continue game */
			if (chooseSave(21))
            {
				reinitAll(-1);
				newGame();

				if (mainLoop())
                    rungame();

				CopyScreen(frontVideoBuffer, workVideoBuffer);
			}
            break;

        case 22: /* quit */
			printf("\nLBA/Relentless TwinEngine < %s / %s >\n\nOK.\n", __DATE__, __TIME__);
            cont = 0;
            break;

        case 23: /* options */
			CopyScreen(workVideoBuffer, frontVideoBuffer);
			os_flip(frontVideoBuffer);
			soundMenuData[5] = 26;
			optionMenu();
            break;

        case 9999: /* ?? */
			Load_HQR("ress.hqr", workVideoBuffer, 14);
			CopyScreen(workVideoBuffer, frontVideoBuffer);

			SetBackPal();
			os_flip(frontVideoBuffer);
			FadeToPal((char *) menuPalRGBA);
            break;
        }
    }
#endif
}

void HQ_StopSample(void) {
	// todo: implement this
}

int chooseSave(/*int param*/)
{
	FILE* fHandle;

	fHandle = fopen("SAVE.LBA", "rb");
	if (!fHandle) {
		return 0;
	}

	fclose(fHandle);

	/*  var_14 = 1;
	  var_0C = 50;

	  CopyScreen(workVideoBuffer, frontVideoBuffer);
	  os_flip(frontVideoBuffer);
	  var_24 = alloc(1500);
	  var_28 = alloc(200);

	  var_8 = findAllSaves(var_28,var_24);

	  if(!var_8)
	      return(0);

	  InitDial(0);

	  drawSaveNameButton(320,40,GetMultiText(21,var_12C));*/


	return (1);
}

char *GetMultiText(int a, char *b) {
	if (a == mainMenu3Var1) {
		if (mainMenu3Var2 == currentTextBank) {
			strcpy(b, mainMenu3Var3);
			return (b);
		}
	}
	if (!findString(a)) {
		b[0] = 0;   // si on a pas trouve le text...
		return (0);
	}

	if ((currentTextLength - 1) > 0xFF)
		currentTextLength = 0xFF;

	copyStringToString((char *) currentTextPtr, b, currentTextLength);  // on copie dans b
	// *(currentTextPtr+currentTextLength)=0; // on ajoute un \n a la fin
	currentTextLength++;  // on augmente la taille de la string de 1
	copyStringToString(b, mainMenu3Var3, currentTextLength);  // enfin, on copie le tout dans mainMenu3Var3

	mainMenu3Var1 = a;
	mainMenu3Var2 = currentTextBank;

	return (b);
}

int processMenu(short int *menuData) {
	int localTime;
	int numEntry;
	int buttonNeedRedraw;
	int maxButton;
	short int *localData = menuData;
	short int currentButton;
	short int temp3;
	int musicChanged;
	int buttonReleased = 0;

	musicChanged = 0;

	buttonNeedRedraw = 1;

	Load_HQR("ress.hqr", bufSpeak, 51);

	numEntry = localData[1];
	currentButton = localData[0];
	localTime = lba_time;
	maxButton = numEntry - 1;

	do {
		/*if (localData == mainMenuData) { // si on est dans le menu principal
			if (lba_time - localTime <= 11650) {
				if (skipIntro == 46)
					if (key1 != 32)
						return (9999);
			} else {
				return (9999);
			}
		} TODO: WTF is this ??? */

		if (!os_isPressed(KEY_MENU_UP) && !os_isPressed(KEY_MENU_DOWN))
			buttonReleased = 1;

		if (buttonReleased)
		{
			if (os_isPressed(KEY_MENU_DOWN)) { // on passe au bouton d'en dessous
				currentButton++;
				if (currentButton == numEntry) {
					currentButton = 0;  // on se retrouve au dessus
				}
				buttonNeedRedraw = 1;
				buttonReleased = 0;
			}

			if (os_isPressed(KEY_MENU_UP)) { // on passe au bouton d'au dessus
				currentButton--;
				if (currentButton < 0) {
					currentButton = maxButton;  // on se retrouve au dessous
				}
				buttonNeedRedraw = 1;
				buttonReleased = 0;
			}

			temp3 = *(localData + currentButton * 4 + 8); // on recupere le parametre du
			// bouton
			if (temp3 <= 5) { // si c'est un bouton de volume
				switch (temp3) {
				case 1: {
					}
				case 2: {
					}
				case 3: {
					}
				case 4: {
					}
				case 5: {
					}
				}
			}
		}

		if (buttonNeedRedraw == 1) {
			*localData = currentButton;

			drawButton(localData, 0); // le bouton actuel
			drawButton(localData, 1);
			buttonNeedRedraw = 0;
		} else {
			if (musicChanged) {
				// TODO: update des volumes...
			}

			buttonNeedRedraw = 0;
			drawButton(localData, 1);
		}
	} while (!os_isPressed(KEY_MENU_ENTER));

	currentButton = *(localData + 5 + currentButton * 2); // on recupere le point de sortie

	os_delay(100);

	return (currentButton);
}

void drawSelectableLetters(void) {
	char x, y;

	for (x = 0; x < 5; x++)
		for (y = 0; y < 14; y++)
			drawSelectableLetter(x, y, 0);
}

void drawSelectableLetter(int x, int y, int arg) {
	char buffer[256];

	// char param;
	int centerX, left, top, centerY, bottom, right, right2;

	buffer[0] = allowedCharIndex[y + x * 14];

	centerX = y * 45 + 25;

	left = centerX - 20;

	right = centerX + 20;

	top = x * 56 + 200 - 25;

	buffer[1] = 0;

	centerY = x * 56 + 200;

	bottom = x * 56 + 200 + 25;

	if (arg != 0) {
		Box(left, top, right, bottom, 91);
	} else {
		blitRectangle(left, top, right, bottom, (char *) workVideoBuffer, left, top, (char *)frontVideoBuffer);
		right2 = right;
		drawBoxInsideTrans(left, top, right2, bottom, 4);
	}

	DrawCadre(left, top, right, bottom);
	right2 = right;

	CoulFont(15);

	Font(centerX - SizeFont(buffer) / 2, centerY - 18, buffer);

	os_copyBlockPhys(left, top, right2, bottom);

}

int enterPlayerName(short int param)
{
	char buffer[256];

	short int a, b, c, e;

	e = 1;
	a = -1;
	b = 0;
	c = 0;

	CopyScreen(workVideoBuffer, frontVideoBuffer);
	os_flip(frontVideoBuffer);
	InitDial(0);
	GetMultiText(param, buffer);
	CoulFont(15);
	Font(320 - (SizeFont(buffer) / 2), 20, buffer);
	os_copyBlockPhys(0, 0, 639, 99);
	drawSelectableLetters();

	// TODO: implement this

	CopyScreen(workVideoBuffer, frontVideoBuffer);
	os_flip(frontVideoBuffer);

	return 1;
}

char* itoa(int nb) {
    int i, j;
	char* text;

    j = nb;
    while (j >= 10) {
        j /= 10;
        i++;
    }

	text = malloc(sizeof(char) * i);
	sprintf(text, "%d", nb);
	return text;
}

void drawButton(short int *data, int a) {
	int buttonNumber;
	int maxButton;

	unsigned short int *localData = (unsigned short int*)data;
	int temp;
	unsigned char temp2;
	unsigned short temp3;
	char currentButton;

	buttonNumber = *localData;
	localData += 1;
	maxButton = *localData;
	localData += 1;
	temp = *localData;
	localData += 2;

	if (temp == 0) {
		temp = 35;
	} else {
		temp = temp - (((maxButton - 1) * 6) + ((maxButton) * 50)) / 2;
	}

	if (maxButton <= 0) {
		return;

	}

	currentButton = 0;

	do {
		temp2 = (unsigned char) * localData;
		localData += 1;
		temp3 = *localData;
		localData += 1;
		if (a != 0) {

			if (currentButton == buttonNumber) {
				drawButtonGFX(320, temp, temp2, temp3, 1);

			}
		} else {
			if (currentButton == buttonNumber) {
				drawButtonGFX(320, temp, temp2, temp3, 1);
			} else {
				drawButtonGFX(320, temp, temp2, temp3, 0);
			}
		}

		currentButton++;
		temp += 56;
	} while (currentButton < maxButton);
}

void drawButtonGFX(int largeur, int posY, int c, int d, int mode) {

	int right;
	int top;
	int left;
	int bottom2;
	int bottom;

	/*
	 * int CDvolumeRemaped; int musicVolumeRemaped; int masterVolumeRemaped; int lineVolumeRemaped;
	 * int waveVolumeRemaped;
	 */

	char buttonText[256];
	int stringLength;

	left = largeur - buttonDrawVar1 / 2;
	right = largeur + buttonDrawVar1 / 2;

	top = posY - 25;
	bottom = bottom2 = posY + 25;

	if (mode != 0)
	{
		if (c <= 5 && c >= 1)
		{
			switch (c) {
			case 1: {
					// musicVolumeRemaped=RegleTrois32(left,right,255,musicVolume);
				}
			case 2: {
					// waveVolumeRemaped=RegleTrois32(left,right,255,waveVolume);

				}
			case 3: {
					// CDVolumeRemaped=RegleTrois32(left,right,255,CDVolume);
				}
			case 4: {
					// lineVolume=RegleTrois32(left,right,255,lineVolume);
				}
			case 5:

				{
					// masterVolume=RegleTrois32(left,right,255,masterVolume);
				}
			};
		}
		else
			makeFireEffect(top, 64);
	}
	else
	{
		blitRectangle(left, top, right, bottom, (char *) workVideoBuffer, left, top,
					  (char *) frontVideoBuffer);

		drawBoxInsideTrans(left, top, right, bottom2, 4);
	}

	DrawCadre(left, top, right, bottom);
	CoulFont(15);
	GetMultiText(d, buttonText);
	stringLength = SizeFont(buttonText);
	Font(largeur - (stringLength / 2), posY - 18, buttonText);

	// TODO: implement volume buttons

	os_copyBlockPhys(left, top, right, bottom);

}

void blitRectangle(int left, int top, int right, int bottom, char *source, int leftDest, int topDest, char *dest) {
	int largeur;
	int hauteur;

	char *s;
	char *d;

	int interligne;
	int temp3;
	int i;
	int j;

	assert(left >= 0);
	assert(right < 640);
	assert(top >= 0);
	assert(bottom < 480);

	s = screenLockupTable[top] + source + left;
	d = screenLockupTable[topDest] + dest + leftDest;

	largeur = right - left + 1;

	hauteur = bottom - top + 1;

	interligne = WINDOW_X - largeur;
	temp3 = left;

	left >>= 2;
	temp3 &= 3;

	for (j = 0; j < hauteur; j++) {
		for (i = 0; i < largeur; i++) {
			*(d++) = *(s++);
		}

		d += interligne;
		s += interligne;
	}
}

void drawBoxInsideTrans(int left, int top, int right, int bottom, int mode) {

	byte *pos;
	int largeur;
	int hauteur;
	int hauteur2;
	int temp;
	int localMode;
	int var1;
	char color;
	char color2;

	if (left > textWindowRight)
		return;
	if (right < textWindowLeft)
		return;
	if (top > textWindowBottom)
		return;
	if (bottom < textWindowTop)
		return;

	if (left < textWindowLeft)
		left = textWindowLeft;
	if (right > textWindowRight)

		right = textWindowRight;
	if (top < textWindowTop)
		top = textWindowTop;
	if (bottom > textWindowBottom)
		bottom = textWindowBottom;

	pos = screenLockupTable[top] + frontVideoBuffer + left;
	hauteur2 = hauteur = bottom - top;
	hauteur2++;

	largeur = right - left + 1;

	temp = WINDOW_X - largeur;
	localMode = mode;

	do {
		var1 = largeur;
		do {
			color2 = color = *pos;
			color2 &= 0xF0;
			color &= 0x0F;
			color -= localMode;
			if (color < 0)
				color = color2;
			else
				color += color2;
			*pos++ = color;
			var1--;
		} while (var1 > 0);
		pos += temp;
		hauteur2--;
	} while (hauteur2 > 0);
}

void DrawCadre(int left, int top, int right, int bottom) {
	drawLine(left, top, right, top, 79);  // ligne du haut
	drawLine(left, top, left, bottom, 79);  // ligne de gauche
	drawLine(right, ++top, right, bottom, 73);  // ligne de droite
	drawLine(++left, bottom, right, bottom, 73);  // ligne du bas
}

int SizeFont(char *string) {
	unsigned char caractere;
	stringLenght = 0;

	for (;;)
    {
		caractere = (unsigned char) * (string++);

		if (caractere == 0)
			break;
        else if (caractere == 0x20)
			stringLenght += spaceLenght;
		else {
			stringLenght += interCharSpace; // space inter-letters
			stringLenght += *(fntFont + convertWFromLE(*((short int *)(fntFont + caractere * 4))));
		}
	}

	return stringLenght;
}

void copyStringToString(char *a, char *b, int c) {
	int i;

	for (i = 0; i < c; i++)
		*(b++) = *(a++);
}

int optionMenu(void) { // Options menu
    byte temp;
	byte temp2 = 0;

	CopyScreen(frontVideoBuffer, workVideoBuffer);
	HQ_StopSample();

	playMusic(9);

	do {
		temp = processMenu(soundMenuData);

		if (temp == 15 || temp == 26) { // Return to Game
			temp2 = 1;
		} else if (temp == 30) { // Volume Settings
			CopyScreen(workVideoBuffer, frontVideoBuffer);
			os_flip(frontVideoBuffer);
			// volumeMenu();
		} else if (temp == 46) { // Saved Gamee Management

			CopyScreen(workVideoBuffer, frontVideoBuffer);
			os_flip(frontVideoBuffer);
			// saveManipMenu();

		} else if (temp == 47) { // Advenced Options
			CopyScreen(workVideoBuffer, frontVideoBuffer);
			os_flip(frontVideoBuffer);
			optionMenu2();
		}
	} while (temp2 != 1);

	CopyScreen(workVideoBuffer, frontVideoBuffer);
	os_flip(frontVideoBuffer);

	return (0);
}

void optionMenu2(void) { // Advanced Options
	byte quit = 0;

	CopyScreen(frontVideoBuffer, workVideoBuffer);
    
    while (!quit)
    {
        switch (processMenu(subMenuData))
        {
            case 33: // No Scenery Zoom
                subMenuData[13] = 233;
                // zoomMode = 0;
                break;
            case 231:
                subMenuData[9] = 31;
                break;
            case 232: // Shadows for all animated objects
    			subMenuData[11] = 32;
    			shadowMode = 2;
                break;
            case 233: // Scenery Zoom On
    			subMenuData[13] = 33;
    			// zoomMode = 1;
                break;
            case 131:
    			subMenuData[9] = 231;
                break;
            case 132: // No Shadows
    			subMenuData[11] = 232;
    			shadowMode = 0;
                break;
            case 26: // Back to previous menu
                quit = 1;
                break;
            case 31:
    			subMenuData[9] = 131;
                break;
            case 32: // Character shadows
    			subMenuData[11] = 132;
    			shadowMode = 1;
                break;
            case 2: // Agressive Manual
    			subMenuData[7] = 4;
    			autoAgressivity = 0;
                break;
            case 4: // Agressize Auto
    			subMenuData[7] = 2;
    			autoAgressivity = 1;
                break;
    	}
    }

	// todo: implement internal process

	CopyScreen(workVideoBuffer, frontVideoBuffer);
	os_flip(frontVideoBuffer);
}

