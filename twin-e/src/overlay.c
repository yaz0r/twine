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

#include "lba.h"

#include "hqr.h"

overlayObjectListStruct overlayObjectList[10];

void addOverlayObject(int arg1, int arg2, int arg3, int arg4, int arg5, int arg6, int lifeTime) {
	int i;

	for (i = 0;i < 10;i++) {
		if (overlayObjectList[i].field_0 == -1) {
			overlayObjectList[i].field_0 = arg2;
			overlayObjectList[i].X = arg3;
			overlayObjectList[i].Y = arg4;
			overlayObjectList[i].type = arg1;
			overlayObjectList[i].followedActor = arg5;
			overlayObjectList[i].positionType = arg6;
			overlayObjectList[i].timeToDie = lba_time + lifeTime * 50;
			return;
		}
	}
}

void updateOverlayObjectsPosition(short int arg_0, short int arg_4, short int arg_8, short int arg_C) {
	int i;
	short int var1;
	short int var2;

	var1 = arg_8 - arg_0;
	var2 = arg_C - arg_4;

	for (i = 0; i < 10; i++) {
		if (overlayObjectList[i].type == 1) {
			overlayObjectList[i].X = var1;
			overlayObjectList[i].Y = var2;
		}
	}
}
