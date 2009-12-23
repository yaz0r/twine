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
#include "vox.h"
#include "font.h"
#include "mainLoop.h"
#include "main.h"
#include "hqr.h"

#include "text.h"

short int pt8s4[96];

int progressiveTextStartColor;
int progressiveTextStopColor;
int progressiveTextStepSize;
int progressiveTextBufferSize;

int actualDial = -1;
int languageCD1 = 0;

int printTextVar5;
int printTextVar13;

int dialogueBoxLeft = 16;
int dialogueBoxTop = 334;
int dialogueBoxRight = 623;
int dialogueBoxBottom = 463;
int dialogueBoxParam1 = 3;
int dialogueBoxParam2 = 591;

int flagDisplayText = 1;

char *printText8Ptr2;

int printText8Var1;
int printText8Var3;

int printText8Var5;
int printText8Var6;
char *printText8Var8;

int TEXT_CurrentLetterX;
int TEXT_CurrentLetterY;

int printText10Var1;

int spaceLength;

char buf1[256];
char buf2[256]; /* TODO: check size */

int addLineBreakX;

int printText8PrepareBufferVar2;

int wordSizeChar;
int wordSizePixel;

char spaceChar = ' ';

int textWindowTop = 0;
int textWindowTopSave = 0;
int textWindowLeftSave = 0;
int textWindowLeft = 0;
int textWindowRight = 639;
int textWindowRightSave = 639;
int textWindowBottom = 479;
int textWindowBottomSave = 479;

int currentTextLength;
char *currentTextPtr;

short int numTextEntry;

void InitDial(int index) {
	int bundleEntryPoint;
	int size;
	int textSize;

	if (index == actualDial)
		return;

	actualDial = index;

	bundleEntryPoint = (LANGUAGE * 14) * 2  + index * 2;

	size = Load_HQR("text.hqr", (byte *) bufOrder, bundleEntryPoint);

	numTextEntry = size / 2;

	bundleEntryPoint++;

	textSize = Load_HQR("text.hqr", (byte *) bufText, bundleEntryPoint);

	if (languageCD1 != 0)
		loadVox();

	return;
}

void printTextFullScreen(int textIndex) {
	int isVoxSet = 0;
	int temp2;
	int temp3 = 0;

	saveTextWindow();
	UnSetClip();
	CopyScreen(frontVideoBuffer, workVideoBuffer);

	if (languageCD1 != 0)
		isVoxSet = setVoxFileAtDigit(textIndex);

	if (!flagDisplayText && isVoxSet) { // si on doit player le Vox sans afficher le text
		while (temp2 && os_isPressed(KEY_SKIP))
			temp2 = printText4(voxFileHandle);

		printTextVar5 = 0;

		if (!languageCD1 || !voxFileHandle) {
			loadSavedTextWindow();
			return;
		}
		// todo: recheck, je pense avoir oublie des truc la..

		if (!printText6(4660)) {
			loadSavedTextWindow();
			return;
		}

		printText7(4660);

		loadSavedTextWindow();
		return;
	}

	initText(textIndex);  // prépare le text
	InitDialWindow();

	temp3 = 0;
	while (!temp3) {      // debut de la boucle d'affichage du text+sons
		waitRetrace();
		isVoxSet = temp2 = printText10(); // on doit player le son ?
		
		printText4(voxFileHandle);

		if (isVoxSet == 2) {
			while (!os_isPressed(KEY_CONTTEXT))
				printText4(voxFileHandle);
			while (!os_isPressed(KEY_CONTTEXT))
				printText4(voxFileHandle);
		}

		if (os_isPressed(KEY_SKIP)) {
			temp3 = 1;
			break;
		}

		if (!temp2)
			if (!printText11())
				break;
	}

	printTextVar5 = 0;

	if (languageCD1 != 0 && voxFileHandle != 0 && printText6(4660) != 0)
		printText7(4660);

	printTextVar13 = 0;

	if (temp2 != 0) {
		loadSavedTextWindow();
		return;
	}

	if (temp3 != 0) {
		loadSavedTextWindow();
		return;
	}

	while (!os_isPressed(KEY_CONTTEXT))
		;

	while (!os_isPressed(KEY_CONTTEXT))
		loadSavedTextWindow();

	loadSavedTextWindow();
	return;
}

