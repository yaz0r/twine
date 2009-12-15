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

#include "images.h"
#include "cube.h"
#include "actors.h"
#include "renderer.h"
#include "text.h"
#include "mainLoop.h"
#include "save.h"
#include "mainMenu.h"
#include "extra.h"
#include "anim.h"
#include "hqr.h"
#include "music.h"
#include "body.h"
#include "main.h"
#include "script.h"
#include "comportementMenu.h"

#include "room.h"

#define NUM_MAX_BRICKS 9000

unsigned char* brickTable[NUM_MAX_BRICKS];
unsigned char* brickMaskTable[NUM_MAX_BRICKS];
unsigned int brickSizeTable[NUM_MAX_BRICKS];
unsigned char brickUsageTable[NUM_MAX_BRICKS];

byte *videoPtr1;
byte *videoPtr2;
byte *videoPtr3;
byte *videoPtr4;
byte *videoPtr5;
byte *videoPtr6;
byte *videoPtr7;
byte *videoPtr8;
byte *videoPtr9;
byte *videoPtr10;
byte *videoPtr11;
byte *videoPtr12;
byte *videoPtr13;

flagDataStruct flagData[NUM_MAX_FLAGS];

sceneStruct sceneVar2;
sceneStruct sceneVar3;
sceneStruct sceneVar4;

ZONE_Box zoneData[NUM_MAX_ZONES];

byte palette2[256 * 3];

char currentRoom;
char needChangeRoom = -1;

short int newTwinsenX;
short int newTwinsenZ;
short int newTwinsenY;

short int newTwinsenXByZone;
short int newTwinsenZByZone;
short int newTwinsenYByZone;

short int newTwinsenXByScene;
short int newTwinsenYByScene;
short int newTwinsenZByScene;

short int numOfZones;
short int numFlags;

int numActorInRoom;

short int changeRoomVar10 = 1;

int reinitVar1;
int reinitVar2;

short int holomapTraj = -1;

short int roomMusic = -1;

short int twinsenPositionModeInNewCube;

short int startupAngleInCube;
short int startupComportementHeroInCube;

unsigned char *scenePtr;

static void RestartPerso(void) {
	twinsen->comportement = 1;
	memset(&twinsen->dynamicFlagsBF, 0, 2);
	memset(&twinsen->staticFlagsBF, 0, 2);

	twinsen->staticFlagsBF.computeCollisionWithObj = true;
	twinsen->staticFlagsBF.computeCollisionWithBricks = true;
	twinsen->staticFlagsBF.isZonable = true;
	twinsen->staticFlagsBF.canDrown = true;
	twinsen->staticFlagsBF.isFallable = true;

	twinsen->field_14 = 1;
	twinsen->positionInMoveScript = -1;
	twinsen->label = -1;
	twinsen->positionInActorScript = 0;
	twinsen->zone = -1;
	twinsen->angle = startupAngleInCube;
	setActorAngleSafe(twinsen->angle, twinsen->angle, 0, &twinsen->time);
	SetComportement(startupComportementHeroInCube);
}

