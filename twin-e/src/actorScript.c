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
//char string[256];
 short int OPbreak;
 actor *lactor;
 unsigned char opcode;
 unsigned char *localScriptPtr;
 unsigned char *opcodePtr;
 int temp;

 lactor=&actors[actorNumber];
 
 OPbreak=0;
 
 actorScriptPtr=lactor->positionInActorScript+lactor->actorScript;
 
 while(OPbreak!=-1)
 {
  opcodePtr=actorScriptPtr++;
 	opcode=*(opcodePtr);
//  printf("Actor: %d Opcode= %2d\n",actorNumber,opcode);
  if(opcode<=105)
  {
   localScriptPtr=actorScriptPtr+1;
  	switch(opcode)
   {
    case 0:
    	OPbreak=-1;
     lactor->positionInActorScript=-1;
     break;
	case 2:
		manipActor(lactor);
		if(!doCalc())
		{
			*opcodePtr=13;
		}
		actorScriptPtr=lactor->actorScript+*(short int*)actorScriptPtr;
		break;
    case 3:
    	actorScriptPtr=lactor->actorScript+*(short int*)actorScriptPtr;
     break;
    case 4:
    	manipActor(lactor);
     doCalc();
     actorScriptPtr=lactor->actorScript+*(short int*)actorScriptPtr;
     break;
    case 5:
    	break;
    case 12:
    	manipActor(lactor);
     if(!doCalc())
     {
      actorScriptPtr=lactor->actorScript+*(short int*)actorScriptPtr;
     }
     else
     {
      actorScriptPtr+=2;
     }
     break;
    case 13:
      manipActor(lactor);
      if(!doCalc())
      {
        actorScriptPtr=lactor->actorScript+*(short int*)actorScriptPtr;
      }
      else
      {
        actorScriptPtr+=2;
        *opcodePtr=2;
      }
      break;
    case 14:
    	manipActor(lactor);
    	if(!doCalc())
     {
      actorScriptPtr=lactor->actorScript+*(short int*)actorScriptPtr;
     }
     else
     {
      actorScriptPtr+=2;
      *opcodePtr=4;           // le met en always branch
     }
    	break;
    case 15:
    	actorScriptPtr=lactor->actorScript+*(short int*)actorScriptPtr;
     break;
	case 17:
		loadActorCostume(*(actorScriptPtr++),actorNumber);
		break;
	case 18:
		int param1;
		int param2;

		param1=*(actorScriptPtr++);
		param2=*(actorScriptPtr++);

		loadActorCostume(param2,param1);
	break;
	case 19:
		playAnim(*(actorScriptPtr++),0,0,actorNumber);
		break;
    case 20:
    	playAnim(*(actorScriptPtr+1),0,0,*actorScriptPtr);
     actorScriptPtr+=2;
     break;
    case 23:
    	lactor->positionInMoveScript=*(unsigned short int*)actorScriptPtr;
     actorScriptPtr+=2;
     break;
	case 24:
		int actorNumTemp;
		actorNumTemp=*(actorScriptPtr++);
		actors[actorNumTemp].positionInMoveScript=*((short int*)actorScriptPtr);
		actorScriptPtr+=2;
		break;
	case 25:
		freezeTime();
		mainLoop2(1);
		//if(showTalkIcon)
		//	drawTalkIcon(actorNumber);
		setNewTextColor(lactor->talkColor); // setTextColor
		//talkingActor=actorNumber;
		printTextFullScreen(*(short int*)actorScriptPtr);
		actorScriptPtr+=2;
		unfreezeTime();
		fullRedraw(1);
		//waitForKey();
		break;
	case 26:
		actorScriptPtr++;
		printf("Ignoring actor opcode 26\n");
		break;
    case 27:
    	temp=*(actorScriptPtr++);
		lactor->field_40=temp;
		if(temp==2)
		{
		lactor->field_54=*(actorScriptPtr++);
		}
     break;
	case 29:
		int newActorToFollow;

		newActorToFollow=*(actorScriptPtr++);

		if(reinitVar8==newActorToFollow)
		{
			break;
		}

		newCameraX=actors[newActorToFollow].X>>9;
		newCameraZ=actors[newActorToFollow].Z>>8;
		newCameraY=actors[newActorToFollow].Y>>8;

		reinitVar8=newActorToFollow;
		mainLoopVar2=1;

		break;
	case 30:
		playAnim(0,0,-1,0);
		changeTwinsenComp(*(actorScriptPtr++));
		break;
    case 31:
    	roomData1[*(actorScriptPtr++)]=*(actorScriptPtr++);
     break;
    case 33:
    	lactor->positionInActorScript=*(unsigned short int*)actorScriptPtr;
     actorScriptPtr+=2;
     break;
	case 34:
		int tempActorNum;
		tempActorNum=*(actorScriptPtr++);
		actors[tempActorNum].positionInActorScript=*((short int*)actorScriptPtr);
		actorScriptPtr+=2;
		break;
    case 35:
    	OPbreak=-1;
     break;
    case 36:
    	vars[*(actorScriptPtr++)]=*(actorScriptPtr++);
     break;
	case 38:
		//removeActorFromRoom(actorNumber); //todo: implement
		lactor->field_62|=0x20;
		lactor->costumeIndex=-1;
		lactor->field_5A=-1;
		lactor->life=0;
		break;
	case 41:
		OPbreak=-1;
		lactor->positionInActorScript=-1;
		break;
	case 42:
		lactor->field_5E=lactor->field_A;
		lactor->positionInMoveScript=-1;
		break;
	case 43:
		lactor->positionInMoveScript=lactor->field_5E;
		break;
	case 45:
		newGameVar2++;
		break;
	case 47:
		lactor->angle=0x300;
		lactor->X=lactor->field_6C-*((short int*)actorScriptPtr);
		lactor->field_62&=0xFFBF;
		lactor->field_34=0;
		actorScriptPtr+=2;
		break;
	case 48:
		lactor->angle=0x100;
		lactor->X=lactor->field_6C+*((short int*)actorScriptPtr);
		lactor->field_62&=0xFFBF;
		lactor->field_34=0;
		actorScriptPtr+=2;
		break;
	case 49:
		lactor->angle=0x200;
		lactor->Z=lactor->field_70+*((short int*)actorScriptPtr);
		lactor->field_62&=0xFFBF;
		lactor->field_34=0;
		actorScriptPtr+=2;
		break;
	case 50:
		lactor->angle=0;
		lactor->Z=lactor->field_70+*((short int*)actorScriptPtr);
		lactor->field_62&=0xFFBF;
		lactor->field_34=0;
		actorScriptPtr+=2;
		break;
	case 51:
		{
			char temp;

			if(lactor->field_10&0x1F0)
			{
				addObject(lactor);
			}

			temp=*actorScriptPtr;

			actorScriptPtr++;
			if(temp!=0)
			{
				lactor->field_10|=1;
			}
		}
	case 53:
		if(*(actorScriptPtr++)!=0)
		{
			lactor->field_60|=1;
		}
		else
		{
			lactor->field_60&=0xFFFE;
		}
		break;
	case 54:
		char temp;
		temp=*(actorScriptPtr++);

		lactor->field_60&=0xFFDD;

		if(temp==1)
		{
			lactor->field_60&=0xFFDD;
			lactor->field_60|=2;
		}
		else
		if(temp==2)
		{
			lactor->field_60|=0x22;
		}
		break;
    case 55:
    	manipActor(lactor);
		if(doCalc())
		{
			actorScriptPtr=lactor->actorScript+*(short int*)actorScriptPtr; 
		}
		else
		{
			actorScriptPtr+=2;
		}
    	break;
	case 56:
		if(*(actorScriptPtr++)!=0)
		{
			lactor->field_60|=0x200;
		}
		else
		{
			lactor->field_60&=0xFDFF;
		}
		break;
 
	case 57:
		actorScriptPtr++;
		printf("Warning: ignoring opcode 57.\n");
		break;
	case 58:
		manipActorResult=*actorScriptPtr++; // position flag number

		mainTab[18]=flagData[manipActorResult].x;
		mainTab[19]=flagData[manipActorResult].z;
		mainTab[20]=flagData[manipActorResult].y;

		lactor->X=mainTab[18];
		lactor->Z=mainTab[19];
		lactor->Y=mainTab[20];
		break;

    case 61:
    	actors[*(actorScriptPtr++)].life=*(actorScriptPtr++);
     break;
	case 67:
		int entryTemp;

		entryTemp=*(actorScriptPtr++);
		if(entryTemp<24)
			GV16[entryTemp]=-1;

		break;
	case 71:
		byte newActor;
		newActor=*(actorScriptPtr++);
		actors[newActor].field_62|=0x20;
		reinitVar4=newActor;
		actors[newActor].costumeIndex=-1;
		actors[newActor].field_5A=-1;
		break;
	case 76:
		currentGrid2=*(actorScriptPtr++);
		//load_LBAGRI(currentGrid2);
		printf("Skipping grid reload\n");
		break;
	case 77:
		printf("Unsupported actor opcode 77\n");
		actorScriptPtr+=2;
		break;
	case 80:
		actorScriptPtr+=2;
		printf("Ignoring opcode 80 in runActorScript\n");
		break;
	case 102:
		resetVideoBuffer1();
		copyToBuffer(videoBuffer1,videoBuffer2);
		osystem->drawBufferToScreen(videoBuffer1);
		changeRoomVar10=0;
		//changeCameraAngle2(320,240,1024,1024);
		//setCameraAngle(0,1500,0,25,-128,0,13000);
		setSomething4(896,950,0);
		loadTextBank(1);
		break;
	case 104:
		//drawVar1=0;
		drawBlackBox(0,0,639,240,0);
		osystem->refresh(videoBuffer1,0,0,639,240);
		break;
    default:
    	printf("Unhandled actorscript opcode %d\n",opcode);
     exit(1);
   } 
  } 
  else
  {
   printf("Error: opcode too big !\n");
   exit(1);
  }
 }
 
}

