#include "lba.h"

void
  LBA_engine::drawComportementEntry(int lcomportement, int arg, int arg2)
{
    unsigned char *currentAnim;
    int currentAnimSate;

    int box_left;
    int box_right;
    int box_top;
    int box_bottom;
    int var_10;

    box_left = lcomportement * 110 + 110;
    box_right = box_left + 99;
    box_top = 110;
    box_bottom = 229;

    currentAnim = getHqrdataPtr(HQRanims, TCOS[lcomportement]);

    currentAnimSate = winTab[lcomportement];

    if (draw3D1(currentAnimSate, (char *) currentAnim, (char *) menuCostumeIndex)) {
	currentAnimSate++;	// keyFrame
	if (currentAnimSate == getAnimMaxIndex((char *) currentAnim))	// max for anim
	{
	    currentAnimSate = getAnimStartIndex((char *) currentAnim);	// start of anim
	}
	winTab[lcomportement] = currentAnimSate;
    }

    if (arg2 == 0) {
	drawBoxOutLine(box_left - 1, box_top - 1, box_right + 1, box_bottom + 1);
    }

    saveTextWindow();
    maximizeTextWindow();

    if (lcomportement != comportement) {
	drawBlackBox(box_left, box_top, box_right, box_bottom, 0);	// color of unselected
    } else {
	drawBlackBox(box_left, box_top, box_right, box_bottom, 68);	// color of selected

	drawBlackBox(110, 239, 540, 279, 0);	// comportement name box
	drawBoxOutLine(110, 239, 540, 279);

	setTextColor(15);

	if (comportement == 2 && agressivity == 1) {
	    printString(4, dataString);	// pour afficher le "auto" ou non
	} else {
	    printString(comportement, dataString);
	}

	printStringSimple((650 - getStringLength(dataString)) / 2, 240, dataString);	// ecrit le 
											// 
       // 
       // nom du
       // comportement 
       // dans
       // lequel
       // on se
       // trouve
    }

    var_10 = box_top;

    draw3D4(box_left, box_top, box_right, box_bottom, -600, arg, menuCostumeIndex);	// dessine
   // le model

    osystem->refresh(videoBuffer1, box_left, var_10, box_right, box_bottom);
    osystem->refresh(videoBuffer1, 110, 239, 540, 279);

    loadSavedTextWindow();
}

void LBA_engine::drawMenuWin2(short int arg_0, short int arg_4)
{
    int var_10;
    int var_4;
    int var_8;
    int var_C;

    int var_14;

    int temp;
    int i;

    maximizeTextWindow();
    drawBoxOutLine(arg_0, arg_4, arg_0 + 450, arg_4 + 80);
    drawBlackBox(arg_0 + 1, arg_4 + 1, arg_0 + 449, arg_4 + 79, 0);
    var_14 = arg_0 + 9;
    drawSprite(0, var_14, arg_4 + 13, getHqrdataPtr(HQRPtrSpriteExtra, 4));
    var_10 = arg_0 + 325;
    temp = arg_0 + 25;
    var_4 = remapComposante(temp, var_10, 50, twinsen->life);

    var_8 = arg_4 + 10;
    var_C = arg_4 + 25;
    drawBlackBox(temp, var_8, var_4, var_C, 91);
    drawBlackBox(var_4, var_8, var_10, var_C, 0);

    drawBoxOutLine(arg_0 + 25, arg_4 + 10, arg_0 + 325, arg_4 + 10 + 15);

    if (!vars[28] && vars[4])	// ca doit etre le mana

    {
	drawSprite(0, var_14, arg_4 + 36, getHqrdataPtr(HQRPtrSpriteExtra, 5));
	printf("Unimplemented drawMenuWin2\n");
	exit(1);
       // implementer la suite

    }

    var_4 = arg_0 + 340;

    drawSprite(0, var_4, arg_4 + 15, getHqrdataPtr(HQRPtrSpriteExtra, 3));	// draw coins
    setTextColor(155);
    printStringSimple(arg_0 + 370, arg_4 + 5, getStringFromNum(numCoin));	// amount of coins

    drawSprite(0, var_4, arg_4 + 55, getHqrdataPtr(HQRPtrSpriteExtra, 6));	// draw key
    setTextColor(155);
    printStringSimple(arg_0 + 370, arg_4 + 40, getStringFromNum(numKey));

    for (i = 0; i < numCloverBox; i++)	// boites à trefles
    {
	drawSprite(0, remapComposante(arg_0 + 25, arg_0 + 325, 10, i),
		   arg_4 + 58, getHqrdataPtr(HQRPtrSpriteExtra, 41));
    }

    for (i = 0; i < numClover; i++)	// trefles
    {
	drawSprite(0, remapComposante(arg_0 + 25, arg_0 + 325, 10, i) + 2,
		   arg_4 + 60, getHqrdataPtr(HQRPtrSpriteExtra, 7));
    }

    osystem->refresh(videoBuffer1, arg_0, arg_4, arg_0 + 450, arg_4 + 80);

}

