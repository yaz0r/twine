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
   They are reseted to their maximum value at the begining of the renderer */

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
    return(renderM2(ptr));

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

  if(temp)
  {
    renderV19=(unsigned char*)renderTab4;
    renderV21=renderTab2;
    primitiveCounter=numOfPri2;
    ptr3=pri2Ptr2=pri2Ptr+18;

    do      // pour chaque poly ?
    {
      temp=*(short int*)ptr3;
      if(temp)
      {
        rs1s2v1=temp;

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
          ebx=*ptr5;
          ebp=(short int)ebx;
          ebx>>=16;

          ecx=*(ptr5+1);

          eax=mainTab[21];
          edx=mainTab[22];
          eax*=ebp;
          edx*=ebx;
          eax+=edx;
          edx=mainTab[23];
          edx*=ecx;
          eax+=edx;
          edi=mainTab[24];

          edx=mainTab[25];
          edi*=ebp;
          edx*=ebx;
          eax+=edi;
          edi=mainTab[26];

          edi*=ecx;
          eax+=edx;
          eax+=edi;
          edi=0;
          ebp*=mainTab[27];
          ebx*=mainTab[28];
          eax+=ebp;
          eax*=mainTab[29];
          eax+=ebx;
          eax+=ecx;

          if(eax>=0)
          {
            eax>>=14;
            ptr6=(unsigned char*)ptr5;
            eax/=*(short int*)(ptr6+6);
            edi=(short int)eax;
          }

          *(short int*)renderV19=edi;
          renderV19+=2;
          ptr5+=2;

        }while(--rs1s2v1);

      }
      ptr3=pri2Ptr2=pri2Ptr2+38;
      ptr4=renderV21=renderV21+9;
    }while(--primitiveCounter);
  }

  return(finishRender((unsigned char*)ptr5));


}