void LBA_engine::manipActor(actor* lactor)
{
	unsigned char opcode;
 unsigned char *localScriptPtr;
 actor* lactor2;
 int temp;
 
	manipActorVar1=0;
 opcode=*(actorScriptPtr++);
 
 if(opcode > 29)
 	return ;
 
 localScriptPtr=actorScriptPtr+1;
 
 switch(opcode)
 {
 	case 0:
  	if(lactor->life <= 0)
   	manipActorResult = -1;
   else
   	manipActorResult=lactor->field_56;
   break;
  case 1:
  	lactor2=&actors[*(actorScriptPtr++)];
   if(lactor2->life<=0)
   {
    manipActorResult=-1;
   }
   else
   {
    manipActorResult=lactor2->field_56;
   }
   break;
  case 2:
 		lactor2=&actors[*actorScriptPtr];
   manipActorVar1=1;
   actorScriptPtr=localScriptPtr;
   if(!(lactor2->field_62&0x20))
   {
	   if(lactor2->Z-lactor->Z>=1500)
	   {
		   manipActorResult=32000;
	   }
	   else
	   {
		   manipActorResult=getDistanceToward(lactor->X,lactor->Y,lactor2->X,lactor2->Y);
		   if(manipActorResult>32000)
			   manipActorResult=32000;
	   }
   }
   else
   {
   	manipActorResult=32000; 
   }
  	break;  	
  case 3:
  	manipActorResult=lactor->field_5A;
   break;
  case 4:
  	manipActorResult=actors[*(actorScriptPtr++)].field_5A;
   break;
  case 5:
	  manipActorResult=lactor->field_0;
		  break;
  case 6:
 		manipActorResult=actors[*(actorScriptPtr++)].field_0;
   break;
  case 7:
	  manipActorResult=lactor->costume;
	  break;
  case 8:
	  manipActorResult=actors[*(actorScriptPtr++)].costume;
	  break;
  case 9:
  	manipActorResult=lactor->field_5C;
  	break;
  case 10:
  	manipActorResult=actors[*(actorScriptPtr++)].field_5C;
   break;
  case 11:
  	manipActorResult=roomData1[*(actorScriptPtr++)];
   break;
  case 12:
	short int angle;
	int newActor;

	angle=0; //todo: not supposed to have that

	newActor=*actorScriptPtr;
	lactor2=&actors[newActor];
	manipActorVar1=1;
	actorScriptPtr=localScriptPtr;
	if(!(lactor2->field_62&0x20))
	{
		if(lactor2->Z-lactor->Z<1500)
		{
			angle=calcAngleToward(lactor->X,lactor->Y,lactor2->X,lactor2->Y);
			if(moveActorVar1>32000)
				moveActorVar1=32000;
		}
		else
		{
			moveActorVar1=32000;
		}

		if(!newActor)
		{
			int newAngle;

			newAngle=(lactor->angle+0x480)-(angle+0x400);
			newAngle&=0x3FF;

			if(newAngle>=0x100)
			{
				manipActorResult=32000;
			}
			else
			{
				manipActorResult=moveActorVar1;
			}
		}
	}
	else
	{
		manipActorResult=32000; 
	}
	break;
  case 13:
  	manipActorResult=lactor->field_64;
   break;
  case 14:
	  manipActorResult=updateActorScript;
	  break;
  case 15:
  	temp=*(actorScriptPtr++);
   if(!vars[28] || ( vars[28] && temp>=28))
   {
	  	manipActorResult=vars[temp];
   }
   else
   {
    if(temp==70)
    	manipActorResult=vars[temp];
    else
    	manipActorResult=0;
   }
   break;
  case 16:
	  manipActorResult=lactor->life;
	  break;
  case 17:
	  manipActorResult=actors[*(actorScriptPtr++)].life;
	  break;
  case 18:
	  manipActorResult=numKey;
	  break;
  case 19:
	  manipActorVar1=1;
	  manipActorResult=numCoin;
	  break;
  case 20:
  	manipActorResult=comportement;
   break;
  case 21:
  	manipActorResult=newGameVar2;
   break;
  case 22:
    lactor2=&actors[*actorScriptPtr];
    manipActorVar1=1;
    actorScriptPtr=localScriptPtr;
    if(!(lactor2->field_62&0x20))
    {
		manipActorResult=anotherSqrt(lactor->X,lactor->Z,lactor->Y,lactor2->X,lactor2->Z,lactor2->Y);

		if(manipActorResult>32000)
			manipActorResult=32000;
    }
    else
    {
    	manipActorResult=32000;
    }
	break;
  case 25:
	  actorScriptPtr=localScriptPtr;
	  printf("Ignoring manipActor opcode 25..\n");
	  break;
  case 28:
	  manipActorResult=lactor->field_58;
	  break;
  default:
			printf("Unhandled manipActor opcode %d\n",opcode);
   exit(1);
 }
}

