#include "lba.h"

char magicLevelForceTable[]=
{
    2,
    3,
    4,
    6,
    8,
    0,
};

struct bodyHeaderStruct
{
	short int bodyFlag;	// 2
	short int unk0;		// 2
	short int unk1;		// 2
	short int unk2;		// 2
	short int unk3;		// 2
	short int unk4;		// 2
	short int unk5;		// 2
	short int offsetToData;	// 2
	char *ptrToKeyFrame;
	int keyFrameTime;
};

int setAnimAtKeyFrame(int index, unsigned char *anim, unsigned char *body)
{
    short int numOfIndexInAnim;
    short int numOfPointInAnim;
    char *ptrToData;
    char *ptrToDataBackup;
    char *ptrToBodyData;
    short int bodyHeader;
    short int numOfElementInBody;
    short int numOfPointInBody;
    int i;

    numOfIndexInAnim = READ_LE_S16(anim);

    if (index >= numOfIndexInAnim)
	return (numOfIndexInAnim);

    numOfPointInAnim = READ_LE_S16(anim + 2);

    ptrToData = (char *) ((numOfPointInAnim * 8 + 8) * index + anim + 8);

    bodyHeader = READ_LE_S16(body);

    if (!(bodyHeader & 2))
	return (0);

    ptrToBodyData = (char *) (body + 14);

	WRITE_LE_U32(ptrToBodyData + 2, (uint32)ptrToData);
    WRITE_LE_S32(ptrToBodyData + 6, time );

    ptrToBodyData = ptrToBodyData + READ_LE_S16(ptrToBodyData) + 2;

    numOfElementInBody = READ_LE_S16(ptrToBodyData);

    ptrToBodyData = ptrToBodyData + numOfElementInBody * 6 + 12;

    numOfPointInBody = READ_LE_S16(ptrToBodyData - 10);

    if (numOfPointInAnim > numOfPointInBody)
	{
	    numOfPointInAnim = numOfPointInBody;
	}

    ptrToDataBackup = ptrToData;

    ptrToData += 8;

    do
	{
	    for (i = 0; i < 8; i++)
		{
		    *(ptrToBodyData++) = *(ptrToData++);
		}

	    ptrToBodyData += 30;

	}
    while (--numOfPointInAnim);

    ptrToData = ptrToDataBackup + 2;

    currentX = READ_LE_S16(ptrToData);
    currentZ = READ_LE_S16(ptrToData + 2);
    currentY = READ_LE_S16(ptrToData + 4);

    processActorVar5 = READ_LE_S16(ptrToData + 6);
    processActorSub2Var0 = READ_LE_S16(ptrToData + 8);
    processActorVar6 = READ_LE_S16(ptrToData + 10);
    processActorSub2Var1 = READ_LE_S16(ptrToData + 12);

    return (1);
}

int GetNbFramesAnim(char *ptr)
{
    return (READ_LE_S16(ptr));
}

int GetBouclageAnim(char *ptr)
{
    return (READ_LE_S16(ptr + 4));
}

