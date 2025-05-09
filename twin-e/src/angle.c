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

void clearRealAngle(actor * ptr) {
	setActorAngleSafe(ptr->angle, ptr->angle, 0, &ptr->time);
}

void setActorAngle(short int from, short int to, short int numOfStep, time *ptr)
{
	ptr->from = from;
	ptr->to = to;
	ptr->numOfStep = numOfStep;
	ptr->timeOfChange = lba_time;
}

void setActorAngleSafe(short int arg_0, short int arg_4, short int arg_8, time *timePtr)
{
	timePtr->from = arg_0 & 0x3FF;
	timePtr->to = arg_4 & 0x3FF;
	timePtr->numOfStep = arg_8 & 0x3FF;
	timePtr->timeOfChange = lba_time;
}

int getRealAngle(time *angleData)
{
	int timePassed;
	int remainingAngle;

	if (angleData->numOfStep)
	{
		timePassed = lba_time - angleData->timeOfChange;

		if (timePassed >= angleData->numOfStep) {	// rotation is finished
			angleData->numOfStep = 0;
			return angleData->to;
		}

		remainingAngle = angleData->to - angleData->from;

		if (remainingAngle < -0x200)
			remainingAngle += 0x400;
		else if (remainingAngle > 0x200)
			remainingAngle -= 0x400;

		remainingAngle *= timePassed;
		remainingAngle /= angleData->numOfStep;
		remainingAngle += angleData->from;

		return remainingAngle;
	}

	return angleData->to;
}

int getRealValue(time *angleData)
{
	int tempAngle;

	if (!angleData->numOfStep)
		return (angleData->to);

	if (!(lba_time - angleData->timeOfChange < angleData->numOfStep)) {
		angleData->numOfStep = 0;
		return (angleData->to);
	}

	tempAngle = angleData->to - angleData->from;
	tempAngle *= lba_time - angleData->timeOfChange;
	tempAngle /= angleData->numOfStep;

	return (tempAngle + angleData->from);
}
