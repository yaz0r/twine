/***************************************************************************
                          3Dmodels.cpp  -  description
                             -------------------
    begin                : Tue Jun 4 2002
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

/* That's the entry point of the 3D renderer */

int LBA_engine::startRenderer(int arg_0,int arg_4,int arg_8,int arg_C,int arg_10,int arg_14,unsigned char* costumePtr)
{
	unsigned char *ptr;

	mainTab[12]=arg_C;
	mainTab[13]=arg_10;
	mainTab[14]=arg_14;

	/* Those 4 vars are used to know the size of the rendered model.
	They are resetted to their maximum value at the begining of the renderer */

	renderLeft=32767;
	renderTop=32767;

	renderRight=-32767;
	renderBottom=-32767;

	/* the use of setSomethingVar4 is still unknown */

	if(setSomethingVar4==0)
	{
		printf("Unimplemented startRenderer\n");
		exit(1);
	}
	else
	{
		renderV9=arg_0;
		renderV11=arg_4;
		renderV10=arg_8;
	}


	numOfPrimitives=0; // we reset the number of primitives in the model

	renderTabEntryPtr=renderTab; // we restart at the beginning of the renderTable

	costumeHeader=*(short int*)costumePtr; // costumeHeader seems to contains important flags for the rendering

	// note: most of the LBA1 models are using renderM1
	// renderM2 may be used for render in Bu temple (light from the bottom)

	ptr=costumePtr+16+*(short int*)(costumePtr+14); // we jump after the header

	if(costumeHeader&2)
		return(renderM1(ptr));  // That's the mostly used renderer code
	else
		return(renderM2(ptr));  // probably render in Bù temple

}