int SetInterAnimObjet2(int animState, char *animData, char *body)
{
    short int animOpcode;

    short int var0;

    char *edi;
    char *ebx;
    int ebp;
    int eax;
    int keyFrameLength;
    int numOfPointInBody;
    int numOfPointInAnim;
    char *keyFramePtrOld;

    numOfPointInAnim = READ_LE_U16(animData + 2);

    keyFramePtr = ((numOfPointInAnim * 8 + 8) * animState) + animData + 8;

    keyFrameLength = READ_LE_U16(keyFramePtr);

    var0 = READ_LE_U16(body);

    if (!(var0 & 2))
	{
	    return (0);
	}

    edi = body + 16;

    animVar1 = edi;

    ebx = (char*)READ_LE_U32(edi);
    ebp = READ_LE_U32(edi + 4);

    if (!ebx)
	{
	    ebx = keyFramePtr;
	    ebp = keyFrameLength;
	}
    else
    {
        assert_ptr(ebx);
    }

    lastKeyFramePtr = ebx;

    eax = READ_LE_S16(edi - 2);
    edi += eax;

    eax = READ_LE_S16(edi);
    eax = eax + eax * 2;
    edi = edi + eax * 2 + 12;

    numOfPointInBody = READ_LE_S16(edi - 10);

    if (numOfPointInAnim > numOfPointInBody)
	{
	    numOfPointInAnim = numOfPointInBody;
	}

    eax = time - ebp;

    if (eax >= keyFrameLength)
	{
	    int *destPtr;	// keyFrame
	    int *sourcePtr;

	    sourcePtr = (int *) (keyFramePtr + 8);
	    destPtr = (int *) edi;

	    do
		{
			WRITE_LE_U32( destPtr++, READ_LE_U32(sourcePtr++));
			WRITE_LE_U32( destPtr++, READ_LE_U32(sourcePtr++));

		    destPtr = (int *) (((char *) destPtr) + 30);

		}
	    while (--numOfPointInAnim);

		WRITE_LE_U32(animVar1, (uint32)keyFramePtr);
	    WRITE_LE_U32(animVar1 + 4, time);

	    currentX = READ_LE_S16(keyFramePtr + 2);
	    currentZ = READ_LE_S16(keyFramePtr + 4);
	    currentY = READ_LE_S16(keyFramePtr + 6);

	    processActorVar5 = READ_LE_S16(keyFramePtr + 8);
	    processActorSub2Var0 = READ_LE_S16(keyFramePtr + 10);
	    processActorVar6 = READ_LE_S16(keyFramePtr + 12);
	    processActorSub2Var1 = READ_LE_S16(keyFramePtr + 14);

	    return (1);
	}
    else
	{
	    keyFramePtrOld = keyFramePtr;

	    lastKeyFramePtr += 8;
	    keyFramePtr += 8;

	    processActorVar5 = READ_LE_S16(keyFramePtr);
	    processActorSub2Var0 = (READ_LE_S16(keyFramePtr + 2) * eax) / keyFrameLength;
	    processActorVar6 = (READ_LE_S16(keyFramePtr + 4) * eax) / keyFrameLength;
	    processActorSub2Var1 = (READ_LE_S16(keyFramePtr + 6) * eax) / keyFrameLength;

	    lastKeyFramePtr += 8;
	    keyFramePtr += 8;

	    edi += 38;

	    if (--numOfPointInAnim)
		{
		    animVar4 = numOfPointInAnim;

		    do
			{
			    animOpcode = PatchType(&edi);

			    switch (animOpcode)
				{
				case 0:	// allow global rotate
				   {
				       PatchInterAngle(&edi, eax, keyFrameLength);
				       PatchInterAngle(&edi, eax, keyFrameLength);
				       PatchInterAngle(&edi, eax, keyFrameLength);
				       break;
				   }
				case 1:	// dissallow global rotate
				   {
				       PatchInterStep(&edi, eax, keyFrameLength);
				       PatchInterStep(&edi, eax, keyFrameLength);
				       PatchInterStep(&edi, eax, keyFrameLength);
				       break;
				   }
				case 2:	// dissallow global rotate + hide
				   {
				       PatchInterStep(&edi, eax, keyFrameLength);
				       PatchInterStep(&edi, eax, keyFrameLength);
				       PatchInterStep(&edi, eax, keyFrameLength);
				       break;
				   }
				default:
				   {
				       printf("Unsupported rotaton mode %d in SetInterAnimObjet!\n",
					      animOpcode);
				       exit(1);
				   }
				}

			    edi += 30;
			}
		    while (--animVar4);
		}

	    currentX = ( READ_LE_S16(keyFramePtrOld + 2) * eax) / keyFrameLength;
	    currentZ = ( READ_LE_S16(keyFramePtrOld + 4) * eax) / keyFrameLength;
	    currentY = ( READ_LE_S16(keyFramePtrOld + 6) * eax) / keyFrameLength;
	}

    return (0);
}

