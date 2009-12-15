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

#include "lba.h"

#include "anim.h"
#include "room.h"
#include "main.h"

#include "actors.h"

actor actors[100];
actor *twinsen = &actors[0];

void initActor(short int id)
{
	actor *lactor;

	lactor = &actors[id];

	if (lactor->staticFlagsBF.isSpriteActor) // if sprite actor
	{
		if (lactor->field_66 != 0)
			lactor->dynamicFlagsBF.bUnk0002 = 1;

		lactor->costumeIndex = -1;

		initSprite(lactor->field_8, id);

		setActorAngleSafe(0, 0, 0, &lactor->time);

		if (lactor->staticFlagsBF.isUsingClipping)
		{
			lactor->lastX = lactor->X;
			lactor->lastZ = lactor->Y;
			lactor->lastY = lactor->Z;
		}
	}
	else
	{
		lactor->costumeIndex = -1;

		InitBody(lactor->body, id);

		lactor->previousAnimIndex = -1;
		lactor->field_78 = 0;

		if (lactor->costumeIndex != -1)
			InitAnim(lactor->anim, 0, 255, id);

		setActorAngleSafe(lactor->angle, lactor->angle, 0, &lactor->time);
	}

	lactor->positionInMoveScript = -1;
	lactor->label = -1;
	lactor->positionInActorScript = 0;
}

void resetActor(short int id)
{
	actor *localActor;

	localActor = &actors[id];

	localActor->body = 0;
	localActor->anim = 0;
	localActor->X = 0;
	localActor->Y = -1;
	localActor->Z = 0;

	localActor->boudingBox.X.bottomLeft = 0;
	localActor->boudingBox.X.topRight = 0;
	localActor->boudingBox.Y.bottomLeft = 0;
	localActor->boudingBox.Y.topRight = 0;
	localActor->boudingBox.Z.bottomLeft = 0;
	localActor->boudingBox.Z.topRight = 0;

	localActor->angle = 0;
	localActor->speed = 40;
	localActor->comportement = 0;

	localActor->cropLeft = 0;
	localActor->cropTop = 0;
	localActor->cropRight = 0;
	localActor->cropBottom = 0;

	localActor->field_3 = 0;
	localActor->collision = -1;
	localActor->standOn = -1;
	localActor->zone = -1;
	memset(&localActor->staticFlagsBF, 0, 2);
	memset(&localActor->dynamicFlagsBF, 0, 2);
	localActor->life = 50;
	localActor->field_14 = 1;
	localActor->hitBy = -1;
	localActor->lastRotationSpeed = 0;
	localActor->lastX = 0;
	localActor->lastZ = 0;
	localActor->lastY = 0;
	localActor->costumeIndex = -1;
	localActor->previousAnimIndex = -1;
	localActor->field_78 = 0;
	localActor->animPosition = 0;

	setActorAngleSafe(0, 0, 0, &localActor->time);

	localActor->positionInMoveScript = -1;
	localActor->positionInActorScript = 0;
}

void initSprite(int imageId, short int actorId)
{
	actor *lactor = &actors[actorId];

	if (lactor->staticFlagsBF.isSpriteActor && imageId != -1 && lactor->costumeIndex != imageId)
	{
		short int *ptr;

		lactor->costumeIndex = imageId;

		ptr = (short int *)(spriteActorData + imageId * 16 + 4);

		lactor->boudingBox.X.bottomLeft = READ_LE_S16(ptr++);
		lactor->boudingBox.X.topRight = READ_LE_S16(ptr++);
		lactor->boudingBox.Y.bottomLeft = READ_LE_S16(ptr++);
		lactor->boudingBox.Y.topRight = READ_LE_S16(ptr++);
		lactor->boudingBox.Z.bottomLeft = READ_LE_S16(ptr++);
		lactor->boudingBox.Z.topRight = READ_LE_S16(ptr++);
	}
}

void checkCarrier(short int id)
{
	short int i;

	if (actors[id].staticFlagsBF.isCarrier)
		for (i = 0; i < numActorInRoom; i++)
			if (actors[i].standOn == id)
				actors[i].standOn = -1;
}