int LBA_engine::renderM1(unsigned char * costumePtr)
{
	unsigned int var1;
	//  unsigned char* ptr1;
	//  unsigned char* ptr2;
	unsigned char* ptr3;
	int *ptr4;
	int *ptr5;

	int coX;
	int coY;
	int coZ;

	unsigned char* ptr6;
	int eax;
	int ebx;
	int ebp;
	int edx;
	int ecx;
	int edi;
	short int flag;
	int temp;
	pointEntry *ptEntryPtr;
	pointTab *pointPtr;
	pointTab *pointPtr2;

	numOfPri1=*(short int*)costumePtr;
	costumePtr+=2;

	pri1Ptr=costumePtr;

	var1=numOfPri1+numOfPri1*2;
	costumePtr=costumePtr+var1*2;

	numOfPri2=*(short int*)costumePtr; // that's the number of points in the model

	costumePtr+=2; // we jump to the points data

	pri2Ptr2=pri2Ptr=costumePtr; // we save the point data ptr

	renderV19=(unsigned char *)renderTab2;

	renderS1(mainTab[14],mainTab[13],mainTab[12],(pointEntry*)pri2Ptr2); // that's the process of the "base point"

	pri2Ptr2+=38; // jump to the next point data

	ptEntryPtr=(pointEntry*)pri2Ptr2; // that's the structure used to load the points

	if(numOfPri2 -1 !=0 ) // if there is points after the base point
	{
		primitiveCounter=numOfPri2-1; // Since the base point is already processed, we need to load numOfPri2-1 points
		renderV19=(unsigned char *)renderTab3; // the use of renderTab3 is still unknown. It's a table with 36byte long element

		do   // That's the loop that load and convert all the points in the model
		{
			ebx=ptEntryPtr->flag1;  // actualy, there should be 4 flags in that order = ebx,flag,edx,ecx. So this part may be buggy. at the momment
			flag=ebx;
			ebx>>=16;

			edx=ptEntryPtr->flag2;
			ecx=(short int)edx;
			edx>>=16;

			if(flag == 0)
			{
				renderS1(edx,ecx,ebx,ptEntryPtr); // renderS1 load the points, rotate them and store them in renderTab5 (it may do other things too..)
			}
			else
			{
				if(flag == 1)
				{

					//renderS2      // this part seems to be rarely used...

					printf("renderM1\n");
					exit(1);
				}
			}

			renderV19+=36; // next entry in renderTab3

			pri2Ptr2+=38;  // next point data
			ptEntryPtr=(pointEntry*)pri2Ptr2; // we use the structure to load the point data
		}while(--primitiveCounter);
	}

	primitiveCounter = numOfPri1;

	pointPtr2=(pointTab*)renderTab5;
	pointPtr=renderTab6;

	if(setSomethingVar4!=0) // boucle de convertion des points de 3D (de renderTab5) à 2D (de renderTab6)
	{
		do{  // this loop convert the 3D points (renderTab5) into 2D points (renderTab6)
			coX=pointPtr2->x+renderV9;
			coY=pointPtr2->y+renderV11;
			coZ=-(pointPtr2->z+renderV10);

			pointPtr->x=(((coX+coZ)*8+(coX+coZ)*16)>>9)+setSomethingVar1; // those lines may be buggy
			pointPtr->y=((((coX-coZ)*4+(coX-coZ)*8)+(2*coY-(coY<<5)))>>9)+setSomethingVar2;
			pointPtr->z=coZ-coX-coY;

			if(pointPtr->x < renderLeft)  // this part setup the size of the rendered model (to be used by the main drawing function)
				renderLeft=pointPtr->x;
			if(pointPtr->x > renderRight)
				renderRight=pointPtr->x;

			if(pointPtr->y < renderTop)
				renderTop=pointPtr->y;
			if(pointPtr->y > renderBottom)
				renderBottom=pointPtr->y;

			pointPtr2++; // next 3D point entry
			pointPtr++;  // next 2D point entry

		}while(--primitiveCounter);

		ptr5=(int*)pri2Ptr2;
		eax=-1;

	}
	else
	{
		printf("renderM1\n"); // another rarely used part.
		exit(1);
	}

#ifdef DRAWPOINTS
	/* This part draw all the points of the model */
	primitiveCounter = numOfPri1;
	pointPtr=renderTab6;
	do{
		if((pointPtr->x>0 && pointPtr->x < 640) && (pointPtr->y>0 && pointPtr->y <480))
		{
			*(videoBuffer1+screenLockupTable[pointPtr->y]+pointPtr->x)=255;
		}
		else
		{
			printf("Buggy point...\n");
		}

		pointPtr++;

	}while(--primitiveCounter);

	osystem->drawBufferToScreen(videoBuffer1);
#endif

	temp=*(short int*)ptr5;

	ptr5=(int*)(((unsigned char*)ptr5)+2);

	if(temp) // process shading table
	{
		renderV19=(unsigned char*)shadeTable;
		renderV21=renderTab2;
		primitiveCounter=numOfPri2;
		ptr3=pri2Ptr2=pri2Ptr+18;

		do      // pour chaque poly ?
		{
			temp=*(short int*)ptr3;
			if(temp)
			{
				rs1s2v1=temp;

				mainTab[15]=0; //light position
				mainTab[16]=40;
				mainTab[17]=40;

				mainTab[21]=(*renderV21)*mainTab[15];
				mainTab[22]=(*(renderV21+1))*mainTab[15];
				mainTab[23]=(*(renderV21+2))*mainTab[15];

				mainTab[24]=(*(renderV21+3))*mainTab[16];
				mainTab[25]=(*(renderV21+4))*mainTab[16];
				mainTab[26]=(*(renderV21+5))*mainTab[16];

				mainTab[27]=(*(renderV21+6))*mainTab[17];
				mainTab[28]=(*(renderV21+7))*mainTab[17];
				mainTab[29]=(*(renderV21+8))*mainTab[17];

				do    // pour chaque vertex ?
				{
					short int col1;
					short int col2;
					short int col3;

					short int *colPtr;

					colPtr=(short int*)ptr5;

					col1=*(colPtr++);
					col2=*(colPtr++);
					col3=*(colPtr++);

					eax=mainTab[21]*col1+mainTab[22]*col2+mainTab[23]*col3;
					eax+=mainTab[24]*col1+mainTab[25]*col2+mainTab[26]*col3;
					eax+=mainTab[27]*col1+mainTab[28]*col2+mainTab[29]*col3;

					edi=0;

					if(eax>0)
					{
						eax>>=14;
						ptr6=(unsigned char*)ptr5;
						eax/=*(unsigned short int*)(ptr6+6);
						edi=(unsigned short int)eax;
					}

					*(short int*)renderV19=edi;
					renderV19+=2;
					ptr5+=2;

				}while(--rs1s2v1);

			}
			ptr3=pri2Ptr2=pri2Ptr2+38;
			ptr4=renderV21=renderV21+9;
		}while(--primitiveCounter);

/*		FILE* fileHandle;

		fileHandle=fopen("shade.dmp","wb+");

		int i;

		for(i=0;i<500;i++)
		{
			fwrite(&shadeTable[i],1,2,fileHandle);
		}

		fclose(fileHandle);*/
	}



	return(finishRender((unsigned char*)ptr5));
}