void loadGfxSub(unsigned char *bodyPtr)
{
    bodyHeaderStruct *bodyHeader;
    short int offsetToData;
    unsigned char *bodyDataPtr;
    short int numOfElement1;
    short int numOfPoint;
    unsigned char *ptrToKeyData;
    int i;
    int bp = 36;
    int bx = 38;

    bodyHeader = (bodyHeaderStruct *) bodyPtr;

    if (!(bodyHeader->bodyFlag & 2))	// no animation applicable
	{
	    return;
	}

    offsetToData = bodyHeader->offsetToData;

    bodyDataPtr = bodyPtr + offsetToData + 16;

    numOfElement1 = READ_LE_S16(bodyDataPtr);
    unsigned char *ptr2 = bodyDataPtr + 2 + numOfElement1 * 6;

    numOfPoint = READ_LE_S16(ptr2);

    ptrToKeyData = ptr2 + 2;

    for (i = 0; i < numOfPoint; i++)
	{
	    ptrToKeyData += 38;
	    WRITE_LE_S16(ptrToKeyData + 6,(READ_LE_S16(ptrToKeyData + 6) * bp) / bx);
	}
}

int SetInterAnimObjet(int animState, char *animData, char *body)
{
    short int animOpcode;

    short int var0;

    char *edi;
    char *ebx;
    int ebp;
    int eax;
    int keyFrameLength;
    int numOfPointInBody;
    int numOfPointInAnim;
    char *keyFramePtrOld;

    numOfPointInAnim = READ_LE_S16(animData + 2);

    keyFramePtr = ((numOfPointInAnim * 8 + 8) * animState) + animData + 8;

    keyFrameLength = READ_LE_S16(keyFramePtr);

    var0 = READ_LE_S16(body);

    if (!(var0 & 2))
	{
	    return (0);
	}

    edi = body + 16;

    animVar1 = edi;

    ebx = (char*)READ_LE_U32(edi);
    ebp = READ_LE_U32(edi + 4);

    if (!ebx)
	{
	    ebx = keyFramePtr;
	    ebp = keyFrameLength;
	}

    lastKeyFramePtr = ebx;

    eax = READ_LE_S16(edi - 2);
    edi += eax;

    eax = READ_LE_S16(edi);
    eax = eax + eax * 2;
    edi = edi + eax * 2 + 12;

    numOfPointInBody = READ_LE_S16(edi - 10);

    if (numOfPointInAnim > numOfPointInBody)
	{
	    numOfPointInAnim = numOfPointInBody;
	}

    eax = time - ebp;

    if (eax >= keyFrameLength)
	{
	    int *destPtr;	// keyFrame
	    int *sourcePtr;

	    sourcePtr = (int *) (keyFramePtr + 8);
	    destPtr = (int *) edi;

	    do
		{
			WRITE_LE_U32( destPtr++, READ_LE_U32(sourcePtr++));
			WRITE_LE_U32( destPtr++, READ_LE_U32(sourcePtr++));

		    destPtr = (int *) (((char *) destPtr) + 30);

		}
	    while (--numOfPointInAnim);

	    WRITE_LE_U32(animVar1, (uint32)keyFramePtr);
	    WRITE_LE_S32(animVar1 + 4, time);

	    currentX = READ_LE_S16(keyFramePtr + 2);
	    currentZ = READ_LE_S16(keyFramePtr + 4);
	    currentY = READ_LE_S16(keyFramePtr + 6);

	    processActorVar5 = READ_LE_S16(keyFramePtr + 8);
	    processActorSub2Var0 = READ_LE_S16(keyFramePtr + 10);
	    processActorVar6 = READ_LE_S16(keyFramePtr + 12);
	    processActorSub2Var1 = READ_LE_S16(keyFramePtr + 14);

	    return (1);
	}
    else
	{
	    keyFramePtrOld = keyFramePtr;

	    lastKeyFramePtr += 8;
	    keyFramePtr += 8;

	    processActorVar5 = READ_LE_S16(keyFramePtr);
	    processActorSub2Var0 = (READ_LE_S16(keyFramePtr + 2) * eax) / keyFrameLength;
	    processActorVar6 = (READ_LE_S16(keyFramePtr + 4) * eax) / keyFrameLength;
	    processActorSub2Var1 = (READ_LE_S16(keyFramePtr + 6) * eax) / keyFrameLength;

	    lastKeyFramePtr += 8;
	    keyFramePtr += 8;

	    edi += 38;

	    if (--numOfPointInAnim)
		{
		    animVar4 = numOfPointInAnim;

		    do
			{
			    animOpcode = PatchType(&edi);

				assert( animOpcode >= 0 && animOpcode <= 2);

			    switch (animOpcode)
				{
				case 0:	// anim angle
				   {
				       PatchInterAngle(&edi, eax, keyFrameLength);
				       PatchInterAngle(&edi, eax, keyFrameLength);
				       PatchInterAngle(&edi, eax, keyFrameLength);
				       break;
				   }
				case 1:	// anim step
				   {
				       PatchInterStep(&edi, eax, keyFrameLength);
				       PatchInterStep(&edi, eax, keyFrameLength);
				       PatchInterStep(&edi, eax, keyFrameLength);
				       break;
				   }
				case 2:	// anim step
				   {
				       PatchInterStep(&edi, eax, keyFrameLength);
				       PatchInterStep(&edi, eax, keyFrameLength);
				       PatchInterStep(&edi, eax, keyFrameLength);
				       break;
				   }
				}

			    edi += 30;
			}
		    while (--animVar4);
		}

	    currentX = ( READ_LE_S16(keyFramePtrOld + 2) * eax) / keyFrameLength;
	    currentZ = ( READ_LE_S16(keyFramePtrOld + 4) * eax) / keyFrameLength;
	    currentY = ( READ_LE_S16(keyFramePtrOld + 6) * eax) / keyFrameLength;
	}

    return (0);
}

