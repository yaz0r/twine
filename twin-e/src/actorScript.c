/***************************************************************************
                          actorScript.cpp  -  description
                             -------------------
    begin                : Thu May 2 2002
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

#include "lba.h"
#include "math.h"

void LBA_engine::runActorScript(short int actorNumber)
{
    char string[256];
    short int OPbreak;
    actor *lactor;
    unsigned char opcode;
    unsigned char *localScriptPtr;
    unsigned char *opcodePtr;
    int temp;

   // return;

    lactor = &actors[actorNumber];

    OPbreak = 0;

    actorScriptPtr = lactor->positionInActorScript + lactor->actorScript;

    while (OPbreak != -1)
	{
	    opcodePtr = actorScriptPtr++;
	    opcode = *(opcodePtr);
	   // printf("(%d)opcode: %d\n",actorNumber,opcode);
	    if (opcode <= 105)
		{
		    localScriptPtr = actorScriptPtr + 1;
		    switch (opcode)
			{
			case 0:
			   {
			       OPbreak = -1;
			       lactor->positionInActorScript = -1;
			       break;
			   }

			case 2:
			   {
			       manipActor(lactor);
			       if (!doCalc())
				   {
				       *opcodePtr = 13;
				   }
			       actorScriptPtr = lactor->actorScript + *(short int *) actorScriptPtr;
			       break;
			   }

			case 3:
			   {
			       actorScriptPtr = lactor->actorScript + *(short int *) actorScriptPtr;
			       break;
			   }

			case 4:
			   {
			       manipActor(lactor);
			       doCalc();
			       actorScriptPtr = lactor->actorScript + *(short int *) actorScriptPtr;
			       break;
			   }
			case 10:
			   {
			       actorScriptPtr++;
			       break;
			   }

			case 11:
			   {
			       OPbreak = -1;
			       break;
			   }

			case 12:
			   {
			       manipActor(lactor);
			       if (!doCalc())
				   {
				       actorScriptPtr =
					   lactor->actorScript + *(short int *) actorScriptPtr;
				   }
			       else
				   {
				       actorScriptPtr += 2;
				   }
			       break;
			   }

			case 13:
			   {
			       manipActor(lactor);
			       if (!doCalc())
				   {
				       actorScriptPtr =
					   lactor->actorScript + *(short int *) actorScriptPtr;
				   }
			       else
				   {
				       actorScriptPtr += 2;
				       *opcodePtr = 2;
				   }
			       break;
			   }

			case 14:
			   {
			       manipActor(lactor);
			       if (!doCalc())
				   {
				       actorScriptPtr =
					   lactor->actorScript + *(short int *) actorScriptPtr;

				   }
			       else
				   {
				       actorScriptPtr += 2;
				       *opcodePtr = 4;	// le met en always branch

				   }
			       break;
			   }

			case 15:
			   {
			       actorScriptPtr = lactor->actorScript + *(short int *) actorScriptPtr;
			       break;
			   }

			case 17:	// loadCostume
			   {
			       loadActorCostume(*(actorScriptPtr++), actorNumber);
			       break;
			   }

			case 18:	// loadCostumeAnother
			   {
			       int param1;
			       int param2;

			       param1 = *(actorScriptPtr++);
			       param2 = *(actorScriptPtr++);

			       loadActorCostume(param2, param1);
			       break;
			   }

			case 19:	// playAnim
			   {
			       playAnim(*(actorScriptPtr++), 0, 0, actorNumber);
			       break;
			   }

			case 20:	// playAnimAnother
			   {
			       playAnim(*(actorScriptPtr + 1), 0, 0, *actorScriptPtr);
			       actorScriptPtr += 2;
			       break;
			   }
			case 21:
			   {
			       lactor->positionInActorScript = *(short int *) actorScriptPtr;
			       actorScriptPtr += 2;
			       break;
			   }
			case 22:
			   {
			       actor *tempActor;

			       tempActor = &actors[*(actorScriptPtr++)];

			       tempActor->positionInActorScript = *(short int *) actorScriptPtr;
			       actorScriptPtr += 2;
			       break;
			   }

			case 23:	// SET_TRACK
			   {
			       lactor->positionInMoveScript =
				   *(unsigned short int *) actorScriptPtr;
			       actorScriptPtr += 2;
			       break;
			   }

			case 24:	// setTrackAnother
			   {
			       int actorNumTemp;

			       actorNumTemp = *(actorScriptPtr++);
			       actors[actorNumTemp].positionInMoveScript =
				   *((short int *) actorScriptPtr);
			       actorScriptPtr += 2;
			       break;
			   }

			case 25:
			   {
			       freezeTime();
			       mainLoop2(1);
			      //                             if(showTalkIcon)
			      //                             drawTalkIcon(actorNumber);
			       setNewTextColor(lactor->talkColor);	// setTextColor
			      //                             talkingActor=actorNumber;
			       printTextFullScreen(*(short int *) actorScriptPtr);
			       actorScriptPtr += 2;
			       unfreezeTime();
			       fullRedraw(1);
			      //                             waitForKey();
			       break;
			   }

			case 26:
			   {
			       int ltemp = *(actorScriptPtr++);

			       ltemp <<= 11;
			       lactor->field_60 &= 0xF7FF;
			       lactor->field_60 |= ltemp;
			       break;
			   }

			case 27:	// SET_COMPORTEMENT
			   {
			       temp = *(actorScriptPtr++);
			       lactor->field_40 = temp;
			       if (temp == 2)
				   {
				       lactor->field_54 = *(actorScriptPtr++);
				   }
			       break;
			   }

			case 28:
			   {
			       actor *tempActor;

			       tempActor = &actors[*(actorScriptPtr++)];
			       temp = *(actorScriptPtr++);
			       tempActor->field_40 = temp;

			       if (temp == 2)
				   {
				       tempActor->field_54 = *(actorScriptPtr++);
				   }

			       break;
			   }

			case 29:	// setCameraActor
			   {
			       int newActorToFollow;

			       newActorToFollow = *(actorScriptPtr++);

			       if (reinitVar8 != newActorToFollow)
				   {
				       newCameraX = (actors[newActorToFollow].X + 256) / 512;
				       newCameraZ = (actors[newActorToFollow].Z + 256) / 256;
				       newCameraY = (actors[newActorToFollow].Y + 256) / 512;

				       reinitVar8 = newActorToFollow;
				       mainLoopVar2 = 1;
				   }

			       break;
			   }

			case 30:
			   {
			       playAnim(ANIM_static, 0, 255, 0);
			       changeTwinsenComp(*(actorScriptPtr++));
			       break;
			   }

			case 31:
			   {
			       unsigned char temp1;
			       unsigned char temp2;

			       temp1 = *(actorScriptPtr++);
			       temp2 = *(actorScriptPtr++);

			       cubeFlags[temp1] = temp2;
			       break;
			   }

			case 32:
			   {
			       actorScriptPtr++;
			       break;
			   }

			case 33:
			   {
			       lactor->positionInActorScript = *(short int *) actorScriptPtr;
			       actorScriptPtr += 2;
			       break;
			   }

			case 34:
			   {
			       int tempActorNum;

			       tempActorNum = *(actorScriptPtr++);
			       actors[tempActorNum].positionInActorScript =
				   *((short int *) actorScriptPtr);
			       actorScriptPtr += 2;
			       break;
			   }

			case 35:
			   {
			       OPbreak = -1;
			       break;
			   }

			case 36:	// SET_VAR_GAME
			   {
			       unsigned char temp1;
			       char temp2;

			       temp1 = *(actorScriptPtr++);
			       temp2 = *(actorScriptPtr++);

			       vars[temp1] = temp2;
			       break;
			   }
			case 37:
			   {
			       int temp;

			       temp = *(actorScriptPtr++);

			      //                             removeActorFromRoom(temp);

			       actors[temp].field_62 |= 0x20;
			       actors[temp].costumeIndex = -1;
			       actors[temp].zone = -1;
			       actors[temp].life = 0;

			       break;
			   }

			case 38:
			   {
			      //                             removeActorFromRoom(actorNumber);
			       lactor->field_62 |= 0x20;
			       lactor->costumeIndex = -1;
			       lactor->zone = -1;
			       lactor->life = 0;
			       break;
			   }

			case 39:
			   {
			       numKey--;

			       if (numKey < 0)
				   numKey = 0;

			      //                                      showUsedItem2D(0,6,0,0,0,1,1);
			       break;
			   }
			case 40:
			   {
			       int eax;
			       int ecx;
			       roomData2Struct *edi;
			       int oldNumCoin;

			       short int cost;

			       cost = *(short int *) actorScriptPtr;
			       actorScriptPtr += 2;

			       oldNumCoin = numCoin;
			       numCoin -= cost;
			       if (numCoin < 0)
				   numCoin = 0;

			      //                                      showUsedItem2D(0,3,10,15,0,0,3);

			       eax = 0;
			       ecx = 0;
			       edi = roomData2;

			       while (eax < 10)
				   {
				       if (edi->field_0 != -1 && edi->field_6 == 2)
					   {
					      //                                                      edi->field_0=addRoomData2Entry(edi->field_8,edi->field_0,100,edi->field_C-time-50);
					       edi->field_8 = numCoin;
					       edi->field_C = time + 150;
					       ecx = 1;
					       break;
					   }
				       edi++;
				       eax++;
				   }

			       if (!ecx)
				   {
				      //                                              showUsedItem2D(2,oldNumCoin,30,20,numCoin,ecx,3);
				   }

			       break;

			   }
			case 41:
			   {
			       OPbreak = -1;
			       lactor->positionInActorScript = -1;
			       break;
			   }
			case 42:	// stopTrack
			   {
			       lactor->field_5E = lactor->currentLabelPtr;
			       lactor->positionInMoveScript = -1;
			       break;
			   }
			case 43:	// resumeTrack
			   {
			       lactor->positionInMoveScript = lactor->field_5E;
			       break;
			   }
			case 44:
			   {
			       int temp;
			       int textNum;

			       freezeTime();
			       mainLoop2(1);

			       temp = *(actorScriptPtr++);

			      /*
			         if(showTalkVar) 
			         {
			         drawTalkIcon(temp);
			         } 
			       */

			       setNewTextColor(actors[temp].talkColor);
			      //                             talkingActor=temp;

			       textNum = *(short int *) actorScriptPtr;

			       printTextFullScreen(textNum);

			       actorScriptPtr += 2;

			       unfreezeTime();

			       fullRedraw(1);

			      //                             waitForKey();

			       break;
			   }

			case 45:
			   {
			       chapter++;
			       break;
			   }
			case 46:
			   {
			       int temp;

			       freezeTime();
			       mainLoop2(1);

			       temp = *(actorScriptPtr++);

			      //                                      actorTalk(temp);

			       unfreezeTime();
			       fullRedraw(1);
			       break;
			   }
			case 47:
			   {
			       lactor->angle = 0x300;
			       lactor->X = lactor->lastX - *((short int *) actorScriptPtr);
			       lactor->field_62 &= 0xFFBF;
			       lactor->field_34 = 0;
			       actorScriptPtr += 2;
			       break;
			   }
			case 48:
			   {
			       lactor->angle = 0x100;
			       lactor->X = lactor->lastX + *((short int *) actorScriptPtr);
			       lactor->field_62 &= 0xFFBF;
			       lactor->field_34 = 0;
			       actorScriptPtr += 2;
			       break;
			   }
			case 49:
			   {
			       lactor->angle = 0x200;
			       lactor->Y = lactor->lastY - *((short int *) actorScriptPtr);
			       lactor->field_62 &= 0xFFBF;
			       lactor->field_34 = 0;
			       actorScriptPtr += 2;
			       break;
			   }
			case 50:
			   {
			       lactor->angle = 0;
			       lactor->Y = lactor->lastY + *((short int *) actorScriptPtr);
			       lactor->field_62 &= 0xFFBF;
			       lactor->field_34 = 0;
			       actorScriptPtr += 2;
			       break;
			   }
			case 51:
			   {
			       char temp;

			       if (lactor->field_10 & 0x1F0)
				   {
				       addObject(lactor);
				   }

			       temp = *(actorScriptPtr++);

			       if (temp != 0)
				   {
				       lactor->field_10 |= 1;
				   }
			       break;
			   }
			case 52:
			   {
			       reinitVar11 = 2;
			       needChangeRoom = *(actorScriptPtr++);
			       break;
			   }
			case 53:
			   {
			       if (*(actorScriptPtr++) != 0)
				   {
				       lactor->field_60 |= 1;	// toggle actor collision
				   }
			       else
				   {
				       lactor->field_60 &= 0xFFFE;
				   }
			       break;
			   }
			case 54:	// toggle wall collision
			   {
			       char temp;

			       temp = *(actorScriptPtr++);

			       lactor->field_60 &= 0xFFDD;

			       if (temp == 1)
				   {
				       lactor->field_60 &= 0xFFDD;
				       lactor->field_60 |= 2;
				   }
			       else if (temp == 2)
				   {
				       lactor->field_60 |= 0x22;
				   }
			       break;
			   }
			case 55:
			   {
			       manipActor(lactor);
			       if (doCalc())
				   {
				       actorScriptPtr =
					   lactor->actorScript + *(short int *) actorScriptPtr;
				   }
			       else
				   {
				       actorScriptPtr += 2;
				   }
			       break;
			   }
			case 56:	// invisible
			   {
			       if (*(actorScriptPtr++) != 0)
				   {
				       lactor->field_60 |= 0x200;
				   }
			       else
				   {
				       lactor->field_60 &= 0xFDFF;
				   }
			       break;
			   }
			case 57:
			   {
			       char temp;

			       temp = *(actorScriptPtr++);

			       if (temp != 0)
				   {
				       if (drawInGameTransBox == 0)
					   {
					      /*
					         if(zoomMode!=0)
					         {
					         fadeOut(menuPal);
					         //                                                           enterZoom();
					         resetPalette();
					         mainLoopVar3=1;
					         }
					       */
					   }
				   }
			       else
				   {
				       if (drawInGameTransBox != 0)
					   {
					      //                                                      fadeOut(menuPal);
					      //                                                      exitZoom();
					       resetPalette();
					       mainLoopVar3 = 1;
					       mainLoopVar2 = 1;
					   }
				   }
			       break;
			   }
			case 58:
			   {
			       manipActorResult = *(actorScriptPtr++);	// position flag number

			       destX = flagData[manipActorResult].x;
			       destZ = flagData[manipActorResult].z;
			       destY = flagData[manipActorResult].y;

			       lactor->X = destX;
			       lactor->Z = destZ;
			       lactor->Y = destY;
			       break;
			   }
			case 59:
			   {
			       magicLevel = *(actorScriptPtr++);
			       magicPoint = magicLevel * 20;
			       break;
			   }
			case 60:
			   {
			       magicPoint = *(actorScriptPtr++);
			       if (magicPoint < 0)
				   magicPoint = 0;

			       break;
			   }
			case 61:
			   {
			       char temp1;
			       char temp2;

			       temp1 = *(actorScriptPtr++);
			       temp2 = *(actorScriptPtr++);

			       actors[temp1].life = temp2;
			       break;
			   }
			case 62:	// LM_SUB_LIFE_POINT_OBJ
			   {
			       char localActorNumber;
			       char subLife;

			       localActorNumber = *(actorScriptPtr++);
			       subLife = *(actorScriptPtr++);

			       actors[localActorNumber].life -= subLife;

			       if (actors[localActorNumber].life < 0)
				   actors[localActorNumber].life = 0;

			   }
			case 63:	//LM_HIT_OBJ
			   {
			       printf("Ignoring actorScript opcode 63\n");
			       actorScriptPtr += 2;
			       break;
			   }
			case 64:	//LM_PLAY_FLA
			   {
			       printf("Play FLA: %s\n", actorScriptPtr);
			       actorScriptPtr += strlen((char *) actorScriptPtr) + 1;
			       break;
			   }
			case 67:
			    int entryTemp;

			    entryTemp = *(actorScriptPtr++);
			    if (entryTemp < 24)
				itemUsed[entryTemp] = 1;

			    break;
			case 68:
			    printf("Ignoring actorScript opcode 68\n");
			    actorScriptPtr += 2;
			    break;
			case 69:
			    printf("Ignoring actorScript opcode 69\n");
			    actorScriptPtr += 2;
			    break;
			case 70:
			   {
			       int textNumber;

			       freezeTime();
			       mainLoop2(1);
			       newGame2();

			      /*
			       * if(showTalkVar) { drawTalkIcon(actorNumber); } 
			       */

			       setNewTextColor(lactor->talkColor);

			      // talkingActor=actorNumber;

			       textNumber = *(short int *) actorScriptPtr;

			       printTextFullScreen(textNumber);

			       actorScriptPtr += 2;

			       newGame4();

			       fullRedraw(1);

			       unfreezeTime();

			      // waitForKey();
			       break;
			   }

			case 71:
			    byte newActor;
			    newActor = *(actorScriptPtr++);
			    actors[newActor].field_62 |= 0x20;
			    currentPingouin = newActor;
			    actors[newActor].costumeIndex = -1;
			    actors[newActor].zone = -1;
			    break;

			case 72:
			   {
			       printf("Skipping actorScript opcode 72\n");
			       actorScriptPtr++;
			       break;
			   }

			case 76:
			    currentGrid2 = *(actorScriptPtr++);
			   // load_LBAGRI(currentGrid2);
			    printf("Skipping grid reload\n");
			    break;
			case 77:
			    printf("Unsupported actor opcode 77\n");
			    actorScriptPtr += 2;
			    break;
			case 80:
			   {
			       short int newAngle;

			       newAngle = *(short int *) actorScriptPtr;
			       actorScriptPtr += 2;
			       lactor->angle = newAngle;
			       changeActorAngle(lactor);
			       break;
			   }
			case 88:
			   {
			       actorScriptPtr++;
			       printf("Ignoring opcode 88 in runActorScript\n");
			       break;
			   }
			case 95:
			   {
			       lactor->anim = -1;
			       lactor->currentAnim = -1;
			       playAnim(*(actorScriptPtr++), 0, 0, actorNumber);
			       break;
			   }
			case 99:
			   {
			       printf("Stop music!\n");
			       break;
			   }
			case 100:
			   {
			       int temp;

			       temp = *(actorScriptPtr++);
			       printf("Play cd track %d\n", temp);
			       break;
			   }

			case 102:	//LM_PROJ_3D
			    resetVideoBuffer1();
			    copyToBuffer(videoBuffer1, videoBuffer2);
			    osystem->drawBufferToScreen(videoBuffer1);
			    changeRoomVar10 = 0;
			   // changeCameraAngle2(320,240,1024,1024);
			   // setCameraAngle(0,1500,0,25,-128,0,13000);
			    setSomething4(896, 950, 0);
			    loadTextBank(1);
			    break;
			case 103:
			   {
			       int temp;
			       int esi;
			       int edi;

			       if (drawVar1 < 440)
				   {
				       temp = *(short int *) actorScriptPtr;
				       if (setup_lst != 0)
					   {
					       if (!temp)
						   temp = 16;
					   }

				       printString(temp, string);

				       actorScriptPtr += 2;

				       esi = edi = getStringLength(string);

				       setTextColor(15);

				       printStringSimple(0, drawVar1, string);

				       if (esi > 639)
					   edi = 639;

				       osystem->refresh(videoBuffer1, 0, drawVar1, edi,
							lactor->field_28);
				       drawVar1 += 40;
				   }

			       break;
			   }
			case 104:
			    drawVar1 = 0;
			    drawBlackBox(0, 0, 639, 240, 0);
			    osystem->refresh(videoBuffer1, 0, 0, 639, 240);
			    break;
			default:
			   {
			       printf("Unhandled actorscript opcode %d\n", opcode);
			       exit(1);
			       break;
			   }
			}
		}
	    else
		{
		    printf("Warning: opcode too big: %d !\n", opcode);
		   // exit (1);
		}
	}

}