int LBA_engine::finishRender(unsigned char *esi)
{
	unsigned char *edi;
	short int temp;
	int eax, ecx;
	unsigned char temp2;
	short int counter;
	short int type;
	short int color;
	float tempFloat;
	//  short int ebx;
	// short int edx;
	lineData* lineDataPtr;
	lineCoordinates* lineCoordinatesPtr;

	int point1;

	int point2;

	int depth;
	int bestDepth;
	int currentDepth;
	short int bestZ;
	int j;
	int bestPoly=0;
	short int shadeEntry;
	short int shadeValue;

	short int ax,bx,cx;
	//  short int borrom;

	unsigned char *destPtr;
	int i;

	struct polyHeader
	{
		unsigned char polyRenderType;
		unsigned char numOfVertex;
		short int colorIndex;
	};

	struct polyVertexHeader
	{
		short int shadeEntry;
		short int dataOffset;
	};

	struct computedVertex
	{
		short int shadeValue;
		short int x;
		short int y;
	};

	polyVertexHeader* currentPolyVertex;
	polyHeader* currentPolyHeader;
	polyHeader* destinationHeader;
	computedVertex* currentComputedVertex;
	pointTab* currentVertex;
	pointTab* destinationVertex;

	edi=renderTab7; // renderTab7 c'est le buffer de coordonnées
	temp=*(short int*)esi; // we read the number of polygones
	esi+=2;

  if(temp) // if there is polygones
  {
    primitiveCounter=temp; // the number of primitives = the number of polygones

    do // loop that load all the polygones
    {
      render23=edi;
	  currentPolyHeader=(polyHeader*)esi;
	  ecx=*(int*)esi;
      esi+=2;
      polyRenderType=currentPolyHeader->polyRenderType;

      if(polyRenderType>=9)
      {
		destinationHeader=(polyHeader*)edi;

		destinationHeader->polyRenderType	=	currentPolyHeader->polyRenderType-2;
		destinationHeader->numOfVertex		=	currentPolyHeader->numOfVertex;
		destinationHeader->colorIndex		=	currentPolyHeader->colorIndex;

        esi+=2;
        edi+=4;

        counter=destinationHeader->numOfVertex;

        bestDepth=-32000;
        renderV19=edi;

		do
		{
			currentPolyVertex=(polyVertexHeader*)esi;

			shadeValue=currentPolyHeader->colorIndex+shadeTable[currentPolyVertex->shadeEntry];

			currentComputedVertex=(computedVertex*)edi;

			currentComputedVertex->shadeValue=shadeValue;


			currentVertex=&renderTab6[currentPolyVertex->dataOffset/6];
			destinationVertex=(pointTab*)(edi+2);

			destinationVertex->x=currentVertex->x;
			destinationVertex->y=currentVertex->y;

			edi+=6;
			esi+=4;

			currentDepth=currentVertex->z;

			if(currentDepth>bestDepth)
				bestDepth=currentDepth;
		}while(--counter);
      }
      else
      if(polyRenderType>=7) // only 1 shade value is used
      {
		destinationHeader=(polyHeader*)edi;

		destinationHeader->polyRenderType	=	currentPolyHeader->polyRenderType-7;
		destinationHeader->numOfVertex		=	currentPolyHeader->numOfVertex;
      
        color=currentPolyHeader->colorIndex;

		shadeEntry=*(short int*)(esi+2);

        esi+=4;
 
        *(short int*)(edi+2)=color+shadeTable[shadeEntry];

        edi+=4;
        renderV19=edi;
        bestDepth=-32000;
        counter=destinationHeader->numOfVertex;

        do
        {
			eax=*(short int*)esi;
			esi+=2;

			currentVertex=&renderTab6[eax/6];

			destinationVertex=(pointTab*)(edi+2);

			destinationVertex->x=currentVertex->x;
			destinationVertex->y=currentVertex->y;

			edi+=6;
			
			currentDepth=currentVertex->z;

          if(currentDepth>bestDepth)
            bestDepth=currentDepth;
        }while(--counter);
      }
      else // no shade is used
      {
		destinationHeader=(polyHeader*)edi;

		destinationHeader->polyRenderType	=	currentPolyHeader->polyRenderType-2;
		destinationHeader->numOfVertex		=	currentPolyHeader->numOfVertex;
		destinationHeader->colorIndex		=	currentPolyHeader->colorIndex;

        esi+=2;
        edi+=4;

        bestDepth=-32000;
        renderV19=edi;
        eax=0;
        counter=currentPolyHeader->numOfVertex;

        do
        {
			eax=*(short int*)esi;
			esi+=2;

			currentVertex=&renderTab6[eax/6];

			destinationVertex=(pointTab*)(edi+2);

			destinationVertex->x=currentVertex->x;
			destinationVertex->y=currentVertex->y;

			edi+=6;
			
			currentDepth=currentVertex->z;

          if(currentDepth>bestDepth)
            bestDepth=currentDepth;
        }while(--(counter));
      }

      render24=edi;
      edi=renderV19;

      render25=bestDepth;

      ax=*(short int*)(edi+4);
      bx=*(short int*)(edi+8);

      ax-=*(short int*)(edi+16);
      bx-=*(short int*)(edi+2);


      ax*=bx;

      bestDepth=ax;
      bx=currentDepth;

      ax=*(short int*)(edi+2);
      cx=*(short int*)(edi+10);

      ax-=*(short int*)(edi+14);
      cx-=*(short int*)(edi+4);

      ax*=cx;

      ax-=bestDepth;
      currentDepth-=(bx)-1; // peut-etre une erreur là

      if(currentDepth<0)
      {
        edi=render23;
      }
      else
      {
        numOfPrimitives++;

        renderTabEntryPtr->depth=render25;
        renderTabEntryPtr->renderType=1;
        renderTabEntryPtr->dataPtr=render23;
        renderTabEntryPtr++;

        edi=render24;
      }

    }while(--primitiveCounter);
  }




  temp=*(short int*)esi;
  esi+=2;
  if(temp) // pour les lignes (0)
  {
    numOfPrimitives+=temp;
    do
    {
      lineDataPtr=(lineData*)esi;
      lineCoordinatesPtr=(lineCoordinates*)edi;

      if(lineDataPtr->p1%6 != 0 || lineDataPtr->p2%6 !=0)
      {
        printf("lineDataPtr reference is malformed !\n");
        exit(1);
      }

      point1=lineDataPtr->p1/6;
      point2=lineDataPtr->p2/6;
      lineCoordinatesPtr->data=lineDataPtr->data;
      lineCoordinatesPtr->x1=renderTab6[point1].x;
      lineCoordinatesPtr->y1=renderTab6[point1].y;
      lineCoordinatesPtr->x2=renderTab6[point2].x;
      lineCoordinatesPtr->y2=renderTab6[point2].y;
      bestDepth=renderTab6[point1].z;
      depth=renderTab6[point2].z;

      if(depth>=bestDepth)
        bestDepth=depth;

      renderTabEntryPtr->depth=bestDepth;
      renderTabEntryPtr->renderType=0;
      renderTabEntryPtr->dataPtr=edi;
      renderTabEntryPtr++;

      esi+=8;
      edi+=12;
    }while(--temp);
  }



  temp=*(short int*)esi;
  esi+=2;
  if(temp) // pour les cercles (2)
  {
//    numOfPrimitives+=temp;
    do
    {
      esi+=8;
    }while(--temp);
  }

  renderTabEntryPtr2=renderTab;


  renderTabSortedPtr=renderTabSorted;
 for(i=0;i<numOfPrimitives;i++)  // then we sort the polygones (FIXME: very slow !)
  {
    renderTabEntryPtr2=renderTab;
    bestZ=-0x7FFF;
    for(j=0;j<numOfPrimitives;j++)
    {
      if(renderTabEntryPtr2->depth > bestZ)
      {
        bestZ=renderTabEntryPtr2->depth;
        bestPoly=j;
      }
      renderTabEntryPtr2++;
    }
    renderTabSortedPtr->depth=renderTab[bestPoly].depth;
    renderTabSortedPtr->renderType=renderTab[bestPoly].renderType;
    renderTabSortedPtr->dataPtr=renderTab[bestPoly].dataPtr;
    renderTabSortedPtr++;
    renderTab[bestPoly].depth=-0x7FFF;
  }
  renderTabEntryPtr2=renderTabSorted;

  if(numOfPrimitives)
  {
    primitiveCounter=numOfPrimitives;
    renderV19=esi;

    do
    {
      type=renderTabEntryPtr2->renderType;
      esi=renderTabEntryPtr2->dataPtr;
      renderV19+=8;
      switch(type)
      {
        case 0:  // draw a line
                {
                 lineCoordinatesPtr=(lineCoordinates*)esi;
                 color=(lineCoordinatesPtr->data&0xFF00)>>8;
                 //drawLine(lineCoordinatesPtr->x1,lineCoordinatesPtr->y1,lineCoordinatesPtr->x2,lineCoordinatesPtr->y2,color);
                 break;
                }
        case 1:  // draw a polygon
                {
                  eax=*(int*)esi;
                  esi+=4;

                  polyRenderType=eax&0xFF;
                  numOfVertex=(eax&0xFF00)>>8;
                  color=(eax&0xFF0000)>>16;

                  destPtr=(unsigned char*)vertexCoordinates;

                  for(i=0;i<(numOfVertex*3);i++)
                  {
                    *(short int*)destPtr=*(short int*)esi;
                    destPtr+=2;
                    esi+=2;
                  }

                  if(prepareRender()!=2)
					polyRender(polyRenderType,color);

                 break;
                }


        default:{
                  break;
                }
      };
      esi=renderV19;
      renderTabEntryPtr2++;
    }while(--primitiveCounter);
	//while(0);
  }
  else
  {
    renderRight=-1;
    renderBottom=-1;
    renderLeft=-1;
    renderTop=-1;
    return(-1);
  }

  return(0);
}

