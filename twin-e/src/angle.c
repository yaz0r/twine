#include "lba.h"

void
LBA_engine::changeActorAngle (actor * ptr)
{
  setActorAngleSafe (ptr->angle, ptr->angle, 0, &ptr->time);
}

void
LBA_engine::setActorAngle (short int arg0, short int arg4, short int arg8,
			   timeStruct * ptr)
{
  ptr->from = arg0;
  ptr->to = arg4;
  ptr->numOfStep = arg8;
  ptr->timeOfChange = time;
}

void
LBA_engine::setActorAngleSafe (short int arg_0, short int arg_4,
			       short int arg_8, timeStruct * timePtr)
{
  timePtr->from = arg_0 & 0x3FF;
  timePtr->to = arg_4 & 0x3FF;
  timePtr->numOfStep = arg_8 & 0x3FF;
  timePtr->timeOfChange = time;
}

int
LBA_engine::processActorAngle (timeStruct * angleData)
{
  int timePassed;
  int remainingAngle;


  if (angleData->numOfStep)
    {
      timePassed = time - angleData->timeOfChange;

      if (timePassed >= angleData->numOfStep)	// rotation is finished
	{
	  angleData->numOfStep = 0;
	  return (angleData->to);
	}

      remainingAngle = angleData->to - angleData->from;

      if (remainingAngle < -0x200)
	{
	  remainingAngle += 0x400;
	}
      else if (remainingAngle > 0x200)
	{
	  remainingAngle -= 0x800;
	  remainingAngle += 0x400;
	}

      remainingAngle *= timePassed;
      remainingAngle /= angleData->numOfStep;
      remainingAngle += angleData->from;

      return (remainingAngle);

    }

  return (angleData->to);

}

int
LBA_engine::mainLoopSub17 (timeStruct * angleData)
{
  int tempAngle;

  if (!angleData->numOfStep)
    return (angleData->to);

  if (!(time - angleData->timeOfChange < angleData->numOfStep))
    {
      angleData->numOfStep = 0;
      return (angleData->to);
    }

  tempAngle = angleData->to - angleData->from;
  tempAngle *= time - angleData->timeOfChange;
  tempAngle /= angleData->numOfStep;

  return (tempAngle + angleData->from);
}
