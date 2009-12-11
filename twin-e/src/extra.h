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

void reinitExtraObjectList(void);
int FullWorldColBrick(int currentX, int currentZ, int currentY, int oldX, int oldZ, int oldY);
void GereExtras(void);
int ThrowExtra(int actorNum, int X, int Y, int Z, int sprite, int var2, int var3, int var4, int var5, int strength);
void InitSpecial(int var0, int var1, int var2, int var3);
void InitFly(extraListStruct* extraEntry, int var1, int var2, int var3, int var4);
void AffSpecial(int extraNum, int X, int Y);
void Aff2DShape(short int* extraData, int X, int Y, int param0, int time, int param1);
int ExtraBonus(int X, int Y, int Z, int param, int angle, int type, int param2);
int ExtraCheckObjCol(extraListStruct* extra, int param);
void ThrowMagicBall(int X, int Z, int Y, int param1, int angle, int param2, int param4);
int ExtraSearch(int actorNum, int X, int Z, int Y, int spriteIdx, int targetActor, int maxSpeed, int param4);

extern short int extraDataTable1[21];
extern short int extraDataTable2[37];