void LBA_engine::renderS1(int edx, int ecx, int ebx, pointEntry* ptr)
{
  int *ebp;
 short int var;
// int* ptr1;

 mainTab[12]=ebx;
 mainTab[13]=ecx;
 mainTab[14]=edx;

 rs1v1=ptr->data1;

 rs1v2=ptr->data2;


  if(rs1v1%6)
    {
      printf("Error: rs1V1\n");
      exit(1);
    }

 var=ptr->param;

 if(var == -1)    // si c'est le premier point
 {
  ebp=&mainTab[39];

  mainTab[18]=0;
  mainTab[19]=0;
  mainTab[20]=0;
 }
 else
 {
  ebp=(int*)(((unsigned char*)renderTab2)+var);

  mainTab[18]=renderTab5[ptr->data3/6].x;
  mainTab[19]=renderTab5[ptr->data3/6].y;
  mainTab[20]=renderTab5[ptr->data3/6].z;
 }

 //renderV19= dest
 //ebp= source
 renderS1S1((int*)renderV19,ebp); // copie dans renderTab2 + application de la rotation

 // ? , numOfPoint , destination, rotation data
 renderS1S2(pri1Ptr+rs1v1,rs1v2,&renderTab5[rs1v1/6],(int*)renderV19);
}

void LBA_engine::renderS1S2(unsigned char * esi, int ecx, pointTab *dest,int* eax)
{
//  int i;
 short int param1;
 short int param2;
 short int param3;

 int ebx;
 int edx;
// int var;
// int var2;
 int edi;
 int esip;
 int ebp;

 short int* tempPtr;

  rs1s2v1=ecx;

 do
 {
  rs1s2v2=esi;
  tempPtr=(short int*)(esi);

  param1=tempPtr[0];
  param2=tempPtr[1];
  param3=tempPtr[2];

  dest->x=((eax[0]*param1+eax[1]*param2+eax[2]*param3)>>14)+mainTab[18];
  dest->y=((eax[3]*param1+eax[4]*param2+eax[5]*param3)>>14)+mainTab[19];
  dest->z=((eax[6]*param1+eax[7]*param2+eax[8]*param3)>>14)+mainTab[20];

  dest++;
  esi=rs1s2v2+6;
 }while(--rs1s2v1);
}

