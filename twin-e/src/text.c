#include "lba.h"

void
  LBA_engine::loadTextBank(int index)	// ca devrais plutot etre un truc du genre loadTextBank
{

    int bundleEntryPoint;
    int size;
    int textSize;

    if (index == textVar1)
	return;

    textVar1 = index;
    textVar2[0] = textVar3;

    bundleEntryPoint = (language * 14) * 2 + index * 2;

    size = loadImageToPtr("text.hqr", (byte *) bufOrder, bundleEntryPoint);

    numTextEntry = size / 2;

    bundleEntryPoint++;

    textSize = loadImageToPtr("text.hqr", (byte *) bufText, bundleEntryPoint);

#ifdef DEBUG
    dumpFile("currentBufOrder", bufOrder, printStringVar1);
    dumpFile("currentBufText", bufText, textSize);
#endif

    if (languageCD1 != 0)
	loadVox(index);

    return;

}

void LBA_engine::printTextFullScreen(int textIndex)
{
    int isVoxSet;
    int temp2;
    int temp3 = 0;

    saveTextWindow();
    maximizeTextWindow();
    copyToBuffer(videoBuffer1, videoBuffer2);

    if (languageCD1 != 0)	// si on doit player le vox, on le met le pointeur à la bonne
       // position
    {
	isVoxSet = setVoxFileAtDigit(textIndex);
    }

    if (!flagDisplayText && isVoxSet)	// si on doit player le Vox sans afficher le text
    {
	do {
	    temp2 = printText4(voxFileHandle);
	}
	while (temp2 && skipIntro == 1);

	printTextVar5 = 0;

	if (!languageCD1 || !voxFileHandle) {
	    loadSavedTextWindow();
	    return;
	}
       // todo: recheck, je pense avoir oublie des truc la..

	if (!printText6(4660)) {
	    loadSavedTextWindow();
	    return;
	}

	printText7(4660);

	loadSavedTextWindow();
	return;
    }

    printText8(textIndex);	// prépare le text
    drawTextBox();

    do				// debut de la boucle d'affichage du text+sons
    {
	readKeyboard();
	isVoxSet = temp2 = printText10();	// on doit player le son ?
	printText4(voxFileHandle);

	if (isVoxSet == 2) {
	    do {
		readKeyboard();
		if (skipIntro == 0 && key1 == 0 && printTextVar12 == 0)
		    break;
		printText4(voxFileHandle);
	    }
	    while (1);

	    do {
		readKeyboard();
		if (skipIntro != 0 || key1 != 0 || printTextVar12 != 0)
		    break;
		printText4(voxFileHandle);
	    }
	    while (1);
	}

	if (skipIntro == 1) {
	    temp3 = 1;
	    break;
	}

	if (!temp2) {
	    if (!printText11()) {
		break;
	    }
	}
    }
    while (!temp3);

    printTextVar5 = 0;
    if (languageCD1 != 0)
	if (voxFileHandle != 0)
	    if (printText6(4660) != 0)
		printText7(4660);

    printTextVar13 = 0;

    if (temp2 != 0) {
	loadSavedTextWindow();
	return;
    }

    if (temp3 != 0) {
	loadSavedTextWindow();
	return;
    }

    do				// ca doit etre la boucle d'attente de la fin de l'affichage du
       // text
    {
	readKeyboard();
    }
    while (skipIntro || key1 || printTextVar12);

    do {
	readKeyboard();
	if (skipIntro != 0) {
	    loadSavedTextWindow();
	    return;
	}

	if (key1 != 0) {
	    loadSavedTextWindow();
	    return;
	}
    }
    while (!printTextVar12);

    loadSavedTextWindow();
    return;

}

void LBA_engine::saveTextWindow(void)
{
    textWindowLeftSave = textWindowLeft;
    textWindowTopSave = textWindowTop;
    textWindowRightSave = textWindowRight;
    textWindowBottomSave = textWindowBottom;
}

void LBA_engine::maximizeTextWindow(void)
{
    textWindowTop = textWindowLeft = 0;
    textWindowRight = largeurEcran - 1;
    textWindowBottom = hauteurEcran - 1;
}