int getAnimIndexForBody(byte anim, short int actorNumber)
{
    actor *act;
    char type;

    unsigned char *bodyPtr;
    unsigned char *ptr, *ptr2;
    unsigned char *costumePtr = NULL;
    unsigned short int var1;

    act = &actors[actorNumber];
    bodyPtr = act->entityDataPtr;

    do
	{
	    type = *(bodyPtr++);

	    if (type == -1)
		{
		    loadTwinsenCostumesVar1 = NULL;
		    return (-1);
		}

	    ptr = (bodyPtr + 1);

	    if (type == 3)
		{
		    if (anim == *bodyPtr)
			{
			    ptr++;
			    var1 = READ_LE_S16(ptr);
			    ptr += 2;
			    ptr2 = ptr;
			    ptr++;
			    if (*ptr2 != 0)
				{
				    costumePtr = ptr - 1;
				}
			    loadTwinsenCostumesVar1 = costumePtr;
			    return (var1);
			}
		}

	    bodyPtr = *ptr + ptr;

	}
    while (1);

    return (0);
}

int InitAnim(char newAnim, short int arg_4, unsigned char arg_8, short int actorNum)
{
    actor *lactor;
    int animIndex;

    lactor = &actors[actorNum];

    if (lactor->costumeIndex == -1)
	    return (0);

    if (lactor->staticFlagsBF.bIsSpriteActor)	// si c'est un sprite
	    return (0);

    if (newAnim == lactor->anim && lactor->previousAnimIndex != -1)	// le costume est deja loade
	    return (1);

    if (arg_8 == 255 && lactor->field_78 != 2)
	    arg_8 = lactor->anim;

    animIndex = getAnimIndexForBody(newAnim, actorNum);

    if (animIndex == -1)
	    animIndex = getAnimIndexForBody(0, actorNum);

    if (arg_4 != 4 && lactor->field_78 == 2)
	{
	    lactor->field_2 = newAnim;
	    return (0);
	}

    if (arg_4 == 3)
	{
	    arg_4 = 2;

	    arg_8 = lactor->anim;

	    if (arg_8 == 15 || arg_8 == 7 || arg_8 == 8 || arg_8 == 9)
		{
		    arg_8 = 0;
		}

	}

    if (arg_4 == 4)
	    arg_4 = 2;

    if (lactor->previousAnimIndex == -1)	// if no previous animation
	{
	    setAnimAtKeyFrame(0, HQR_Get(HQR_Anims, animIndex), bodyPtrTab[lactor->costumeIndex]);	// set animation directly to first keyFrame
	}
    else // interpolation between animations
	{
	    bufAni2 += StockInterAnim((char *) bufAni2, (char *) bodyPtrTab[lactor->costumeIndex]);
	    if (bufAni1 + 4488 > bufAni2)
		    bufAni2 = bufAni1;
	}

    lactor->previousAnimIndex = animIndex;
    lactor->anim = newAnim;
    lactor->field_2 = arg_8;
    lactor->animExtraData = loadTwinsenCostumesVar1;
    lactor->field_78 = arg_4;
    lactor->animPosition = 0;
	lactor->dynamicFlagsBF.bUnk0002 = 0;
	lactor->dynamicFlagsBF.bUnk0004 = 0;
	lactor->dynamicFlagsBF.bUnk0008 = 1;

    if (lactor->animExtraData)
	{
	    GereAnimAction(lactor, actorNum);
	}

    lactor->lastRotationSpeed = 0;
    lactor->lastX = 0;
	lactor->lastY = 0;
    lactor->lastZ = 0;

    return (1);
}