void LBA_engine::manipActor(actor * lactor)
{
    unsigned char opcode;
    unsigned char *localScriptPtr;
    actor *lactor2;
    int temp;

    manipActorVar1 = 0;
    opcode = *(actorScriptPtr++);

   // printf("opcode:%d\n",opcode);

    if (opcode > 29)
	{
	    printf("Manip actor too big!\n");
	    return;
	}

    localScriptPtr = actorScriptPtr + 1;

    switch (opcode)
	{
	case 0:
	    if (lactor->life <= 0)
		manipActorResult = -1;
	    else
		manipActorResult = lactor->collision;
	    break;
	case 1:
	    lactor2 = &actors[*(actorScriptPtr++)];
	    if (lactor2->life <= 0)
		{
		    manipActorResult = -1;
		}
	    else
		{
		    manipActorResult = lactor2->collision;
		}
	    break;
	case 2:
	    lactor2 = &actors[*(actorScriptPtr++)];
	    manipActorVar1 = 1;
	    if (!(lactor2->field_62 & 0x20))
		{
		    if (lactor2->Z - lactor->Z >= 1500)
			{
			    manipActorResult = 32000;
			}
		    else
			{
			    manipActorResult =
				getDistanceToward(lactor->X, lactor->Y, lactor2->X, lactor2->Y);
			    if (manipActorResult > 32000)
				manipActorResult = 32000;
			}
		}
	    else
		{
		    manipActorResult = 32000;
		}
	    break;
	case 3:
	    manipActorResult = lactor->zone;
	    break;
	case 4:
	    manipActorResult = actors[*(actorScriptPtr++)].zone;
	    break;
	case 5:
	    manipActorResult = lactor->body;
	    break;
	case 6:
	    manipActorResult = actors[*(actorScriptPtr++)].body;
	    break;
	case 7:
	    manipActorResult = lactor->anim;
	    break;
	case 8:
	    manipActorResult = actors[*(actorScriptPtr++)].anim;
	    break;
	case 9:
	    manipActorResult = lactor->label;
	    break;
	case 10:
	    manipActorResult = actors[*(actorScriptPtr++)].label;
	    break;
	case 11:
	    manipActorResult = cubeFlags[*(actorScriptPtr++)];
	    break;
	case 12:
	    short int angle;
	    int newActor;

	    angle = 0;		// todo: not supposed to have that

	    newActor = *actorScriptPtr;
	    lactor2 = &actors[newActor];
	    manipActorVar1 = 1;
	    actorScriptPtr = localScriptPtr;
	    if (!(lactor2->field_62 & 0x20))
		{
		    if (lactor2->Z - lactor->Z < 1500)
			{
			    angle = calcAngleToward(lactor->X, lactor->Y, lactor2->X, lactor2->Y);
			    if (moveActorVar1 > 32000)
				moveActorVar1 = 32000;
			}
		    else
			{
			    moveActorVar1 = 32000;
			}

		    if (!newActor)
			{
			    int newAngle;

			    newAngle = (lactor->angle + 0x480) - (angle + 0x400);
			    newAngle &= 0x3FF;

			    if (newAngle >= 0x100)
				{
				    manipActorResult = 32000;
				}
			    else
				{
				    manipActorResult = moveActorVar1;
				}
			}
		}
	    else
		{
		    manipActorResult = 32000;
		}
	    break;
	case 13:
	    manipActorResult = lactor->hitBy;
	    break;
	case 14:
	    manipActorResult = action;
	    break;
	case 15:		// VAR_GAME
	    temp = *(actorScriptPtr++);
	    if (!vars[28] || (vars[28] && temp >= 28))
		{
		    manipActorResult = vars[temp];
		}
	    else
		{
		    if (temp == 70)
			manipActorResult = vars[temp];
		    else
			manipActorResult = 0;
		}
	    break;
	case 16:
	    manipActorResult = lactor->life;
	    break;
	case 17:
	    manipActorResult = actors[*(actorScriptPtr++)].life;
	    break;
	case 18:
	    manipActorResult = numKey;
	    break;
	case 19:
	    manipActorVar1 = 1;
	    manipActorResult = numCoin;
	    break;
	case 20:
	    manipActorResult = comportementHero;
	    break;
	case 21:
	    manipActorResult = chapter;
	    break;
	case 22:
	    lactor2 = &actors[*actorScriptPtr];
	    manipActorVar1 = 1;
	    actorScriptPtr = localScriptPtr;
	    if (!(lactor2->field_62 & 0x20))
		{
		    manipActorResult =
			anotherSqrt(lactor->X, lactor->Z, lactor->Y, lactor2->X, lactor2->Z,
				    lactor2->Y);

		    if (manipActorResult > 32000)
			manipActorResult = 32000;
		}
	    else
		{
		    manipActorResult = 32000;
		}
	    break;
	case 25:
	   {
	       int temp;

	       temp = *actorScriptPtr;
	       actorScriptPtr = localScriptPtr;
	       if (holomapMode == 0)
		   {
		       if (temp == mainLoopVar9)
			   {
			       manipActorResult = 1;
			   }
		       else
			   {
			       if (itemUsed[temp] == 1 && vars[temp] == 1)
				   {
				       manipActorResult = 1;
				   }
			       else
				   {
				       manipActorResult = 0;
				   }
			   }

		       if (manipActorResult == 1)
			   printf("Partially unimplemented actor manip opcode 25\n");
		   }
	       else
		   {
		       manipActorResult = 0;
		   }
	   }

	case 28:
	    manipActorResult = lactor->standOn;
	    break;

	case 29:
	   {
	       manipActorResult = 1;
	       break;
	   }

	default:
	    printf("Unhandled manipActor opcode %d\n", opcode);
	    exit(1);
	}
}