int LBA_engine::printText4(FILE * fileHandle)
{
    int result;

    if (!languageCD1)
	return (0);

    if (!fileHandle)
	return (0);

    result = printText6(4660);

    if (result)
	return (1);

    if (printTextVar5 == 0)
	return (result);

    readBufferSpeak(fileHandle);
    return (1);
}

void LBA_engine::readBufferSpeak(FILE * fileHandle)
{
    int a;
    int b;
    int c;

    readResourceData(fileHandle, (char *) &b, 4);
    readResourceData(fileHandle, (char *) &c, 4);
    readResourceData(fileHandle, (char *) &a, 2);

   // todo: implementer la suite
}

void LBA_engine::loadSavedTextWindow(void)
{
    textWindowLeft = textWindowLeftSave;
    textWindowTop = textWindowTopSave;
    textWindowRight = textWindowRightSave;
    textWindowBottom = textWindowBottomSave;
}

int LBA_engine::printText6(int var)
{
   // todo: implement this
   // sound function
    return (0);
}

int LBA_engine::printText7(int var)
{
   // todo: implement this
   // sound function
    return (0);

}

int LBA_engine::printText8(int var)
{
    int result;

    printTextVar13 = 0;

    result = findString(var);
    if (!result)
	return (0);

    printText8Ptr1 = buf1;
    printText8Ptr2 = buf2;

    printTextVar13 = 1;

    printText8Var1 = 0;
    buf1[0] = 0;
    buf2[0] = 0;
    printText8Var2 = var;
    printText8Var3 = 0;
    printText8Var4 = dialogueBoxLeft + 8;
    printText8Var5 = 0;
    printText8Var6 = 0;
    printText8Var7 = dialogueBoxTop + 8;
    printText8Var8 = currentTextPtr;

    fontInit(lbaFont, 2, 7);

    return (0);
}

void LBA_engine::drawTextBox(void)
{
    drawBoxTrans(dialogueBoxLeft, dialogueBoxTop, dialogueBoxRight,
		 dialogueBoxBottom, (char *) videoBuffer2, dialogueBoxLeft,
		 dialogueBoxTop, (char *) videoBuffer1);
    if (newGameVar4 != 0) {
	drawBoxOutLine(dialogueBoxLeft, dialogueBoxTop, dialogueBoxRight, dialogueBoxBottom);
	drawBoxInsideTrans(dialogueBoxLeft + 1, dialogueBoxTop + 1,
			   dialogueBoxRight - 1, dialogueBoxBottom - 1, 3);
    }

    osystem->refresh(videoBuffer1, dialogueBoxLeft, dialogueBoxTop,
		     dialogueBoxRight, dialogueBoxBottom);

    printText8Var3 = 0;

    drawBoxTrans(dialogueBoxLeft, dialogueBoxTop, dialogueBoxRight,
		 dialogueBoxBottom, (char *) videoBuffer1, dialogueBoxLeft,
		 dialogueBoxTop, (char *) videoBuffer2);
}