int LBA_engine::finishRender(unsigned char *esi)
{
  unsigned char *edi;
  short int temp;
  int eax, ecx;
  short int bp;
  unsigned char temp2;
  unsigned char counter;
  short int temp3;
  short int type;
  short int color;
//  short int ebx;
 // short int edx;
  lineData* lineDataPtr;
  lineCoordinates* lineCoordinatesPtr;

  int point1;

  int point2;

  int depth;
  int bestDepth;
  short int bestZ;
  int j;
  int bestPoly=0;

  short int ax,bx,cx;
//  short int borrom;

  unsigned char *destPtr;
  int i;

  edi=renderTab7; // renderTab7 c'est le buffer de coordonnées
  temp=*(short int*)esi; // we read the number of polygones
  esi+=2;

  if(temp) // if there is polygones
  {
    primitiveCounter=temp; // the number of primitives = the number of polygones

    do // loop that load all the polygones
    {
      render23=edi;
      ecx=*(int*)esi;
      esi+=2;
      temp2=ecx&0xFF;

      if(temp2>=9)
      {
        ecx-=2;
        *(int*)edi=ecx;
        esi+=2;
        edi+=4;

        ecx>>=8;
        counter=ecx&0xFF;
        bp=0x8300;
        renderV19=edi;
        eax=0;
        do
        {
          *(short int*)edi=((*(unsigned char*)renderTab4)+(*(short int*)esi)*2)+((ecx&0xFF00)>>8);
          eax=*(short int*)(esi+2);
          *(int*)(edi+2)=*(int*)((unsigned char*)renderTab6+eax);
          esi+=4;
          edi+=6;

          temp3=*(short int*)((((unsigned char*)renderTab6)+4)+eax);
          if(temp3>bp)
            bp=temp3;
        }while(--counter);
      }
      else
      if(temp2>=7)
      {
        ecx-=7;
        *(short int*)edi=(short int) ecx;
        bp=*(short int*)esi;
        eax=*(short int*)(esi+2);
        esi+=4;
        bp+=*(short int*)(((unsigned char*)renderTab4)+eax*2);
        *(short int*)(edi+2)=bp;
        edi+=4;
        renderV19=edi;
        bp=0x8300;
        counter=(ecx&0xFF00)>>8;
        do
        {
          eax=*(short int*)esi;
          esi+=2;
          *(int*)(edi+2)=*(int*)(((unsigned char*)renderTab6)+eax);
          edi+=6;
          temp3=*(short int*)((((unsigned char*)renderTab6)+4)+eax);

          if(temp3>bp)
            bp=temp3;
        }while(--counter);
      }
      else
      {
        *(int*)edi=ecx;
        esi+=2;
        edi+=4;

        bp=0x8300;
        renderV19=edi;
        eax=0;
        counter=(ecx&0xFF00)>>8;
        do
        {
          eax=*(short int*)esi;
          esi+=2;

          *(int*)(edi+2)=*(int*)(((unsigned char*)renderTab6)+eax);
          edi+=6;
          temp3=*(short int*)((((unsigned char*)renderTab6)+4)+eax);

          if(temp3>bp)
            bp=temp3;
        }while(--(counter));
      }

      render24=edi;
      edi=renderV19;

      render25=bp;

      ax=*(short int*)(edi+4);
      bx=*(short int*)(edi+8);

      ax-=*(short int*)(edi+16);
      bx-=*(short int*)(edi+2);


      ax*=bx;

      bp=ax;
      bx=temp3;

      ax=*(short int*)(edi+2);
      cx=*(short int*)(edi+10);

      ax-=*(short int*)(edi+14);
      cx-=*(short int*)(edi+4);

      ax*=cx;

      ax-=bp;
      temp3-=(bx)-1; // peut-etre une erreur là

      if(temp3<0)
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
                 drawLine(lineCoordinatesPtr->x1,lineCoordinatesPtr->y1,lineCoordinatesPtr->x2,lineCoordinatesPtr->y2,color);
                 break;
                }
        case 1:  // draw a polygone
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

                  prepareRender();
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

 renderS1S1((int*)renderV19,ebp); // copie dans renderTab2


 renderS1S2(pri1Ptr+rs1v1,rs1v2,&renderTab5[rs1v1/6],(int*)renderV19);
}

void LBA_engine::renderS1S2(unsigned char * esi, int ecx, pointTab *dest,int* eax)
{
//  int i;
 int ebx;
 int edx;
// int var;
// int var2;
 int edi;
 int esip;
 int ebp;


  rs1s2v1=ecx;

 do
 {
  rs1s2v2=(unsigned char*)esi;

  edx=ebx=*(int*)esi;
  ebx>>=16;
  ecx=*(short int*)(esi+4);

  esip=eax[0];
  ebp=eax[1];
  esip*=edx;
  ebp*=ebx;
  esip+=ebp;
  ebp=eax[2];
  ebp*=ecx;
  esip+=ebp;
  esip>>=14;

  esip+=mainTab[18];
  dest->x=esip;

  esip=eax[3];

  ebp=eax[4];
  esip*edx;
  ebp*=ebx;
  esip+=ebp;


  ebp=eax[5];

  ebp*=ecx;
  esip+=ebp;
  esip>>=14;
  esip+=mainTab[19];
  dest->y=esip;

  edx*=eax[6];
  ebx*=eax[7];
  ecx*=eax[8];
  ecx+=edx;
  ecx+=ebx;
  ecx>>=14;
  ecx+=mainTab[20];
  dest->z=ecx;

/*  dest->x=(((*eax)*edx+(*(eax+1))*ebx+(*(eax+2))*ecx)>>14+(short int)mainTab[18]);
  dest->y=(((*eax+3)*edx+(*(eax+4))*ebx+(*(eax+5))*ecx)>>14+(short int)mainTab[19]);
  dest->z=(((*eax+6)*edx+(*(eax+7))*ebx+(*(eax+8))*ecx)>>14+(short int)mainTab[20]);*/

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

  if(mainTab[12])
 {
  edx=mainTab[12];
  edx&=0x3FF;
  esi=tab1[edx];
  edx+=0x100;
  edx&=0x3FF;
  ecx=tab1[edx];

  edi=ebp[0];
  edx=ebp[3];
  eax[0]=edi;
  edi=ebp[6];
  eax[3]=edx;
  eax[6]=edi;

  edx=ebp[1];
  edi=ebp[2];
  edx*=ecx;
  edi*=esi;
  edi+=edx;
  edi>>=14;
  eax[1]=edi;

  edx=ebp[1];
  edi=ebp[2];
  edx*=esi;
  edi*=ecx;
  edi-=edx;
  edi>>=14;
  eax[2]=edi;

  // pause

  edx=ebp[4];
  edi=ebp[5];
  edx*=ecx;

  edi*=esi;
  edi+=edx;
  edi>>=14;
  eax[4]=edi;

  edx=ebp[4];
  edi=ebp[5];
  edx*=esi;
  edi*=ecx;
  edi-=edx;
  edi>>=14;
  eax[5]=edi;

  edx=ebp[7];
  edi=ebp[8];
  edx*=ecx;
  edi*=esi;
  edi+=edx;
  edi>>=14;
  eax[7]=edi;

  edx=ebp[7];

  edi=ebp[8];
  edx*=esi;
  edi*=ecx;
  edi-=edx;
  edi>>=14;
  eax[8]=edi;

  ebp=eax;

 }
 if(mainTab[14])
 {
  printf("renderS1S1\n");
  exit(1);
 }
 if(mainTab[13])
 {

  if(ebp==eax)
  {
    edx=mainTab[13];

    ebp=&mainTab[30];

    ptr=ebp;
    for(i=0;i<9;i++)
      *(ptr++)=*(eax++);
  }

  edx&=0x3FF;
  esi=tab1[edx];
  edx+=0x100;
  edx&=0x3FF;
  ecx=tab1[edx];

  edi=ebp[1];
  edx=ebp[4];
  eax[1]=edi;
  edi=ebp[7];
  eax[4]=edx;
  eax[7]=edi;

  edx=ebp[2];
  edi=ebp[0];
  edx*=esi;
  edi*=ecx;
  edi-=edx;
  edi>>=14;
  eax[0]=edi;

  edx=ebp[2];
  edi=ebp[0];
  edx*=ecx;
  edi*=esi;
  edi+=edx;
  edi>>=14;
  eax[2]=edi;

  // pause

  edx=ebp[5];
  edi=ebp[3];
  edx*=esi;
  edi*=ecx;

  edi-=edx;
  edi>>=14;
  eax[3]=edi;

  edx=ebp[5];
  edi=ebp[3];
  edx*=ecx;
  edi*=ecx;
  edi+=edx;
  edi>>=14;
  eax[5]=edi;

  edx=ebp[8];
  edi=ebp[6];
  edx*=esi;
  edi*=ecx;
  edi-=edx;
  edi>>=14;
  eax[6]=edi;

  edx=ebp[8];
  edi=ebp[6];
  edx*=ecx;
  edi*=esi;
  edi+=edx;
  edi>>=14;
  eax[8]=edi;

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