void ChangeCube(void) {
	int i;
	int oldRoom;

	if (needChangeRoom == 4 && vars[30] != 0) // if twinsen house has been destroyed, manualy patch the room number
		needChangeRoom = 118;

	oldRoom = currentRoom;

	currentRoom = needChangeRoom;
	HQ_StopSample();

	ClearScene();
	LoadFicPerso();

	twinsen->comportement = 1;
	twinsen->zone = -1;
	twinsen->positionInActorScript = 0;
	twinsen->positionInMoveScript = -1;
	twinsen->label = -1;

	LoadScene(needChangeRoom);

	if (holomapTraj != -1) {
		TestRestoreModeSVGA(0);
		SetBackPal();
		HoloTraj(holomapTraj);
		holomapTraj = -1;
	}

	if (currentRoom == 116 || currentRoom == 117)
		currentTextBank = 10;

	InitDial(currentTextBank + 3);

	initGrid(needChangeRoom);

	if (twinsenPositionModeInNewCube == 1) {
		newTwinsenX = newTwinsenXByZone;
		newTwinsenZ = newTwinsenZByZone;
		newTwinsenY = newTwinsenYByZone;
	}

	if (twinsenPositionModeInNewCube == 2 || twinsenPositionModeInNewCube == 0) {
		newTwinsenX = newTwinsenXByScene;
		newTwinsenZ = newTwinsenYByScene;
		newTwinsenY = newTwinsenZByScene;
	}

	twinsen->X = newTwinsenX;
	twinsen->Y = twinsenZBeforeFall = newTwinsenZ;
	twinsen->Z = newTwinsenY;

	SetLightVector(reinitVar1, reinitVar2, 0);

	if (oldRoom != needChangeRoom) { // backup comportement and angle in case of gameover/reload
		startupComportementHeroInCube = comportementHero;
		startupAngleInCube = twinsen->angle;
		SaveGame();
	}

	RestartPerso();

	for (i = 1; i < numActorInRoom; i++)
		initActor(i);

	numKey = 0;
	disableScreenRecenter = 0;
	twinsenPositionModeInNewCube = 0;

	newCameraX = actors[currentlyFollowedActor].X >> 9;
	newCameraZ = actors[currentlyFollowedActor].Y >> 8;
	newCameraY = actors[currentlyFollowedActor].Z >> 9;

	magicBallIdx = -1;
	twinsenMoved = 1;
	useAnotherGrm = -1;
	currentGrid2 = -1;
	requestBackgroundRedraw = 1;
	lockPalette = 1;

	needChangeRoom = -1;
	changeRoomVar10 = 1;

	SetLightVector(reinitVar1, reinitVar2, 0);

	if (roomMusic != -1)
		playMusic(roomMusic);

	printf("ChangeCube done\n");
}

void ClearScene(void) {
	int i;

	reinitExtraObjectList();

	for (i = 0; i < 80; i++)
		cubeFlags[i] = 0;

	for (i = 0; i < 10; i++)
		overlayObjectList[i].field_0 = -1;

#ifndef PRELOAD_ALL
	HQR_Reset_Ressource(HQR_Anims);
#endif

	currentPositionInBodyPtrTab = 0;
	useAlternatePalette = 0;
}

