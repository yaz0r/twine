#include "lba.h"

struct bodyHeaderStruct
{
	short int bodyFlag; // 2
	short int unk0; //2
	short int unk1; //2
	short int unk2; //2
	short int unk3; //2
	short int unk4; //2
	short int unk5; //2
	short int offsetToData; //2
	char* ptrToKeyFrame;
	int keyFrameTime;
};

int
LBA_engine::setAnimAtKeyFrame (int index, unsigned char *anim,
			       unsigned char *body)
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

  numOfIndexInAnim = *(short int *) anim;

  if (index >= numOfIndexInAnim)
    return (numOfIndexInAnim);

  numOfPointInAnim = *(short int *) (anim + 2);

  ptrToData = (char *) ((numOfPointInAnim * 8 + 8) * index + anim + 8);

  bodyHeader = *(short int *) body;

  if (!(bodyHeader & 2))
    return (0);

  ptrToBodyData = (char *) (body + 14);

  *(char **) (ptrToBodyData + 2) = ptrToData;
  *(int *) (ptrToBodyData + 6) = time;

  ptrToBodyData = ptrToBodyData + *(short int *) ptrToBodyData + 2;

  numOfElementInBody = *(short int *) ptrToBodyData;

  ptrToBodyData = ptrToBodyData + numOfElementInBody * 6 + 12;

  numOfPointInBody = *(short int *) (ptrToBodyData - 10);

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

  currentX = *(short int *) ptrToData;
  currentZ = *(short int *) (ptrToData + 2);
  currentY = *(short int *) (ptrToData + 4);

  processActorVar5 = *(short int *) (ptrToData + 6);
  processActorSub2Var0 = *(short int *) (ptrToData + 8);
  processActorVar6 = *(short int *) (ptrToData + 10);
  processActorSub2Var1 = *(short int *) (ptrToData + 12);

  return (1);
}

int
LBA_engine::applyAnim (int animState, char *animData, char *body)
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

  numOfPointInAnim = *(short int *) (animData + 2);

  keyFramePtr = ((numOfPointInAnim * 8 + 8) * animState) + animData + 8;

  keyFrameLength = *(short int *) keyFramePtr;

  var0 = *(short int *) body;

  if (!(var0 & 2))
    {
      return (0);
    }

  edi = body + 16;

  animVar1 = edi;

  ebx = *(char **) edi;
  ebp = *(int *) (edi + 4);

  if (!ebx)
    {
      ebx = keyFramePtr;
      ebp = keyFrameLength;
    }

  lastKeyFramePtr = ebx;

  eax = *(short int *) (edi - 2);
  edi += eax;

  eax = *(short int *) (edi);
  eax = eax + eax * 2;
  edi = edi + eax * 2 + 12;

  numOfPointInBody = *(short int *) (edi - 10);

  if (numOfPointInAnim > numOfPointInBody)
    {
      numOfPointInAnim = numOfPointInBody;
    }

  eax = time - ebp;

  if (eax >= keyFrameLength)
    {
      int *destPtr;		//keyFrame
      int *sourcePtr;

      sourcePtr = (int *) (keyFramePtr + 8);
      destPtr = (int *) edi;

      do
	{
	  *(destPtr++) = *(sourcePtr++);
	  *(destPtr++) = *(sourcePtr++);

	  destPtr = (int *) (((char *) destPtr) + 30);

	}
      while (--numOfPointInAnim);

      *(char **) animVar1 = keyFramePtr;
      *(int *) (animVar1 + 4) = time;

      currentX = *(short int *) (keyFramePtr + 2);
      currentZ = *(short int *) (keyFramePtr + 4);
      currentY = *(short int *) (keyFramePtr + 6);

      processActorVar5 = *(short int *) (keyFramePtr + 8);
      processActorSub2Var0 = *(short int *) (keyFramePtr + 10);
      processActorVar6 = *(short int *) (keyFramePtr + 12);
      processActorSub2Var1 = *(short int *) (keyFramePtr + 14);

      return (1);
    }
  else
    {
      keyFramePtrOld = keyFramePtr;

      lastKeyFramePtr += 8;
      keyFramePtr += 8;

      processActorVar5 = *(short int *) keyFramePtr;
      processActorSub2Var0 =
	(*(short int *) (keyFramePtr + 2) * eax) / keyFrameLength;
      processActorVar6 =
	(*(short int *) (keyFramePtr + 4) * eax) / keyFrameLength;
      processActorSub2Var1 =
	(*(short int *) (keyFramePtr + 6) * eax) / keyFrameLength;

      lastKeyFramePtr += 8;
      keyFramePtr += 8;

      edi += 38;

      if (--numOfPointInAnim)
	{
	  animVar4 = numOfPointInAnim;

	  do
	    {
	      animOpcode = getAnimOpcode (&edi);

	      switch (animOpcode)
		{
		case 0:	//allow global rotate
		  {
		    applyAnimMode0 (&edi, eax, keyFrameLength);
		    applyAnimMode0 (&edi, eax, keyFrameLength);
		    applyAnimMode0 (&edi, eax, keyFrameLength);
		    break;
		  }
		case 1:	//dissallow global rotate
		  {
		    applyAnimMode1 (&edi, eax, keyFrameLength);
		    applyAnimMode1 (&edi, eax, keyFrameLength);
		    applyAnimMode1 (&edi, eax, keyFrameLength);
		    break;
		  }
		case 2:	//dissallow global rotate + hide
		  {
		    applyAnimMode1 (&edi, eax, keyFrameLength);
		    applyAnimMode1 (&edi, eax, keyFrameLength);
		    applyAnimMode1 (&edi, eax, keyFrameLength);
		    break;
		  }
		default:
		  {
		    printf ("Unsupported rotaton mode %d in draw3D1!\n",
			    animOpcode);
		    exit (1);
		  }
		}

	      edi += 30;
	    }
	  while (--animVar4);
	}

      currentX = (*(short int *) (keyFramePtrOld + 2) * eax) / keyFrameLength;
      currentZ = (*(short int *) (keyFramePtrOld + 4) * eax) / keyFrameLength;
      currentY = (*(short int *) (keyFramePtrOld + 6) * eax) / keyFrameLength;
    }

  return (0);
}

