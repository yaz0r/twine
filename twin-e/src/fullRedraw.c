/***************************************************************************
                          fullRedraw.cpp  -  description
                             -------------------
    begin                : Sun Apr 28 2002
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

void LBA_engine::fullRedraw(int param)
{
	short int temp1;
	short int temp2;
	int counter;
	int counter2;
	//  int i;

	int arg_1A;
	int a12;
	int arg_46;
	int arg_42;
	int a0E;
	int arg_1E;
	actor* lactor;
	int result,result2;
	short int arg_36;
	short int arg_4A;
	int a2E;
	int temp3;

	temp1=fullRedrawVar1;
	temp2=fullRedrawVar2;

	maximizeTextWindow();

	if(!param) // redraw only the actors
	{
		fullRedrawSub1();
	}
	else      // redraw the background as well
	{
		freezeTime();
		resetVideoBuffer1();
		fullRedrawSub2();
		fullRedrawSub3(temp1,temp2,fullRedrawVar1,fullRedrawVar2);
		//  fullRedrawSub4();
		copyToBuffer(videoBuffer1,videoBuffer2);
	}

	arg_1A=0;
	a12=0;
	arg_46=arg_1A+0x1000;
	arg_42=arg_1A+0x0C00;

	for(arg_1A=0;arg_1A<numActorInRoom;arg_1A++,arg_46++,arg_42++) // process shadows/priority ?
	{
		lactor=&actors[arg_1A];
		*(byte*)&lactor->field_62 &= 0xEF;

		if(!(changeRoomVar9!=-1 && lactor->field_1C>*(short int*)(currentGrid2*24+sceneVarPtr+8)))
		{
			if(lactor->field_60 & 0x2000 && param == 0)
			{
				fullRedrawS2S1(lactor->field_1A-changeRoomVar4Bis,lactor->field_1C-changeRoomVar5Bis,lactor->field_1E-changeRoomVar6Bis);

				if(fullRedrawVar3 > -50 && fullRedrawVar3 < 680 && fullRedrawVar4> -30 && fullRedrawVar4< 580)
					*(byte*)&lactor->field_62|=0x10;
			}
			else
			{
				if(lactor->costumeIndex != -1 && !(lactor->field_60 & 0x200)) // if the costume is set and if the actor is a sprite actor
				{
					fullRedrawS2S1(lactor->field_1A-changeRoomVar4Bis,lactor->field_1C-changeRoomVar5Bis,lactor->field_1E-changeRoomVar6Bis);

					if((lactor->field_60 & 8 && fullRedrawVar2 >-112 && fullRedrawVar2 < 752 && fullRedrawVar4 > -50 && fullRedrawVar4 < 651)
					|| ((!(lactor->field_60 & 8)) && fullRedrawVar3> -50 && fullRedrawVar3 < 680 && fullRedrawVar4 > -30 && fullRedrawVar4< 580))
					{
						temp3=(lactor->field_1E+((lactor->field_1A)-changeRoomVar4Bis))-changeRoomVar6Bis;
						if(lactor->field_58!=-1)
						{
							printf("Sprite actor %d follow actor %d !\n",arg_1A,lactor->field_58);
						}
					//	            printf("process sprite actor %d\n",arg_1A);
					}
				}
			}
		}
	}

	arg_1A=0;
	counter=arg_1A+0x1800;
	counter2=0;

	do
	{
		if(reinitAll2SubVar1[counter2].field_0 != -1)
		{
			if(reinitAll2SubVar1[counter2].field_15 & 4)
			{
				if(time-reinitAll2SubVar1[counter2].field_18 > 35)
				{
					reinitAll2SubVar1[counter2].field_18 = time;
					reinitAll2SubVar1[counter2].field_15 &= 0xFB;
				    fullRedrawS3(11,0x1000,1,reinitAll2SubVar1[counter2].field_2,reinitAll2SubVar1[counter2].field_4,reinitAll2SubVar1[counter2].field_6);
				}
			}
			else
			{
				fullRedrawS2S1(roomData2->field_2-changeRoomVar4Bis,roomData2->field_4-changeRoomVar5Bis,roomData2->field_6-changeRoomVar6Bis);
			}
		//process
		}
		arg_1A++;
		counter++;
		counter2++;
	}while(arg_1A<50);

// fullRedrawSub6(&fullRedrawVar6,a12,12);

	if(twinsen->costumeIndex != -1 && !(twinsen->field_60 & 0x200))
	{
		twinsen->field_46=twinsen->field_1A+twinsen->field_26;
		arg_36=twinsen->field_1E+twinsen->field_2E;
		twinsen->field_52=twinsen->field_1A+twinsen->field_28;
		arg_4A=twinsen->field_1E+twinsen->field_30;
		a2E=0;
		result=-1;

		while(arg_1A<a12)
		{
			if(fullRedrawVar6[arg_1A].field_2 == 0)
			{
				result=arg_1A;
				result2=fullRedrawVar6[arg_1A].field_0;
				break;
			}
		}

	//twinsen process
	}

  a0E=0;
// fullRedrawVar8=0;
  arg_1A=0;
  arg_1E=0;

  if(a12 > 0)
  {
  //unknownLoop

   // another process...
  }

  counter2=0;

  do
  {
    if(roomData2[counter2].field_0 != -1)
    {
   // another process
    }
    arg_1A++;
    counter2++;
  }while(arg_1A<10);

// if(fullRedrawVar9!=0)
 //  waitRetrace();

  if(drawInGameTransBox!=0)
  {
//  fullRedrawSub10(arg_16,arg_12,arg_E,arg_A);
    fullRedrawSub5();
    if(param!=0)
      unfreezeTime();
  }
  else
  {
    if(param==0)
    {
//    fullRedrawSub11();
    }
    else
    {
      osystem->drawBufferToScreen(videoBuffer1);
      fullRedrawSub5();
      unfreezeTime();
    }
  }

  if(!mainLoopVar3)
    return;

  loadImageToPtr("ress.hqr",(byte*)&palette,0);
  osystem->setPalette((byte*)&palette);

  if(isMenuDisplayed)
    fadeIn2((char*)menuPal);
  else
    fadeIn2((char*)palette);

  mainLoopVar3=0;

  return;
}

void LBA_engine::fullRedrawSub3(short int arg_0, short int arg_4, short int arg_8, short int arg_C)
{
  int i;
 short int var1;
 short int var2;

 var1=arg_8-arg_0;
 var2=arg_C-arg_4;

 for(i=0;i<10;i++)
 {
  if(roomData2[i].field_6==1)
  {
   roomData2[i].field_2=var1;
   roomData2[i].field_4=var2;
  }
 }
}

void LBA_engine::fullRedrawSub1(void)
{
  int i;

  if(numOfRedrawBox <= 0)
   return;

 for(i=0;i<numOfRedrawBox;i++)
 {
  drawBoxTrans(refreshBoxList[i].field_0,refreshBoxList[i].field_2,refreshBoxList[i].field_4,refreshBoxList[i].field_6,(char*)videoBuffer2,refreshBoxList[i].field_0,refreshBoxList[i].field_2,(char*)videoBuffer1);
 }

}

void LBA_engine::fullRedrawSub5(void)
{
  int i;

 numOfRedrawBox=0;

 if(0 >= fullRedrawVar8 )
   return;

 for(i=0;i<fullRedrawVar8;i++)
 {
  addToRedrawBox(refreshBoxList2[i].field_0,refreshBoxList2[i].field_2,refreshBoxList2[i].field_4,refreshBoxList2[i].field_6);
 }
}

void LBA_engine::addToRedrawBox(short int arg_0, short int arg_4, short int arg_8, short int arg_C)
{
  int var1;
 int i=0;
 int var_C;
 int var_8;
 int var_10;
 int var_4;

 var1=(arg_8-arg_0)*(arg_C-arg_4);

 while(i<numOfRedrawBox)
 {
  if(refreshBoxList[i].field_0>=arg_0)
    var_C=arg_0;
  else
   var_C=refreshBoxList[i].field_0;

  if(refreshBoxList[i].field_4<=arg_8)
    var_8=arg_8;
  else
   var_8=refreshBoxList[i].field_4;

  if(refreshBoxList[i].field_2>=arg_4)
    var_10=arg_4;
  else
   var_10=refreshBoxList[i].field_2;

  if(refreshBoxList[i].field_6<=arg_C)
    var_4=arg_C;
  else
   var_4=refreshBoxList[i].field_6;

   if((var_8 - var_C)*(var_4 - var_10) < ((refreshBoxList[i].field_6 - refreshBoxList[i].field_2)*(refreshBoxList[i].field_4 - refreshBoxList[i].field_0) + var1))
   {
     refreshBoxList[i].field_0=var_C;
    refreshBoxList[i].field_2=var_10;
    refreshBoxList[i].field_4=var_8;
    refreshBoxList[i].field_6=var_4;
    return;
   }

   i++;
 };

 refreshBoxList[i].field_0=arg_0;
 refreshBoxList[i].field_2=arg_4;
 refreshBoxList[i].field_4=arg_8;
 refreshBoxList[i].field_6=arg_C;

 numOfRedrawBox++;
}

void LBA_engine::fullRedrawSub2(void)
{

  int i;
 int x,y,z;
 unsigned char *localBufCube=bufCube;
 unsigned char val;

  changeRoomVar4Bis=changeRoomVar4<<9;
 changeRoomVar5Bis=changeRoomVar5<<8;
 changeRoomVar6Bis=changeRoomVar6<<9;

 fullRedrawS2S1(-changeRoomVar4Bis,-changeRoomVar5Bis,-changeRoomVar6Bis);

 fullRedrawVar1=fullRedrawVar3;
 fullRedrawVar2=fullRedrawVar4;

 for(i=0;i<56;i+=2)
 {
  *(short int*)&(zbufferTab[i])=0;
 }

  if(changeRoomVar10 == 0)
   return;

 for(x=0;x<64;x++)
 {
  for(y=0;y<64;y++)
  {
   for(z=0;z<25;z++)
   {
     val=*(localBufCube++);
     if(val)
    {
       zbuffer(--val,*(localBufCube),y,z,x);
    }
    localBufCube++;
   }
  }
 }
}


void LBA_engine::zbuffer(int var1, int var2, int y, int z, int x)
{
  unsigned char *ptr;
 unsigned short int bx;

  ptr=zbufferSub1(var1)+3+var2*4+2;

 bx=*(unsigned short int*)ptr;

 if(!bx)
   return;

 zbufferSub2(y-changeRoomVar4 ,z-changeRoomVar5, x-changeRoomVar6);

  if(zbufferVar1<-24)
   return;
 if(zbufferVar1>=640)
   return;
 if(zbufferVar2<-38)
   return;
 if(zbufferVar2>=480)
   return;

 drawSprite(bx-1,zbufferVar1, zbufferVar2,bufferBrick);

}

unsigned char * LBA_engine::zbufferSub1(int var)
{
  return(currentBll+*(unsigned int*)(currentBll+4*var));
}

void LBA_engine::zbufferSub2(int y, int z, int x)
{
  zbufferVar1=((y-x)*16)+(y-x)*8+288;          // x pos
 zbufferVar2=(((y+x)*8+(y+x)*4)-((z*16)-z))+215;      // y pos
}

void LBA_engine::drawSprite(int num, int var1, int var2, unsigned char* localBufferBrick)
{
  unsigned char* ptr;
 int top;
 int bottom;
 int left;
 int right;
 unsigned char* outPtr;
 unsigned char* outPtr2;
 int offset;
 int c1;
 int c2;
 int vc3;

 int temp;
 int iteration;
 int i;

 ptr=localBufferBrick+*(unsigned int*)(localBufferBrick+num*4);

 left=var1+*(ptr+2);
 top=var2+*(ptr+3);
 right=*ptr+left-1;
 bottom=*(ptr+1)+top-1;

 ptr+=4;

 //check des bords

 if(left>=textWindowLeft && top>=textWindowTop && right<=textWindowRight && bottom<=textWindowBottom)
 {
   right++;
   bottom++;

   outPtr=videoBuffer1+screenLockupTable[top]+left;

   offset=-((right-left)-largeurEcran);

   for(c1=0;c1<bottom-top;c1++)
   {
    vc3=*(ptr++);
    for(c2=0;c2<vc3;c2++)
    {
      temp=*(ptr++);
     iteration=temp&0x3F;
     if(temp&0xC0)
     {
      iteration++;
      if(!(temp&0x40))
      {
       temp=*(ptr++);
       for(i=0;i<iteration;i++)
         *(outPtr++)=temp;
      }
      else
      {
       for(i=0;i<iteration;i++)
        *(outPtr++)=*(ptr++);
      }
     }
     else
     {
      outPtr+=iteration+1;
     }
    }
    outPtr+=offset;
   }
 }
 else
 {
  if(left<=textWindowRight && top<=textWindowBottom && right>=textWindowLeft && bottom>=textWindowTop)
  {
   if(top< textWindowTop) // si il manque un bout du haut de la brique
   {

        for(c1=0;c1<(textWindowTop-top);c1++)
       {
        vc3=*(ptr++);
        for(c2=0;c2<vc3;c2++)
        {
          temp=*(ptr++);
         if(temp&0xC0)
         {
       iteration = 0;
       if(temp&0x40)
          {
           iteration = temp & 0x3F;
          }
       ptr+=iteration+1;
         }
        }
       }
    top=textWindowTop;
   }

    if(bottom> textWindowBottom)
      bottom=textWindowBottom;

    if(left<textWindowLeft)
      cropLeft=textWindowLeft-left;
    else
      cropLeft=0;

    offset=-(left+cropLeft-1-right);

    right-=textWindowRight;
    if(right>0)
      offset-=right;

   outPtr2=screenLockupTable[top]+videoBuffer1;
   if(left>=0)
     outPtr2+=left;
   outPtr=outBuffer;

     for(c1=0;c1<=bottom-top;c1++)
     {
    outPtr=outBuffer;

    for(i=0;i<512;i++)
      outBuffer[i]=0;

      vc3=*(ptr++);
      for(c2=0;c2<vc3;c2++)
      {
        temp=*(ptr++);
       iteration=temp&0x3F;
       if(temp&0xC0)
       {
        iteration++;
        if(!(temp&0x40))
        {
         temp=*(ptr++);
         for(i=0;i<iteration;i++)
           *(outPtr++)=temp;
        }
        else
        {
         for(i=0;i<iteration;i++)
          *(outPtr++)=*(ptr++);
        }
       }
       else
       {
        outPtr+=iteration+1;
       }
      }

      for(i=0;i<offset;i++)
    {
     if(outBuffer[cropLeft+i]!=0)
        *(outPtr2)=outBuffer[cropLeft+i];
     outPtr2++;
    }
    outPtr2+=640-offset;
   }

  }
 }
}

int LBA_engine::fullRedrawS2S1(int arg_0, int arg_4, int arg_8)
{

  int temp_arg_0;
 int temp2_arg_0;
 int temp_arg_4;

  if(setSomethingVar4!=1)
 {
  printf("Unimplemented fullRedrawS2S1\n");
  exit(1);
 }
 else
 {
  temp_arg_0=arg_0;
  arg_0-=arg_8;
  arg_8+=temp_arg_0;
  arg_0*=8;
  temp2_arg_0=arg_0;
  arg_0=temp2_arg_0+arg_0*2;
  arg_0=arg_0>>9;
  arg_0+=setSomethingVar1;
  
  arg_8=arg_8*4+arg_8*4;  
  temp_arg_4=arg_4+arg_4;
  arg_4=arg_4<<5;
  arg_4-=temp_arg_4;
  arg_4-=arg_8;
  arg_4=arg_4>>9;  
  arg_4=-arg_4;
  arg_4+=setSomethingVar2;

  fullRedrawVar3=arg_0;
  fullRedrawVar4=arg_4;
  
  return(-1);
 }
 return(-1);
}


int LBA_engine::fullRedrawS3(int param0,int param1,int param2,int param3,int param4,int param5)
{
	if(samplesLoaded!=0)
	{
		printf("sampleLoaded != 0 in fullRedrawS3!\n");
		exit(1);
	}
	return(0);
}