void saveTextWindow(void) {
	textWindowLeftSave = textWindowLeft;
	textWindowTopSave = textWindowTop;
	textWindowRightSave = textWindowRight;
	textWindowBottomSave = textWindowBottom;
}

void UnSetClip(void) {
	textWindowTop = textWindowLeft = 0;
	textWindowRight = WINDOW_X - 1;
	textWindowBottom = WINDOW_Y - 1;
}

int printText4(FILE * fileHandle) {
	int result;

	if (!languageCD1)
		return (0);

	if (!fileHandle)
		return (0);

	result = printText6(4660);

	if (result)
		return (1);

	if (printTextVar5 == 0)
		return (result);

	readBufferSpeak(fileHandle);
	return (1);
}

void readBufferSpeak(FILE * fileHandle) {
	int a, b, c;

	fread(&b, sizeof(int), 1, fileHandle);
	fread(&c, sizeof(int), 1, fileHandle);
	fread(&a, sizeof(short int), 1, fileHandle);

	// todo: implementer la suite
}

void loadSavedTextWindow(void) {
	textWindowLeft = textWindowLeftSave;
	textWindowTop = textWindowTopSave;
	textWindowRight = textWindowRightSave;
	textWindowBottom = textWindowBottomSave;
}

int printText6(/* int var */) {
	// todo: implement this
	// sound function
	return (0);
}

int printText7(/*int var*/) {
	// todo: implement this
	// sound function
	return (0);
}

int initText(int var) {
	printTextVar13 = 0;

	if (!findString(var))
		return (0);

	printText8Ptr2 = buf2;

	printTextVar13 = 1;

	printText8Var1 = 0;
	buf1[0] = 0;
	buf2[0] = 0;
	printText8Var3 = 0;
	TEXT_CurrentLetterX = dialogueBoxLeft + 8;
	printText8Var5 = 0;
	printText8Var6 = 0;
	TEXT_CurrentLetterY = dialogueBoxTop + 8;
	printText8Var8 = currentTextPtr;

	SetFont(lbaFont, 2, 7);

	return (0);
}

void InitDialWindow(void) {
	blitRectangle(dialogueBoxLeft, dialogueBoxTop, dialogueBoxRight, dialogueBoxBottom, (char *) workVideoBuffer, dialogueBoxLeft, dialogueBoxTop, (char *) frontVideoBuffer);
	if (newGameVar4 != 0) {
		DrawCadre(dialogueBoxLeft, dialogueBoxTop, dialogueBoxRight, dialogueBoxBottom);
		drawBoxInsideTrans(dialogueBoxLeft + 1, dialogueBoxTop + 1, dialogueBoxRight - 1, dialogueBoxBottom - 1, 3);
	}

	os_copyBlockPhys(dialogueBoxLeft, dialogueBoxTop, dialogueBoxRight, dialogueBoxBottom);

	printText8Var3 = 0;

	blitRectangle(dialogueBoxLeft, dialogueBoxTop, dialogueBoxRight, dialogueBoxBottom, (char *) frontVideoBuffer, dialogueBoxLeft, dialogueBoxTop, (char *) workVideoBuffer);
}

int printText10(void) {

	int a;
	int b;

	if (printTextVar13 == 0)
		return (0);

	if (*printText8Ptr2 == 0) {
		if (printText8Var5 != 0) {
			if (newGameVar5 != 0)
				printText10Sub();
			printTextVar13 = 0;
			return (0);
		}
		if (printText8Var6 != 0) {
			blitRectangle(dialogueBoxLeft, dialogueBoxTop, dialogueBoxRight, dialogueBoxBottom, (char *) workVideoBuffer, dialogueBoxLeft, dialogueBoxTop, (char *) frontVideoBuffer);
			os_copyBlockPhys(dialogueBoxLeft, dialogueBoxTop, dialogueBoxRight, dialogueBoxBottom);
			printText8Var3 = 0;
			printText8Var6 = 0;
			TEXT_CurrentLetterX = dialogueBoxLeft + 8;
			TEXT_CurrentLetterY = dialogueBoxTop + 8;
		}

		if (*printText8Var8 == 0) {
			initProgressiveTextBuffer();
			printText8Var5 = 1;
			return (1);
		}
		processTextLine();
	}

	if (*printText8Ptr2 == 0)
		return (1);

	printText8Sub4(TEXT_CurrentLetterX, TEXT_CurrentLetterY, *printText8Ptr2);
	printText10Sub2();    // fonction responsable de l'affichage du text

	TEXT_GetLetterSize(*printText8Ptr2, &a, &b, lbaFont);

	if (*printText8Ptr2 != 0x20) {
		TEXT_CurrentLetterX += a + 2;
	} else {
		if (printText10Var1 != 0) {
			TEXT_CurrentLetterX++;
			printText10Var1--;
		}
		TEXT_CurrentLetterX += spaceLength;
	}

	printText8Ptr2++;   // on passe au caractere suivant ?

	if (*printText8Ptr2 != 0)
		return (1);

	TEXT_CurrentLetterY += 38;
	TEXT_CurrentLetterX = dialogueBoxLeft + 8;
	if (printText8Var6 == 1)
		if (printText8Var5 == 0) {
			printText10Sub();
			return (2);
		}

	printText8Var1++;
	if (printText8Var1 < dialogueBoxParam1)
		return (1);

	initProgressiveTextBuffer();
	printText8Var6 = 1;

	if (*printText8Var8 == 0)
		printText8Var5 = 1;

	return (1);
}