void LoadScene(int sceneNumber) {
	unsigned char *temp;
	short int temp3;
	int i;

	short int currentActor;

	int modelNumber;
	int size;

	size = HQRM_Load("scene.hqr", sceneNumber, &scenePtr);

	/* TODO: make the loading error handling */

	temp = (unsigned char*)scenePtr;

	currentTextBank = READ_LE_BYTE(temp);
	temp++;
	needChangeRoom = sceneNumber;
	/* skip sceneRoomNumber */
	temp += 5;

	reinitVar1 = READ_LE_U16(temp);
	temp += 2;
	reinitVar2 = READ_LE_U16(temp);
	temp += 2;

	sceneVar2.field_0 = READ_LE_U16(temp);
	temp += 2;
	sceneVar3.field_0 = READ_LE_U16(temp);
	temp += 2;
	sceneVar4.field_0 = READ_LE_U16(temp);
	temp += 2;
	sceneVar2.field_2 = READ_LE_U16(temp);
	temp += 2;
	sceneVar3.field_2 = READ_LE_U16(temp);
	temp += 2;
	sceneVar4.field_2 = READ_LE_U16(temp);
	temp += 2;
	sceneVar2.field_4 = READ_LE_U16(temp);
	temp += 2;
	sceneVar3.field_4 = READ_LE_U16(temp);
	temp += 2;
	sceneVar4.field_4 = READ_LE_U16(temp);
	temp += 2;
	sceneVar2.field_6 = READ_LE_U16(temp);
	temp += 2;
	sceneVar3.field_6 = READ_LE_U16(temp);
	temp += 2;
	sceneVar4.field_6 = READ_LE_U16(temp);
	temp += 6; /* skip var14 and var15 */
	roomMusic = READ_LE_BYTE(temp);
	temp++;
	newTwinsenXByScene = READ_LE_U16(temp);
	temp += 2; // ok jusque la
	newTwinsenYByScene = READ_LE_U16(temp);
	temp += 2;
	newTwinsenZByScene = READ_LE_U16(temp);
	temp += 2;

	temp3 = READ_LE_U16(temp);
	temp += 2;
	twinsen->moveScript = temp;
	temp += temp3;

	currentActor = 1;

	temp3 = READ_LE_U16(temp);
	temp += 2;
	twinsen->actorScript = temp;
	temp += temp3;

	numActorInRoom = READ_LE_U16(temp);
	temp += 2;

	while (currentActor < numActorInRoom) {
		unsigned short int staticFlags;
		resetActor(currentActor);
		staticFlags = READ_LE_U16(temp);
		temp += 2;

		if (staticFlags & 0x1) {
			actors[currentActor].staticFlagsBF.computeCollisionWithObj = 1;
		}
		if (staticFlags & 0x2) {
			actors[currentActor].staticFlagsBF.computeCollisionWithBricks = 1;
		}
		if (staticFlags & 0x4) {
			actors[currentActor].staticFlagsBF.isZonable = 1;
		}
		if (staticFlags & 0x8) {
			actors[currentActor].staticFlagsBF.isUsingClipping = 1;
		}
		if (staticFlags & 0x10) {
			actors[currentActor].staticFlagsBF.isPushable = 1;
		}
		if (staticFlags & 0x20) {
			actors[currentActor].staticFlagsBF.isDead = 1;
		}
		if (staticFlags & 0x40) {
			actors[currentActor].staticFlagsBF.canDrown = 1;
		}
		if (staticFlags & 0x80) {
			actors[currentActor].staticFlagsBF.bUnk80 = 1;
		}

		if (staticFlags & 0x100) {
			actors[currentActor].staticFlagsBF.bUnk0100 = 1;
		}
		if (staticFlags & 0x200) {
			actors[currentActor].staticFlagsBF.noDisplay = 1;
		}
		if (staticFlags & 0x400) {
			actors[currentActor].staticFlagsBF.isSpriteActor = 1;
		}
		if (staticFlags & 0x800) {
			actors[currentActor].staticFlagsBF.isFallable = 1;
		}
		if (staticFlags & 0x1000) {
			actors[currentActor].staticFlagsBF.doesntCastShadow = 1;
		}
		if (staticFlags & 0x2000) {
			//actors[currentActor].staticFlagsBF.isBackgrounded = 1;
		}
		if (staticFlags & 0x4000) {
			actors[currentActor].staticFlagsBF.isCarrier = 1;
		}
		if (staticFlags & 0x8000) {
			actors[currentActor].staticFlagsBF.isUsingMiniZv = 1;
		}

		modelNumber = READ_LE_U16(temp);
		temp += 2;

		if (!(actors[currentActor].staticFlagsBF.isSpriteActor)) { // if not sprite actor
#ifdef PRELOAD_ALL
			actors[currentActor].entityDataPtr = HQR_GetCopy(HQR_Fic, modelNumber);
#else
			HQRM_Load("file3d.hqr", modelNumber, &actors[currentActor].entityDataPtr);
#endif
		}

		actors[currentActor].body = READ_LE_BYTE(temp);
		temp++;
		actors[currentActor].anim = READ_LE_BYTE(temp);
		temp++;

		actors[currentActor].field_8 = READ_LE_U16(temp);
		temp += 2;
		actors[currentActor].field_20 = actors[currentActor].X = READ_LE_U16(temp);
		temp += 2;
		actors[currentActor].field_22 = actors[currentActor].Y = READ_LE_U16(temp);
		temp += 2;
		actors[currentActor].field_24 = actors[currentActor].Z = READ_LE_U16(temp);
		temp += 2;

		actors[currentActor].field_66 = READ_LE_BYTE(temp);
		temp++;

		actors[currentActor].field_10 = READ_LE_U16(temp);
		temp += 2;
		actors[currentActor].field_10 &= 0xFE;
		actors[currentActor].angle = READ_LE_U16(temp);
		temp += 2;
		actors[currentActor].speed = READ_LE_U16(temp);
		temp += 2;
		actors[currentActor].comportement = READ_LE_U16(temp);
		temp += 2;
		actors[currentActor].cropLeft = READ_LE_S16(temp);
		temp += 2;
		actors[currentActor].cropTop = READ_LE_S16(temp);
		temp += 2;
		actors[currentActor].cropRight = READ_LE_S16(temp);
		temp += 2;
		actors[currentActor].cropBottom = READ_LE_S16(temp);
		temp += 2;
		actors[currentActor].followedActor = actors[currentActor].cropBottom;

		actors[currentActor].field_12 = READ_LE_BYTE(temp);
		temp++;
		actors[currentActor].talkColor = READ_LE_BYTE(temp);
		temp++;
		actors[currentActor].field_14 = READ_LE_BYTE(temp);
		temp++;
		actors[currentActor].life = READ_LE_BYTE(temp);
		temp++;

		temp3 = READ_LE_U16(temp);
		temp += 2;
		actors[currentActor].moveScript = temp;
		temp += temp3;

		temp3 = READ_LE_U16(temp);
		temp += 2;
		actors[currentActor].actorScript = temp;
		temp += temp3;

		currentActor++;
	};

	numOfZones = READ_LE_U16(temp);
	temp += 2;
	assert(numOfZones <= NUM_MAX_ZONES);
	for (i = 0; i < numOfZones; i++) {
		zoneData[i].bottomLeft.X = READ_LE_S16(temp);
		temp += 2;
		zoneData[i].bottomLeft.Y = READ_LE_S16(temp);
		temp += 2;
		zoneData[i].bottomLeft.Z = READ_LE_S16(temp);
		temp += 2;

		zoneData[i].topRight.X = READ_LE_S16(temp);
		temp += 2;
		zoneData[i].topRight.Y = READ_LE_S16(temp);
		temp += 2;
		zoneData[i].topRight.Z = READ_LE_S16(temp);
		temp += 2;

		zoneData[i].zoneType = READ_LE_S16(temp);
		temp += 2;

		zoneData[i].data.generic.data1 = READ_LE_S16(temp);
		temp += 2;
		zoneData[i].data.generic.data2 = READ_LE_S16(temp);
		temp += 2;
		zoneData[i].data.generic.data3 = READ_LE_S16(temp);
		temp += 2;
		zoneData[i].data.generic.data4 = READ_LE_S16(temp);
		temp += 2;

		zoneData[i].dummy = READ_LE_S16(temp);
		temp += 2;
	}

	numFlags = READ_LE_U16(temp);
	temp += 2;
	assert(numFlags <= NUM_MAX_FLAGS);
	// copy data to alligned space
	for (i = 0; i < numFlags; i++) {
		flagData[i].x = READ_LE_S16(temp);
		temp += 2;
		flagData[i].y = READ_LE_S16(temp);
		temp += 2;
		flagData[i].z = READ_LE_S16(temp);
		temp += 2;
	}
}

