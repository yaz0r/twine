#include "lba.h"

int LBA_engine::applyAnim(int animState,char* animData,char* body)
{
	short int animOpcode;

	short int var0;
	short int var1;

	var1=*(short int*)(animData+2);

	keyFramePtr=(var1*8)*animState+animData+8;

	var0=*(short int*)body;

	if(!(var0&2))
	{
		return(0);
	}

	animVar1=body+16;

	printf("applyAnim ok\n");
	exit(1);


/*	while(--animVar4)
	{
		animOpcode=getAnimOpcode();
		if(animOpcode==0)
		{
			applyAnimMode0();
			applyAnimMode0();
			applyAnimMode0();
		}
		else
		if(animOpcode==1)
		{
			applyAnimMode1();
			applyAnimMode1();
			applyAnimMode1();
		}
		else
		if(animOpcode==2)
		{
			applyAnimMode2();
			applyAnimMode2();
			applyAnimMode2();
		}
		edi+=30;
	}*/
	return(0);
}

int LBA_engine::draw3D1(int animState,char* animData,char* body)
{
	short int animOpcode;

	short int var0;

	char* edi;
	char* ebx;
	int ebp;
	int eax;
	int keyFrameLength;
	int numOfPointInBody;
	int numOfPointInAnim;

	numOfPointInAnim=*(short int*)(animData+2);

	keyFramePtr=((numOfPointInAnim*8+8)*animState)+animData+8;

	keyFrameLength=*(short int*)keyFramePtr;

	var0=*(short int*)body;

	if(!(var0&2))
	{
		return(0);
	}

	edi=body+16;

	animVar1=edi;

	ebx=*(char**)edi;
	ebp=*(int*)(edi+4);

	if(!ebx)
	{
		ebx=keyFramePtr;
		ebp=keyFrameLength;
	}

	animVar2=ebx;

	eax=*(short int*)(edi-2);
	edi+=eax;

	eax=*(short int*)(edi);
	eax=eax+eax*2;
	edi=edi+eax*2+12;

	numOfPointInBody=*(short int*)(edi-10);

	if(numOfPointInAnim>numOfPointInBody)
	{
		numOfPointInAnim=numOfPointInBody;
	}

	eax=time-ebp;

	if(eax>=keyFrameLength)
	{
		int* destPtr; //keyFrame
		int* sourcePtr;

		sourcePtr=(int*)(keyFramePtr+8);
		destPtr=(int*)edi;

		do
		{
			*(destPtr++)=*(sourcePtr++);
			*(destPtr++)=*(sourcePtr++);
			
			destPtr=(int*)(((char*)destPtr)+30);

		}while(--numOfPointInAnim);

		*(char**)animVar1=keyFramePtr;
		*(int*)(animVar1+4)=time;

		currentX=*(short int*)(keyFramePtr+2);
		currentZ=*(short int*)(keyFramePtr+4);
		currentY=*(short int*)(keyFramePtr+6);

		processActorVar5=*(short int*)(keyFramePtr+8);
		processActorSub2Var0=*(short int*)(keyFramePtr+10);
		processActorVar6=*(short int*)(keyFramePtr+12);
		processActorSub2Var1=*(short int*)(keyFramePtr+14);
	
		return(1);
	}
	else
	{
		printf("Interpole animation\n");
	}

	return(0);
}

int LBA_engine::playAnim(char costume, short int arg_4, unsigned char arg_8, short int actorNum)
{
 actor* lactor;
 int var_4;
 
 lactor=&actors[actorNum];
 
 if(lactor->costumeIndex==-1)
 	return(0);
  
 if(lactor->field_60 & 0x400) // si c'est un sprite
 	return(0);

 if(costume== lactor->costume && lactor->currentAnim!=-1) // le costume est deja loadé
 	return(1); 
  
 if(arg_8==255 && lactor->field_78 !=2)
 	arg_8=lactor->costume;
  
 var_4=initCostume(costume,actorNum);
 
 if(var_4 == -1)
	 var_4=initCostume(0,actorNum);
  
 if(arg_4!=4 && lactor->field_78==2)
 {
	lactor->field_2=costume;
	return(0);
 }
 
 if(arg_4==3)
 {
  arg_4=2;
  
  arg_8=lactor->costume;
  
  if(arg_8 == 15 || arg_8 ==7 || arg_8==8 || arg_8==9)
  {
   	arg_8=0;
  }
  
 }
 
 if(arg_4==4)
 	arg_4=2;
 
 if(lactor->currentAnim == -1)
 {
	drawMenuWin1(0,getHqrdataPtr(HQRanims,var_4),bodyPtrTab[lactor->costumeIndex]);
 }
 else
 {
	bufAni2+=increaseAnim((char*)bufAni2,(char*)bodyPtrTab[lactor->costumeIndex]);
	if(bufAni1+4488<bufAni2)
  		bufAni2=bufAni1;
 } 
 
 lactor->currentAnim=var_4;
 lactor->costume=costume;
 lactor->field_2=arg_8;
 lactor->field_4=loadTwinsenCostumesVar1;
 lactor->field_78=arg_4;
 lactor->animPosition=0;
 *(unsigned char*)&lactor->field_62 &= 0xF9;
 *(unsigned char*)&lactor->field_62 |= 8;
 
 if(lactor->field_4!=0)
 {
	 initNewCSub(lactor,actorNum);
 }
  
 lactor->field_6A=0;
 lactor->lastX=0;
 lactor->lastZ=0;
  
	return(1);
}