void
LBA_engine::loadGfxSub (unsigned char *bodyPtr)
{
	bodyHeaderStruct *bodyHeader;
	short int offsetToData;
	unsigned char* bodyDataPtr;
	short int numOfElement1;
	short int numOfPoint;
	unsigned char* ptrToKeyData;
	int i;
	int bp=36;
	int bx=38;

	bodyHeader=(bodyHeaderStruct*)bodyPtr;

	if(!(bodyHeader->bodyFlag&2)) // no animation applicable
	{
		return;
	}

	offsetToData=bodyHeader->offsetToData;

	bodyDataPtr= bodyPtr+offsetToData+16;

	numOfElement1=*(short int*)bodyDataPtr;
	unsigned char *ptr2=bodyDataPtr+2+numOfElement1*6;

	numOfPoint=*(short int*)ptr2;

	ptrToKeyData=ptr2+2;

	for(i=0;i<numOfPoint;i++)
	{
		ptrToKeyData+=38;
		*(short int*)(ptrToKeyData+6) = (*(short int*)(ptrToKeyData+6)*bp)/bx;
	}
}

int
LBA_engine::draw3D1 (int animState, char *animData, char *body)
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

  //return(0);

  numOfPointInAnim = *(short int *) (animData + 2);

  keyFramePtr = ((numOfPointInAnim * 8 + 8) * animState) + animData + 8;

  keyFrameLength = *(short int *) keyFramePtr;

  var0 = *(short int *) body;

  if (!(var0 & 2))
    {
      return (0);
    }

  edi = body + 16;

  animVar1 = edi;

  ebx = *(char **) edi;
  ebp = *(int *) (edi + 4);

  if (!ebx)
    {
      ebx = keyFramePtr;
      ebp = keyFrameLength;
    }

  lastKeyFramePtr = ebx;

  eax = *(short int *) (edi - 2);
  edi += eax;

  eax = *(short int *) (edi);
  eax = eax + eax * 2;
  edi = edi + eax * 2 + 12;

  numOfPointInBody = *(short int *) (edi - 10);

  if (numOfPointInAnim > numOfPointInBody)
    {
      numOfPointInAnim = numOfPointInBody;
    }

  eax = time - ebp;

  if (eax >= keyFrameLength)
    {
      int *destPtr;		//keyFrame
      int *sourcePtr;

      sourcePtr = (int *) (keyFramePtr + 8);
      destPtr = (int *) edi;

      do
	{
	  *(destPtr++) = *(sourcePtr++);
	  *(destPtr++) = *(sourcePtr++);

	  destPtr = (int *) (((char *) destPtr) + 30);

	}
      while (--numOfPointInAnim);

      *(char **) animVar1 = keyFramePtr;
      *(int *) (animVar1 + 4) = time;

      currentX = *(short int *) (keyFramePtr + 2);
      currentZ = *(short int *) (keyFramePtr + 4);
      currentY = *(short int *) (keyFramePtr + 6);

      processActorVar5 = *(short int *) (keyFramePtr + 8);
      processActorSub2Var0 = *(short int *) (keyFramePtr + 10);
      processActorVar6 = *(short int *) (keyFramePtr + 12);
      processActorSub2Var1 = *(short int *) (keyFramePtr + 14);

      return (1);
    }
  else
    {
      keyFramePtrOld = keyFramePtr;

      lastKeyFramePtr += 8;
      keyFramePtr += 8;

      processActorVar5 = *(short int *) keyFramePtr;
      processActorSub2Var0 =
	(*(short int *) (keyFramePtr + 2) * eax) / keyFrameLength;
      processActorVar6 =
	(*(short int *) (keyFramePtr + 4) * eax) / keyFrameLength;
      processActorSub2Var1 =
	(*(short int *) (keyFramePtr + 6) * eax) / keyFrameLength;

      lastKeyFramePtr += 8;
      keyFramePtr += 8;

      edi += 38;

      if (--numOfPointInAnim)
	{
	  animVar4 = numOfPointInAnim;

	  do
	    {
	      animOpcode = getAnimOpcode (&edi);

	      switch (animOpcode)
		{
		case 0:	//allow global rotate
		  {
		    applyAnimMode0 (&edi, eax, keyFrameLength);
		    applyAnimMode0 (&edi, eax, keyFrameLength);
		    applyAnimMode0 (&edi, eax, keyFrameLength);
		    break;
		  }
		case 1:	//dissallow global rotate
		  {
		    applyAnimMode1 (&edi, eax, keyFrameLength);
		    applyAnimMode1 (&edi, eax, keyFrameLength);
		    applyAnimMode1 (&edi, eax, keyFrameLength);
		    break;
		  }
		case 2:	//dissallow global rotate + hide
		  {
		    applyAnimMode1 (&edi, eax, keyFrameLength);
		    applyAnimMode1 (&edi, eax, keyFrameLength);
		    applyAnimMode1 (&edi, eax, keyFrameLength);
		    break;
		  }
		default:
		  {
		    printf ("Unsupported rotaton mode %d in draw3D1!\n",
			    animOpcode);
		    exit (1);
		  }
		}

	      edi += 30;
	    }
	  while (--animVar4);
	}

      currentX = (*(short int *) (keyFramePtrOld + 2) * eax) / keyFrameLength;
      currentZ = (*(short int *) (keyFramePtrOld + 4) * eax) / keyFrameLength;
      currentY = (*(short int *) (keyFramePtrOld + 6) * eax) / keyFrameLength;
    }

  return (0);
}