void LBA_engine::renderS1S1(int* eax, int* ebp)
{
  int i;
  int edx;
  int esi;
  int ecx;
  int edi;
  int *ptr;

  int angle;
  int angleVar1; //esi
  int angleVar2; //ecx

  if(mainTab[12]) // rotation par vers l'avant
 {
	angle=mainTab[12]&0x3FF;
	angleVar2=tab1[angle];
	angle+=0x100;
	angle&=0x3FF;
	angleVar1=tab1[angle];

	eax[0]=ebp[0];
	eax[3]=ebp[3];
	eax[6]=ebp[6];

	eax[1]=(ebp[2]*angleVar2+ebp[1]*angleVar1)>>14;
	eax[2]=(ebp[2]*angleVar1-ebp[1]*angleVar2)>>14;
	eax[4]=(ebp[5]*angleVar2+ebp[4]*angleVar1)>>14;
	eax[5]=(ebp[5]*angleVar1-ebp[4]*angleVar2)>>14;
	eax[7]=(ebp[8]*angleVar2+ebp[7]*angleVar1)>>14;
	eax[8]=(ebp[8]*angleVar1-ebp[7]*angleVar2)>>14;

	ebp=eax;

 }
 if(mainTab[14])
 {
  printf("renderS1S1 -> unhandled maintTab[14] rotation\n");
  exit(1);
 }
 if(mainTab[13]) // rotation de coté (la plus courante)
 {

  if(ebp==eax)
  {
    ebp=&mainTab[30];

    for(i=0;i<9;i++)
      *(ebp++)=*(eax++);
  }

  angle=mainTab[13]&0x3FF;
  angleVar2=tab1[angle]; //esi
  angle+=0x100;
  angle&=0x3FF;
  angleVar1=tab1[angle]; //ecx


  eax[1]=ebp[1];
  eax[4]=ebp[4];
  eax[7]=ebp[7];


  eax[0]=(ebp[0]*angleVar1-ebp[2]*angleVar2)>>14;
  eax[2]=(ebp[0]*angleVar2+ebp[2]*angleVar1)>>14;
  eax[3]=(ebp[3]*angleVar2-ebp[5]*angleVar1)>>14;
  eax[5]=(ebp[5]*angleVar2+ebp[3]*angleVar1)>>14;
  eax[6]=(ebp[6]*angleVar1-ebp[8]*angleVar2)>>14;
  eax[8]=(ebp[6]*angleVar2+ebp[8]*angleVar1)>>14;

  return;


 }
 else
 {
  if(ebp!=eax)
    for(i=0;i<9;i++)
     *(eax++)=*(ebp++);

  return;
 }

}

int LBA_engine::renderM2(unsigned char * ptr)
{
  printf("renderM2\n");
 exit(1);
  return(-1);
}