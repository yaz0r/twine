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

#ifndef _OVERLAY_H_
#define _OVERLAY_H_

struct overlayObjectListStruct {
	short int field_0;
	short int X;
	short int Y;
	short int type;
	short int followedActor;
	short int positionType;
	int timeToDie;
};

typedef struct overlayObjectListStruct overlayObjectListStruct;

extern overlayObjectListStruct overlayObjectList[10];

void addOverlayObject(int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int lifeTime);
void updateOverlayObjectsPosition(short int arg_0, short int arg_4, short int arg_8, short int arg_C);

#endif
