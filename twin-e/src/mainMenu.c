#include "lba.h"

void LBA_engine::mainMenu(void)
{
    short int var4;
    int temp;
    int temp2;
    int temp3;

   // FILE* saveGame;

    mainMenu2();
    var4 = 0;
    copyToBuffer(videoBuffer1, videoBuffer2);

    if (var4 != 0)
	return;

    do
	{
	    loadTextBank(0);
	    playCDtrack(9);
	    mainMenu2();
	    printString(49, mainMenuVar1);
	    temp = processMenu(mainMenuData);

	    if (temp == 20)
		{
		    if (enterPlayerName(42))
			{
			   /*
			    * do // Cette boucle essai des noms de fichiers jusqu'a trouver un nom de fichier
			    * vide. Probleme si on a plus de 9999 sauvegarde dans le meme repertoire {
			    * saveGameFileName[0]=mainMenuVar3; saveGameFileName[1]=0; // pas dans le code
			    * original.. // random pour avoir un num entre 0000 et 9999
			    * strcat(saveGameFileName,mainMenu7(10000*rand()));
			    * strcat(saveGameFileName,".LBA"); saveGame=fileOpen(saveGameFileName);
			    * }while(!saveGame);
			    */
			    reinitAll(1);
			    newGame();
			    if (mainLoop())
				{
				    mainMenuVar4 = 1;
				    temp2 = shadowMode;

				    temp3 = languageCD1;
				    languageCD1 = 0;
				    shadowMode = 0;
				    reinitAll(1);
				    currentRoom = 119;
				    needChangeRoom = 119;
				    mainLoop();
				    mainMenuVar4 = 0;
				    languageCD1 = temp3;
				    shadowMode = temp2;
				    resetVideoBuffer1();
				    osystem->drawBufferToScreen(videoBuffer1);
				    playFLA("The_end");
				    resetVideoBuffer1();
				    osystem->drawBufferToScreen(videoBuffer1);
				    osystem->setPalette(menuPalRGBA);
				}
			    copyToBuffer(videoBuffer1, videoBuffer2);
			    do
				{
				    do
					{
					}
				    while (key1 != 0);
				}
			    while (skipIntro != 0);
			}
		}
	    else if (temp == 21)
		{
		    if (choosePlayerName(21))
			{
			    reinitAll(-1);
			    newGame();
			    if (mainLoop())
				{
				    mainMenuVar4 = 1;
				    temp2 = shadowMode;
				    temp3 = languageCD1;
				    languageCD1 = 0;
				    shadowMode = 0;
				    reinitAll(1);
				    currentRoom = 119;
				    needChangeRoom = 119;
				    mainLoop();
				    mainMenuVar4 = 0;
				    languageCD1 = temp3;
				    shadowMode = temp2;
				    resetVideoBuffer1();
				    osystem->drawBufferToScreen(videoBuffer1);
				    playFLA("The_end");

				    resetVideoBuffer1();
				    osystem->drawBufferToScreen(videoBuffer1);
				    osystem->setPalette(menuPalRGBA);
				}
			    copyToBuffer(videoBuffer1, videoBuffer2);
			    do
				{
				    do
					{
					    readKeyboard();
					}
				    while (key1 != 0);
				}
			    while (skipIntro != 0);
			}
		}
	    else if (temp == 22)
		{
		    var4 = 1;
		}
	    else if (temp == 23)
		{
		    copyToBuffer(videoBuffer2, videoBuffer1);
		    osystem->drawBufferToScreen(videoBuffer1);
		    soundMenuData[5] = 26;
		    optionMenu();
		}
	    else if (temp == 9999)
		{
		   // playSubVideo();

		    loadImageToPtr("ress.hqr", videoBuffer2, 14);
		    copyToBuffer(videoBuffer2, videoBuffer1);

		    resetPalette();
		    osystem->drawBufferToScreen(videoBuffer1);
		    fadeIn2((char *) menuPalRGBA);
		}
	}
    while (!var4);
}

void LBA_engine::mainMenu2(void)
{
   // todo: implement this
}

int LBA_engine::choosePlayerName(int param)
{
    return (0);
}