void initProgressiveTextBuffer(void) {
	int i = 0;

	buf2[0] = 0;

	while (i < progressiveTextBufferSize) {
		strcat(buf2, " ");
		i++;
	};

	printText8Ptr2 = buf2;
	addLineBreakX = 16;
	printText8Var1 = 0;
}

void printText10Sub2(void) {
	int currentLetter;
	int currentIndex;
	int counter;
	int counter2;
	short int *ptr;

	currentLetter = printText8Var3;
	currentLetter--;

	currentIndex = currentLetter * 3;

	ptr = pt8s4 + currentIndex;

	// todo: gerer le delay ici...

	counter = printText8Var3;
	counter2 = progressiveTextStartColor;

	while (--counter >= 0) {
		CoulFont(counter2);
		drawDoubleLetter(*(ptr + 1), *(ptr + 2), *ptr, counter2);
		counter2 -= progressiveTextStepSize;
		if (counter2 > progressiveTextStopColor)
			counter2 = progressiveTextStopColor;
		ptr -= 3;
	};

}

void drawDoubleLetter(int a, int b, int c, int d) {
	int left, top, right, bottom;

	if (c == 0x20)
		return;

	/* Grey color */
	CoulFont(0);
	drawCharacter(a + 2, b + 4, c); // le caractere derriere en gris

	/* Real color */
	CoulFont(d);
	drawCharacter(a, b, c);

	left = a;
	top = b;
	right = a + 32;
	bottom = b + 38;

	os_copyBlockPhys(left, top, right, bottom);
}

void TEXT_GetLetterSize(byte character, int *pLetterWidth, int *pLetterHeight, byte * pFont) {
	byte *temp;

	temp = pFont + convertWFromLE(*((short int *)(pFont + character * 4)));
	*pLetterWidth = *(temp);
	*pLetterHeight = *(temp + 1);
}

void printText8Sub4(short int a, short int b, short int c) {
	int temp;
	int counter2 = 0;
	int var1;
	int var2;

	if (printText8Var3 < 32) {
		temp = printText8Var3 * 3;
		pt8s4[temp] = c;
		pt8s4[temp+1] = a;
		pt8s4[temp+2] = b;

		printText8Var3++;
	} else {
		while (counter2 < 31) {
			var1 = (counter2 + 1) * 3;
			var2 = counter2 * 3;
			pt8s4[var2] = pt8s4[var1];
			pt8s4[var2+1] = pt8s4[var1+1];
			pt8s4[var2+2] = pt8s4[var1+2];
			counter2++;
		};
		pt8s4[93] = c;
		pt8s4[94] = a;
		pt8s4[95] = b;
	}
}

void printText10Sub(void) {
	vertexCoordinates[0] = progressiveTextStopColor;  // les 3 vertex ?
	vertexCoordinates[1] = dialogueBoxRight - 3;
	vertexCoordinates[2] = dialogueBoxBottom - 24;
	vertexCoordinates[3] = progressiveTextStopColor;
	vertexCoordinates[4] = dialogueBoxRight - 24;
	vertexCoordinates[5] = dialogueBoxBottom - 3;
	vertexCoordinates[6] = progressiveTextStartColor;
	vertexCoordinates[7] = vertexCoordinates[1];
	vertexCoordinates[8] = vertexCoordinates[5];

	FillVertic_AType = 0;
	numOfVertex = 3;

	ComputePoly();
	FillVertic(FillVertic_AType, progressiveTextStopColor);

	os_copyBlockPhys(dialogueBoxRight - 24, dialogueBoxBottom - 24, dialogueBoxRight - 3, dialogueBoxBottom - 3);

}