int LBA_engine::doCalc(void)
{
    unsigned char *localActorScript = actorScriptPtr;
    unsigned char opcode;
    int opcode2;
    int localManipActorResult = manipActorResult;
    unsigned char result = 0;

    opcode = *(localActorScript++);

    switch (manipActorVar1)
	{
	case 0:
	   {
	       opcode2 = *(localActorScript++);
	       break;
	   }
	case 1:
	   {
	       opcode2 = *(short int *) localActorScript;
	       localActorScript += 2;
	       break;
	   }
	default:
	   {
	       printf("Unsupported manipActorVar1 in docalc!\n");
	       exit(1);
	   }
	}
    switch (opcode)
	{
	case 0:
	    if (localManipActorResult == opcode2)
		result = 1;
	    break;
	case 1:
	    if (localManipActorResult > opcode2)
		result = 1;
	    break;
	case 2:
	    if (localManipActorResult < opcode2)
		result = 1;
	    break;
	case 3:
	    if (localManipActorResult >= opcode2)
		result = 1;
	    break;
	case 4:
	    if (localManipActorResult <= opcode2)
		result = 1;
	    break;
	case 5:
	    if (localManipActorResult != opcode2)
		result = 1;
	    break;
	default:
	    printf("Unhandled doCalc %d\n", opcode);
	    exit(1);
	}

    manipActorResult = localManipActorResult;
    actorScriptPtr = localActorScript;

    return (result);

}