int LBA_engine::doCalc(void)
{
	unsigned char *localActorScript=actorScriptPtr;
 unsigned char opcode;
 int opcode2;
 int localManipActorResult=manipActorResult;
 unsigned char result=0;
 
 opcode=*(localActorScript++);
 
 if(!manipActorVar1)
 {
 	opcode2=*(localActorScript++);
  if(opcode<=5)
  {
   switch(opcode)
   {
   	case 0:
		   if(localManipActorResult==opcode2)
 		  	result=1;	
    	break;
    case 1:
    	if(localManipActorResult>opcode2)
     	result=1;
     break;
    case 2:
    	if(localManipActorResult<opcode2)
     	result=1;
     break;
    case 3:
      if(localManipActorResult>=opcode2)
        result=1;
	  break;
    case 4:
      if(localManipActorResult<=opcode2)
        result=1;
	  break;
    case 5:
    	if(localManipActorResult!=opcode2)
     	result=1;
     break;
    default:
    	printf("Unhandled doCalc %d in mode %d\n",opcode,manipActorVar1);
     exit(1);
   }
  }
  else
  {
   printf("Unhandled doCalc %d in mode %d\n",opcode,manipActorVar1);
  } 
 }
 else
 if(manipActorVar1==1)
 {
 	opcode2=*(short int *)(localActorScript);
  localActorScript+=2;
  if(opcode<=5)
  {
   switch(opcode)
   {
   	case 0:
		   if(localManipActorResult==opcode2)
 		  	result=1;	
    	break;
    case 1:
    	if(localManipActorResult>opcode2)
     	result=1;
     break;
    case 2:
    	if(localManipActorResult<opcode2)
     	result=1;
     break;
    case 3:
      if(localManipActorResult>=opcode2)
        result=1;
	  break;
	case 4:
      if(localManipActorResult<=opcode2)
        result=1;
	  break;
    case 5:
    	if(localManipActorResult!=opcode2)
     	result=1;
     break;
    default:
    	printf("Unhandled doCalc %d in mode %d\n",opcode,manipActorVar1);
     exit(1);
   }	
  }
  else
  {
   printf("Unhandled doCalc %d in mode %d\n",opcode,manipActorVar1);
  } 
 }
 else
 {
  printf("Unhandled doCalc mode %d\n",manipActorVar1);
 }
 
 manipActorResult=localManipActorResult;
 actorScriptPtr=localActorScript;
 
 return(result);
 
}