int LBA_engine::increaseAnim(char* lBufAnim,char* lBody)
{
	int temp;
	char* ptr;
	int* edi;
	int* esi;
	int var0;
	int var1;
	int var2;
	int counter;

	temp=*(short int*)lBody;

	if(temp&2)
	{
		ptr=(lBody+0x10);
		*(int*)(ptr+4)=time;
		*(char**)(ptr)=lBufAnim;

		var0=*(short int*)(ptr-2);
		ptr=ptr+var0;

		var1=*(short int*)(ptr);
		var1=var1+var1*2;

		ptr=ptr+var1*2+2;

		var2=*(short int*)(ptr);
		counter=var2;
		var2=var2*8;

		edi=(int*)(lBufAnim+8);
		esi=(int*)(ptr+10);

		do
		{
			*(edi++)=*(esi++);
			*(edi++)=*(esi++);

		}while(--counter);
	}

	return(0);
}

void LBA_engine::initNewCSub(actor* lactor,int actorNum)
{
	char* ebx;
	int var_4;
	int var_18;
	char al;
	int var_54;
	char* edi;
	char* var_48;
	char* var_50;
	char* var_44;
	char* var_40;
	char* var_3C;
	char* var_4C;
	char* var_38;
	char* var_34;
	char* var_30;
	char* edx;
	char* eax;
	short int temp;

	return;

	ebx=(char*)lactor->field_4;

	var_4=0;
	var_18=*(ebx++);

	while(var_4<var_18)
	{
		al=*(ebx++);

		al-=5;
		if(al>16)
			return;

		var_54=al;

		edi=ebx+1;

		var_48=edi+2;
		var_50=var_48+2;
		var_44=var_50+2;
		var_40=var_44+2;
		var_3C=var_40+2;
		var_4C=var_3C+2;
		var_38=var_4C+2;

		var_34=var_38+1;
		var_30=var_34+1;

		edx=edi+11;
		eax=edi+15;

		switch(var_54)
		{
		case 0:
			temp=*(ebx++);
			temp--;
			if(temp==lactor->animPosition)
			{
				lactor->field_66=*(ebx++);
				lactor->field_62|=2;
			}
			else
			{
				ebx++;
			}
			break;
		case 1:
			temp=*(ebx++);

			if(temp==lactor->animPosition)
			{
				fullRedrawS3(*(short int*)ebx,0x1000,1,lactor->X,lactor->Z,lactor->Y);
			}

			ebx+=2;
			break;
		case 2:
			temp=*(ebx++);
			if(temp==lactor->animPosition)
			{
				printf("Skipping initNewCSub 2\n");
				ebx+=2;
			}
			else
			{
				ebx+=4;
			}
			break;
		case 5:
			temp=*(ebx++);
			if(temp==lactor->animPosition)
			{
				int dx;
				int cx;

				dx=*(short int*)ebx;
				ebx+=2;
				cx=*(short int*)ebx;
				ebx+=2;

				fullRedrawS3(dx,0x1000,cx,lactor->X,lactor->Z,lactor->Y);
			}
			else
			{
				ebx+=4;
			}
			break;
		case 8:
			printf("Skipping initNewCSub opcode 8\n");
			ebx=var_48;
			break;
		case 10:
			ebx=edi;
			printf("Skipping initNewCSub opcode 10\n");
			break;
		case 11:
			ebx=edi;
			printf("Skipping initNewCSub opcode 11\n");
			break;
		default:
			printf("Unsupported opcode %d in initNewCSub\n",var_54);
			exit(1);
		}

		var_4++;
	}

}