#include "lba.h"

void LBA_engine::setActorAngle(short int arg0, short int arg4, short int arg8, timeStruct *ptr)
{
  ptr->from=arg0;
  ptr->to=arg4;
  ptr->numOfStep=arg8;
  ptr->timeOfChange=time;
}

void LBA_engine::setActorAngleSafe(short int arg_0, short int arg_4, short int arg_8, timeStruct *timePtr)
{
  timePtr->from = arg_0&0x3FF;
  timePtr->to = arg_4&0x3FF;
  timePtr->numOfStep = arg_8&0x3FF;
  timePtr->timeOfChange = time;
}

int LBA_engine::processActorAngle(timeStruct* arg_0)
{
	int edx;
	int eax;

	eax=arg_0->to;

	if(arg_0->numOfStep)
	{
		edx=time-arg_0->timeOfChange;

		if(edx>=arg_0->numOfStep)
		{
			arg_0->numOfStep=0;
			return(eax);
		}

		eax-=arg_0->from;
		if(eax<-0x200)
		{
			eax+=0x400;
			eax*=edx;
			eax/=arg_0->numOfStep;
			return(eax+arg_0->from);
		}
		if(eax<=0x200)
		{
			eax*=edx;
			eax/=arg_0->numOfStep;
			return(eax+arg_0->from);
		}

		eax-=0x800;
		eax+=0x400;
		eax*=edx;
		eax/=arg_0->numOfStep;
		return(eax+arg_0->from);

	}

	return(eax);
}