int LBA_engine::printText10(void)
{

    int a;
    int b;

    if (printTextVar13 == 0)
	return (0);

    if (*printText8Ptr2 == 0) {
	if (printText8Var5 != 0) {
	    if (newGameVar5 != 0)
		printText10Sub();
	    printTextVar13 = 0;
	    return (0);
	}
	if (printText8Var6 != 0) {
	    drawBoxTrans(dialogueBoxLeft, dialogueBoxTop, dialogueBoxRight,
			 dialogueBoxBottom, (char *) videoBuffer2,
			 dialogueBoxLeft, dialogueBoxTop, (char *) videoBuffer1);
	    osystem->refresh(videoBuffer1, dialogueBoxLeft, dialogueBoxTop,
			     dialogueBoxRight, dialogueBoxBottom);
	    printText8Var3 = 0;
	    printText8Var6 = 0;
	    printText8Var4 = dialogueBoxLeft + 8;
	    printText8Var7 = dialogueBoxTop + 8;
	}
	if (*printText8Var8 == 0) {
	    printText8Sub2();
	    printText8Var5 = 1;
	    return (1);
	}
	processTextLine();
    }

    if (*printText8Ptr2 == 0)
	return (1);

    printText8Sub4(printText8Var4, printText8Var7, *printText8Ptr2);	// printText8var4 ==
   // currentLetterX
   // printText8Var7 ==
   // currentLetter7
    printText10Sub2();		// fonction responsable de l'affichage du text

    printText10Sub3(*printText8Ptr2, &a, &b, lbaFont);

    if (*printText8Ptr2 != 0x20) {
	printText8Var4 += a + 2;
    } else {
	if (printText10Var1 != 0) {
	    printText8Var4++;
	    printText10Var1--;
	}
	printText8Var4 += spaceLength;
    }

    printText8Ptr2++;		// on passe au caractere suivant ?

    if (*printText8Ptr2 != 0)
	return (1);

    printText8Var7 += 38;
    printText8Var4 = dialogueBoxLeft + 8;
    if (printText8Var6 == 1)
	if (printText8Var5 == 0) {
	    printText10Sub();
	    return (2);
	}

    printText8Var1++;
    if (printText8Var1 < dialogueBoxParam1)
	return (1);

    printText8Sub2();
    printText8Var6 = 1;

    if (*printText8Var8 == 0)
	printText8Var5 = 1;

    return (1);

}

void LBA_engine::printText8Sub2(void)
{
    int i = 0;

    buf2[0] = 0;

    while (i < initVar5) {
	strcat(buf2, " ");
	i++;
    };

    printText8Ptr2 = buf2;
    addLineBreakX = 16;
    printText8Var1 = 0;
    printText8PrepareBufferVar3 = initVar5;
}

void LBA_engine::printText10Sub2(void)
{
    int currentLetter;
    int currentIndex;
    int counter;
    int counter2;
    short int *ptr;

    currentLetter = printText8Var3;
    currentLetter--;

    currentIndex = currentLetter * 3;

    ptr = pt8s4var1 + currentIndex;

   // todo: gerer le delay ici...

    counter = printText8Var3;
    counter2 = initVar2;

    while (--counter >= 0) {
	setTextColor(counter2);
	drawDoubleLetter(*(ptr + 1), *(ptr + 2), *ptr, counter2);
	counter2 -= initVar4;
	if (counter2 > initVar3)
	    counter2 = initVar3;
	ptr -= 3;
    };

}

void LBA_engine::drawDoubleLetter(int a, int b, int c, int d)
{
    int left, top, right, bottom;

    if (c == 0x20)
	return;

    setTextColor(0);		// on met la couleur grise

    drawLetter2(a + 2, b + 4, c);	// le caractere derriere en gris

    setTextColor(d);		// on met la bonne couleur

    drawLetter2(a, b, c);

    left = a;
    top = b;
    right = a + 32;
    bottom = b + 38;

   // manque les check pour la taille de la boite de dialogue...

    osystem->refresh(videoBuffer1, left, top, right, bottom);
}

void LBA_engine::drawLetter2(int x, int y, int c)
{
    char temp[2];		// todo: faire une vrais implementation de la chose...

    temp[1] = 0;
    temp[0] = (char) c;

    printStringSimple(x, y, temp);
}

void LBA_engine::printText10Sub3(byte c, int *b, int *a, byte * font)
{
    byte *temp;

    temp = font + *((short int *) (font + c * 4));
    *b = *(temp);
    *a = *(temp + 1);

}

void LBA_engine::printText8Sub4(short int a, short int b, short int c)
{
    int counter;
    int temp;
    int counter2 = 0;
    int var1;
    int var2;

    counter = printText8Var3;

    if (counter < 32) {
	temp = counter * 3;
	pt8s4var1[temp] = c;
	pt8s4var2[temp] = a;
	pt8s4var3[temp] = b;

	counter++;
    } else {
	while (counter2 < 31) {
	    var1 = (counter2 + 1) * 3;
	    var2 = counter2 * 3;
	    pt8s4var1[var2] = pt8s4var1[var1];
	    pt8s4var2[var2] = pt8s4var2[var1];
	    pt8s4var3[var2] = pt8s4var3[var1];
	    counter2++;
	};
	pt8s4var4[0] = c;
	pt8s4var5[0] = a;
	pt8s4var6[0] = b;
    }

    printText8Var3 = counter;
}

