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

#include "cube.h"
#include "mainLoop.h"
#include "anim.h"
#include "text.h"
#include "fullRedraw.h"
#include "main.h"
#include "actorScript.h"
#include "extra.h"
#include "room.h"
#include "actors.h"
#include "renderer.h"
#include "script.h"
#include "comportementMenu.h"

int climbing;

short int useAnotherGrm = -1;

void CheckZoneSce(actor * lactor, int actorNumber) {
	int currentX = lactor->X;
	int currentY = lactor->Z;
	int currentZ = lactor->Y;

	int var_C = 0;
	int i;
	ZONE_Box* pZone = zoneData;

	lactor->zone = -1;

	if (actorNumber == 0)
		climbing = 0;

	for (i = 0; i < numOfZones; i++) {
		if ((currentX >= pZone->bottomLeft.X && currentX <= pZone->topRight.X) &&
				(currentZ >= pZone->bottomLeft.Y && currentZ <= pZone->topRight.Y) &&
				(currentY >= pZone->bottomLeft.Z && currentY <= pZone->topRight.Z)) { // if actor in zone
			switch (pZone->zoneType) {
			case 0: { // change room
					if (!actorNumber) { // only twinsen can change room
						if (lactor->life > 0) { // if not dead
							needChangeRoom = (char)pZone->data.ZONE_ChangeRoom.newRoomNumber;
							newTwinsenXByZone = lactor->X - pZone->bottomLeft.X + pZone->data.ZONE_ChangeRoom.positionX;
							newTwinsenZByZone = lactor->Y - pZone->bottomLeft.Y + pZone->data.ZONE_ChangeRoom.positionY;
							newTwinsenYByZone = lactor->Z - pZone->bottomLeft.Z + pZone->data.ZONE_ChangeRoom.positionZ;
							twinsenPositionModeInNewCube = 1;
						}
					}
					break;
				}
			case 1: { // opcode camera
					if (currentlyFollowedActor == actorNumber) {
						disableScreenRecenter = 1;
						if (newCameraX != pZone->data.ZONE_ForceCamera.newCameraX
								|| newCameraZ != pZone->data.ZONE_ForceCamera.newCameraY
								|| newCameraY != pZone->data.ZONE_ForceCamera.newCameraZ) {
							newCameraX = pZone->data.ZONE_ForceCamera.newCameraX;
							newCameraZ = pZone->data.ZONE_ForceCamera.newCameraY;
							newCameraY = pZone->data.ZONE_ForceCamera.newCameraZ;
							requestBackgroundRedraw = 1;
						}
					}
					break;
				}
			case 2: { // set zone
					lactor->zone = pZone->data.ZONE_SetActorZone.zoneNumber;
					break;
				}
			case 3: { // cube clip
					if (currentlyFollowedActor == actorNumber) {
						var_C = 1;
						if (useAnotherGrm != pZone->data.ZONE_PatchCube.newGrid) {
							if (pZone->data.ZONE_PatchCube.newGrid != -1) {
								createCube();
							}

							useAnotherGrm = pZone->data.ZONE_PatchCube.newGrid;
							currentGrid2 = i;
							freezeTime();
							IncrustGrm(pZone->data.ZONE_PatchCube.newGrid);
							unfreezeTime();
						}
					}
					break;
				}
			case 4: { // find object
					if (!actorNumber) { // only twinsen matter
						if (action != 0) { // if action button is pressed
							InitAnim(ANIM_activate, 1, 0, 0);
							ZoneGiveExtraBonus(pZone);
						}
					}
					break;
				}
			case 5: { // display text
					if (!actorNumber && action) {
						freezeTime();
						TestRestoreModeSVGA(1);
						TestCoulDial(pZone->data.ZONE_DisplayText.textColor);
						printTextFullScreen(pZone->data.ZONE_DisplayText.textIndex);
						unfreezeTime();
						fullRedraw(1);
					}
					break;
				}
			case 6: { // ladder
					if (!actorNumber && (comportementHero != 4) && ((lactor->anim == ANIM_walk) || (lactor->anim == ANIM_reachTopOfLadder) || (lactor->anim == ANIM_climbUpLadder))) {
						Rotate(lactor->boudingBox.X.bottomLeft, lactor->boudingBox.Z.bottomLeft, lactor->angle + 0x580);
						destX += processActorX;
						destZ += processActorY;

						if (destX >= 0 && destZ >= 0 && destX <= 0x7E00 && destZ <= 0x7E00) {
							if (WorldColBrick(destX, lactor->Y + 0x100, destZ)) {
								climbing = 1;
								if (lactor->Y >= abs(pZone->bottomLeft.Y + pZone->topRight.Y) / 2) {
									InitAnim(ANIM_reachTopOfLadder, 2, 0, actorNumber);  //get out of ladder
								} else {
									InitAnim(ANIM_climbUpLadder, 0, 255, actorNumber); //go up ladder
								}
							}
						}
					}
					break;
				}
			default: {
					printf("Unsupported CheckZoneSce opcode %d for actor %d!\n", pZone->zoneType, actorNumber);
					exit(1);
				}
			}
		}
		pZone++;
	}

	if (!var_C) {
		if (actorNumber == currentlyFollowedActor) {
			if (useAnotherGrm != -1) {
				useAnotherGrm = -1;
				currentGrid2 = -1;
				createCube();
				requestBackgroundRedraw = 1;
			}
		}
	}
}

void ZoneGiveExtraBonus(ZONE_Box* pZone) {
	char* esi = (char*)pZone;
	int i;
	int di = 0;
	char var_8[8];
	char currentBonus;
	int index;
	int angle;

	if (!*(short int*)(esi + 0x14)) { // bonus not yet used
		for (i = 0;i < 5; i++) {
			if (*(short int*)(esi + 0x10)&(1 << (i + 4))) {
				var_8[di++] = i;
			}
		}

		if (di) {
			currentBonus = var_8[rand()%di];

			if (!magicLevel && currentBonus == 2) // if bonus is magic and magicLevel 1 not reached
				currentBonus = 1; //bonus is life

			angle = GetAngle(abs(pZone->topRight.X + pZone->bottomLeft.X) / 2, abs(pZone->topRight.Z + pZone->bottomLeft.Z) / 2, twinsen->X, twinsen->Z);
			index = ExtraBonus(abs(pZone->topRight.X + pZone->bottomLeft.X) / 2, pZone->topRight.Y, abs(pZone->topRight.Z + pZone->bottomLeft.Z) / 2, 180, angle, currentBonus + 3, *(short int*)(esi + 0x12));

			if (index != -1) {
				extraList[index].field_14 |= 0x400;
				*(short int*)(esi + 0x14) = 1;
			}
		}
	}
}

