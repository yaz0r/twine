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
 
 actorScriptPtr=lactor->positionInActorScript+lactor->scenePtr2;
 
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
		actorScriptPtr=lactor->scenePtr2+*(short int*)actorScriptPtr;
		break;
    case 3:
    	actorScriptPtr=lactor->scenePtr2+*(short int*)actorScriptPtr;
     break;
    case 4:
    	manipActor(lactor);
     doCalc();
     actorScriptPtr=lactor->scenePtr2+*(short int*)actorScriptPtr;
     break;
    case 5:
    	break;
    case 12:
    	manipActor(lactor);
     if(!doCalc())
     {
      actorScriptPtr=lactor->scenePtr2+*(short int*)actorScriptPtr;
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
        actorScriptPtr=lactor->scenePtr2+*(short int*)actorScriptPtr;
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
      actorScriptPtr=lactor->scenePtr2+*(short int*)actorScriptPtr;
     }
     else
     {
      actorScriptPtr+=2;
      *opcodePtr=4;           // le met en always branch
     }
    	break;
    case 15:
    	actorScriptPtr=lactor->scenePtr2+*(short int*)actorScriptPtr;
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
		initNewCostume(*(actorScriptPtr++),0,0,actorNumber);
		break;
    case 20:
    	initNewCostume(*(actorScriptPtr+1),0,0,*actorScriptPtr);
     actorScriptPtr+=2;
     break;
    case 23:
    	lactor->field_46=*(unsigned short int*)actorScriptPtr;
     actorScriptPtr+=2;
     break;
	case 24:
		int actorNumTemp;
		actorNumTemp=*(actorScriptPtr++);
		actors[actorNumTemp].field_46=*((short int*)actorScriptPtr);
		actorScriptPtr+=2;
		break;
    case 27:
    	temp=*(actorScriptPtr++);
     lactor->field_40=temp;
     if(temp==2)
     {
      lactor->field_54=*(actorScriptPtr++);
     }
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
		lactor->field_46=-1;
		break;
	case 43:
		lactor->field_46=lactor->field_5E;
		break;
	case 45:
		newGameVar2++;
		break;
	case 47:
		lactor->field_32=0x300;
		lactor->field_1A=lactor->field_6C-*((short int*)actorScriptPtr);
		lactor->field_62&=0xFFBF;
		lactor->field_34=0;
		actorScriptPtr+=2;
		break;
	case 48:
		lactor->field_32=0x100;
		lactor->field_1A=lactor->field_6C+*((short int*)actorScriptPtr);
		lactor->field_62&=0xFFBF;
		lactor->field_34=0;
		actorScriptPtr+=2;
		break;
	case 49:
		lactor->field_32=0x200;
		lactor->field_1E=lactor->field_70+*((short int*)actorScriptPtr);
		lactor->field_62&=0xFFBF;
		lactor->field_34=0;
		actorScriptPtr+=2;
		break;
	case 50:
		lactor->field_32=0;
		lactor->field_1E=lactor->field_70+*((short int*)actorScriptPtr);
		lactor->field_62&=0xFFBF;
		lactor->field_34=0;
		actorScriptPtr+=2;
		break;
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
    case 55:
    	manipActor(lactor);
		if(doCalc())
		{
			actorScriptPtr=lactor->scenePtr2+*(short int*)actorScriptPtr; 
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
		mainTab[19]=flagData[manipActorResult].y;
		mainTab[20]=flagData[manipActorResult].z;

		lactor->field_1A=mainTab[18];
		lactor->field_1C=mainTab[19];
		lactor->field_1E=mainTab[20];
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
	case 80:
		actorScriptPtr+=2;
		printf("Ignoring opcode 80 in runActorScript\n");
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
   if(!lactor2->field_62&0x20)
   {
    printf("Unhandled manip actor opcode 2...\n");
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
  	lactor2=&actors[*actorScriptPtr];
   manipActorVar1=1;
   actorScriptPtr=localScriptPtr;
   if(!lactor2->field_62&0x20)
   {
    printf("Unhandled manip actor opcode 12...\n");
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
    if(!lactor2->field_62&0x20)
    {
     printf("Unhandled manip actor opcode 22...\n");
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