char *LBA_engine::printString(int a, char *b)
{
    if (a == mainMenu3Var1)
	{
	    if (mainMenu3Var2 == currentTextBank)
		{
		    strcpy(b, mainMenu3Var3);
		    return (b);
		}
	}
    if (!findString(a))
	{
	    b[0] = 0;		// si on a pas trouve le text...
	    return (0);
	}

    if ((currentTextLength - 1) > 0xFF)
	currentTextLength = 0xFF;

    copyStringToString((char *) currentTextPtr, b, currentTextLength);	// on copie dans b
   // *(currentTextPtr+currentTextLength)=0; // on ajoute un \n a la fin
    currentTextLength++;	// on augmente la taille de la string de 1
    copyStringToString(b, mainMenu3Var3, currentTextLength);	// enfin, on copie le tout dan
   // mainMenu3Var3

    mainMenu3Var1 = a;
    mainMenu3Var2 = currentTextBank;

    return (b);

}

int LBA_engine::processMenu(short int *menuData)
{
    int localTime;
    int numEntry;
    int buttonNeedRedraw;
    int maxButton;
    short int *localData = menuData;
    short int currentButton;
    short int temp3;
    int musicChanged;
    int buttonReleased = 1;

    musicChanged = 0;

    buttonNeedRedraw = 1;

    loadImageToPtr("ress.hqr", bufSpeak, 51);

    numEntry = localData[1];
    currentButton = localData[0];
    localTime = time;
    maxButton = numEntry - 1;

    do
	{
	    if (localData == mainMenuData)	// si on est dans le menu principal
		{
		    if (time - localTime <= 11650)
			{
			    if (skipIntro == 46)
				if (key1 != 32)
				    return (9999);
			}
		    else
			{
			    return (9999);
			}
		}

	    if (printTextVar12 == 0)
		{
		    buttonReleased = 1;
		}

	    if (buttonReleased)
		{
		    key = printTextVar12;

		    if (((byte) key & 2))	// on passe au bouton d'en dessous
			{
			    currentButton++;
			    if (currentButton == numEntry)
				{
				    currentButton = 0;	// on se retrouve au dessus
				}
			    buttonNeedRedraw = 1;
			    buttonReleased = 0;
			}

		    if (((byte) key & 1))	// on passe au bouton d'au dessus
			{
			    currentButton--;
			    if (currentButton < 0)
				{
				    currentButton = maxButton;	// on se retrouve au dessous
				}
			    buttonNeedRedraw = 1;
			    buttonReleased = 0;
			}

		    temp3 = *(localData + currentButton * 4 + 8);	// on recupere le parametre du
		   // bouton
		    if (temp3 <= 5)	// si c'est un bouton de volume
			{
			    switch (temp3)
				{
				case 1:
				   {
				   }
				case 2:
				   {
				   }
				case 3:
				   {
				   }
				case 4:
				   {
				   }
				case 5:
				   {
				   }
				}
			}
		}

	    if (buttonNeedRedraw == 1)
		{
		    *localData = currentButton;

		    drawButton(localData, 0);	// le bouton actuel
		    do
			{
			    readKeyboard();
			    drawButton(localData, 1);
			}
		    while (printTextVar12 == 0 && key1 == 0 && skipIntro == 0);
		    buttonNeedRedraw = 0;
		}
	    else
		{
		    if (musicChanged)
			{
			   // todo: update des volumes...
			}

		    buttonNeedRedraw = 0;
		    drawButton(localData, 1);
		    readKeyboard();
		}

	}
    while (!(key1 & 2) && !(key1 & 1));

    currentButton = *(localData + 5 + currentButton * 2);	// on recupere le point de sortie

    readKeyboard();

    return (currentButton);
}

void LBA_engine::drawSelectableLetters(void)
{
    char x, y;

    for (x = 0; x < 5; x++)
	for (y = 0; y < 14; y++)
	    drawSelectableLetter(x, y, 0);
}

void LBA_engine::drawSelectableLetter(int x, int y, int arg)
{
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

    if (arg != 0)
	{
	    drawBlackBox(left, top, right, bottom, 91);
	}
    else
	{
	    blitRectangle(left, top, right, bottom, (char *) videoBuffer2, left, top,
			 (char *) videoBuffer1);
	    right2 = right;
	    drawBoxInsideTrans(left, top, right2, bottom, 4);
	}

    drawBoxOutLine(left, top, right, bottom);
    right2 = right;

    setTextColor(15);

    printStringSimple(centerX - getStringLength(buffer) / 2, centerY - 18, buffer);

    osystem->refresh(videoBuffer1, left, top, right2, bottom);

}

