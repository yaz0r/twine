/***************************************************************************
                          polyRender.cpp  -  description
                             -------------------
    begin                : Thu May 23 2002
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

void LBA_engine::polyRender(int ecx, int edi)
{
	 unsigned char *out,*out2;
  short int *ptr1,*ptr2;
  int vsize,hsize;
  int color;
  int eax;
  int i,j;
  short int var1, var2, var3, var4;
//  char borrow;
  short int start,stop;
  short int startColor, stopColor;
  short int colorStep;
  short int colorSize;

  out=videoBuffer1+screenLockupTable[vtop];
  
  ptr1=&polyTab[vtop];
  ptr2=&polyTab2[vtop];
  
  vsize=vbottom-vtop;
  vsize++;
  
  color=edi;

  switch(ecx)
  {
   case 8:
   case 7:
   case 0:   // flat polygone
   {
   	eax=(color&0xFF) | ((color&0xFF)<<8) | ((color&0xFF)<<16) | ((color&0xFF)<<24);
    for(i=0;i<vsize;i++)
    {
     stop=ptr1[480];
     start=ptr1[0];

     ptr1++;
     hsize=stop-start;
     
     if(hsize>0)
     {
      hsize++;
      out2=start+out;
      for(j=0;j<hsize;j++)
      {
       *(out2++)=color;
      }
     }    
     out+=640;
    }
    break;
   }
   case -1: // main codec
   {
    renderLoop=vsize;
    do
    {
/*     stop=ptr1[480];
     start=ptr1[0];

     startColor=ptr2[0];
     stopColor=ptr2[480];

     ptr1++;
     ptr2++;

     colorSize=stopColor-startColor;
     hsize=stop-start;


     if(hsize>0)
     {
      colorStep=abs(colorSize)/hsize;

     color=startColor;

      hsize++;
      out2=start+out;
      for(j=0;j<hsize;j++)
      {
       *(out2++)=color;
       color+=colorStep;
      }
      drawBuffer1ToScreen();
     }
     out+=640; */
/*      var2=ptr1[480];
      var1=ptr1[0];

      var4=ptr2[480];
      var3=ptr2[0];

      if(var2==0)
        var2=var1;

      if(var3==0)
        var3=var4;

      drawLine(var2,vtop+vsize-renderLoop,var1,vtop+vsize-renderLoop,color);
      ptr1++;
      ptr2++;*/

      stop=ptr1[480]; // stop
      start=ptr1[0];   // start

      ptr1++;
      out2=start+out;
      hsize=stop-start;

      var2=stopColor=ptr2[480];
      var3=startColor=ptr2[0];

      if(hsize==0) // ok
      {
        stopColor+=startColor;
        ptr2++;
        stopColor>>=1;
        *out2=((stopColor&0xFF00)>>8);
      }
      else
      if(hsize>0)      // face dans le bon sens
      {
//        printf("startColor= %d, stopColor= %d\n",var3,var2);
        var2=stopColor-=startColor;
        if(stopColor<0) // ok
        {
          if(hsize<=2) // ok
          {
            var3=ptr2[480];
            var2=ptr2[0];
            ptr2++;
            *(out2+hsize)=(var3&0xFF00)>>8;
            if(--hsize)
            {
              var3+=var2;
              var3>>=1;
              *(out2+1)=(var3&0xFF00)>>8;
            }
            *(out2)=(var2&0xFF00)>>8;
          }
          else  // ok
          {
            var2=-var2;
            var3=0;
            var2/=hsize;
            var3=ptr2[0];
            ptr2++;
            hsize++;
            do
            {
              (*out2++)=var3;
              var3-=var2;
            }while(--hsize);
          }
        }
        else
        {
          if(hsize<=2)
          {
            var2=ptr2[480];
            ptr2++;
            *(out2+hsize)=(var2&0xFF00)>>8;
            if(--hsize)
            {
              var3+=var2;
              var3>>=1;
              *(out2+1)=(var3&0xFF00)>>8;
            }
            *(out2)=(var2&0xFF00)>>8;
          }
          else // le plus interessant, face visible + couleurs normales
          {
            var3=0;
            var2/=hsize;
            var3=ptr2[0];
            ptr2++;
            hsize++;
            do
            {
              (*out2++)=var3;
              var3+=var2;
            }while(--hsize);
          }
        }
      }
      out+=640;
    }while(--renderLoop);

    break;
   }
  default:
   {
      printf("Unsuported render type %d\n",polyRenderType);
      break;
    }
  };
}