#include "lba.h"

void LBA_engine::moveActor(int actorNumber)
{
	int continueMove;
	int positionInScript;
	char currentOpcode;
	char* scriptPtr;

	return;

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
		case 3:
			if(initNewCostume(*(scriptPtr++),0,0,actorNumber))
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
			
		case 9:
			lactor->field_5C=*scriptPtr;
			lactor->positionInMoveScript++;
			lactor->field_A=lactor->positionInMoveScript-2;
			break;
		default:
			printf("Unsupported move opcode %d\n",currentOpcode);
			exit(1);
		}

	}while(continueMove);

}