#include "lba.h"

void LBA_engine::allocBufCube(void)
{
   // todo: mettre les codes d'erreur

    bufCube = (byte *) malloc(204800);	// 204800 = 64*64*25*2
    bufferBrick = (byte *) malloc(361472);
}

byte *LBA_engine::allocateBuf(int size)
{
    byte *ptr;

   // normalement, on a tout un code en fonction du DPMI pour la gestion des pages...
   // C'est maintenant completement obsolete

    ptr = (byte *) malloc(size);

    if (!ptr)
	{
	    printf("Can't alloc %d!\n", size);
	    exit(1);
	}

    return (ptr);
}

int LBA_engine::allocHQRMemory(int dataSize, byte ** ptr)
{
   /*
    * byte *temp1;
    * 
    * temp1=HQMemory2;
    * 
    * if(!HQMemory) { *ptr=0; HQMemory2=temp1; return(0); }
    * 
    * if(dataSize>(HQMemorySize+12)) { *ptr=0; HQMemory2=temp1; return(0); }
    * 
    * *ptr=(HQMemory2+12);
    * 
    * *(int*)HQMemory2=0x12345678; *(int*)(HQMemory2+1)=dataSize; *(byte**)(HQMemory2+2)=*ptr;
    * 
    * HQMemorySize2=dataSize+12;
    * 
    * HQMemory2=HQMemory2+dataSize+12;
    */

    *ptr = (byte *) malloc(dataSize + 5000);

    return (1);
}