void HoloTraj(int arg_0) {
	int reinitVar1Copy;
	int progressiveTextStartColorCopy;

// int i;
	short int var_14;
	byte *localmakeHolomapTrajectoryVar;
	int j;
	unsigned short int var;

	freezeTime();

	var_14 = 1;
	reinitVar1Copy = reinitVar1;
	progressiveTextStartColorCopy = progressiveTextStartColor;

	UnSetClip();
	Cls();
	os_flip(frontVideoBuffer);
	loadHolomapGFX();

	localmakeHolomapTrajectoryVar = videoPtr12;

	j = 0;

	while (j < arg_0) {    /* implementer la suite */
		localmakeHolomapTrajectoryVar += 12;
		var = READ_LE_U16(localmakeHolomapTrajectoryVar);
		var += var;
		localmakeHolomapTrajectoryVar += 2;
		j++;
		localmakeHolomapTrajectoryVar += var;
	}

	/*
	 * implementer la suite
	 */

}

void loadHolomapGFX(void) {
	int i;
	int j;

	videoPtr1 = workVideoBuffer;
	videoPtr2 = workVideoBuffer + 4488;
	videoPtr3 = workVideoBuffer + 7854;
	videoPtr4 = workVideoBuffer + 8398;

	videoPtr5 = workVideoBuffer + 73934;

	Load_HQR("ress.hqr", videoPtr3, 6);
	Load_HQR("ress.hqr", videoPtr4, 7);
	videoPtr6 = videoPtr5 + Load_HQR("ress.hqr", videoPtr5, 9);
	videoPtr7 = videoPtr6 + Load_HQR("ress.hqr", videoPtr6, 10);
	videoPtr8 = videoPtr7 + Load_HQR("ress.hqr", videoPtr7, 11);
	videoPtr11 = videoPtr8 + Load_HQR("ress.hqr", videoPtr8, 29);

// loadGfxSub(videoPtr5);
// loadGfxSub(videoPtr6);
// loadGfxSub(videoPtr7);

// loadGfxSub(videoPtr8);

	videoPtr10 = videoPtr11 + 4488;
	videoPtr12 = videoPtr10 + Load_HQR("ress.hqr", videoPtr10, 8);
	videoPtr13 = videoPtr12 + Load_HQR("ress.hqr", videoPtr12, 30);

	Load_HQR("ress.hqr", (byte *) & palette, 5);

	j = 576;
	for (i = 0; i < 96; i += 3, j += 3) {
		palette2[i] = palette[j];
		palette2[i + 1] = palette[j + 1];
		palette2[i + 2] = palette[j + 2];
	}

	j = 576;
	for (i = 96; i < 189; i += 3, j += 3) {
		palette2[i] = palette[j];
		palette2[i + 1] = palette[j + 1];
		palette2[i + 2] = palette[j + 2];
	}

// loadGFXSub1();
// loadGFXSub2();

// needToLoadHolomapGFX=0;
}