int StockInterAnim(char *lBufAnim, char *lBody)	// copy the next keyFrame from a different buffer
{
    int temp;
    char *ptr;
    int *edi;
    int *esi;
    int var0;
    int var1;
    int var2;
    int counter;

    assert_ptr(lBufAnim);
    assert_ptr(lBody);

    temp = READ_LE_S16(lBody);

    if (temp & 2)
	{
	    ptr = (lBody + 0x10);

        todo("remove hack to prevent time warp in anim");

        if(time1)
	        WRITE_LE_S32(ptr + 4, time3);
        else
            WRITE_LE_S32(ptr + 4, time);

		WRITE_LE_U32(ptr,(uint32)lBufAnim);

	    var0 = READ_LE_S16(ptr - 2);
	    ptr = ptr + var0;

	    var1 = READ_LE_S16(ptr);
	    var1 = var1 + var1 * 2;

	    ptr = ptr + var1 * 2 + 2;

	    var2 = READ_LE_S16(ptr);
	    counter = var2;
	    var2 = (var2 * 8) + 8;

	    edi = (int *) (lBufAnim + 8);
	    esi = (int *) (ptr + 10);

	    do
		{
			WRITE_LE_U32(edi++, READ_LE_U32(esi++));
			WRITE_LE_U32(edi++, READ_LE_U32(esi++));

		    esi = (int *) (((char *) esi) + 30);

		}
	    while (--counter);

	    return (var2);

	}

    return (0);
}

