/***************************************************************************
                          costume.cpp  -  description
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
 
void LBA_engine::loadTwinsenCostumes(void)
{
		loadDataFileToPtr("file3d.hqr",1,&file3D1);
		twinsen->bodyPtr=file3D1;
		TCos1Init=initCostume(0,0);

		loadDataFileToPtr("file3d.hqr",2,&file3D2);
		twinsen->bodyPtr=file3D2;
		TCos2Init=initCostume(0,0);

		loadDataFileToPtr("file3d.hqr",3,&file3D3);
		twinsen->bodyPtr=file3D3;
		TCos3Init=initCostume(0,0);

		loadDataFileToPtr("file3d.hqr",4,&file3D4);
		twinsen->bodyPtr=file3D4;
		TCos4Init=initCostume(0,0);		

		loadDataFileToPtr("file3d.hqr",0,&file3D0);
		twinsen->bodyPtr=file3D0;
		TCos0Init=initCostume(0,0);

		twinsen->field_4=loadTwinsenCostumesVar1;

}

int LBA_engine::initCostume(byte arg_0,short int actorNumber)
{
	actor* act;
	char type;
//	int temp;
 unsigned char * bodyPtr;
 unsigned char * ptr,*ptr2;
 unsigned char * costumePtr=0;
 unsigned short int var1;

	act=&actors[actorNumber];
	bodyPtr=act->bodyPtr;

 do
 {
  
  type=*(bodyPtr++);
  
		if(type==-1)
		{
   loadTwinsenCostumesVar1=costumePtr;
			return(-1);
		}
  
  ptr=(bodyPtr+1);
  
  if(type==3)
  {
  	if(arg_0==*bodyPtr)
   {
    ptr++;
    var1=*(unsigned short int*)ptr;
    ptr+=2;
    ptr2=ptr;
    ptr++;
    if(*ptr2!=0)
    {
     costumePtr=ptr-1;
    }
    loadTwinsenCostumesVar1=costumePtr;
    return(var1);
   }
  }  
  
  bodyPtr=*ptr+ptr;
  
 }while(1);
	
	return(0);
}

void LBA_engine::loadActorCostume(char arg_0, short int arg_4)
{
	actor* lactor;
 int temp;
 int temp2;
 int currentIndex;
 unsigned short int *ptr;
 short int var1, var2, var3, var4;
// short int var5, var6;
 int result,result1, result2;

 result=0;
 
 lactor= &actors[arg_4];
 
 if(lactor->field_60 & 0x400) // 0x400 -> actor = sprite
 	return;
  
 if(arg_4==0 && comportement==4 && lactor->field_14 != 0 && lactor->field_14 !=1 ) // Si c'est twinsen qu'on load
 {
  changeTwinsenComp(0);
 }
 
 if(arg_0 != -1)
 {
  temp=loadBody(arg_0,arg_4);
 }
 else
 {
  temp=-1;
 }
 
 if(temp != -1)
 {
  if(lactor->costumeIndex == temp)
  	return;
   
  temp2=lactor->costumeIndex;
  lactor->costumeIndex=temp;
  lactor->field_0=(char)lactor->field_14;
  currentIndex=lactor->costumeIndex;
  
  if(loadCostumeVar==-32000)
  {
   ptr=(unsigned short int*)bodyPtrTab[lactor->costumeIndex];
   ptr++;
   
   var1=*(ptr++);
   var2=*(ptr++);
   lactor->field_2A=*(ptr++);
   lactor->field_2C=*(ptr++);
   var3=*(ptr++);
   var4=*ptr;
   
   if(lactor->field_60 & 0x8000)
   {
    result1=var2-var1;
    result2=var4-var3;
    if(result1>=result2)
    	result=result1;
     
    result-=result1;
    result=result>>1;
   }
   else
   {
    result1=var2-var1;
    result2=var4-var2;
    
    result=result2+result1;
    result2=result2<<2;
    result-=result2;
    result=result>>2;
   }
   
   lactor->field_26=-result;
   lactor->field_28=result;
   lactor->field_2E=-result;
   lactor->field_30=result;
  }
  else
  {
   lactor->field_26=loadCostumeVar;
   lactor->field_28=loadCostumeVar4;
   lactor->field_2A=loadCostumeVar2;
   lactor->field_2C=loadCostumeVar5;
   lactor->field_2E=loadCostumeVar3;
   lactor->field_30=loadCostumeVar6;
  }
  
  if(currentIndex==-1)
  	return;
   
  if(lactor->field_74==-1)
  	return;
   
  loadActorCostumeSub(bodyPtrTab[currentIndex],bodyPtrTab[lactor->costumeIndex]);
  
  return;
 }
 
 lactor->field_0=-1;
 lactor->costumeIndex=-1;
 lactor->field_2A=0;
 lactor->field_2C=0;
 lactor->field_26=0;
 lactor->field_28=0;
 lactor->field_2E=0;
 lactor->field_30=0;	
 
}

/* cette fonction doit permettre de transferer l'etat de l'anim d'un costume à l'autre */

void LBA_engine::loadActorCostumeSub(unsigned char *arg_0, unsigned char* arg_4)
{
// allé hop ! à implementer plus tard....
}