void FillVertic(int arg_0, int arg_4) {
	FillVertic_A(arg_0, arg_4);
}

void ComputePoly(void) {
	pRenderV1 = vertexCoordinates;
	ComputePoly_A();
}

int findString(int index) {
	int temp = 0;
	int temp2 = 0;
	int nEntry;

	int ptrCurrentEntry;
	int ptrNextEntry;

	short int *localTextBuf = (short int *) bufText;
	short int *localOrderBuf = (short int *) bufOrder;

	nEntry = numTextEntry;  // nombre d'entrées dans la bank

	goto e2;

e1:

	temp2 = 0;
	temp2 = convertWFromLE(*(localOrderBuf));
	localOrderBuf++;
	if (temp2 == index)
		goto e3;
	temp++;

e2:

	temp2 = 0;
	temp2 = nEntry;
	if (temp < temp2)
		goto e1;
	temp = -1;

e3:

	if (temp != -1)
		goto ex;
	temp = 0;
	numTextEntry = nEntry;

	return (0);

ex:

	ptrCurrentEntry = convertWFromLE(localTextBuf[temp]); // entrée courante
	ptrNextEntry = convertWFromLE(localTextBuf[temp + 1]);  // entrée d'apres

	currentTextPtr = (bufText + ptrCurrentEntry);

	currentTextLength = ptrNextEntry - ptrCurrentEntry; // largeur de l'entre courante

	numTextEntry = nEntry;

	return (1);
}

int printText11(void) {
	return (printText4(voxFileHandle));
}

void processTextLine(void) {
	short int var4;
	char *buffer;
	char *temp;

	buffer = printText8Var8;
	spaceLength = 7;
	var4 = 1;

	addLineBreakX = 0;
	printText8PrepareBufferVar2 = 0;
	buf2[0] = 0;

	for (;;)
	{
		if (*buffer == 0x20) {
			buffer++;
			continue;
		}

		if (*buffer != 0) {
			printText8Var8 = buffer;
			getWordSize(buffer, buf1);
			if (addLineBreakX + spaceLength + wordSizePixel < dialogueBoxParam2) {
				temp = buffer + 1;
				if (*buffer == 1) {
					var4 = 0;
					buffer = temp;
				} else {
					if (*buf1 == 0x40) {
						var4 = 0;
						buffer = temp;
						if (addLineBreakX == 0) {
							addLineBreakX = 7;
							*buf2 = spaceChar;
						}
						if (buf1[1] == 0x50) {
							printText8Var1 = dialogueBoxParam1;
							buffer++;
						}
					} else {
						buffer += wordSizeChar;
						printText8Var8 = buffer;
						strcat(buf2, buf1);
						strcat(buf2, " ");  // not 100% accurate
						printText8PrepareBufferVar2++;

						addLineBreakX += wordSizePixel + spaceLength;
						if (*printText8Var8 != 0) {
							printText8Var8++;
							continue;
						}
					}
				}
			}
		}
		break;
	}

	if (printText8PrepareBufferVar2 != 0)
		printText8PrepareBufferVar2--;

	if (*printText8Var8 != 0 && var4 == 1) {
		spaceLength += (dialogueBoxParam2 - addLineBreakX) / printText8PrepareBufferVar2;
		printText10Var1 = dialogueBoxParam2 - addLineBreakX - dialogueBoxParam2 - addLineBreakX;  // stupid... recheck
	}

	printText8Var8 = buffer;

	printText8Ptr2 = buf2;

}

void getWordSize(char *arg1, char *arg2) {
	int temp = 0;
	char *arg2Save = arg2;

	while (*arg1 != 0 && *arg1 != 1 && *arg1 != 0x20) {
		temp++;
		*arg2++ = *arg1++;
	}

	wordSizeChar = temp;
	*arg2 = 0;
	wordSizePixel = SizeFont(arg2Save);
}
