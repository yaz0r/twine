/***************************************************************************
                          costume.h  -  description
                             -------------------
    begin                : Sat Mar 16 2002
    copyright            : (C) 2002 by Yaz0r
    email                : yaz0r@yaz0r.net
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

struct MDL_BoundingBoxPoint
{
	short int bottomLeft;
	short int topRight;
};

struct MDL_BoundingBox
{
	MDL_BoundingBoxPoint X;
	MDL_BoundingBoxPoint Y;
	MDL_BoundingBoxPoint Z;
};

struct actor
    {
	char body;
	char anim;
	byte field_2;
	byte field_3;
	unsigned char *animExtraData;
	short int field_8;
	short int currentLabelPtr;
   	byte *entityDataPtr;
	short int field_10;
	short int field_12;
	short int field_14;
	short int talkColor;
	short int costumeIndex;
	short int X;		/* \ */
	short int Y;		/* |----coordiantes */
	short int Z;		/* / */
	short int field_20;
	short int field_22;
	short int field_24;

	MDL_BoundingBox boudingBox;

	short int angle;	// rotation ?
	short int speed;
	timeStruct time;
	short int comportement;
	short int positionInMoveScript;
	short int positionInActorScript;

	short int cropLeft; 
	short int cropTop;
	short int cropRight;

	union{
		short int cropBottom; 
		short int followedActor;
	};

	short int collision;
	short int standOn;
	short int zone;
	short int label;
	short int pausedTrackPtr;

    struct
    {
        U16 bComputeCollisionWithObj            : 1; // 0x0001
        U16 bComputeCollisionWithBricks         : 1; // 0x0002
        U16 bIsZonable                          : 1; // 0x0004
        U16 bIsUsingClipping                    : 1; // 0x0008
        U16 bIsPushable                         : 1; // 0x0010
        U16 bIsDead                             : 1; // 0x0020
        U16 bCanDrown                           : 1; // 0x0040
        U16 bUnk80                              : 1; // 0x0080
        U16 bUnk0100                            : 1; // 0x0100
        U16 bNoDisplay                          : 1; // 0x0200
        U16 bIsSpriteActor                      : 1; // 0x0400
        U16 bIsFallable                         : 1; // 0x0800
        U16 bDoesntCastShadow                   : 1; // 0x1000
        U16 bIsBackgrounded                     : 1; // 0x2000
        U16 bIsCarrier                          : 1; // 0x4000
        U16 bIsUsingMiniZv                      : 1; // 0x8000
    } staticFlagsBF;

    struct
    {
        U16 bUnk0001                            : 1; // 0x0001
        U16 bUnk0002                            : 1; // 0x0002
        U16 bUnk0004                            : 1; // 0x0004
        U16 bUnk0008                            : 1; // 0x0008
        U16 bUnk0010                            : 1; // 0x0010
        U16 bUnk0020                            : 1; // 0x0020
        U16 bIsMoving                           : 1; // 0x0040
        U16 bUnk0080                            : 1; // 0x0080
        U16 bUnk0100                            : 1; // 0x0100
        U16 bUnk0200                            : 1; // 0x0200
        U16 bUnk0400                            : 1; // 0x0400
        U16 bUnk0800                            : 1; // 0x0800
        U16 bUnk1000                            : 1; // 0x1000
        U16 bUnk2000                            : 1; // 0x2000
        U16 bUnk4000                            : 1; // 0x4000
        U16 bUnk8000                            : 1; // 0x8000
    } dynamicFlagsBF;

	short int hitBy;
	short int field_66;
	short int life;
	short int lastRotationSpeed;
	short int lastX;
	short int lastZ;
	short int lastY;
	short int previousAnimIndex;
	short int doorStatus;
	short int animPosition;	// animPosition
	short int field_78;

	unsigned char *moveScript;
	unsigned char *actorScript;
    };