void GereAnimAction(actor * lactor, int actorNum)
{
    char *ebx;
    int var_4;
    int var_18;
    char al;
    int var_54;
    char *edi;
    char *var_48;
    char *var_50;
    char *var_44;
    char *var_40;
    char *var_3C;
    char *var_4C;
    char *var_38;
    char *var_34;
    char *var_30;
    char *edx;
    char *eax;
    short int temp;

    ebx = (char *) lactor->animExtraData;

    var_4 = 0;
    var_18 = *(ebx++);

    while (var_4 < var_18)
	{
	    al = *(ebx++);

	    al -= 5;
	    if (al > 16)
		return;

	    var_54 = al;

	    edi = ebx + 1;

	    var_48 = edi + 2;
	    var_50 = var_48 + 2;
	    var_44 = var_50 + 2;
	    var_40 = var_44 + 2;
	    var_3C = var_40 + 2;
	    var_4C = var_3C + 2;
	    var_38 = var_4C + 2;

	    var_34 = var_38 + 1;
	    var_30 = var_34 + 1;

	    edx = edi + 11;
	    eax = edi + 15;

//        printf("Opcode: %d\n",var_54);

        switch (var_54)
		{
		case 0: // attacking !
            {
		        temp = *(ebx++);
		        temp--;
		        if (temp == lactor->animPosition)
			    {
			        lactor->field_66 = *(ebx++);
					lactor->dynamicFlagsBF.bUnk0002 = 1;
			    }
		        else
			    {
			        ebx++;
			    }
		        break;
            }
		case 1:
            {
		        temp = *(ebx++);

		        if (temp == lactor->animPosition)
			    {
			        HQ_3D_MixSample(READ_LE_S16(ebx), 0x1000, 1, lactor->X, lactor->Z, lactor->Y);
			    }

		        ebx += 2;
		        break;
            }
		case 2:
		    temp = *(ebx++);
		    if (temp == lactor->animPosition)
			{
                int temp_28;
                int temp_1C;

                temp_28 = READ_LE_S16(ebx);
                ebx+=2;

                temp_1C = READ_LE_S16(ebx);
                ebx+=2;
                
                HQ_3D_MixSample(temp_28, (rand()%temp_1C) + 0x1000 - (abs(temp_1C)>>1), 1, lactor->X, lactor->Z, lactor->Y);
			}
		    else
			{
			    ebx += 4;
			}
		    break;
        case 3:
            temp = *(ebx++);
		    if (temp == lactor->animPosition)
			{
                int temp_8;
                int temp_C;
                int temp_24;
                int temp_14;
                int cx;
                int dx;

                temp_8 = READ_LE_S16(ebx);
                ebx+=2;

                temp_C = *(ebx++);

                cx = READ_LE_S16(ebx);
                ebx+=2;

                dx = lactor->angle + READ_LE_S16(ebx);
                ebx+=2;

                temp_24 = READ_LE_S16(ebx);
                ebx+=2;

                temp_14 = *(ebx++);

                ThrowExtra(actorNum, lactor->X, lactor->Z + temp_8, lactor->Y, temp_C, cx, dx, temp_24, temp_14, *(ebx++));
			}
		    else
			{
			    ebx += 11;
			}
		    break;
        case 4:
            if(magicBallIdx==-1)
            {
                temp = *(ebx++);
                if( temp == lactor->animPosition)
                {
                    int var_8;
                    int dx;
                    int var_24;
                    int var_14;

                    var_8 = READ_LE_S16(ebx);
                    ebx +=2;
                    dx = READ_LE_S16(ebx);
                    ebx +=2;
                    var_24 = READ_LE_S16(ebx);
                    ebx +=2;
                    var_14 = *(ebx++);

                    ThrowMagicBall(lactor->X, lactor->Z + var_8, lactor->Y, dx, lactor->angle, var_24, var_14);
                }
                else
                {
                    ebx += 7;
                }

            }
            else
            {
                ebx += 8;
            }
            break;
		case 5:
		    temp = *(ebx++);
		    if (temp == lactor->animPosition)
			{
			    int dx;
			    int cx;

			    dx = READ_LE_S16(ebx);
			    ebx += 2;
			    cx = READ_LE_S16(ebx);
			    ebx += 2;

			    HQ_3D_MixSample(dx, 0x1000, cx, lactor->X, lactor->Z, lactor->Y);
			}
		    else
			{
			    ebx += 4;
			}
		    break;
		case 6: // shoot auto aiming missile
            temp = *(ebx++);
            if (temp == lactor->animPosition)
            {
                int var_8;
                int var_C;
                int dx;
                int var_24;
                int temp;

                var_8 = READ_LE_S16(ebx);
                ebx += 2;
                var_C = *(ebx++);
                dx = *(ebx++);
                var_24 = READ_LE_S16(ebx);
                ebx += 2;
                temp = *(ebx++);

                ExtraSearch(actorNum, lactor->X, lactor->Z + var_8, lactor->Y, var_C, dx, var_24, temp);
            }
            else
            {
                ebx += 6;
            }
		    break;
		case 7: // shoot with angle offset
            temp = *(ebx);
            ebx = edi;
            if(temp == lactor->animPosition)
            {
                int distance;
                int angle;
                int var_8;
                int var_C;
                int dx;
                int cx;
                int var_24;
                int var_14;
                int temp;

                distance = Distance2D(lactor->X,lactor->Y,twinsen->X,twinsen->Y);
                angle = GetAngle(lactor->Z, 0, twinsen->Z, distance);

                var_8 = READ_LE_S16(edi);
                ebx = edi+2;
                var_C =*(ebx++);
                dx = READ_LE_S16(ebx);
                ebx += 2;
                cx = lactor->angle + READ_LE_S16(ebx);
                ebx += 2;
                var_24 = READ_LE_S16(ebx);
                ebx +=2;
                var_14 = *(ebx++);
                temp = *(ebx++);

                ThrowExtra(actorNum,lactor->X, lactor->Z + var_8, lactor->Y, var_C, dx, cx, var_24, var_14, temp);
            }
            else
            {
                ebx = edi + 11;
            }
		    break;
		case 8:
            temp = *(ebx);
            if(temp == lactor->animPosition)
            {
                // TODO: implement
                //playSound7(READ_LE_S16(edi));
                ebx = var_48;
            }
            else
            {
                ebx = var_48;
            }
		    break;
		case 10:
		    ebx = edi;
		    printf("Skipping GereAnimAction opcode 10 (sound)\n");
		    break;
		case 11:
		    ebx = edi;
		    printf("Skipping GereAnimAction opcode 11 (sound)\n");
		    break;
        case 12: // twinsen attacking
		    temp = *(ebx++);
		    temp--;
            ebx = edi;
		    if (temp == lactor->animPosition)
			{
			    lactor->field_66 = magicLevelForceTable[magicLevel];
				lactor->dynamicFlagsBF.bUnk0002 = 1;
			}
		    break;
        case 13:
            ebx= eax;
            printf("Skipping GereAnimAction opcode 13 (Send projectile)\n");
            break;
        case 14: // shoot at twinsen
            if(*ebx == lactor->animPosition)
            {
                int newAngle;
                int var20;
                int varC;
                int var10;
                int var24;
                int var14;
                int di;

                newAngle = GetAngle(lactor->Z, 0, twinsen->Z, Distance2D(lactor->X, lactor->Y, twinsen->X, twinsen->Y));
                var20 = READ_LE_S16(edi);
                di = READ_LE_S16(var_48);
                Rotate(var20,READ_LE_S16(var_50),lactor->angle);

                var20 = destX + lactor->X;

                varC = *var_44;
                var10 = READ_LE_S16(var_40) + newAngle;

                var24=READ_LE_S16(var_4C);
                var14=*var_38;

                ThrowExtra(actorNum, var20, di + lactor->Z, destZ + lactor->Y, varC, var10, lactor->angle + *(short int*)var_3C, var24, var14, *var_34);
                ebx = edx;
            }
            else
            {
                ebx = edx;
            }
            break;
        case 15:
            ebx = edx;
            printf("Skipping GereAnimAction opcode 15 (Send projectile)\n");
            break;
		default:
            printf("Unsupported opcode %d in GereAnimAction\n", var_54);
		    exit(1);
		}

	    var_4++;
	}

}