int
LBA_engine::playAnim (char costume, short int arg_4, unsigned char arg_8,
		      short int actorNum)
{
  actor *lactor;
  int var_4;

  lactor = &actors[actorNum];

  if (lactor->costumeIndex == -1)
    return (0);

  if (lactor->field_60 & 0x400)	// si c'est un sprite
    return (0);

  if (costume == lactor->costume && lactor->currentAnim != -1)	// le costume est deja loadé
    return (1);

  if (arg_8 == 255 && lactor->field_78 != 2)
    arg_8 = lactor->costume;

  var_4 = initCostume (costume, actorNum);

  if (var_4 == -1)
    var_4 = initCostume (0, actorNum);

  if (arg_4 != 4 && lactor->field_78 == 2)
    {
      lactor->field_2 = costume;
      return (0);
    }

  if (arg_4 == 3)
    {
      arg_4 = 2;

      arg_8 = lactor->costume;

      if (arg_8 == 15 || arg_8 == 7 || arg_8 == 8 || arg_8 == 9)
	{
	  arg_8 = 0;
	}

    }

  if (arg_4 == 4)
    arg_4 = 2;

  if (lactor->currentAnim == -1)	//if no previous animation
    {
      setAnimAtKeyFrame (0, getHqrdataPtr (HQRanims, var_4),	//set animation directly to first keyFrame
			 bodyPtrTab[lactor->costumeIndex]);
    }
  else
    {
      bufAni2 +=
	increaseAnim ((char *) bufAni2,
		      (char *) bodyPtrTab[lactor->costumeIndex]);
      if (bufAni1 + 4488 < bufAni2)
	bufAni2 = bufAni1;
    }

  lactor->currentAnim = var_4;
  lactor->costume = costume;
  lactor->field_2 = arg_8;
  lactor->field_4 = loadTwinsenCostumesVar1;
  lactor->field_78 = arg_4;
  lactor->animPosition = 0;
  *(unsigned char *) &lactor->field_62 &= 0xF9;
  *(unsigned char *) &lactor->field_62 |= 8;

  if (lactor->field_4 != 0)
    {
      initNewCSub (lactor, actorNum);
    }

  lactor->field_6A = 0;
  lactor->lastX = 0;
  lactor->lastZ = 0;

  return (1);
}

