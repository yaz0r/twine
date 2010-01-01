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

int mainLoop(void);
void reinitAll(int save);
void reinitAll1(void);
void TestRestoreModeSVGA(int arg_0);
void waitRetrace(void);
void mainLoop2sub1(void);
void freezeTime(void);
void unfreezeTime(void);
int makeGiveUpMenu(void);
void reinitAll3(void);
void reinitVars(void);
void Box(int a, int b, int c, int d, unsigned char e);
void DrawObj3D(short int arg_0, short int arg_4, short int arg_8, short int arg_C,
			   short int arg_10, short int arg_14, unsigned char *arg_18);
void SetClip(int left, int top, int right, int bottom);
void DoDir(int actorNum);
void DoAnim(int actorNum);
void HitObj(int actorAttacking, int actorAttacked, int param, int angle);
int CheckZvOnZv(int var0, int var1);
void Rotate(int initialX, int initialY, int angle);
int WorldColBrick(int var0, int var1, int var2);
void ReceptionObj(void);
int CheckObjCol(int param);
void ReajustPos(int param);
void DoCornerReadjustTwinkel(int var0, int var1, int var2, int var3);
void DoCornerReajust(int var0, int var1, int var2, int var3);
int BoundRegleTrois(int var0, int var1, int var2, int var3);
int WorldColBrickFull(int var0, int var1, int var2, int var3);
int mainLoopInteration(void);

extern short int selectedInventoryObj;
extern short int usingSword;
extern short int twinsenZBeforeFall;
extern short int disableScreenRecenter;

extern short int mainLoopVar5;
extern short int mainLoopVar6;
extern short int mainLoopVar7;

extern short int processActorVar5;
extern short int processActorVar6;

extern short int useAlternatePalette;

extern short int processActorX;
extern short int processActorY;
extern short int processActorZ;

extern short int currentlyProcessedActorNum;

extern int getPosVar1;
extern int getPosVar2;
extern int getPosVar3;

extern short int drawInGameTransBox;
extern short int twinsenMoved;

extern short int requestBackgroundRedraw;

extern short int lockPalette;

extern short int currentTextBank;

extern int newCameraX;
extern int newCameraZ;
extern int newCameraY;

extern short int currentY;
extern short int currentX;
extern short int currentZ;

extern time timeVar;

extern int action;

extern int time1;
extern int time3;

