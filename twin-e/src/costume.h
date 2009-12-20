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

#ifndef COSTUME_H
#define COSTUME_H

typedef struct {
       short int bottomLeft;
       short int topRight;
} MDL_BoundingBoxPoint;

typedef struct
{
       MDL_BoundingBoxPoint X;
       MDL_BoundingBoxPoint Y;
       MDL_BoundingBoxPoint Z;
} MDL_BoundingBox;

typedef struct {
	char* ptr;
	int time;
} animTimerData;

typedef struct
{
	int body;
	int anim;
	int field_2;
	int field_3;
	unsigned char *animExtraData;
	int field_8;
	int currentLabelPtr;
	byte *entityDataPtr;
	int canGiveBonus;
	int gaveBonus;
	int bonusInfo;
	int field_12;
	int field_14;
	int talkColor;
	int costumeIndex;
	int X;		/* \ */
	int Y;		/* |----coordiantes */
	int Z;		/* / */
	int field_20;
	int field_22;
	int field_24;

	MDL_BoundingBox boudingBox;

	int angle;	// rotation ?
	int speed;
	time time;
	int comportement;
	int positionInMoveScript;
	int positionInActorScript;

	int cropLeft;
	int cropTop;
	int cropRight;
	int cropBottom;

	int followedActor;

	int collision;
	int standOn;
	int zone;
	int label;
	int pausedTrackPtr;

	struct {
		uint16 computeCollisionWithObj : 1; // 0x0001
		uint16 computeCollisionWithBricks : 1; // 0x0002
		uint16 isZonable : 1; // 0x0004
		uint16 isUsingClipping : 1; // 0x0008
		uint16 isPushable : 1; // 0x0010
		uint16 isDead : 1; // 0x0020
		uint16 canDrown : 1; // 0x0040
		uint16 bUnk80 : 1; // 0x0080
		uint16 bUnk0100 : 1; // 0x0100
		uint16 noDisplay : 1; // 0x0200
		uint16 isSpriteActor : 1; // 0x0400
		uint16 isFallable : 1; // 0x0800
		uint16 doesntCastShadow : 1; // 0x1000
		uint16 isBackgrounded : 1; // 0x2000
		uint16 isCarrier : 1; // 0x4000
		uint16 isUsingMiniZv : 1; // 0x8000
	} staticFlagsBF;

	struct {
		uint16 bUnk0001 : 1; // 0x0001
		uint16 bUnk0002 : 1; // 0x0002
		uint16 animEnded : 1; // 0x0004
		uint16 bUnk0008 : 1; // 0x0008
		uint16 wasDrawn : 1; // 0x0010
		uint16 isDead : 1; // 0x0020
		uint16 isMoving : 1; // 0x0040
		uint16 isRotationByAnim : 1; // 0x0080
		uint16 isFalling : 1; // 0x0100
		uint16 bUnk0200 : 1; // 0x0200
		uint16 bUnk0400 : 1; // 0x0400
		uint16 bUnk0800 : 1; // 0x0800
		uint16 bUnk1000 : 1; // 0x1000
		uint16 bUnk2000 : 1; // 0x2000
		uint16 bUnk4000 : 1; // 0x4000
		uint16 bUnk8000 : 1; // 0x8000
	} dynamicFlagsBF;

	int hitBy;
	int field_66;
	int life;
	int lastRotationSpeed;
	int lastX;
	int lastZ;
	int lastY;
	int previousAnimIndex;
	int doorStatus;
	int animPosition;	// animPosition
	int field_78;

	unsigned char *moveScript;
	unsigned char *actorScript;

	animTimerData animTimerData; // new
} actor;

void LoadFicPerso(void);
void InitBody(char arg_0, short int arg_4);
void CopyInterAnim(unsigned char *arg_0, unsigned char *arg_4);

extern byte *file3D[];
extern short int TCos[];
extern unsigned char *loadTwinsenCostumesVar1;

extern short int loadCostumeVar;
extern short int loadCostumeVar2;
extern short int loadCostumeVar3;
extern short int loadCostumeVar4;
extern short int loadCostumeVar5;
extern short int loadCostumeVar6;

#endif