void LBA_engine::printText10Sub(void)
{
    vertexCoordinates[0] = initVar3;	// les 3 vertex ?
    vertexCoordinates[1] = dialogueBoxRight - 3;
    vertexCoordinates[2] = dialogueBoxBottom - 24;
    vertexCoordinates[3] = initVar3;
    vertexCoordinates[4] = dialogueBoxRight - 24;
    vertexCoordinates[5] = dialogueBoxBottom - 3;
    vertexCoordinates[6] = initVar2;
    vertexCoordinates[7] = vertexCoordinates[1];
    vertexCoordinates[8] = vertexCoordinates[5];

    polyRenderType = 0;
    numOfVertex = 3;

    if (printText10SubSub2()) {
	printText10SubSub(polyRenderType, initVar3);
    }

    osystem->refresh(videoBuffer1, dialogueBoxRight - 24,
		     dialogueBoxBottom - 24, dialogueBoxRight - 3, dialogueBoxBottom - 3);

}

void LBA_engine::printText10SubSub(int arg_0, int arg_4)
{
    polyRender(arg_0, arg_4);
}

int LBA_engine::printText10SubSub2(void)
{
    pRenderV1 = vertexCoordinates;
    return (prepareRender());
}

int LBA_engine::prepareRender(void)
{
    short int vertexX, vertexY;
    short int *ptr1, *ptr3;
    int i;
    short int psh1, psh2;
    char direction = 1;
    int echange;
    short int oldVertexX, oldVertexY;
    short int currentVertexX, currentVertexY;
    short int size;
    int temp2, temp4, temp5, temp6;
    int step;
    float vfloat, vfloat2;
    float vcfloat, vcfloat2;

    pRenderV1 = vertexCoordinates;
    pRenderV2 = pRenderV3;
    numOfVertexRemaining = numOfVertex;
    polyCropped = 0;

    vleft = vtop = 32767;
    vright = vbottom = -32768;

    ptr1 = vertexCoordinates;

    for (i = 0; i < numOfVertex; i++) {
	ptr1++;			// discarding the 1st parameter

	vertexX = *(ptr1++);

	if (vertexX < vleft)
	    vleft = vertexX;
	if (vertexX > vright)
	    vright = vertexX;

	vertexY = *(ptr1++);

	if (vertexY < vtop)
	    vtop = vertexY;
	if (vertexY > vbottom)
	    vbottom = vertexY;
    }

    ptr1[0] = pRenderV1[0];
    ptr1[1] = pRenderV1[1];
    ptr1[2] = pRenderV1[2];

    if (vbottom < vtop)
	return (0);

    if (vleft < textWindowLeft) {
	if (vright < textWindowLeft)
	    return (0);
       // pRenderSub();
       // printf("3Dcropping...\n");
       // exit(1);
       // if(vright==textWindowLeft)
	return (2);
    }

    if (vright > textWindowRight) {
	if (vleft > textWindowRight)
	    return (0);
       // pRenderSub2();
       // printf("3Dcropping...\n");
       // exit(1);
       // if(vleft==textWindowRight)
	return (2);
    }

    if (vtop < textWindowTop) {
	if (vbottom < textWindowTop)
	    return (0);
       // pRenderSub3();
       // printf("3Dcropping...\n");
       // exit(1);
       // if(vbottom==textWindowBottom)
	return (2);
    }

    if (vbottom > textWindowBottom) {
	if (vtop > textWindowBottom)
	    return (0);
       // pRenderSub4();
       // printf("3Dcropping...\n");
       // exit(1);
       // if(vtop==textWindowBottom)
	return (2);
    }

    if (polyCropped) {
	printf("prepareRender-> cropped poly !\n");
	exit(1);
    }

    ptr1 = pRenderV1;		// on retourne au debut de la liste

    vertexParam1 = vertexParam2 = (*(ptr1++)) & 0xFF;
    oldVertexX = *(ptr1++);
    oldVertexY = *(ptr1++);

    do {
	oldVertexParam = vertexParam1;

	vertexParam1 = vertexParam2 = (*(ptr1++)) & 0xFF;
	currentVertexX = *(ptr1++);
	currentVertexY = *(ptr1++);

       // drawLine(oldVertexX,oldVertexY,currentVertexX,currentVertexY,255);

	if (currentVertexY == oldVertexY)	// since it's scanline based, we don't care when we 
						// 
	   // 
	   // are only moving along X
	{
	    oldVertexX = size = currentVertexX;
	} else {
	    psh1 = currentVertexX;	// let's save the current coordinates since we are going to 
					// 
	   // 
	   // modify the values
	    psh2 = currentVertexY;

	    if (currentVertexY < oldVertexY)	// if we are going up
	    {
		size = oldVertexY - currentVertexY;
		direction = -1;

		if (oldVertexX < currentVertexX)	// if we are going up right
		{
		    echange = oldVertexX;	// we invert the vertex to draw from new to old
		    oldVertexX = currentVertexX;
		    currentVertexX = echange;

		    echange = currentVertexY;
		    currentVertexY = oldVertexY;

		    oldVertexY = echange;

		    echange = oldVertexParam;
		    oldVertexParam = vertexParam2;
		    vertexParam2 = echange;

		    direction = 1;	// we will draw by going down the tab
		}

		temp2 = oldVertexY;	// temp2 is the starting Y position
		oldVertexY = size;	// oldVertexY now become the number of pixel
		size = temp2 * 2;

		ptr3 = &polyTab[temp2 + 480];	// ptr3 is the output ptr in the renderTab

		temp4 = ((oldVertexX - currentVertexX) << 16);	// temp4 = size in X << 16

		temp5 = temp4 / oldVertexY;	// temp5 is the size of a step << 16
		temp6 = temp4 % oldVertexY;	// temp6 is the remaining << 16

		vfloat = ((float) (oldVertexX - currentVertexX)) / ((float) oldVertexY);

		temp6 >>= 1;
		temp6 += 0x7FFF;

		step = (unsigned short) temp5;	// retrieve the size of a step

	       // temp7 = (((unsigned short)temp6) | ((oldVertexX & 0xFFFF)<<16));
		vfloat2 = oldVertexX;

		oldVertexX = oldVertexY;	// oldVertexX is now the number of vertical pixels

		oldVertexY += 2;

		for (i = 0; i < oldVertexY; i++) {
		   // *(ptr3)=((temp7&0xFFFF0000)>>16);
		    *(ptr3) = (short int) vfloat2;
		    ptr3 += direction;
		   // temp7+=step;
		    vfloat2 -= vfloat;
		}

		if (polyRenderType >= 7) {
		    ptr3 = &polyTab2[temp2 + 480];

		    temp4 = ((vertexParam2 - oldVertexParam));

		    if (temp4 >= 0) {
		       /*
		        * temp5=temp4/oldVertexX; temp6=temp4%oldVertexX;
		        */

			vcfloat = ((float) (temp4)) / ((float) oldVertexX);

		       /*
		        * (*(unsigned char*)&temp6)>>=1; (*(unsigned char*)&temp6)+=0x7F;
		        */

		       // temp6=(temp6&0xFF) | (oldVertexParam)<<8;

			vcfloat2 = oldVertexParam;

		       // temp6=oldVertexParam;

			oldVertexX += 2;

			for (i = 0; i <= oldVertexX; i++) {
			    *(ptr3) = (short int) vcfloat2;
			    ptr3 += direction;
			    vcfloat2 += vcfloat;
			}
		    } else {
			temp5 = temp4 / oldVertexX;
			temp6 = temp4 % oldVertexX;

			vcfloat = -((float) (temp4)) / ((float) oldVertexX);

			(*(unsigned char *) &temp6) >>= 1;
			(*(unsigned char *) &temp6) += 0x7F;

			temp6 = (temp6 & 0xFF) | (oldVertexParam & 0xFF) << 8;

			vcfloat2 = oldVertexParam;

			temp6 = oldVertexParam;

			oldVertexX += 2;

			for (i = 0; i <= oldVertexX; i++) {
			    *(ptr3) = (short int) vcfloat2;
			    ptr3 += direction;
			    vcfloat2 -= vcfloat;
			}
		    }
		}
		direction = 1;
		oldVertexY = psh2;
		oldVertexX = psh1;
	    }

	    else		// if we are going down
	    {

		size = currentVertexY - oldVertexY;	// size is the number of pixel we must go
	       // verticaly

		if (oldVertexX > currentVertexX)	// if we are going down and to the left
		{
		    echange = oldVertexX;	// in that case, we will draw the line the other
		   // side (from new point to old point)
		    oldVertexX = currentVertexX;
		    currentVertexX = echange;

		    echange = currentVertexY;
		    currentVertexY = oldVertexY;
		    oldVertexY = echange;

		    echange = oldVertexParam;
		    oldVertexParam = vertexParam2;
		    vertexParam2 = echange;

		    direction = -1;	// since we are going backward in the screen
		}

		temp2 = oldVertexY;	// temp2 is the starting Y position
		oldVertexY = size;	// oldVertexY now become the number of pixel
		size = temp2 * 2;

		ptr3 = &polyTab[temp2];	// ptr3 is the output ptr in the renderTab

		temp4 = ((currentVertexX - oldVertexX) << 16);	// temp4 = size in X << 16

		temp5 = temp4 / oldVertexY;	// temp5 is the size of a step << 16
		temp6 = temp4 % oldVertexY;	// temp6 is the remaining << 16

		vfloat = ((float) (currentVertexX - oldVertexX)) / ((float) oldVertexY);

		temp6 >>= 1;
		temp6 += 0x7FFF;

		step = (unsigned short) temp5;	// retrieve the size of a step

	       // temp7 = (((unsigned short)temp6) | ((oldVertexX & 0xFFFF)<<16));
		vfloat2 = oldVertexX;

		oldVertexX = oldVertexY;	// oldVertexX is now the number of vertical pixels

		oldVertexY += 2;

		for (i = 0; i < oldVertexY; i++) {
		   // *(ptr3)=((temp7&0xFFFF0000)>>16);
		    *(ptr3) = (short int) vfloat2;
		    ptr3 += direction;
		   // temp7+=step;
		    vfloat2 += vfloat;
		}

		if (polyRenderType >= 7) {
		    ptr3 = &polyTab2[temp2];

		    temp4 = ((vertexParam2 - oldVertexParam));

		    if (temp4 >= 0) {
			temp5 = temp4 / oldVertexX;
			temp6 = temp4 % oldVertexX;

			vcfloat = ((float) (vertexParam2 - oldVertexParam)) / ((float) oldVertexX);

			(*(unsigned char *) &temp6) >>= 1;
			(*(unsigned char *) &temp6) += 0x7F;

			temp6 = (temp6 & 0xFF) | (oldVertexParam) << 8;

			vcfloat2 = oldVertexParam;

			temp6 = oldVertexParam;

			oldVertexX += 2;

			for (i = 0; i <= oldVertexX; i++) {
			    *(ptr3) = (short int) vcfloat2;
			    ptr3 += direction;
			    vcfloat2 += vcfloat;
			}
		    } else {
			temp5 = temp4 / oldVertexX;
			temp6 = temp4 % oldVertexX;

			vcfloat = -((float) (vertexParam2 - oldVertexParam)) / ((float) oldVertexX);

			(*(unsigned char *) &temp6) >>= 1;
			(*(unsigned char *) &temp6) += 0x7F;

			temp6 = (temp6 & 0xFF) | (oldVertexParam & 0xFF) << 8;

			vcfloat2 = oldVertexParam;

			temp6 = oldVertexParam;

		       // oldVertexX+=2;

			for (i = 0; i <= oldVertexX; i++) {
			    *(ptr3) = (short int) vcfloat2;
			    ptr3 += direction;
			    vcfloat2 -= vcfloat;
			}
		    }
		}

		direction = 1;
		oldVertexY = psh2;
		oldVertexX = psh1;
	    }
	}
    }
    while (--numOfVertexRemaining);

    return (1);
}

