#include "lba.h"

void LBA_engine::moveActor(int actorNumber)
{
	int continueMove;
	int positionInScript;
	char currentOpcode;
	char* scriptPtr;

	actor* lactor;
	timeStruct* timePtr;

	lactor=&actors[actorNumber];
	continueMove=1;
	timePtr=&lactor->time;

	do
	{
		positionInScript=lactor->positionInMoveScript;
		scriptPtr=(char*)(lactor->positionInMoveScript+lactor->moveScript);
		currentOpcode=*(scriptPtr++);
		lactor->positionInMoveScript++;

		switch(currentOpcode)
		{
		case 2:
			loadActorCostume(*scriptPtr,actorNumber);
			lactor->positionInMoveScript++;
			break;
		case 3:
			if(playAnim(*(scriptPtr++),0,0,actorNumber))
			{
				lactor->positionInMoveScript++;
			}
			else
			{
				lactor->positionInMoveScript=positionInScript;
				continueMove=0;
			}
			break;
		case 4:
			int newAngle;
			lactor->positionInMoveScript++;
			manipActorResult=*scriptPtr;

			setSomething3Var11=flagData[manipActorResult].x;
			setSomething3Var13=flagData[manipActorResult].z;
			setSomething3Var15=flagData[manipActorResult].y;

			newAngle=calcAngleToward(lactor->X,lactor->Y,setSomething3Var11,setSomething3Var15);

			if(lactor->field_60&0x400)
			{
				lactor->angle;
			}
			else
			{
				updateActorAngle(lactor->angle,newAngle,lactor->field_34,&lactor->time);
			}

			if(moveActorVar1>500)
			{
				continueMove=0;
				lactor->positionInMoveScript-=2;
			}

			break;
		case 5:
			if(!(lactor->field_60&0x400))
			{
				continueMove=0;
				lactor->positionInMoveScript--;
			}
			else
			{
			//	changeActorTime(lactor);
			}
			break;
		case 7:
			printf("skipping actor move opcode 7\n");
			lactor->positionInMoveScript+=2;
			break;
		case 8: //move to flag
			lactor->positionInMoveScript++;
			manipActorResult=*scriptPtr;

			setSomething3Var11=flagData[manipActorResult].x;
			setSomething3Var13=flagData[manipActorResult].z;
			setSomething3Var15=flagData[manipActorResult].y;

			if(lactor->field_60&0x400)
			{
				lactor->field_34=0;
			}

			lactor->X=setSomething3Var11;
			lactor->Z=setSomething3Var13;
			lactor->Y=setSomething3Var15;

			break;

		case 9:
			lactor->field_5C=*scriptPtr;
			lactor->positionInMoveScript++;
			lactor->field_A=lactor->positionInMoveScript-2;
			break;
		case 10:
			lactor->positionInMoveScript=*(short int*)scriptPtr;
			break;
		case 11:
			continueMove=0;
			lactor->positionInMoveScript=-1;
			break;
		case 12:
			lactor->positionInMoveScript++;
			manipActorResult=*scriptPtr;

			setSomething3Var11=flagData[manipActorResult].x;
			setSomething3Var13=flagData[manipActorResult].z;
			setSomething3Var15=flagData[manipActorResult].y;

			newAngle=calcAngleToward(lactor->X,lactor->Y,setSomething3Var11,setSomething3Var15);

			newAngle+=0x200;

			if(lactor->field_60&0x400)
			{
				lactor->angle=newAngle;
			}
			else
			{
				updateActorAngle(lactor->angle,newAngle,lactor->field_34,timePtr);
			}

			if(moveActorVar1>500)
			{
				continueMove=0;
				lactor->positionInMoveScript-=2;
			}

			break;
		case 13:
			lactor->positionInMoveScript+=2;
			if(lactor->field_62&4)
			{
				(*(scriptPtr+1))++;

				if(*(scriptPtr+1)==*scriptPtr)
				{
					(*(scriptPtr+1))=0;
				}
				else
				{
					continueMove=0;
				}
			}
			else
			{
				continueMove=0;
			}

			if(continueMove==0)
			{
				lactor->positionInActorScript-=3;
			}
			break;
		case 14:
			fullRedrawS3(*(short int*)scriptPtr,0x1000,1,lactor->X,lactor->Z,lactor->Y);
			lactor->positionInMoveScript+=2;
			break;
		case 15: // move to ?
			lactor->positionInMoveScript++;
			if(lactor->field_60&0x400)
			{
				manipActorResult=*scriptPtr;

				setSomething3Var11=flagData[manipActorResult].x;
				setSomething3Var13=flagData[manipActorResult].z;
				setSomething3Var15=flagData[manipActorResult].y;

				lactor->angle=calcAngleToward(lactor->X,lactor->Y,setSomething3Var11,setSomething3Var15);
				lactor->field_78=calcAngleToward(lactor->Z,0,setSomething3Var13,moveActorVar1);

				if(moveActorVar1>100)
				{
					continueMove=0;
					lactor->positionInMoveScript-=2;
				}
				else
				{
					lactor->X=setSomething3Var11;
					lactor->Z=setSomething3Var13;
					lactor->Y=setSomething3Var15;
				}
			}
			break;
		case 16:
			lactor->positionInMoveScript+=2;
			lactor->field_34=*(short int*)scriptPtr;
			if(lactor->field_60&0x400)
			{
				setActorAngle(0,lactor->field_34,50,timePtr);
			}
			break;
		case 17:
			lactor->positionInMoveScript+=1;
			if(*scriptPtr!=0)
			{
				if(!(lactor->field_60&0x2000))
				{
					lactor->field_60|=0x2000;
					if(lactor->field_60&0x1000)
					{
						mainLoopVar2=1;
					}
				}
			}
			else
			{
				if(lactor->field_60&0x2000)
				{
					lactor->field_60&=0xDFFF;
					if(lactor->field_60&0x1000)
					{
						mainLoopVar2=1;
					}
				}
			}
			break;
		case 18:
			printf("skipping actor move opcode 18\n");
			lactor->positionInMoveScript+=5;
			break;
		case 21:
		case 22:
		case 23:
		case 24:
			int temp;
			lactor->positionInMoveScript+=2;
			temp=lactor->field_60;
			temp&=0x408;
			if(!temp)
			{
				switch(currentOpcode-21)
				{
				case 0:
					lactor->angle=0x300;
					break;
				default:
					printf("Unsupported subopcode of actor move opcode 21-24!\n");
					exit(1);
				}

				lactor->field_72=*scriptPtr;
				lactor->field_62|=0x40;
				lactor->field_34=1000;
				setActorAngle(0,1000,50,timePtr);
			}
			break;
		case 25:
			printf("skipping actor move opcode 25\n");
			break;
		case 26:
			printf("skipping actor move opcode 26\n");
			break;
		case 28:
			fullRedrawS3(*(short int*)scriptPtr,0x1000,0,lactor->X,lactor->Z,lactor->Y);
			lactor->positionInMoveScript+=2;
			break;
		case 29:
			printf("skipping actor move opcode 29 (playSound)\n");
			lactor->positionInMoveScript+=2;
			break;
		default:
			printf("Unsupported move opcode %d\n",currentOpcode);
			exit(1);
		}

	}while(continueMove);

}

void LBA_engine::updateActorAngle(int angleFrom,int angleTo,int angleSpeed,timeStruct* angleStruct)
{
	int numOfStep;

	angleStruct->from=angleFrom&0x3FFF;
	angleStruct->to=angleTo&0x3FFF;

	numOfStep=((abs((angleFrom-angleTo)<<6)>>6)*angleSpeed)>>8;

	angleStruct->numOfStep=numOfStep;
	angleStruct->timeOfChange=time;
}