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

int chooseSave(/* int param */);
void MainGameMenu(void);
void DrawCadre(int left, int top, int right, int bottom);
void optionMenu2(void);
void drawSelectableLetters(void);
void drawSelectableLetter(int y, int x, int arg);
void drawBoxInsideTrans(int left, int top, int right, int bottom, int mode);
void blitRectangle(int left, int top, int right, int bottom, char *dest, int right2, int top2, char *source);
char *GetMultiText(int a, char *b);
void HQ_StopSample(void);
void copyStringToString(char *a, char *b, int c);
void drawButtonGFX(int largeur, int posY, int c, int d, int mode);
int processMenu(short int *menuData);
int enterPlayerName(short int param);
char *my_itoa(int valeur);
void drawButton(short int *data, int a);
int SizeFont(char *string);
int optionMenu(void);

extern short int subMenu2Data[8];
extern short int soundMenuData[12];
extern short int shadowMode;
extern short int autoAgressivity;