int LBA_engine::enterPlayerName(short int param)
{
    char buffer[256];

    short int a, b, c, e;
    char key;

    e = 1;
    a = -1;
    b = 0;
    c = 0;

    copyToBuffer(videoBuffer2, videoBuffer1);
    osystem->drawBufferToScreen(videoBuffer1);
    loadTextBank(0);
    printString(param, buffer);
    setTextColor(15);
    printStringSimple(320 - (getStringLength(buffer) / 2), 20, buffer);
    osystem->refresh(videoBuffer1, 0, 0, 639, 99);
    playerName[0] = enterPlayerNameVar1;
   // drawSmallButton(320,100,playerName,1);
    drawSelectableLetters();

    do
	{
	    readKeyboard();
	}
    while (skipIntro == 0 && key1 == 0);

    do
	{
	    key = (char) getKeyboardChar();
	}
    while (!key);

   // todo: implement this

    enterPlayerNameVar2 = 0;
    copyToBuffer(videoBuffer2, videoBuffer1);
    osystem->drawBufferToScreen(videoBuffer1);

    return (1);

    return (c);
}

char *LBA_engine::getStringFromNum(int valeur)
{
   // toto: implement this
    char *text;

    text = (char *) malloc(256);
    sprintf(text, "%d", valeur);

    return (text);
   // return("test");
}

void LBA_engine::drawButton(short int *data, int a)
{
    int buttonNumber;
    int maxButton;

   // int var4;
    short int *localData = data;
    int temp;
    unsigned char temp2;
    unsigned char temp3;
    char currentButton;

    buttonNumber = *localData;
    localData += 1;
    maxButton = *localData;
    localData += 1;
    temp = *localData;
    localData += 2;

    if (temp == 0)
	{
	    temp = 35;
	}
    else
	{
	    temp = temp - (((maxButton - 1) * 6) + ((maxButton) * 50)) / 2;
	}

    if (maxButton <= 0)
	{
	    return;

	}

    currentButton = 0;

    do
	{
	    temp2 = (unsigned char) *localData;
	    localData += 1;
	    temp3 = (unsigned char) *localData;
	    localData += 1;
	    if (a != 0)
		{

		    if (currentButton == buttonNumber)
			{
			    drawButtonGFX(320, temp, temp2, temp3, 1);

			}
		}
	    else
		{
		    if (currentButton == buttonNumber)
			{
			    drawButtonGFX(320, temp, temp2, temp3, 1);
			}
		    else
			{
			    drawButtonGFX(320, temp, temp2, temp3, 0);
			}
		}

	    currentButton++;
	    temp += 56;
	}
    while (currentButton < maxButton);
}

void LBA_engine::drawButtonGFX(int largeur, int posY, int c, int d, int mode)
{

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
		    switch (c)
			{
			case 1:
			   {
			      // musicVolumeRemaped=remapComposante(left,right,255,musicVolume);
			   }
			case 2:
			   {
			      // waveVolumeRemaped=remapComposante(left,right,255,waveVolume);

			   }
			case 3:
			   {
			      // CDVolumeRemaped=remapComposante(left,right,255,CDVolume);
			   }
			case 4:
			   {
			      // lineVolume=remapComposante(left,right,255,lineVolume);
			   }
			case 5:

			   {
			      // masterVolume=remapComposante(left,right,255,masterVolume);
			   }
			};
		}
	    else
		{
		    makeFireEffect(top, 64);
		    if (!(rand() % 5))
			{
			    bufSpeak[rand() % 320 * 10 + 6400] = 255;
			}
		}

	    if (c <= 5 && c >= 1)
		{
		   // implement this
		}

	}
    else
	{
	    blitRectangle(left, top, right, bottom, (char *) videoBuffer2, left, top,
			 (char *) videoBuffer1);

	    drawBoxInsideTrans(left, top, right, bottom2, 4);
	}

    drawBoxOutLine(left, top, right, bottom);
    setTextColor(15);
    printString(d, buttonText);
    stringLength = getStringLength(buttonText);
    printStringSimple(largeur - (stringLength / 2), posY - 18, buttonText);

   // todo: implementer les boutons de volume...

    osystem->refresh(videoBuffer1, left, top, right, bottom);

}

