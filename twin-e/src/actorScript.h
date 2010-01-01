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

void foundObject(int objectNumber);
void MyDial(int index);
int GetAngle(int X1, int Y1, int X2, int Y2);
void processInGameMenu(int index);
void OpenDial(int textNumber);
void Draw3dObject(int X, int Y, char* objectPtr, int rotation, int param);

extern int numOfOptionsInChoice;
extern short int inGameMenuData[10];

extern int inGameMenuAnswer;

extern short int objectRotation[255];

extern int DoTrackVar1;

