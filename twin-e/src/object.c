#include "lba.h"

void GiveExtraBonus(actor * lactor)
{
    int i;
    char extraTable[8];
    int numOfExtra = 0;
    char currentBonus;
    int angle;

    for(i=0;i<5;i++)
    {
        if(lactor->field_10&(1<<(i+4)))
        {
            extraTable[numOfExtra++]= i;
        }
    }

    if(numOfExtra)
    {
        currentBonus = extraTable[rand()%numOfExtra];

        currentBonus += 3;

        if(!magicLevel && currentBonus ==2) // if bonus is magic and magicLevel 1 not reached
            currentBonus = 1; //bonus is life
        
        if( lactor->dynamicFlagsMask & 0x20)
        {
            ExtraBonus(lactor->X, lactor->Z, lactor->Y, 0x100, 0, currentBonus, lactor->field_12);
            HQ_3D_MixSample(11, 0x1000, 1, lactor->X, lactor->Z, lactor->Y);
        }
        else
        {
            angle = GetAngle(lactor->X, lactor->Y, twinsen->X, twinsen->Y);
            ExtraBonus(lactor->X, lactor->Z + lactor->boudingBox.Y.topRight, lactor->Y, 200, angle, currentBonus, lactor->field_12);
            HQ_3D_MixSample(11, 0x1000, 1, lactor->X, lactor->Z + lactor->boudingBox.Y.topRight, lactor->Y);
        }
    }

}