int LBA_engine::getDistanceToward(int X1,int Y1, int X2,int Y2)
{
	int Xdata;
	int Ydata;

	Xdata=X2-X1;
	Xdata*=Xdata;

	Ydata=Y2-Y1;
	Ydata*=Ydata;

	return(sqrt(Xdata+Ydata));
}

int LBA_engine::calcAngleToward(int X1,int Y1,int X2,int Y2)
{
	int newX;
	int newY;
	int ebp;
	int edi;
	int eax;
	int tempExchange;
	short int *tab3ptr;
	short int *tempPtr;
	int esi;
	int ebx;

	edi=Y2-Y1;
	newY=edi*edi;

	ebp=X2-X1;
	newX=ebp*ebp;

	if(!(newX<newY))
	{
		tempExchange=edi;
		edi=ebp;
		ebp=tempExchange;
		ebp|=1;
	}
	else
	{
		ebp&=0xFFFFFFFE;
	}

	moveActorVar1=eax=sqrt(newX+newY);

	if(!eax)
		return(0);

	tempExchange=eax;
	eax=edi;
	edi=tempExchange;

	eax<<=14;
	eax/=edi;

	esi=(int)tab3;
	edi=esi+0x200;

	do
	{
		ebx=esi;
		ebx+=edi;
		ebx>>=1;

		if(eax>*(short int*)ebx)
		{
			edi=ebx;
		}
		else
		{
			esi=ebx;
			if(eax==*(short int*)ebx)
			{
				goto endCalc;
			}
			ebx=edi;
		}
		ebx-=esi;
	}while(--ebx);

	if((*(short int*)esi+*(short int*)edi)/2<=eax)
	{
		esi=edi;
	}

endCalc:

	esi-=(int)tab2;
	eax=esi;
	eax>>=1;

	if(ebp<=0)
	{
		eax=-eax;
	}

	if(ebp&1)
	{
		eax=-eax;
		eax+=0x100;
	}

	eax&=0x3FF;

	return(eax);


}

int LBA_engine::anotherSqrt(int X1,int Z1,int Y1,int X2,int Z2,int Y2)
{
	int newX;
	int newZ;
	int newY;

	newX=X2-X1;
	newX*=newX;

	newZ=Z2-Z1;
	newZ*=newZ;

	newY=Y2-Y1;
	newY*=newY;

	return(sqrt(newX+newZ+newY));
}