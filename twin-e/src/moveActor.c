#include "lba.h"

void DoTrack(int actorNumber)
{
    int continueMove;
    int positionInScript;
    char currentOpcode;
    char *scriptPtr;

    actor *lactor;
    timeStruct *timePtr;

    lactor = &actors[actorNumber];
    continueMove = 1;
    timePtr = &lactor->time;

    do
	{
	    positionInScript = lactor->positionInMoveScript;
	    scriptPtr = (char *) (lactor->positionInMoveScript + lactor->moveScript);
	    currentOpcode = *(scriptPtr++);
	    lactor->positionInMoveScript++;

	    switch (currentOpcode)
		{
		case 0:	// stop
		   {
		       continueMove = 0;
		       lactor->positionInMoveScript = -1;
		       break;
		   }
		case 2:
		    InitBody(*scriptPtr, actorNumber);
		    lactor->positionInMoveScript++;
		    break;
		case 3:	// ANIM
		    if (InitAnim(*(scriptPtr++), 0, 0, actorNumber))
			{
			    lactor->positionInMoveScript++;
			}
		    else
			{
			    lactor->positionInMoveScript = positionInScript;
			    continueMove = 0;
			}
		    break;
		case 4:	// GOTO_POINT
		    int newAngle;

		    lactor->positionInMoveScript++;
		    manipActorResult = *scriptPtr;

		    destX = flagData[manipActorResult].x;
		    destZ = flagData[manipActorResult].z;
		    destY = flagData[manipActorResult].y;

		    newAngle = GetAngle(lactor->X, lactor->Y, destX, destY);

		    if (lactor->staticFlagsBF.bIsSpriteActor)
			{
			    lactor->angle = newAngle;
			}
		    else
			{
			    ManualRealAngle(lactor->angle, newAngle, lactor->speed,
					     &lactor->time);
			}

		    if (DoTrackVar1 > 500)
			{
			    continueMove = 0;
			    lactor->positionInMoveScript -= 2;
			}

		    break;
		case 5:
			if (!(lactor->dynamicFlagsBF.bUnk0004))
			{
			    continueMove = 0;
			    lactor->positionInMoveScript--;
			}
		    else
			{
			    continueMove = 0;
			    ClearRealAngle(lactor);
			}
		    break;

		case 7:	// set angle
		   {
		       lactor->positionInMoveScript += 2;
		       if (!(lactor->staticFlagsBF.bIsSpriteActor))
			   {
			       manipActorResult = READ_LE_S16(scriptPtr);
			       if (lactor->time.numOfStep == 0)
				   {
				       ManualRealAngle(lactor->angle, manipActorResult,
							lactor->speed, timePtr);
				   }

			       if (lactor->angle != manipActorResult)
				   {
				       continueMove = 0;
				       lactor->positionInMoveScript -= 3;
				   }
			   }
		       break;
		   }

		case 8:	// warp to flag
		    lactor->positionInMoveScript++;
		    manipActorResult = *scriptPtr;

		    printf("Actor %d warp to flag %d\n", actorNumber, manipActorResult);

		    destX = flagData[manipActorResult].x;
		    destZ = flagData[manipActorResult].z;
		    destY = flagData[manipActorResult].y;

		    if (lactor->staticFlagsBF.bIsSpriteActor)
			{
			    lactor->speed = 0;
			}

		    lactor->X = destX;
		    lactor->Z = destZ;
		    lactor->Y = destY;

		    break;

		case 9:	// LABEL
		    lactor->label = *scriptPtr;
		    lactor->positionInMoveScript++;
		    lactor->currentLabelPtr = lactor->positionInMoveScript - 2;
		    break;
		case 10:
		    lactor->positionInMoveScript = READ_LE_S16(scriptPtr);
		    break;
		case 11:	// STOP
		    continueMove = 0;
		    lactor->positionInMoveScript = -1;
		    break;

		case 12:	// move (used in backward walk)
		    lactor->positionInMoveScript++;
		    manipActorResult = *scriptPtr;

		    destX = flagData[manipActorResult].x;
		    destZ = flagData[manipActorResult].z;
		    destY = flagData[manipActorResult].y;

		    newAngle = 0x200 + GetAngle(lactor->X, lactor->Y, destX, destY);

		    if (lactor->staticFlagsBF.bIsSpriteActor)
			{
			    lactor->angle = newAngle;
			}
		    else
			{
			    ManualRealAngle(lactor->angle, newAngle, lactor->speed,
					     &lactor->time);
			}

		    if (DoTrackVar1 > 500)
			{
			    continueMove = 0;
			    lactor->positionInMoveScript -= 2;
			}

		    break;
		case 13:
		    lactor->positionInMoveScript += 2;
			if (lactor->dynamicFlagsBF.bUnk0004)
			{
			    (*(scriptPtr + 1))++;

			    if (*(scriptPtr + 1) == *scriptPtr)
				{
				    (*(scriptPtr + 1)) = 0;
				}
			    else
				{
				    continueMove = 0;
				}
			}
		    else
			{
			    continueMove = 0;
			}

		    if (continueMove == 0)
			{
			    lactor->positionInMoveScript -= 3;
			}
		    break;
		case 14:
		    HQ_3D_MixSample(READ_LE_S16(scriptPtr), 0x1000, 1, lactor->X, lactor->Z,
				 lactor->Y);
		    lactor->positionInMoveScript += 2;
		    break;
		case 15:
		    lactor->positionInMoveScript++;
		    if (lactor->staticFlagsBF.bIsSpriteActor)	// if can move
			{
			    manipActorResult = *scriptPtr;

			    destX = flagData[manipActorResult].x;
			    destZ = flagData[manipActorResult].z;
			    destY = flagData[manipActorResult].y;

			    lactor->angle = GetAngle(lactor->X, lactor->Y, destX, destY);	// X-Y move
			    lactor->field_78 = GetAngle(lactor->Z, 0, destZ, DoTrackVar1);	// Z (vertical) move

			    if (DoTrackVar1 > 100)
				{
				    continueMove = 0;
				    lactor->positionInMoveScript -= 2;
				}
			    else
				{
				    lactor->X = destX;
				    lactor->Z = destZ;
				    lactor->Y = destY;
				}
			}
		    break;
		case 16:
		    lactor->positionInMoveScript += 2;
		    lactor->speed = READ_LE_S16(scriptPtr);
            if (lactor->staticFlagsBF.bIsSpriteActor)
			{
			    setActorAngle(0, lactor->speed, 50, timePtr);
			}
		    break;
		case 17: // move actor to background
		    lactor->positionInMoveScript += 1;
		    if (*scriptPtr != 0)
			{
			    if (!(lactor->staticFlagsBF.bIsBackgrounded)) //if actor wasn't already in background
				{
				    lactor->staticFlagsBF.bIsBackgrounded = true; // set him to background
					if (lactor->dynamicFlagsBF.bUnk0010)
					{
					    requestBackgroundRedraw = 1;
					}
				}
			}
		    else
			{
                if ( lactor->staticFlagsBF.bIsBackgrounded )
				{
				    lactor->staticFlagsBF.bIsBackgrounded = false;
					if (lactor->dynamicFlagsBF.bUnk0010)
					{
					    requestBackgroundRedraw = 1;
					}
				}
			}
		    break;
		case 18:	// wait
			{
				lactor->positionInMoveScript += 5;

				if (READ_LE_S32(scriptPtr + 1) == 0)
				{
					WRITE_LE_S32(scriptPtr + 1, time + *(unsigned char *) scriptPtr *0x50);
				}

				if (time < READ_LE_S32(scriptPtr + 1))
				{
					continueMove = 0;
					lactor->positionInMoveScript -= 6;
				}
				else
				{
					WRITE_LE_S32(scriptPtr + 1, 0);
				}
				break;
			}
		case 19:
			{
				InitBody(-1, actorNumber);
				break;
			}
		case 20:
		   {
		       short int beta;

		       beta = READ_LE_S16(scriptPtr);
		       scriptPtr += 2;

		       lactor->angle = beta;

               if (lactor->staticFlagsBF.bIsSpriteActor)
			   {
			       ClearRealAngle(lactor);
			   }

		       break;
		   }

		case 21:
		case 22:
		case 23:
		case 24:
		    lactor->positionInMoveScript += 2;
		    if ( lactor->staticFlagsBF.bIsSpriteActor && lactor->staticFlagsBF.bIsUsingClipping )
			{
			    switch (currentOpcode - 21)
				{
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
				lactor->dynamicFlagsBF.bIsMoving = 1;
			    lactor->speed = 1000;
			    setActorAngle(0, 1000, 50, timePtr);
			}
		    break;
		case 25: // TM_CLOSE
		   {
		       if (lactor->staticFlagsBF.bIsSpriteActor && lactor->staticFlagsBF.bIsUsingClipping )
			   {
			       lactor->dynamicFlagsBF.bIsMoving = 1;
			       lactor->doorStatus = 0;
			       lactor->speed = -1000;
			       setActorAngle(0, -1000, 50, timePtr);
			   }
		       break;
		   }
		case 26: // TM_WAIT_DOOR
		   {
               if ( lactor->staticFlagsBF.bIsSpriteActor && lactor->staticFlagsBF.bIsUsingClipping )
			   {
			       if (lactor->speed)
				   {
				       continueMove = 0;
				       lactor->positionInMoveScript--;
				   }
			   }
		       break;
		   }
		case 27:
		   {
		       printf("skipping actor move opcode 27\n");
		       lactor->positionInMoveScript += 2;
		       break;
		   }

		case 28:
		    HQ_3D_MixSample(READ_LE_S16(scriptPtr), 0x1000, 0, lactor->X, lactor->Z,
				 lactor->Y);
		    lactor->positionInMoveScript += 2;
		    break;
		case 29:
		    printf("skipping actor move opcode 29 (playSound)\n");
		    lactor->positionInMoveScript += 2;
		    break;
		case 31:
		   {
		       moveVar1 = READ_LE_S16(scriptPtr);
		       lactor->positionInMoveScript += 2;
		       break;
		   }
		case 32:
		   {
		       printf("skipping actor move opcode 31 (playSound at moveVar1)\n");
		       lactor->positionInMoveScript += 2;
		       break;
		   }
		case 33:	// look at twinsen
		   {
		       lactor->positionInMoveScript += 2;
               if (!(lactor->staticFlagsBF.bIsSpriteActor))
			   {
			       manipActorResult = READ_LE_S16(scriptPtr);
			       if (manipActorResult == -1 && lactor->time.numOfStep == 0)
				   {
				       manipActorResult =
					   GetAngle(lactor->X, lactor->Y, twinsen->X,
							   twinsen->Y);
				       ManualRealAngle(lactor->angle, manipActorResult,
							lactor->speed, &lactor->time);
				       WRITE_LE_S16(scriptPtr, manipActorResult);
				   }

			       if (lactor->angle != manipActorResult)
				   {
				       continueMove = 0;
				       lactor->positionInMoveScript -= 3;
				   }
			       else
				   {
				       ClearRealAngle(lactor);
				       WRITE_LE_S16(scriptPtr, -1);
				   }
			   }
		       break;
		   }
        case 34: // angle random
            {
                int var_10;

                lactor->positionInMoveScript +=4 ;
                if (!(lactor->staticFlagsBF.bIsSpriteActor))
                {
                    manipActorResult = READ_LE_S16(scriptPtr+2);

                    if(manipActorResult == -1  && lactor->time.numOfStep == 0)
                    {
                        if(rand()&1)
                        {
                            manipActorResult = READ_LE_S16(scriptPtr);
                            var_10=lactor->angle + 0x100 + (abs(manipActorResult)>>1);

                            manipActorResult = (var_10 - (rand()%manipActorResult)) & 0x3FF;
                        }
                        else
                        {
                            manipActorResult = READ_LE_S16(scriptPtr);
                            var_10=lactor->angle - 0x100 + (abs(manipActorResult)>>1);

                            manipActorResult = (var_10 - (rand()%manipActorResult)) & 0x3FF;
                        }

                        ManualRealAngle(lactor->angle,manipActorResult, lactor->speed, &lactor->time);
                        WRITE_LE_S16(scriptPtr+2, manipActorResult);
                    }

                    if(lactor->angle != manipActorResult)
                    {
                        continueMove = 0;
                        lactor->positionInMoveScript -= 5;
                    }
                    else
                    {
                        ClearRealAngle(lactor);
                        WRITE_LE_S16(scriptPtr+2, -1);
                    }
                }
                break;
            }
		default:
		    printf("Unsupported move opcode %d\n", currentOpcode);
		    exit(1);
		}

	}
    while (continueMove);

}

void ManualRealAngle(int angleFrom, int angleTo, int angleSpeed,
				  timeStruct * angleStruct)
{
    short int numOfStep;
    short int lFrom;
    short int lTo;

    lFrom = angleFrom & 0x3FF;
    lTo = angleTo & 0x3FF;

    angleStruct->from = lFrom;
    angleStruct->to = lTo;

    numOfStep = (lFrom - lTo) << 6;

    if (numOfStep < 0)
	{
	    numOfStep = -numOfStep;
	}

    numOfStep >>= 6;

    numOfStep *= angleSpeed;
    numOfStep >>= 8;

    angleStruct->numOfStep = numOfStep;
    angleStruct->timeOfChange = time;
}
