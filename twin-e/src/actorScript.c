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

void runActorScript(short int actorNumber)
{
    char string[256];
    short int OPbreak;
    actor *lactor;
    unsigned char opcode;
    unsigned char *localScriptPtr;
    unsigned char *opcodePtr;
    int temp;

    lactor = &actors[actorNumber];

    OPbreak = 0;

    actorScriptPtr = lactor->positionInActorScript + lactor->actorScript;

    while (OPbreak != -1)
	{
	    opcodePtr = actorScriptPtr++;
	    opcode = *(opcodePtr);
	    //printf("(%d)opcode: %d\n",actorNumber,opcode);
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
			       actorScriptPtr = lactor->actorScript + READ_LE_S16(actorScriptPtr);
			       break;
			   }

			case 3:
			   {
			       actorScriptPtr = lactor->actorScript + READ_LE_S16(actorScriptPtr);
			       break;
			   }

			case 4:
			   {
			       manipActor(lactor);
			       doCalc();
			       actorScriptPtr = lactor->actorScript + READ_LE_S16(actorScriptPtr);
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
					   lactor->actorScript + READ_LE_S16(actorScriptPtr);
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
					   lactor->actorScript + READ_LE_S16(actorScriptPtr);
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
					   lactor->actorScript + READ_LE_S16(actorScriptPtr);

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
			       actorScriptPtr = lactor->actorScript + READ_LE_S16(actorScriptPtr);
			       break;
			   }

			case 17:	// loadCostume
			   {
			       InitBody(*(actorScriptPtr++), actorNumber);
			       break;
			   }

			case 18:	// loadCostumeAnother
			   {
			       int param1;
			       int param2;

			       param1 = *(actorScriptPtr++);
			       param2 = *(actorScriptPtr++);

			       InitBody(param2, param1);
			       break;
			   }

			case 19:	// InitAnim
			   {
			       InitAnim(*(actorScriptPtr++), 0, 0, actorNumber);
			       break;
			   }

			case 20:	// playAnimAnother
			   {
			       InitAnim(*(actorScriptPtr + 1), 0, 0, *actorScriptPtr);
			       actorScriptPtr += 2;
			       break;
			   }
			case 21:
			   {
			       lactor->positionInActorScript = READ_LE_S16(actorScriptPtr);
			       actorScriptPtr += 2;
			       break;
			   }
			case 22:
			   {
			       actor *tempActor;

			       tempActor = &actors[*(actorScriptPtr++)];

			       tempActor->positionInActorScript = READ_LE_S16(actorScriptPtr);
			       actorScriptPtr += 2;
			       break;
			   }

			case 23:	// SET_TRACK
			   {
			       lactor->positionInMoveScript =READ_LE_U16(actorScriptPtr);
			       actorScriptPtr += 2;
			       break;
			   }

			case 24:	// setTrackAnother
			   {
			       int actorNumTemp;

			       actorNumTemp = *(actorScriptPtr++);
			       actors[actorNumTemp].positionInMoveScript = READ_LE_S16(actorScriptPtr);
			       actorScriptPtr += 2;
			       break;
			   }

			case 25:
			   {
			       freezeTime();
			       TestRestoreModeSVGA(1);
			      //                             if(showTalkIcon)
			      //                             DrawBulle(actorNumber);
			       TestCoulDial(lactor->talkColor);	// CoulFont
			       talkingActor=actorNumber;
			       printTextFullScreen(READ_LE_S16(actorScriptPtr));
			       actorScriptPtr += 2;
			       unfreezeTime();
			       fullRedraw(1);
			      //                             waitForKey();
			       break;
			   }

            case 26: // LM_FALLABLE
			   {
 			       int ltemp = *(actorScriptPtr++);

                   lactor->staticFlagsBF.bIsFallable = ltemp & 1;
			       break;
			   }

			case 27:	// SET_COMPORTEMENT
			   {
			       temp = *(actorScriptPtr++);
			       lactor->comportement = temp;
			       if (temp == 2)
				   {
				       lactor->cropBottom = *(actorScriptPtr++);
				   }
			       break;
			   }

			case 28:
			   {
			       actor *tempActor;

			       tempActor = &actors[*(actorScriptPtr++)];
			       temp = *(actorScriptPtr++);
			       tempActor->comportement = temp;

			       if (temp == 2)
				   {
				       tempActor->followedActor = *(actorScriptPtr++);
				   }

			       break;
			   }

			case 29:	// setCameraActor
			   {
			       int newActorToFollow;

			       newActorToFollow = *(actorScriptPtr++);

			       if (currentlyFollowedActor != newActorToFollow)
				   {
				       newCameraX = (actors[newActorToFollow].X >> 9);
				       newCameraZ = (actors[newActorToFollow].Z >> 8);
				       newCameraY = (actors[newActorToFollow].Y >> 9);

				       currentlyFollowedActor = newActorToFollow;
				       requestBackgroundRedraw = 1;
				   }

			       break;
			   }

			case 30:
			   {
			       InitAnim(ANIM_static, 0, 255, 0);
			       SetComportement(*(actorScriptPtr++));
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
			       lactor->positionInActorScript = READ_LE_S16(actorScriptPtr);
			       actorScriptPtr += 2;
			       break;
			   }

			case 34:
			   {
			       int tempActorNum;

			       tempActorNum = *(actorScriptPtr++);
			       actors[tempActorNum].positionInActorScript = READ_LE_S16(actorScriptPtr);
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

			       CheckCarrier(temp);

				   actors[temp].dynamicFlagsBF.bUnk0020 = 1;
			       actors[temp].costumeIndex = -1;
			       actors[temp].zone = -1;
			       actors[temp].life = 0;

			       break;
			   }

			case 38:
			   {
			       CheckCarrier(actorNumber);
				   lactor->dynamicFlagsBF.bUnk0020 = 1;
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

			      //showUsedItem2D(0,6,0,0,0,1,1);
			       break;
			   }
			case 40:
			   {
			       int eax;
			       int ecx;
			       overlayObjectListStruct *edi;
			       int oldNumCoin;

			       short int cost;

			       cost = READ_LE_S16(actorScriptPtr);
			       actorScriptPtr += 2;

			       oldNumCoin = numCoin;
			       numCoin -= cost;
			       if (numCoin < 0)
				   numCoin = 0;

			      //                                      showUsedItem2D(0,3,10,15,0,0,3);

			       eax = 0;
			       ecx = 0;
			       edi = overlayObjectList;

			       while (eax < 10)
				   {
				       if (edi->field_0 != -1 && edi->field_6 == 2)
					   {
					      //                                                      edi->field_0=BoundRegleTrois(edi->field_8,edi->field_0,100,edi->field_C-time-50);
					       edi->field_8 = numCoin;
					       edi->field_C = lba_time + 150;
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
			       lactor->pausedTrackPtr = lactor->currentLabelPtr;
			       lactor->positionInMoveScript = -1;
			       break;
			   }
			case 43:	// resumeTrack
			   {
			       lactor->positionInMoveScript = lactor->pausedTrackPtr;
			       break;
			   }
			case 44:
			   {
			       int temp;
			       int textNum;

			       freezeTime();
			       TestRestoreModeSVGA(1);

			       temp = *(actorScriptPtr++);

			      /*
			         if(showTalkVar) 
			         {
			         DrawBulle(temp);
			         } 
			       */

			       TestCoulDial(actors[temp].talkColor);
			       talkingActor=temp;

			       textNum = READ_LE_S16(actorScriptPtr);

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
			       TestRestoreModeSVGA(1);

			       temp = *(actorScriptPtr++);

			       foundObject(temp);

			       unfreezeTime();
			       fullRedraw(1);
			       break;
			   }
			case 47:
			   {
			       lactor->angle = 0x300;
			       lactor->X = lactor->lastX - READ_LE_S16(actorScriptPtr);
				   lactor->dynamicFlagsBF.bIsMoving = 0;
			       lactor->speed = 0;
			       actorScriptPtr += 2;
			       break;
			   }
			case 48:
			   {
			       lactor->angle = 0x100;
			       lactor->X = lactor->lastX + READ_LE_S16(actorScriptPtr);
			       lactor->dynamicFlagsBF.bIsMoving = 0;
			       lactor->speed = 0;
			       actorScriptPtr += 2;
			       break;
			   }
			case 49:
			   {
			       lactor->angle = 0x200;
			       lactor->Y = lactor->lastY - READ_LE_S16(actorScriptPtr);
			       lactor->dynamicFlagsBF.bIsMoving = 0;
			       lactor->speed = 0;
			       actorScriptPtr += 2;
			       break;
			   }
			case 50:
			   {
			       lactor->angle = 0;
			       lactor->Y = lactor->lastY + READ_LE_S16(actorScriptPtr);
			       lactor->dynamicFlagsBF.bIsMoving = 0;
			       lactor->speed = 0;
			       actorScriptPtr += 2;
			       break;
			   }
			case 51:
			   {
			       char temp;

			       if (lactor->field_10 & 0x1F0)
				   {
				       GiveExtraBonus(lactor);
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
			       twinsenPositionModeInNewCube = 2;
			       needChangeRoom = *(actorScriptPtr++);
			       break;
			   }
			case 53: // OBJ_COL
			   {
			       if (*(actorScriptPtr++) != 0)
				   {
				       lactor->staticFlagsBF.bComputeCollisionWithObj = 1;
				   }
			       else
				   {
				       lactor->staticFlagsBF.bComputeCollisionWithObj = 0;
				   }
			       break;
			   }
			case 54:	// BRICK_COL
			   {
			       char temp;

			       temp = *(actorScriptPtr++);

                   lactor->staticFlagsBF.bComputeCollisionWithBricks = false;
                   lactor->staticFlagsBF.bIsDead = false;

			       if (temp == 1)
				   {
                       lactor->staticFlagsBF.bComputeCollisionWithBricks = true;
				   }
			       else if (temp == 2)
				   {
                       lactor->staticFlagsBF.bComputeCollisionWithBricks = true;
                       lactor->staticFlagsBF.bIsDead = true;
				   }
			       break;
			   }
			case 55:
			   {
			       manipActor(lactor);
			       if (doCalc())
				   {
				       actorScriptPtr = lactor->actorScript + READ_LE_S16(actorScriptPtr);
				   }
			       else
				   {
				       actorScriptPtr += 2;
				   }
			       break;
			   }
			case 56:	// INVISIBLE
			   {
                   lactor->staticFlagsBF.bNoDisplay = *(actorScriptPtr++);
			       break;
			   }
			case 57:
			   {
			       char temp;

			       temp = *(actorScriptPtr++);

			       if (temp != 0)
				   {
					   printf("Enter zoom\n");
				       if (drawInGameTransBox == 0)
					   {
					      /*
					         if(zoomMode!=0)
					         {
					         FadeToBlack(menuPal);
					         //                                                           ExtInitMcga();
					         SetBackPal();
					         lockPalette=1;
					         }
					       */
					   }
				   }
			       else
				   {
					   printf("Exit zoom\n");
				       if (drawInGameTransBox != 0)
					   {
					      //                                                      FadeToBlack(menuPal);
					      //                                                      ExtInitSvga();
					       SetBackPal();
					       lockPalette = 1;
					       requestBackgroundRedraw = 1;
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
                   char tempActorNumber;
                   char temp;

                   tempActorNumber = *(actorScriptPtr++);
                   temp = *(actorScriptPtr++);
                   HitObj(actorNumber, tempActorNumber, temp, actors[tempActorNumber].angle);
			       break;
			   }
			case 64:	//LM_PLAY_FLA
			   {
				   int length=strlen((char*)actorScriptPtr);
			       PlayAnimFla((char*)actorScriptPtr);
			       actorScriptPtr += length + 1;
			       break;
			   }
			case 65: //LM_PLAY_MIDI
				{
					PlayMusic(*(actorScriptPtr++));
					break;
				}
			case 66: //LM_INC_CLOVER_BOX
				{
					if(numCloverBox<10)
						numCloverBox++;

					break;
				}
			case 67: //LM_SET_USED_INVENTORY
				{
					int entryTemp;

					entryTemp = *(actorScriptPtr++);

					if (entryTemp < 24)
						itemUsed[entryTemp] = 1;

					break;
				}
			case 68: //LM_ADD_CHOICE
				{
					inGameMenuData[numOfOptionsInChoice++]=READ_LE_S16(actorScriptPtr);
					actorScriptPtr+=2;
					break;
				}
			case 69: //LM_ASK_CHOICE
				{
					short int choiceNum;

					freezeTime();
					TestRestoreModeSVGA(1);

					if(showTalkVar)
					{
						//DrawBulle(actorNumber);
					}

					TestCoulDial(lactor->talkColor);

					choiceNum=READ_LE_S16(actorScriptPtr);
					actorScriptPtr+=2;

					processInGameMenu(choiceNum);

					numOfOptionsInChoice=0;

					unfreezeTime();

					fullRedraw(1);

					//waitForKey();
					break;
				}
			case 70: //LM_BIG_MESSAGE
			   {
			       int textNumber;

			       freezeTime();
			       TestRestoreModeSVGA(1);
			       newGame2();

			      /*
			       * if(showTalkVar) { DrawBulle(actorNumber); } 
			       */

			       TestCoulDial(lactor->talkColor);

			       talkingActor=actorNumber;

			       textNumber = READ_LE_S16(actorScriptPtr);

			       printTextFullScreen(textNumber);

			       actorScriptPtr += 2;

			       newGame4();

			       fullRedraw(1);

			       unfreezeTime();

			      // waitForKey();
			       break;
			   }

			case 71: //LM_INIT_PINGOUIN
				{
					byte newActor;
					newActor = *(actorScriptPtr++);
					actors[newActor].dynamicFlagsBF.bUnk0020 = 1;
					currentPingouin = newActor;
					actors[newActor].costumeIndex = -1;
					actors[newActor].zone = -1;
					break;
				}
			case 72: //LM_SET_HOLO_POS
			   {
					char position;

					position=*(actorScriptPtr++);

					printf("Set holomap position %d\n",position);
					break;
			   }
			case 73: //LM_CLR_HOLO_POS
				{
					char position;

					position=*(actorScriptPtr++);

					printf("Clear holomap position %d\n",position);
					break;
				}	
			case 74: //LM_ADD_FUEL
				{
					fuel+=*(actorScriptPtr++);
					if(fuel>100)
					{
						fuel=100;
					}
					break;
				}
			case 75: //LM_SUB_FUEL
				{
					fuel-=*(actorScriptPtr++);
					if(fuel<0)
					{
						fuel=0;
					}
					break;
				}
			case 76: //LM_SET_GRM
				{
					currentGrid2 = *(actorScriptPtr++);
					IncrustGrm(currentGrid2);
					break;
				}
			case 77: //LM_SAY_MESSAGE
				{
					short int messageNumber;

					messageNumber=READ_LE_S16(actorScriptPtr);
					actorScriptPtr+=2;

					//addOverlayObject(4,messageNumber,0,0,actorNumber,1,2);

					freezeTime();
					setVoxFileAtDigit(messageNumber);
					unfreezeTime();
					
					break;
				}
			case 78: //LM_SAY_MESSAGE_OBJ
				{
					char character;
					short int messageNumber;

					character=*(actorScriptPtr++);
					messageNumber=READ_LE_S16(actorScriptPtr);
					actorScriptPtr+=2;

					//addOverlayObject(4,messageNumber,0,0,character,1,2);

					freezeTime();
					setVoxFileAtDigit(messageNumber);
					unfreezeTime();

					break;
				}
			case 79: //LM_FULL_POINT
				{
					twinsen->life=50;
					magicPoint=magicLevel*20;
					break;
				}
			case 80: //LM_BETA
			   {
			       short int newAngle;

			       newAngle = READ_LE_S16(actorScriptPtr);
			       actorScriptPtr += 2;
			       lactor->angle = newAngle;
			       ClearRealAngle(lactor);
			       break;
			   }
			case 81: //LM_GRM_OFF
				{
					if(currentGrid2!=-1)
					{
						useAnotherGrm=-1;
						currentGrid2=-1;
						createCube();
						fullRedraw(1);
						break;
					}
				}
			case 82: //LM_FADE_PAL_RED
				{
					printf("fade pal red\n");
					break;
				}
			case 83: //LM_FADE_ALARM_RED
				{
					printf("fade alarm red\n");
					break;
				}
			case 84: //LM_FADE_ALARM_PAL
				{
					printf("fade alarm pal\n");
					break;
				}
			case 85: //LM_FADE_RED_PAL
				{
					printf("fade red pal\n");
					break;
				}
			case 86: //LM_FADE_RED_ALARM
				{
					printf("fade red alarm\n");
					break;
				}
			case 87: //LM_FADE_PAL_ALARM
				{
					printf("fade pal alarm\n");
					break;
				}
			case 88: //LM_EXPLODE_OBJ
			   {
			       actorScriptPtr++;
			       printf("Ignoring opcode 88 in runActorScript\n");
			       break;
			   }
			case 89: //LM_BULLE_ON
				{
					showTalkVar=1;
					break;
				}
			case 90: //LM_BULLE_OFF
				{
					showTalkVar=0;
					break;
				}
			case 91: //LM_ASK_CHOICE_OBJ
				{
					char currentTalkingActor;
					short int choiceNumber;

					freezeTime();
					currentTalkingActor=*(actorScriptPtr++);
					TestRestoreModeSVGA(1);
					
					if(showTalkVar)
					{
						//DrawBulle(currentTalkingActor);
					}

					TestCoulDial(actors[currentTalkingActor].talkColor);

					choiceNumber=READ_LE_S16(actorScriptPtr);
					actorScriptPtr+=2;

//					processInGameMenu(choiceNumber);
					
					unfreezeTime();
					fullRedraw(1);
					//waitForKey();
					break;
				}
			case 92: //LM_SET_DARK_PAL
				{
					freezeTime();
					Load_HQR("ress.hqr",(byte*)palette,24);
					convertPalToRGBA(palette,paletteRGBA);
					if(!lockPalette)
					{
						osystem->setPalette(paletteRGBA);
					}
					useAlternatePalette=1;
					unfreezeTime();
					break;
				}
			case 93: //LM_SET_NORMAL_PAL
				{
					useAlternatePalette=0;
					if(!lockPalette)
					{
						osystem->setPalette(menuPalRGBA);
					}
					break;
				}
			case 94: //LM_MESSAGE_SENDELL
				{
					int backupFlag;

					freezeTime();
					TestRestoreModeSVGA(1);
					FadeToBlack((char*) paletteRGBA);
					Load_HQR("ress.hqr",workVideoBuffer,25);
					CopyScreen(workVideoBuffer,frontVideoBuffer);
					Load_HQR("ress.hqr",(byte *) & palette,26);
					convertPalToRGBA(palette, paletteRGBA);
					osystem->Flip(frontVideoBuffer);
					FadeToPal((char*)paletteRGBA);
					newGame2();
					TestCoulDial(15);
					newGameVar4=0;
					backupFlag=flagDisplayText;
					flagDisplayText=1;
					printTextFullScreen(6);
					newGameVar4=1;
					newGame4();
					FadeToBlack((char*)paletteRGBA);
					Cls();
					osystem->setPalette(menuPalRGBA);
					flagDisplayText=backupFlag;

					do
					{
						readKeyboard();
					}
					while (skipIntro || key1);

					unfreezeTime();
					break;

				}
			case 95: // LM_ANIM_SET
			   { // play new anim, no transition
			       lactor->anim = -1;
			       lactor->previousAnimIndex = -1;
			       InitAnim(*(actorScriptPtr++), 0, 0, actorNumber);
			       break;
			   }
			case 96: // LM_HOLOMAP_TRAJ
				{
					holomapTraj=*(actorScriptPtr++);
					break;
				}
			case 97: // LM_GAME_OVER
				{
					OPbreak=-1;
					twinsen->dynamicFlagsBF.bUnk0004 = 1;
					twinsen->life=0;
					numClover=0;
					break;
				}
			case 98: // LM_THE_END
				{
					OPbreak=-1;
					numClover=0;
					twinsen->life=50;
					magicPoint=80;
					currentRoom=113;
					comportementHero=startupComportementHeroInCube;
					newTwinsenX=-1;
					twinsen->angle=startupAngleInCube;
					SaveGame();
					brutalExit=1;
					break;
				}
			case 99: // LM_MIDI_OFF
			   {
			       printf("Stop music!\n");
			       break;
			   }
			case 100:
			   {
			       int temp;

			       temp = *(actorScriptPtr++);
				   playCDtrack(temp);
			       break;
			   }
			case 101: //LM_PROJ_ISO
				{
					configureOrthoProjection(311,240,512);
					setSomething2(0,0,0);
					setSomething3(0,0,0);
					SetLightVector(reinitVar1,reinitVar2,0);
					break;
				}

			case 102:	//LM_PROJ_3D
			    Cls();
			    CopyScreen(frontVideoBuffer, workVideoBuffer);
			    osystem->Flip(frontVideoBuffer);
			    changeRoomVar10 = 0;

			    setCameraPosition(320,240,128,1024,1024);
			    setCameraAngle(0,1500,0,25,-128,0,13000);
			    SetLightVector(896, 950, 0);

			    InitDial(1);
			    break;
			case 103:
			   {
			       int temp;
			       int esi;
			       int edi;

			       if (drawVar1 < 440)
				   {
				       temp = READ_LE_S16(actorScriptPtr);
				       if (setup_lst != 0)
					   {
					       if (!temp)
						   temp = 16;
					   }

				       GetMultiText(temp, string);

				       actorScriptPtr += 2;

				       esi = edi = SizeFont(string);

				       CoulFont(15);

				       Font(0, drawVar1, string);

				       if (esi > 639)
					   edi = 639;

				       osystem->CopyBlockPhys(frontVideoBuffer, 0, drawVar1, edi, drawVar1+40);
				       drawVar1 += 40;
				   }

			       break;
			   }
			case 104:
			    drawVar1 = 0;
			    Box(0, 0, 639, 240, 0);
			    osystem->CopyBlockPhys(frontVideoBuffer, 0, 0, 639, 240);
			    break;
			case 105:
				brutalExit=0;
				OPbreak=-1;
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
		    exit (1);
		}
	}

}

void manipActor(actor * lactor)
{
    unsigned char opcode;
    unsigned char *localScriptPtr;
    actor *lactor2;
    int temp;

    manipActorVar1 = 0;
    opcode = *(actorScriptPtr++);

    //printf("manip opcode:%d\n",opcode);

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
		if (!(lactor2->dynamicFlagsBF.bUnk0020))
		{
		    if ( abs(lactor2->Z - lactor->Z) >= 1500)
			{
			    manipActorResult = 32000;
			}
		    else
			{
                manipActorResult = Distance2D(lactor->X, lactor->Y, lactor2->X, lactor2->Y);
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
		if (!(lactor2->dynamicFlagsBF.bUnk0020))
		{
		    if (lactor2->Z - lactor->Z < 1500)
			{
			    angle = GetAngle(lactor->X, lactor->Y, lactor2->X, lactor2->Y);
			    if (DoTrackVar1 > 32000)
				DoTrackVar1 = 32000;
			}
		    else
			{
			    DoTrackVar1 = 32000;
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
				    manipActorResult = DoTrackVar1;
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
	    if (!vars[70] || (vars[70] && temp >= 28))
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
		if (!(lactor2->dynamicFlagsBF.bUnk0020))
		{
		    manipActorResult =
			Distance3D(lactor->X, lactor->Z, lactor->Y, lactor2->X, lactor2->Z,
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
	       if (vars[70] == 0)
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
		   break;
	   }
	case 26:
		{
			manipActorVar1=1;
			manipActorResult=inGameMenuAnswer;
			break;
		}
	case 27:
		{
			manipActorResult=fuel;
			break;
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

int doCalc(void)
{
    unsigned char *localActorScript = actorScriptPtr;
    unsigned char opcode;
    int opcode2 = -1;
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
	       opcode2 = READ_LE_S16(localActorScript);
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

int Distance2D(int X1, int Y1, int X2, int Y2)
{
    int Xdata;
    int Ydata;

    Xdata = X2 - X1;
    Xdata *= Xdata;

    Ydata = Y2 - Y1;
    Ydata *= Ydata;

    return ((int) sqrt(Xdata + Ydata));
}

int GetAngle(int X1, int Y1, int X2, int Y2)
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

    DoTrackVar1 = (int) sqrt(newX + newY);

    if (!DoTrackVar1)
	return (0);

    int destVal;
    int startAngle;
    int stopAngle;
    int finalAngle;

    destVal = (difY << 14) / DoTrackVar1;

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

	    if (eax > READ_LE_S16((void*)ebx))
		{
		    edi = ebx;
		}
	    else
		{
		    esi = ebx;
		    if (eax == READ_LE_S16((void*)ebx))
			{
			    goto endCalc;
			}
		    ebx = edi;
		}
	    ebx -= esi;
	}
    while (--ebx > 2);

    if ((READ_LE_S16((void*)esi) + READ_LE_S16((void*)edi)) / 2 <= eax)
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

int Distance3D(int X1, int Z1, int Y1, int X2, int Z2, int Y2)
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

void processInGameMenu(int index)
{
	int i;

	printf("Ask choice %d\n",index);
	CopyScreen(frontVideoBuffer,workVideoBuffer);

	choiceTab[1]=numOfOptionsInChoice;
	choiceTab[0]=0;
	choiceTab[2]=0;
	choiceTab[3]=currentTextBank+3;

	for(i=0;i<numOfOptionsInChoice;i++)
	{
		choiceTab[i*2+4]=0;
		choiceTab[i*2+5]=inGameMenuData[i];
	}

	MyDial(index);

	processMenu(choiceTab);

	inGameMenuAnswer=inGameMenuData[choiceTab[0]];

	//TODO: missing giving answer vox
}

void MyDial(int index)
{
    int textStatus = 1;
    //int digitStatus;

    if(languageCD1 != 0)
    {
        setVoxFileAtDigit(index);
    }

    initText(index);
    InitDialWindow();

    while(textStatus)
    {
        textStatus = printText10();
        printText4(voxFileHandle);

        if(textStatus == 2)
        {
            while(skipIntro || key1 || printTextVar12)
            {
                printText4(voxFileHandle);
                readKeyboard();
            }

            while(!(skipIntro || key1 || printTextVar12))
            {
                printText4(voxFileHandle);
                readKeyboard();
            }
        }

        waitRetrace();
    }

    while(printText4(voxFileHandle))
    {
        readKeyboard();
    }

    printTextVar5 = 0;

    if(languageCD1 && voxFileHandle)
    {
        if(printText6(4660))
            printText7(4660);
    }

    printTextVar13 = 0;
}

void foundObject(int objectNumber)
{
    int objectX;
    int objectY;
    int objectZ;

    int object2X;
    int object2Y;
    int object2Z;

    int boxTopLeftX;
    int boxTopLeftY;
    int boxBottomRightX;
    int boxBottomRightY;

    bool voxNotFound;

    int textDisplayState;
    bool closeWindow;

    unsigned char* twinsenAnim;

    unsigned char* objectPtr;

    int twinsenAnimKeyframe;

    newCameraX = (twinsen->X + 0x100)>>9;
    newCameraZ = (twinsen->Z + 0x100)>>8;
    newCameraY = (twinsen->Y + 0x100)>>9;

    twinsen->staticFlagsBF.bNoDisplay = true;
    fullRedraw(1);
    twinsen->staticFlagsBF.bNoDisplay = false;

    CopyScreen(frontVideoBuffer, workVideoBuffer);

    objectX = newCameraX << 9;
    objectZ = newCameraZ << 8;
    objectY = newCameraY << 9;

    AffObjetIso(twinsen->X - objectX, twinsen->Z - objectZ, twinsen->Y - objectY, 0, 0x80, 0, bodyPtrTab[twinsen->costumeIndex]);

    SetClip(renderLeft, renderTop, renderRight, renderBottom);

    object2X = (twinsen->X + 0x100)>>9;
    object2Z = (twinsen->Z)>>8;
    if(twinsen->field_3 & 0x7F)
        object2Z++;
    object2Y = (twinsen->Y + 0x100)>>9;

    DrawOverBrick(object2X, object2Z, object2Y);

    osystem->Flip(frontVideoBuffer);

    projectPositionOnScreen(twinsen->X - objectX, twinsen->Z - objectZ, twinsen->Y - objectY);

    projectedPositionY -= 150;

    boxTopLeftX = projectedPositionX - 65;
    boxTopLeftY = projectedPositionY - 65;

    boxBottomRightX = projectedPositionX + 65;
    boxBottomRightY = projectedPositionY + 65;

    playSample(41, 0x1000, 1, 0x80, 0x80); 

/*    if(getFoundVox(2))
    {
        voxNotFound = false;
        InitDial(2);
    }
    else*/
    {
        int temp;

        stopMusic();
        voxNotFound = true;

        temp = languageCD1;
        languageCD1 = 0;
        InitDial(2);
        languageCD1 = temp;
    }

    UnSetClip();
    OpenDial(objectNumber);

    textDisplayState = 1;
    closeWindow = false;

    TestCoulDial(4);

    if(!voxNotFound)
    {
        //startFoundVox(2, objectNumber);
    }
    else
    {
        setVoxFileAtDigit(objectNumber);
    }

    twinsenAnim = HQR_Get(HQR_Anims,getAnimIndexForBody( ANIM_findObject, 0));

    bufAni2+=StockInterAnim((char*)bufAni2,(char*)bodyPtrTab[twinsen->costumeIndex], &twinsen->animTimerData);
    if(bufAni1+4488 > bufAni2)
        bufAni2 = bufAni1;

    twinsenAnimKeyframe = 0;

    objectPtr = HQR_Get(HQR_Inventory, objectNumber);

    if(HQR_Flag)
        loadGfxSub(objectPtr);

    numOfRedrawBox = 0;

    while(!closeWindow)
    {
        UnSetClip();
        fullRedrawVar8=0;
        blitBackgroundOnDirtyBoxes();
        drawBoxInsideTrans(boxTopLeftX, boxTopLeftY, boxBottomRightX, boxBottomRightY, 4);

        SetClip(boxTopLeftX, boxTopLeftY, boxBottomRightX, boxBottomRightY);

        objectRotation[objectNumber] += 8;

//        Draw3dObject(projectedPositionX, projectedPositionY, objectPtr, objectRotation[objectNumber], 10000);

        DrawCadre(boxTopLeftX, boxTopLeftY, boxBottomRightX, boxBottomRightY);
        AddPhysBox(boxTopLeftX, boxTopLeftY, boxBottomRightX, boxBottomRightY);
        UnSetClip();
        reinitAll1();

		if(SetInterAnimObjet(twinsenAnimKeyframe, (char*)twinsenAnim, (char*)bodyPtrTab[twinsen->costumeIndex], &twinsen->animTimerData))
        {
            twinsenAnimKeyframe++;
            if(twinsenAnimKeyframe == GetNbFramesAnim((char*)twinsenAnim))
            {
                twinsenAnimKeyframe = GetBouclageAnim((char*)twinsenAnim);
            }
        }

        AffObjetIso(twinsen->X - objectX, twinsen->Z - objectZ, twinsen->Y - objectY, 0, 0x80, 0, bodyPtrTab[twinsen->costumeIndex]);
        SetClip(renderLeft, renderTop, renderRight, renderBottom);
        DrawOverBrick(object2X, object2Z, object2Y);
        AddPhysBox(renderLeft, renderTop, renderRight, renderBottom);

        if(textDisplayState)
        {
            UnSetClip();
            textDisplayState = printText10();
        }

        if(textDisplayState)
        {
            waitRetrace();
        }

        FlipBoxes();

        readKeyboard();
        if(key1)
        {
            if(!textDisplayState)
            {
                closeWindow = true;
            }

            if( textDisplayState == 2)
                textDisplayState = 1;
        }
    }

 /*   if(!voxNotFound)
    {
        while(closeFoundVox())
        {
            readKeyboard();
            if(skipIntro==1)
                break;
        }
    }
    else*/
    {
        while(printText11())
        {
            readKeyboard();
            if(skipIntro==1)
                break;
        }
    }

//    stopVox();
    reinitAll1();
    InitDial(currentTextBank + 3);

    while(skipIntro==1)
    {
        readKeyboard();
    }
}

void OpenDial(int textNumber)
{
    initText(textNumber);
    InitDialWindow();
}

void Draw3dObject(int X, int Y, char* objectPtr, int rotation, int param)
{
    setCameraPosition(X, Y, 128, 200, 200);
    setCameraAngle(0,0,0,60,0,0,param);

    AffObjetIso(0,0,0,0,rotation, 0, (unsigned char*)objectPtr);
}