int PatchType(char **ptr)
{
    short int *lptr;
    short int opcode;

    lptr = (short int *) *ptr;

    opcode = READ_LE_S16(keyFramePtr);
	WRITE_LE_S16( lptr, opcode );

    keyFramePtr += 2;
    *(ptr) = *(ptr) + 2;
    lastKeyFramePtr += 2;

    return (opcode);
}

void PatchInterAngle(char **ptr, int bp, int bx)
{
    short int *dest;
    short int lastAngle;
    short int newAngle;
    short int angleDif;
    short int computedAngle;

    lastAngle = READ_LE_S16(lastKeyFramePtr);
    lastKeyFramePtr += 2;

    newAngle = READ_LE_S16(keyFramePtr);
    keyFramePtr += 2;

    lastAngle &= 0x3FF;
    newAngle &= 0x3FF;

    angleDif = newAngle - lastAngle;

    if (angleDif)
	{
	    if (angleDif < -0x200)
		{
		    angleDif += 0x400;
		}
	    else if (angleDif > 0x200)
		{
		    angleDif -= 0x400;
		}

	    computedAngle = lastAngle + (angleDif * bp) / bx;
	}
    else
	{
	    computedAngle = lastAngle;
	}

    dest = (short int *) *(ptr);

	WRITE_LE_S16( dest, computedAngle & 0x3FF);

    *(ptr) = *(ptr) + 2;
}

void PatchInterStep(char **ptr, int bp, int bx)
{
    short int *dest;
    short int lastAngle;
    short int newAngle;
    short int angleDif;
    short int computedAngle;

    lastAngle = *(short int *) lastKeyFramePtr;
    lastKeyFramePtr += 2;

    newAngle = *(short int *) keyFramePtr;
    keyFramePtr += 2;

    angleDif = newAngle - lastAngle;

    if (angleDif)
	{
	    computedAngle = lastAngle + (angleDif * bp) / bx;
	}
    else
	{
	    computedAngle = lastAngle;
	}

    dest = (short int *) *(ptr);

    *dest = computedAngle;

    *(ptr) = *(ptr) + 2;
}