int LBA_engine::getDistanceToward(int X1, int Y1, int X2, int Y2)
{
    int Xdata;
    int Ydata;

    Xdata = X2 - X1;
    Xdata *= Xdata;

    Ydata = Y2 - Y1;
    Ydata *= Ydata;

    return ((int) sqrt(Xdata + Ydata));
}

int LBA_engine::calcAngleToward(int X1, int Y1, int X2, int Y2)
{
    int newX;
    int newY;
    int ebp;
    int edi;
    int eax;
    int tempExchange;
    int esi;
    int ebx;
    int flag;
    int difX;
    int difY;

    difY = edi = Y2 - Y1;
    newY = edi * edi;

    difX = ebp = X2 - X1;
    newX = ebp * ebp;

    if (newX < newY)		// exchange X and Y
	{
	    tempExchange = difX;
	    difX = difY;
	    difY = tempExchange;

	    flag = 1;
	}
    else
	{
	    flag = 0;
	}

    moveActorVar1 = (int) sqrt(newX + newY);

    if (!moveActorVar1)
	return (0);

    int destVal;
    int startAngle;
    int stopAngle;
    int finalAngle;

    destVal = (difY << 14) / moveActorVar1;

    startAngle = 0;
    stopAngle = 0x100;

    while (tab3[startAngle] > destVal)
	{
	    startAngle++;
	   /*
	    * if (startAngle > 256) { printf ("Can't find angle %d...\n", destVal); exit (1); } 
	    */
	}

    if (tab3[startAngle] != destVal)
	if ((tab3[startAngle - 1] + tab3[startAngle]) / 2 <= destVal)
	    {
		startAngle--;
	    }

    finalAngle = 128 + startAngle;

    if (difX <= 0)		// if we are going left
	{
	    finalAngle = -finalAngle;
	}

    if (flag & 1)		// X and Y are exchanged -> 90° rotation needed
	{
	    finalAngle = -finalAngle + 0x100;
	}

    return (finalAngle & 0x3FF);

   /*
    * do { currentAngle=(startAngle+stopAngle)/2;
    * 
    * if(destVal>tab3[currentAngle]) { stopAngle=currentAngle; } else { startAngle=currentAngle;
    * if(destVal==tab3[currentAngle]) { goto endCalc; } currentAngle=stopAngle; } }
    * while(--currentAngle);
    */

    esi = (int) tab3;
    edi = esi + 0x200;

    do
	{
	    ebx = esi;
	    ebx += edi;
	    ebx >>= 1;

	    if (eax > *(short int *) ebx)
		{
		    edi = ebx;
		}
	    else
		{
		    esi = ebx;
		    if (eax == *(short int *) ebx)
			{
			    goto endCalc;
			}
		    ebx = edi;
		}
	    ebx -= esi;
	}
    while (--ebx > 2);

    if ((*(short int *) esi + *(short int *) edi) / 2 <= eax)
	{
	    esi = edi;
	}

  endCalc:

    esi -= (int) tab2;
    eax = esi;
    eax >>= 1;

    if (ebp <= 0)
	{
	    eax = -eax;
	}

    if (ebp & 1)		// (newX < newY) ie plus loin sur Y que sur X 
	{
	    eax = -eax;
	    eax += 0x100;
	}

    return (eax & 0x3FF);

}

int LBA_engine::anotherSqrt(int X1, int Z1, int Y1, int X2, int Z2, int Y2)
{
    int newX;
    int newZ;
    int newY;

    newX = X2 - X1;
    newX *= newX;

    newZ = Z2 - Z1;
    newZ *= newZ;

    newY = Y2 - Y1;
    newY *= newY;

    return ((int) sqrt(newX + newZ + newY));
}