void LBA_engine::blitRectangle(int left, int top, int right, int bottom, char *source, int leftDest, int topDest, char *dest)	// pout 
																// 
																// 
																// 
																// 
																// 
																// 
																// 
																// 
																// 
																// 
																// 
																// les 
																// 
																// 
																// 
																// 
																// 
																// 
																// 
																// 
																// 
																// 
																// 
																// boutons 
																// 
																// 
																// 
																// 
																// 
																// 
																// 
																// 
																// 
																// 
																// 
																// actifs
{
   // int localBottom=bottom;

    int largeur;
    int hauteur;

    char *s;
    char *d;

    int interligne;
    int temp3;
    int i;

    s = screenLockupTable[top] + source + left;
    d = screenLockupTable[topDest] + dest + leftDest;

    largeur = right - left + 1;

    hauteur = bottom - top + 1;

    interligne = largeurEcran - largeur;
    temp3 = left;

    left >>= 2;
    temp3 &= 3;

    while (--hauteur)
	{
	    for (i = 0; i < largeur; i++)
		{
		    *(d++) = *(s++);
		}

	    d += interligne;
	    s += interligne;
	}
}

void LBA_engine::drawBoxInsideTrans(int left, int top, int right, int bottom, int mode)
{

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

    pos = screenLockupTable[top] + videoBuffer1 + left;
    hauteur2 = hauteur = bottom - top;
    hauteur2++;

    largeur = right - left + 1;

    temp = largeurEcran - largeur;
    localMode = mode;

    do
	{
	    var1 = largeur;
	    do
		{
		    color2 = color = *pos;
		    color2 &= 0xF0;
		    color &= 0x0F;
		    if (color < localMode)
			color = color2;
		    else
			color += color2 - localMode;
		    *pos++ = color;
		    var1--;
		}
	    while (var1 > 0);
	    pos += temp;
	    hauteur2--;
	}
    while (hauteur2 > 0);
}

void LBA_engine::drawBoxOutLine(int left, int top, int right, int bottom)
{

    --bottom;			// FIXME ! je devrais pas avoir ces 2 lignes.
    --right;
    launchStringProcess(left, top, right, top, 79);	// ligne du haut
    launchStringProcess(left, top, left, bottom, 79);	// ligne de gauche
    launchStringProcess(right, ++top, right, bottom, 73);	// ligne de droite
    launchStringProcess(++left, bottom, right, bottom, 73);	// ligne du bas
}

void LBA_engine::launchStringProcess(int a, int b, int c, int d, int e)
{
    drawLine(a, b, c, d, e);
}

// a=x1
// b=y1
// c=x2
// d=y2;
void LBA_engine::drawLine(int a, int b, int c, int d, int e)
{
    int temp;
    short int flag;
    int flag2;
    unsigned char *out;
    short int color;
    short int var2;
    short int xchg;

    stringProcessVar = e;

    if (a > c)			// pour toujours dessiner de gauche à droite
	{
	    temp = c;
	    c = a;
	    a = temp;

	    temp = d;
	    d = b;
	    b = temp;

	}

    flag = 0;

    if (a < textWindowLeft)
	{
	    flag |= 1;
	}
    else
	{
	    if (a > textWindowRight)
		return;
	}
    if (b < textWindowTop)
	{
	    flag |= 8;
	}
    else
	{
	    if (b > textWindowBottom)
		flag |= 4;
	}

    flag <<= 8;

    if (c < textWindowLeft)
	return;
    if (c <= textWindowLeft)
	flag |= 2;

    if (d < textWindowTop)
	{
	    flag |= 8;
	}
    else
	{
	    if (d > textWindowBottom)
		flag |= 4;
	}

    flag2 = flag;

    if (flag)
	return;

   // implementer la suite

    flag2 = 640;		// esi
    c -= a;
    d -= b;
    if (d < 0)
	{
	    flag2 = -flag2;
	    d = -d;
	}

    out = videoBuffer1 + screenLockupTable[b] + a;

    color = stringProcessVar;
    if (c < d)			// pente importante
	{
	    xchg = c;
	    c = d;
	    d = xchg;
	    var2 = c;
	    var2 <<= 1;
	    b = c;
	    d <<= 1;
	    c++;
	    do
		{
		    *out = (unsigned char) color;
		    b -= d;
		    if (b > 0)
			{
			    out += flag2;
			}
		    else
			{
			    b += var2;
			    out += flag2 + 1;
			}
		}
	    while (--c);
	}
    else			// pente reduite
	{
	    var2 = c;
	    var2 <<= 1;
	    b = c;
	    d <<= 1;
	    c++;
	    do
		{
		    *out = (unsigned char) color;
		    out++;
		    b -= d;
		    if (b < 0)
			{
			    b += var2;
			    out += flag2;
			}
		}
	    while (--c);
	}
}

