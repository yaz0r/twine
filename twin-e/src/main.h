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

void newGame(void);
void newGame4(void);
void newGame2(void);
void TestCoulDial(short int param);
void CoulDial(int a, int b, int c);
void CoulFont(int i);

extern short int newGameVar4;
extern int newGameVar5;

extern unsigned char *frontVideoBuffer;
extern byte *workVideoBuffer;

extern byte *bufSpeak;
extern byte *bufMemoSeek;
extern char *bufText;
extern char *bufOrder;
extern byte *bufAni1;
extern byte *bufAni2;

extern byte *menuPal;
extern byte menuPalRGBA[1024];
extern byte *shadowSprite;
extern byte *spriteActorData;
extern byte *lbaFont;

extern byte *bufCube;