int
LBA_engine::increaseAnim (char *lBufAnim, char *lBody)	//copy the next keyFrame from a different buffer
{
  int temp;
  char *ptr;
  int *edi;
  int *esi;
  int var0;
  int var1;
  int var2;
  int counter;

  temp = *(short int *) lBody;

  if (temp & 2)
    {
      ptr = (lBody + 0x10);

      *(int *) (ptr + 4) = time;
      *(char **) (ptr) = lBufAnim;

      var0 = *(short int *) (ptr - 2);
      ptr = ptr + var0;

      var1 = *(short int *) (ptr);
      var1 = var1 + var1 * 2;

      ptr = ptr + var1 * 2 + 2;

      var2 = *(short int *) (ptr);
      counter = var2;
      var2 = (var2 * 8) + 8;

      edi = (int *) (lBufAnim + 8);
      esi = (int *) (ptr + 10);

      do
	{
	  *(edi++) = *(esi++);
	  *(edi++) = *(esi++);

	  esi = (int *) (((char *) esi) + 30);

	}
      while (--counter);

      return (var2);

    }

  return (0);
}

void
LBA_engine::initNewCSub (actor * lactor, int actorNum)
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

  return;

  ebx = (char *) lactor->field_4;

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

      switch (var_54)
	{
	case 0:
	  temp = *(ebx++);
	  temp--;
	  if (temp == lactor->animPosition)
	    {
	      lactor->field_66 = *(ebx++);
	      lactor->field_62 |= 2;
	    }
	  else
	    {
	      ebx++;
	    }
	  break;
	case 1:
	  temp = *(ebx++);

	  if (temp == lactor->animPosition)
	    {
	      fullRedrawS3 (*(short int *) ebx, 0x1000, 1, lactor->X,
			    lactor->Z, lactor->Y);
	    }

	  ebx += 2;
	  break;
	case 2:
	  temp = *(ebx++);
	  if (temp == lactor->animPosition)
	    {
	      printf ("Skipping initNewCSub 2\n");
	      ebx += 2;
	    }
	  else
	    {
	      ebx += 4;
	    }
	  break;
	case 5:
	  temp = *(ebx++);
	  if (temp == lactor->animPosition)
	    {
	      int dx;
	      int cx;

	      dx = *(short int *) ebx;
	      ebx += 2;
	      cx = *(short int *) ebx;
	      ebx += 2;

	      fullRedrawS3 (dx, 0x1000, cx, lactor->X, lactor->Z, lactor->Y);
	    }
	  else
	    {
	      ebx += 4;
	    }
	  break;
	case 8:
	  printf ("Skipping initNewCSub opcode 8\n");
	  ebx = var_48;
	  break;
	case 10:
	  ebx = edi;
	  printf ("Skipping initNewCSub opcode 10\n");
	  break;
	case 11:
	  ebx = edi;
	  printf ("Skipping initNewCSub opcode 11\n");
	  break;
	default:
	  printf ("Unsupported opcode %d in initNewCSub\n", var_54);
	  exit (1);
	}

      var_4++;
    }

}

int
LBA_engine::getAnimOpcode (char **ptr)
{
  short int *lptr;
  short int opcode;

  lptr = (short int *) *ptr;

  *lptr = opcode = *(short int *) keyFramePtr;

  keyFramePtr += 2;
  *(ptr) = *(ptr) + 2;
  lastKeyFramePtr += 2;

  return (opcode);
}

void
LBA_engine::applyAnimMode0 (char **ptr, int bp, int bx)
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

  *dest = computedAngle & 0x3FF;

  *(ptr) = *(ptr) + 2;
}

void
LBA_engine::applyAnimMode1 (char **ptr, int bp, int bx)
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