int LBA_engine::getStringLength(char *string)
{
    stringLenght = 0;
    unsigned char caractere;

    do
	{
	    caractere = (unsigned char) *(string++);

	    if (caractere == 0)
		break;
	    if (caractere == 0x20)
		{
		    stringLenght += spaceLenght;
		}

	    else
		{
		    stringLenght += interCharSpace;	// espace inter lettre
		    stringLenght += *(fntFont + *((short int *) (fntFont + caractere * 4)));
		}
	}
    while (1);

    return (stringLenght);
}

void LBA_engine::copyStringToString(char *a, char *b, int c)
{
    int i;

    for (i = 0; i < c; i++)
	*(b++) = *(a++);
}

int LBA_engine::optionMenu(void)
{
    byte temp;
    byte temp2 = 0;

    copyToBuffer(videoBuffer1, videoBuffer2);
    mainMenu2();

    playCDtrack(9);		// warning... Truc pas trop gere avec BX là...

    do
	{
	    temp = processMenu(soundMenuData);

	    if (temp == 15 || temp == 26)
		{
		    temp2 = 1;
		}
	    else if (temp == 30)
		{
		    copyToBuffer(videoBuffer2, videoBuffer1);
		    osystem->drawBufferToScreen(videoBuffer1);
		   // volumeMenu();
		}
	    else if (temp == 46)
		{

		    copyToBuffer(videoBuffer2, videoBuffer1);
		    osystem->drawBufferToScreen(videoBuffer1);
		   // saveManipMenu();

		}
	    else if (temp == 47)
		{
		    copyToBuffer(videoBuffer2, videoBuffer1);
		    osystem->drawBufferToScreen(videoBuffer1);
		    optionMenu2();
		}
	}
    while (temp2 != 1);

    copyToBuffer(videoBuffer2, videoBuffer1);
    osystem->drawBufferToScreen(videoBuffer1);

    return (0);
}

void LBA_engine::optionMenu2(void)
{
    byte temp;
    byte quit = 0;

    copyToBuffer(videoBuffer1, videoBuffer2);

    do
	{
	    temp = processMenu(subMenuData);

	    if (temp == 33)
		{
		    subMenuData[13] = 233;
		   // zoomMode=0;
		}
	    else if (temp == 231)
		{
		    subMenuData[9] = 31;
		}
	    else if (temp == 232)
		{
		    subMenuData[11] = 32;
		    shadowMode = 2;
		}
	    else if (temp == 233)
		{
		    subMenuData[13] = 33;
		   // zoomMode=1;
		}
	    else if (temp == 131)
		{
		    subMenuData[9] = 231;
		}
	    else if (temp == 132)
		{
		    subMenuData[11] = 232;
		    shadowMode = 0;
		}
	    else if (temp == 26)
		{
		    quit = 1;

		}
	    else if (temp == 31)
		{
		    subMenuData[9] = 131;
		}
	    else if (temp == 32)
		{
		    subMenuData[11] = 132;
		    shadowMode = 1;
		}
	    else if (temp == 2)
		{
		    subMenuData[7] = 4;
		    agressivity = 0;
		}
	    else if (temp == 4)
		{
		    subMenuData[7] = 2;
		    agressivity = 1;
		}

	}
    while (!quit);

   // todo: implementer le process interne

    copyToBuffer(videoBuffer2, videoBuffer1);
    osystem->drawBufferToScreen(videoBuffer1);
}
