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
#include "actorScript.h"
#include "fullRedraw.h"
#include "room.h"
#include "hqr.h"
#include "actors.h"
#include "renderer.h"
#include "mainLoop.h"

#include "moveActor.h"

short int moveVar1;
short int manipActorResult;

void DoTrack(int actorNumber) {
	int continueMove;
	int positionInScript;
	char currentOpcode;
	char *scriptPtr;

	int newAngle;

	actor *lactor;
	time *timePtr;

	lactor = &actors[actorNumber];
	continueMove = 1;
	timePtr = &lactor->time;

	do {
		positionInScript = lactor->positionInMoveScript;
		scriptPtr = (char *)(lactor->positionInMoveScript + lactor->moveScript);
		currentOpcode = *(scriptPtr++);
		lactor->positionInMoveScript++;

		switch (currentOpcode) {
		case 0: { // stop
				continueMove = 0;
				lactor->positionInMoveScript = -1;
				break;
			}
		case 2:
			InitBody(*scriptPtr, actorNumber);
			lactor->positionInMoveScript++;
			break;
		case 3: // ANIM
			if (InitAnim(*(scriptPtr++), 0, 0, actorNumber)) {
				lactor->positionInMoveScript++;
			} else {
				lactor->positionInMoveScript = positionInScript;
				continueMove = 0;
			}
			break;
		case 4: { // GOTO_POINT
				lactor->positionInMoveScript++;
				manipActorResult = *scriptPtr;

				destX = flagData[manipActorResult].x;
				destY = flagData[manipActorResult].y;
				destZ = flagData[manipActorResult].z;

				newAngle = GetAngle(lactor->X, lactor->Z, destX, destZ);

				if (lactor->staticFlagsBF.isSpriteActor) {
					lactor->angle = newAngle;
				} else {
					ManualRealAngle(lactor->angle, newAngle, lactor->speed, &lactor->time);
				}

				if (DoTrackVar1 > 500) {
					continueMove = 0;
					lactor->positionInMoveScript -= 2;
				}

				break;
			}
		case 5:
			if (!(lactor->dynamicFlagsBF.bUnk0004)) {
				continueMove = 0;
				lactor->positionInMoveScript--;
			} else {
				continueMove = 0;
				clearRealAngle(lactor);
			}
			break;
		case 7: { // set angle
				lactor->positionInMoveScript += 2;
				if (!(lactor->staticFlagsBF.isSpriteActor)) {
					manipActorResult = READ_LE_S16(scriptPtr);
					if (lactor->time.numOfStep == 0) {
						ManualRealAngle(lactor->angle, manipActorResult, lactor->speed, timePtr);
					}

					if (lactor->angle != manipActorResult) {
						continueMove = 0;
						lactor->positionInMoveScript -= 3;
					}
				}
				break;
			}

		case 8: // warp to flag
			lactor->positionInMoveScript++;
			manipActorResult = *scriptPtr;

			printf("Actor %d warp to flag %d\n", actorNumber, manipActorResult);

			destX = flagData[manipActorResult].x;
			destY = flagData[manipActorResult].y;
			destZ = flagData[manipActorResult].z;

			if (lactor->staticFlagsBF.isSpriteActor) {
				lactor->speed = 0;
			}

			lactor->X = destX;
			lactor->Z = destZ;
			lactor->Y = destY;

			break;

		case 9: // LABEL
			lactor->label = *scriptPtr;
			lactor->positionInMoveScript++;
			lactor->currentLabelPtr = lactor->positionInMoveScript - 2;
			break;
		case 10:
			lactor->positionInMoveScript = READ_LE_S16(scriptPtr);
			break;
		case 11:  // STOP
			continueMove = 0;
			lactor->positionInMoveScript = -1;
			break;

		case 12:  // move (used in backward walk)
			lactor->positionInMoveScript++;
			manipActorResult = *scriptPtr;

			destX = flagData[manipActorResult].x;
			destY = flagData[manipActorResult].y;
			destZ = flagData[manipActorResult].z;

			newAngle = 0x200 + GetAngle(lactor->X, lactor->Z, destX, destZ);

			if (lactor->staticFlagsBF.isSpriteActor) {
				lactor->angle = newAngle;
			} else {
				ManualRealAngle(lactor->angle, newAngle, lactor->speed, &lactor->time);
			}

			if (DoTrackVar1 > 500) {
				continueMove = 0;
				lactor->positionInMoveScript -= 2;
			}

			break;
		case 13:
			lactor->positionInMoveScript += 2;
			if (lactor->dynamicFlagsBF.bUnk0004) {
				(*(scriptPtr + 1))++;

				if (*(scriptPtr + 1) == *scriptPtr) {
					(*(scriptPtr + 1)) = 0;
				} else {
					continueMove = 0;
				}
			} else {
				continueMove = 0;
			}

			if (continueMove == 0) {
				lactor->positionInMoveScript -= 3;
			}
			break;
		case 14:
			HQ_3D_MixSample(READ_LE_S16(scriptPtr), 0x1000, 1, lactor->X, lactor->Z, lactor->Y);
			lactor->positionInMoveScript += 2;
			break;
		case 15:
			lactor->positionInMoveScript++;
			if (lactor->staticFlagsBF.isSpriteActor) { // if can move
				manipActorResult = *scriptPtr;

				destX = flagData[manipActorResult].x;
				destY = flagData[manipActorResult].y;
				destZ = flagData[manipActorResult].z;

				lactor->angle = GetAngle(lactor->X, lactor->Z, destX, destZ); // X-Z move
				lactor->field_78 = GetAngle(lactor->Y, 0, destY, DoTrackVar1);  // Y (vertical) move

				if (DoTrackVar1 > 100) {
					continueMove = 0;
					lactor->positionInMoveScript -= 2;
				} else {
					lactor->X = destX;
					lactor->Z = destZ;
					lactor->Y = destY;
				}
			}
			break;
		case 16:
			lactor->positionInMoveScript += 2;
			lactor->speed = READ_LE_S16(scriptPtr);
			if (lactor->staticFlagsBF.isSpriteActor) {
				setActorAngle(0, lactor->speed, 50, timePtr);
			}
			break;
		case 17: // move actor to background
			lactor->positionInMoveScript += 1;
			if (*scriptPtr != 0) {
				if (!(lactor->staticFlagsBF.isBackgrounded)) { //if actor wasn't already in background
					//  lactor->staticFlagsBF.isBackgrounded = true; // set him to background
					if (lactor->dynamicFlagsBF.wasDrawn) {
						requestBackgroundRedraw = 1;
					}
				}
			} else {
				if (lactor->staticFlagsBF.isBackgrounded) {
					lactor->staticFlagsBF.isBackgrounded = false;
					if (lactor->dynamicFlagsBF.wasDrawn) {
						requestBackgroundRedraw = 1;
					}
				}
			}
			break;
		case 18: { // wait
				lactor->positionInMoveScript += 5;

				if (READ_LE_S32(scriptPtr + 1) == 0) {
					WRITE_LE_S32(scriptPtr + 1, lba_time + *(unsigned char *) scriptPtr * 50);
				}

				if (lba_time < READ_LE_S32(scriptPtr + 1)) {
					continueMove = 0;
					lactor->positionInMoveScript -= 6;
				} else {
					WRITE_LE_S32(scriptPtr + 1, 0);
				}
				break;
			}
		case 19: {
				InitBody(-1, actorNumber);
				break;
			}
		case 20: {
				short int beta;

				beta = READ_LE_S16(scriptPtr);
				lactor->positionInMoveScript += 2;

				lactor->angle = beta;

				if (lactor->staticFlagsBF.isSpriteActor) {
					clearRealAngle(lactor);
				}

				break;
			}
		case 21:
		case 22:
		case 23:
		case 24:
			lactor->positionInMoveScript += 2;
			if (lactor->staticFlagsBF.isSpriteActor && lactor->staticFlagsBF.isUsingClipping) {
				switch (currentOpcode - 21) {
				case 0:
					lactor->angle = 0x300;
					break;
				case 1:
					lactor->angle = 0x100;
					break;
				case 2:
					lactor->angle = 0x200;
					break;
				case 3:
					lactor->angle = 0;
					break;
				default:
					printf("Unsupported subopcode of actor move opcode 21-24!\n");
					exit(1);
				}

				lactor->doorStatus = READ_LE_S16(scriptPtr);
				lactor->dynamicFlagsBF.isMoving = 1;
				lactor->speed = 1000;
				setActorAngle(0, 1000, 50, timePtr);
			}
			break;
		case 25: { // TM_CLOSE
				if (lactor->staticFlagsBF.isSpriteActor && lactor->staticFlagsBF.isUsingClipping) {
					lactor->dynamicFlagsBF.isMoving = 1;
					lactor->doorStatus = 0;
					lactor->speed = -1000;
					setActorAngle(0, -1000, 50, timePtr);
				}
				break;
			}
		case 26: { // TM_WAIT_DOOR
				if (lactor->staticFlagsBF.isSpriteActor && lactor->staticFlagsBF.isUsingClipping) {
					if (lactor->speed) {
						continueMove = 0;
						lactor->positionInMoveScript--;
					}
				}
				break;
			}
		case 27: {
				printf("skipping actor move opcode 27\n");
				lactor->positionInMoveScript += 2;
				break;
			}

		case 28:
			HQ_3D_MixSample(READ_LE_S16(scriptPtr), 0x1000, 0, lactor->X, lactor->Z, lactor->Y);
			lactor->positionInMoveScript += 2;
			break;
		case 29:
			HQ_3D_MixSample(READ_LE_S16(scriptPtr), 0x1000, 0, lactor->X, lactor->Z, lactor->Y);
			lactor->positionInMoveScript += 2;
			break;
		case 31: {
				moveVar1 = READ_LE_S16(scriptPtr);
				lactor->positionInMoveScript += 2;
				break;
			}
		case 32: {
				printf("skipping actor move opcode 31 (playSound at moveVar1)\n");
				lactor->positionInMoveScript += 2;
				break;
			}
		case 33: { // look at twinsen
				lactor->positionInMoveScript += 2;
				if (!(lactor->staticFlagsBF.isSpriteActor)) {
					manipActorResult = READ_LE_S16(scriptPtr);
					if (manipActorResult == -1 && lactor->time.numOfStep == 0) {
						manipActorResult = GetAngle(lactor->X, lactor->Z, twinsen->X, twinsen->Z);
						ManualRealAngle(lactor->angle, manipActorResult, lactor->speed, &lactor->time);
						WRITE_LE_S16(scriptPtr, manipActorResult);
					}

					if (lactor->angle != manipActorResult) {
						continueMove = 0;
						lactor->positionInMoveScript -= 3;
					} else {
						clearRealAngle(lactor);
						WRITE_LE_S16(scriptPtr, -1);
					}
				}
				break;
			}
		case 34: { // angle random
				int var_10;

				lactor->positionInMoveScript += 4 ;
				if (!(lactor->staticFlagsBF.isSpriteActor)) {
					manipActorResult = READ_LE_S16(scriptPtr + 2);

					if (manipActorResult == -1  && lactor->time.numOfStep == 0) {
						if (rand()&1) {
							manipActorResult = READ_LE_S16(scriptPtr);
							var_10 = lactor->angle + 0x100 + (abs(manipActorResult) >> 1);

							manipActorResult = (var_10 - (rand() % manipActorResult)) & 0x3FF;
						} else {
							manipActorResult = READ_LE_S16(scriptPtr);
							var_10 = lactor->angle - 0x100 + (abs(manipActorResult) >> 1);

							manipActorResult = (var_10 - (rand() % manipActorResult)) & 0x3FF;
						}

						ManualRealAngle(lactor->angle, manipActorResult, lactor->speed, &lactor->time);
						WRITE_LE_S16(scriptPtr + 2, manipActorResult);
					}

					if (lactor->angle != manipActorResult) {
						continueMove = 0;
						lactor->positionInMoveScript -= 5;
					} else {
						clearRealAngle(lactor);
						WRITE_LE_S16(scriptPtr + 2, -1);
					}
				}
				break;
			}
		default:
			printf("Unsupported move opcode %d\n", currentOpcode);
			exit(1);
		}
	} while (continueMove);
}

void ManualRealAngle(int angleFrom, int angleTo, int angleSpeed, time *angleStruct) {
	// it's supposed to work with shorts, but for some reasons, it doesn't always.... (numOfStep overflow when lFrom is 0x100 and lTo is 0x300)...
	long int numOfStepLong;
	short int numOfStep;
	short int lFrom;
	short int lTo;

	lFrom = angleFrom & 0x3FF;
	lTo = angleTo & 0x3FF;

	angleStruct->from = lFrom;
	angleStruct->to = lTo;

	numOfStep = (lFrom - lTo) << 6;

	if (numOfStep < 0) {
		numOfStepLong = -numOfStep;
	} else {
		numOfStepLong = numOfStep;
	}

	numOfStepLong >>= 6;

	numOfStepLong *= angleSpeed;
	numOfStepLong >>= 8;

	angleStruct->numOfStep = (short int)numOfStepLong;
	angleStruct->timeOfChange = lba_time;
}