int LBA_engine::findString(int index)
{
    int temp = 0;
    int temp2 = 0;
    int nEntry;

    int ptrCurrentEntry;
    int ptrNextEntry;

    short int *localTextBuf = (short int *) bufText;
    short int *localOrderBuf = (short int *) bufOrder;

    nEntry = numTextEntry;	// nombre d'entrées dans la bank

    goto e2;

  e1:

    temp2 = 0;
    temp2 = *(localOrderBuf);
    localOrderBuf++;
    if (temp2 == index)
	goto e3;
    temp++;

  e2:

    temp2 = 0;
    temp2 = nEntry;
    if (temp < temp2)
	goto e1;
    temp = -1;

  e3:

    if (temp != -1)
	goto ex;
    temp = 0;
    numTextEntry = nEntry;

    return (0);

  ex:

    ptrCurrentEntry = localTextBuf[temp];	// entrée courante
    ptrNextEntry = localTextBuf[temp + 1];	// entrée d'apres

    currentTextPtr = (bufText + ptrCurrentEntry);

    currentTextLength = ptrNextEntry - ptrCurrentEntry;	// largeur de l'entre courante

    numTextEntry = nEntry;

    return (1);

}

int LBA_engine::printText11(void)
{
    return (printText4(voxFileHandle));
}

