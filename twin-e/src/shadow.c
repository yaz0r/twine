#include "lba.h"

void GetShadow(int X, int Z, int Y)
{
    int tempX;
    int tempZ;
    int tempY;
    unsigned char* ptr;

    tempX = (X+0x100)>>9;
    tempZ = Z>>8;
    tempY = (Y+0x100)>>9;

    ptr = bufCube + tempZ*2 + tempX * 25 * 2 + (tempY<<6)*25*2;

    while(tempZ) // search down until either ground is found or lower border of the cube is reached
    {
        if(*(short int*)ptr) // found the ground
            break;

        tempZ--;
        ptr-=2;
    }

    shadowVar = 0;

    getPosVar1 = tempX;
    getPosVar2 = tempZ;
    getPosVar3 = tempY;

    processActorX = X;
    processActorZ = (tempZ+1)<<8;
    processActorY = Y;

    if(*ptr)
    {
        unsigned char* tempPtr;

        tempPtr = zbufferSub1(*(ptr++)-1) + 3;

        shadowVar = *(tempPtr + *(ptr)*4);

        ReajustPos(shadowVar);
    }

    shadowX = processActorX;
    shadowZ = processActorZ;
    shadowY = processActorY;

}