void SetComportement(int newComportement) {
	int temp;

	comportementHero = newComportement;
	twinsen->entityDataPtr = file3D[newComportement];

	temp = twinsen->body;

	twinsen->costumeIndex = -1;
	twinsen->body = -1;

	InitBody(temp, 0);

	twinsen->anim = -1;
	twinsen->field_78 = 0;

	InitAnim(ANIM_static, 0, 255, 0);
}

int loadBrk(int gridSize) {
	unsigned int firstBrick = 60000; // should be MAX_UINT
	unsigned int lastBrick = 0; // should be MIN_UINT
	unsigned char* ptrToBllBits;
	unsigned int i;
	unsigned int j;
	unsigned int numUsedBricks;
	unsigned int currentBllEntryIdx = 0;

	memset(brickTable, 0, sizeof(brickTable));
	memset(brickSizeTable, 0, sizeof(brickSizeTable));
	memset(brickUsageTable, 0, sizeof(brickUsageTable));

	ptrToBllBits = currentGrid + (gridSize - 32);

	// build up a map of all used bricks in the room
	for (i = 1;i < 256;i++) {
		unsigned char currentBitByte = *(ptrToBllBits + (i / 8));
		unsigned char currentBitMask = 1 << (7 - (i & 7));

		if (currentBitByte & currentBitMask) {
			unsigned int currentBllOffset = READ_LE_U32(currentBll + currentBllEntryIdx);
			unsigned char* currentBllPtr = currentBll + currentBllOffset;

			unsigned int bllSizeX = currentBllPtr[0];
			unsigned int bllSizeY = currentBllPtr[1];
			unsigned int bllSizeZ = currentBllPtr[2];

			unsigned int bllSize = bllSizeX * bllSizeY * bllSizeZ;

			unsigned char* bllDataPtr = currentBllPtr + 5;

			for (j = 0;j < bllSize;j++) {
				unsigned int brickIdx = READ_LE_U16(bllDataPtr);

				if (brickIdx) {
					brickIdx--;

					if (brickIdx <= firstBrick)
						firstBrick = brickIdx;

					if (brickIdx > lastBrick)
						lastBrick = brickIdx;

					brickUsageTable[brickIdx] = 1;
				}
				bllDataPtr += 4;
			}
		}
		currentBllEntryIdx += 4;
	}

	// compute the number of bricks to load
	numUsedBricks = 0;
	for (i = firstBrick;i <= lastBrick;i++)
		if (brickUsageTable[i]) // was brick noted as used ?
			numUsedBricks++;

	for (i = firstBrick;i <= lastBrick;i++) {
		if (brickUsageTable[i]) {
			brickSizeTable[i] = Size_HQR("LBA_BRK.HQR", i);
			brickTable[i] = (unsigned char*)malloc(brickSizeTable[i]);
			Load_HQR("LBA_BRK.HQR", brickTable[i], i);
		}
	}

	return (0);
}

int CreateMaskGph() {
	int i;

	for (i = 0;i < NUM_MAX_BRICKS;i++) {
		if (brickUsageTable[i]) {
			brickMaskTable[i] = (unsigned char*)malloc(brickSizeTable[i]);

			CalcGraphMsk(brickTable[i], brickMaskTable[i]);
		}
	}
    return 1;
}

