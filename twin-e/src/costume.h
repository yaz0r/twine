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

struct actor {
	byte* bodyPtr;
	char field_0;
	char costume;
	byte field_2;
	byte field_3;
	unsigned char * field_4;
	short int field_8;
	short int currentLabelPtr;
	byte field_10;
	short int field_12;
	short int field_14;
	short int talkColor;
	short int costumeIndex;
	short int X; /*	\																																							*/
	short int Y; /* |----coordiantes                        */
	short int Z; /* /                                       */
	short int field_20;
	short int field_22;
	short int field_24;
	short int field_26; /* \																																						*/
	short int field_28; /* |                                      */
	short int field_2A; /* |____ costume loading vars             */
	short int field_2C; /* |                                      */
	short int field_2E; /* |                                      */
	short int field_30; /* /                                      */
	short int angle; // rotation ?
	short int field_34;
	timeStruct time;
	short int field_40;
	short int positionInMoveScript;
	short int positionInActorScript;
	short int field_4E;
	short int field_50;
	short int field_52;
	short int field_54;
	short int field_56;
	short int field_58;
	short int field_5A;
	short int label;
	short int field_5E;
	unsigned short int field_60; //flags
	short int field_62;          //flags2
	short int field_64;
	short int field_66;
	short int life;
	short int field_6A;
	short int field_6C;
	short int field_6E;
	short int field_70;
	short int field_74; //animNum
	short int field_72;
	short int field_76; //animPosition
	short int field_78;

	unsigned char *moveScript;
	unsigned char *actorScript;
};