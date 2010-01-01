/*
Copyright (C) 2002-2010 The TwinE team

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

void processTextLine(void);
void printTextFullScreen(int textIndex);
int printText7(/*int var*/);
int initText(int var);
void InitDialWindow(void);
int printText10(void);
int printText11(void);
void saveTextWindow(void);
void UnSetClip(void);
int printText4(FILE * var1);
void loadSavedTextWindow(void);
int printText6(/*int var*/);
void InitDial(int index);
int findString(int index);
void drawLetter2(int x, int y, int c);
void drawDoubleLetter(int a, int b, int c, int d);
void printText8Sub4(short int a, short int b, short int c);
void printText10Sub2(void);
void TEXT_GetLetterSize(byte character, int *pLetterWidth, int *pLetterHeight, byte * pFont);
void getWordSize(char *arg1, char *arg2);
void printText10Sub(void);
void readBufferSpeak(FILE * fileHandle);
void initProgressiveTextBuffer(void);

extern int progressiveTextStartColor;
extern int progressiveTextStopColor;
extern int progressiveTextStepSize;
extern int progressiveTextBufferSize;

extern int languageCD1;

extern int printText8Var3;
extern int printTextVar5;
extern int printTextVar13;

extern int dialogueBoxLeft;
extern int dialogueBoxTop;
extern int dialogueBoxRight;
extern int dialogueBoxBottom;
extern int dialogueBoxParam1;
extern int dialogueBoxParam2;

extern int flagDisplayText;

extern int textWindowTop;
extern int textWindowTopSave;
extern int textWindowLeftSave;
extern int textWindowLeft;
extern int textWindowRight;
extern int textWindowRightSave;
extern int textWindowBottom;
extern int textWindowBottomSave;

extern int currentTextLength;
extern char *currentTextPtr;

extern short int numTextEntry;

