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
    unsigned char *out, *out2;
    short int *ptr1, *ptr2;
    int vsize, hsize;
    int color;
    int eax;
    int i, j;
    short int var2, var3;

   // char borrow;
    short int start, stop;
    short int startColor, stopColor;

    float varf2;
    float varf3;
    float varf4;

    if (vtop <= 0 || vbottom <= 0)
	return;
    if (vleft <= 0 || vright <= 0)
	return;
    if (vleft >= 640)
	return;
   // if(vright>=640)
   // return;
    if (vtop >= 480 || vbottom >= 480)
	return;

    out = videoBuffer1 + screenLockupTable[vtop];

    ptr1 = &polyTab[vtop];
    ptr2 = &polyTab2[vtop];

    vsize = vbottom - vtop;
    vsize++;

    color = edi;

   // osystem->drawBufferToScreen(videoBuffer1);

    switch (ecx)
	{
	case 0:		// flat polygone
	   {
	       eax =
		   (color & 0xFF) | ((color & 0xFF) << 8) | ((color & 0xFF) << 16) | ((color & 0xFF)
										      << 24);
	       for (i = 0; i < vsize; i++)
		   {
		       stop = ptr1[480];
		       start = ptr1[0];

		       ptr1++;
		       hsize = stop - start;

		       if (hsize > 0)
			   {
			       hsize++;
			       out2 = start + out;
			       for (j = 0; j < hsize; j++)
				   {
				       *(out2++) = color;
				   }
			   }
		       out += 640;
		   }
	       break;
	   }
	case 7:		// main codec
	case 8:
	   {
	       renderLoop = vsize;
	       do
		   {
		       stop = ptr1[480];	// stop
		       start = ptr1[0];	// start

		       ptr1++;
		       out2 = start + out;
		       hsize = stop - start;

		       var2 = stopColor = ptr2[480];
		       var3 = startColor = ptr2[0];

		       ptr2++;

		       varf2 = var2;
		       varf3 = var3;

		       varf4 = varf2 - varf3;

		       if (hsize == 0)
			   {
			       *out2 = (unsigned char) ((varf3 + varf2) / 2);	// moyenne des 2 couleurs
			   }
		       else if (hsize > 0)
			   {
			       if (hsize == 1)
				   {
				       *(out2 + 1) = (char) varf2;
				       *(out2) = (char) varf3;
				   }
			       else if (hsize == 2)
				   {
				       *(out2 + 2) = (char) varf2;
				       *(out2 + 1) = (char) ((varf2 + varf3) / 2);
				       *(out2) = (char) varf3;
				   }
			       else
				   {
				       varf4 /= hsize;

				       do
					   {
					       *(out2++) = (char) varf3;
					       varf3 += varf4;
					   }
				       while (--hsize);
				   }
			   }

		      /*
		       * if(hsize==0) // ok { stopColor+=startColor; ptr2++; stopColor>>=1;
		       * *out2=((stopColor&0xFF00)>>8); } else if(hsize>0) // face dans le bon sens {
		       * var2=stopColor-=startColor; //largeur de la bande varf2=var2; if(stopColor<0) //
		       * ok { if(hsize<=2) // ok { varf3=ptr2[480]; varf2=ptr2[0]; ptr2++;
		       * *(out2+hsize)=(char)varf3; if(--hsize) { varf3+=varf2; varf3/=2;
		       * *(out2+1)=(char)varf3; } *out2=(char)varf2; } else // ok { varf2=-varf2; varf3=0;
		       * varf2/=hsize; varf3=ptr2[0]; ptr2++; hsize++; do { (*out2++)=(char)varf3;
		       * varf3-=varf2; }while(--hsize); } } else { if(hsize<=2) { var2=ptr2[480]; ptr2++;
		       * *(out2+hsize)=var2; if(--hsize) { var3+=var2; var3>>=1; *(out2+1)=var3; }
		       * *(out2)=var2; } else // le plus interessant, face visible + couleurs normales {
		       * var3=0; //var2/=hsize; //largeur de la bande
		       * 
		       * varf2=var2; varf2/=(hsize);
		       * 
		       * varf3=ptr2[0];
		       * 
		       * ptr2++; hsize++; do { (*out2++)=(char)varf3; varf3+=varf2; }while(--hsize); } } } 
		       */
		       out += 640;
		   }
	       while (--renderLoop);

	       break;
	   }
	default:
	   {
	      // printf("Unsuported render type %d\n",polyRenderType);
	       break;
	   }
	};
}
