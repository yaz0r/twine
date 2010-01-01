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

#define NUM_MAX_FLAGS 200
#define NUM_MAX_ZONES 100

typedef struct
{
	short int x;
	short int z;
	short int y;
} flagDataStruct;

typedef struct
{
	short int field_0;
	short int field_2;
	short int field_4;
	short int field_6;
} sceneStruct;

void ChangeCube(void);
void LoadScene(int sceneNumber);
void ClearScene(void);
void HoloTraj(int arg_0);
int loadBrk(int gridSize);
int CreateMaskGph(void);
void loadHolomapGFX(void);
void SetComportement(int newCostume);

extern flagDataStruct flagData[NUM_MAX_FLAGS];
extern sceneStruct sceneVar2;
extern sceneStruct sceneVar3;
extern sceneStruct sceneVar4;
extern ZONE_Box zoneData[NUM_MAX_ZONES];
extern char currentRoom;
extern char needChangeRoom;

extern short int newTwinsenX;
extern short int newTwinsenZ;
extern short int newTwinsenY;

extern short int newTwinsenXByZone;
extern short int newTwinsenZByZone;
extern short int newTwinsenYByZone;

extern short int numOfZones;
extern short int numFlags;

extern int numActorInRoom;

extern short int changeRoomVar10;

extern int reinitVar1;
extern int reinitVar2;

extern short int holomapTraj;

extern short int roomMusic;

extern short int twinsenPositionModeInNewCube;

extern short int startupAngleInCube;
extern short int startupComportementHeroInCube;