void LBA_engine::processTextLine(void)
{
    short int var4;
    char *buffer;
    char *temp;

    buffer = printText8Var8;
    spaceLength = 7;
    var4 = 1;

    addLineBreakX = 0;
    printText8PrepareBufferVar2 = 0;
    buf2[0] = 0;

  pt8start:
    if (*buffer == 0x20) {
	buffer++;
	goto pt8start;
    }

    if (*buffer != 0) {
	printText8Var8 = buffer;
	getWordSize(buffer, buf1);
	if (addLineBreakX + spaceLength + wordSizePixel < dialogueBoxParam2) {
	    temp = buffer + 1;
	    if (*buffer == 1) {
		var4 = 0;
		buffer = temp;
	    } else {
		if (*buf1 == 0x40) {
		    var4 = 0;
		    buffer = temp;
		    if (addLineBreakX == 0) {
			addLineBreakX = 7;
			*buf2 = spaceChar;
		    }
		    if (buf1[1] == 0x50) {
			printText8Var1 = dialogueBoxParam1;
			buffer++;
		    }
		} else {
		    buffer += wordSizeChar;
		    printText8Var8 = buffer;
		    strcat(buf2, buf1);
		    strcat(buf2, " ");	// not 100% accurate
		    printText8PrepareBufferVar2++;

		    addLineBreakX += wordSizePixel + spaceLength;
		    if (*printText8Var8 != 0) {
			printText8Var8++;
			goto pt8start;
		    }
		}
	    }
	}
    }

    if (printText8PrepareBufferVar2 != 0)
	printText8PrepareBufferVar2--;

    if (*printText8Var8 != 0 && var4 == 1) {

	spaceLength += (dialogueBoxParam2 - addLineBreakX) / printText8PrepareBufferVar2;
	printText10Var1 = dialogueBoxParam2 - addLineBreakX - dialogueBoxParam2 - addLineBreakX;	// stupid... 
													// 
       // 
       // recheck
    }

    printText8Var8 = buffer;

    printText8Ptr2 = buf2;

}

void LBA_engine::getWordSize(char *arg1, char *arg2)
{
    int temp = 0;
    char *arg2Save = arg2;

    while (*arg1 != 0 && *arg1 != 1 && *arg1 != 0x20) {
	temp++;
	*arg2++ = *arg1++;
    };

    wordSizeChar = temp;
    *arg2 = 0;
    wordSizePixel = getStringLength(arg2Save);

}
