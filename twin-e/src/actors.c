/***************************************************************************
                          actors.cpp  -  description
                             -------------------
    begin                : Sat Mar 16 2002
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

void LBA_engine::loadRoomActors(short int arg_0)
{
	actor* lactor;
 
 lactor=&actors[arg_0];
 
 if(lactor->field_60 & 0x400)
 {
  if(lactor->field_66 !=0)
  {
   *(byte*)&lactor->field_62 |= 2;
  }
  
  lactor->costumeIndex=-1;
  
  loadActorSub(lactor->field_8,arg_0);
  
  setActorAngleSafe(0,0,0,&lactor->time);
  
  if(lactor->field_60 & 8)
  {
   lactor->field_6C=lactor->X;
   lactor->field_6E=lactor->Z;
   lactor->field_70=lactor->Y;
  }
  
 }
 else
 {
  lactor->costumeIndex=-1;
  
  loadActorCostume(lactor->field_0,arg_0);
 	
  lactor->field_74=-1;
  lactor->field_78=0;
  
  if(lactor->costumeIndex != -1)
  {
   playAnim(lactor->costume,0,255,arg_0);
  }
  
  setActorAngleSafe(lactor->angle,lactor->angle,0,&lactor->time);
 }
 
 lactor->positionInMoveScript=-1;
 lactor->field_5C=-1;
 lactor->positionInActorScript=0;
}

void LBA_engine::resetActor(int actorNumber)
{
  actor *localActor;
 
  localActor=&actors[actorNumber];
 
  localActor->field_0=0;
  localActor->costume=0;
  localActor->X=0;
  localActor->Z=-1;
  localActor->Y=0;
  localActor->field_26=0;
  localActor->field_28=0;
  localActor->field_2A=0;
  localActor->field_2C=0;
  localActor->field_2E=0;
  localActor->field_30=0;
  localActor->angle=0;
  localActor->field_34=0;
  localActor->field_40=0;
  localActor->field_4E=0;
  localActor->field_50=0;
  localActor->field_52=0;
  localActor->field_54=0;
  localActor->field_3=0;
  localActor->field_56=-1;
  localActor->field_58=-1;
  localActor->field_5A=-1;
  localActor->field_60=0;
  localActor->field_62=0;
  localActor->life=50;
  localActor->field_14=1;
  localActor->field_64=-1;
  localActor->field_6A=0;
  localActor->field_6C=0;
  localActor->field_6E=0;
  localActor->field_70=0;
  localActor->costumeIndex=-1;
  localActor->field_74=-1;
  localActor->field_78=0;
  localActor->field_76=0;

  setActorAngleSafe(0,0,0,&localActor->time);

  localActor->positionInMoveScript=-1;
  localActor->positionInActorScript=0;
}

void LBA_engine::loadActorSub(int imageNumber,int actorNumber)
{
	actor* lactor=&actors[actorNumber];

	if(lactor->field_60&0x400 && imageNumber!=-1 && lactor->costumeIndex!=imageNumber)
	{
		short int* ptr;

		lactor->costumeIndex=imageNumber;

		ptr=(short int*)(HQRess3+imageNumber*16+4);

		lactor->field_26=*(ptr++);
		lactor->field_28=*(ptr++);
		lactor->field_2A=*(ptr++);
		lactor->field_2C=*(ptr++);
		lactor->field_2E=*(ptr++);
		lactor->field_30=*(ptr++);
	}
}