void LBA_engine::drawMenuWin(short int var)
{
    drawBoxOutLine(100, 100, 550, 290);
    drawBoxInsideTrans(101, 101, 549, 289, 2);

    setAnimAtKeyFrame(winTab[0], getHqrdataPtr(HQRanims, TCOS[0]), menuCostumeIndex);
    drawComportementEntry(0, var, 0);

    setAnimAtKeyFrame(winTab[1], getHqrdataPtr(HQRanims, TCOS[1]), menuCostumeIndex);
    drawComportementEntry(1, var, 0);

    setAnimAtKeyFrame(winTab[2], getHqrdataPtr(HQRanims, TCOS[2]), menuCostumeIndex);
    drawComportementEntry(2, var, 0);

    setAnimAtKeyFrame(winTab[3], getHqrdataPtr(HQRanims, TCOS[3]), menuCostumeIndex);
    drawComportementEntry(3, var, 0);

    drawMenuWin2(100, 300);

    osystem->refresh(videoBuffer1, 100, 100, 550, 290);
}

void LBA_engine::processComportementMenu(void)
{

    int languageCD1temp;
    int var_4;
    int savedComportement;

    freezeTime();

    if (comportement == 4) {
	mainMenu2();
	changeTwinsenComp(0);
    }

    menuCostumeIndex = bodyPtrTab[twinsen->costumeIndex];

    TCOS[0] = TCos0Init;
    TCOS[1] = TCos1Init;
    TCOS[2] = TCos2Init;
    TCOS[3] = TCos3Init;

    setActorAngleSafe(twinsen->angle, twinsen->angle - 256, 50, &timeVar);

    copyToBuffer(videoBuffer1, videoBuffer2);

    languageCD1temp = languageCD1;
    languageCD1 = 0;
    var_4 = currentTextBank;
    currentTextBank = -1;

    loadTextBank(0);

    drawMenuWin(twinsen->angle);

    savedComportement = comportement;

    setAnimAtKeyFrame(winTab[comportement],
		      getHqrdataPtr(HQRanims, TCOS[comportement]), menuCostumeIndex);

    readKeyboard();

    while (key1 & 4 || (skipIntro > 59 && skipIntro < 62)) {
	readKeyboard();

	key = printTextVar12;

	if (key & 8)
	    comportement++;

	if (key & 4)
	    comportement--;

	if (comportement < 0)	// J'ai reecrit ca à ma sauce
	    comportement = 3;

	if (comportement >= 4)
	    comportement = 0;

	if (savedComportement != comportement) {
	    drawComportementEntry(savedComportement, twinsen->angle, 1);
	    savedComportement = comportement;
	    setActorAngleSafe(twinsen->angle, twinsen->angle - 256, 50, &timeVar);	// rotate
	   // twinsen
	    setAnimAtKeyFrame(winTab[comportement],
			      getHqrdataPtr(HQRanims, TCOS[comportement]), menuCostumeIndex);

	    while (printTextVar12) {
		readKeyboard();
		drawComportementEntry(comportement, -1, 1);
	    }
	}
	drawComportementEntry(comportement, -1, 1);
    }

    changeTwinsenComp(comportement);

    reinitAll1();

    currentTextBank = var_4;
    loadTextBank(currentTextBank + 3);

    languageCD1 = languageCD1temp;

    unfreezeTime();
}
