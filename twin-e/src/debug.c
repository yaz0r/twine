#include "lba.h"

int manipActorVar1 = 0;

int
  debugger::processDebug(void)
{
    mouseStatusStruct mouseData;
    int actorNum;

    osystem->getMouseStatus(&mouseData);

   // printf("Mouse: %d %d - %d %d\n",mouseData.X,mouseData.Y,mouseData.left,mouseData.right);
   // printf("Num Actors: %d\n",numOfActorOnScreen);

    actorNum = findActor(mouseData.X, mouseData.Y);

    numOfWindows = 0;

    if (actorNum != -1 && mouseData.left) {
	debugActor(actorNum);
	engine->fullRedraw(1);
    }

    return (0);
}

int debugger::findActor(int X, int Y)
{
    int i;

    for (i = numOfActorOnScreen; i >= 0; i--) {
	if (inBox(X, Y, actorBox[i].top, actorBox[i].left, actorBox[i].bottom, actorBox[i].right)) {
	    return (actorBox[i].actorNum);
	}
    }

    return (-1);
}

int debugger::inBox(int X, int Y, int top, int left, int bottom, int right)
{
    if (X > left && X < right)
	if (Y > top && Y < bottom)
	    return (1);

    return (0);
}

void debugger::fillArea(int X, int Y, int width, int height)
{
    int i, j;
    unsigned char *ptr;
    int offset;

    ptr = engine->videoBuffer1 + engine->screenLockupTable[Y] + X;
    offset = 640 - (width);

    for (i = 0; i < height; i++) {
	for (j = 0; j < width; j++) {
	    *(ptr++) = 72;
	}
	ptr += offset;
    }

   // osystem->drawBufferToScreen(engine->videoBuffer1);
   // osystem->refresh(engine->videoBuffer1,0,0,640,480);

}

void debugger::fillArea2(int X, int Y, int width, int height)
{
    int i, j;
    unsigned char *ptr;
    int offset;

    ptr = engine->videoBuffer1 + engine->screenLockupTable[Y] + X;
    offset = 640 - (width);

    for (i = 0; i < height; i++) {
	for (j = 0; j < width; j++) {
	    *(ptr++) = 70;
	}
	ptr += offset;
    }

   // osystem->drawBufferToScreen(engine->videoBuffer1);
   // osystem->refresh(engine->videoBuffer1,0,0,640,480);

}

void debugger::addWin(int index, int X, int Y, int width, int height)
{
    windows[index].X = X;
    windows[index].Y = Y;
    windows[index].Width = width;
    windows[index].Height = height;
    windows[index].numOfButtons = 0;

    numOfWindows++;
}

void debugger::addButton(int winIndex, int X, int Y, int width, int height,
			 char *text, buttonType type)
{
    windows[winIndex].buttons[windows[winIndex].numOfButtons].X = X;
    windows[winIndex].buttons[windows[winIndex].numOfButtons].Y = Y;
    windows[winIndex].buttons[windows[winIndex].numOfButtons].Width = width;
    windows[winIndex].buttons[windows[winIndex].numOfButtons].Height = height;
    windows[winIndex].buttons[windows[winIndex].numOfButtons].type = type;

    windows[winIndex].buttons[windows[winIndex].numOfButtons].text =
	(char *) malloc(strlen(text) + 1);

    strcpy(windows[winIndex].buttons[windows[winIndex].numOfButtons].text, text);

    windows[winIndex].numOfButtons++;
}

buttonType debugger::findButton(int X, int Y)
{
    int i;
    int j;

    for (i = 0; i < numOfWindows; i++) {
	for (j = 0; j < windows[i].numOfButtons; j++) {
	    if (X > (windows[i].X + windows[i].buttons[j].X)
		&& X < (windows[i].X + windows[i].buttons[j].X + windows[i].buttons[j].Width)
		&& Y > (windows[i].Y + windows[i].buttons[j].Y)
		&& Y < (windows[i].Y + windows[i].buttons[j].Y + windows[i].buttons[j].Height)) {
		return (windows[i].buttons[j].type);
	    }
	}
    }

    return (NO_BUTTON);
}

buttonType debugger::processInput()
{
    mouseStatusStruct mouseData;
    buttonType button;

    do {
	engine->readKeyboard();
	osystem->getMouseStatus(&mouseData);

	if (mouseData.left) {
	    button = findButton(mouseData.X, mouseData.Y);

	    if (button != NO_BUTTON)
		return (button);
	}
    }
    while (1);
}

void debugger::drawAll()
{
    int i;
    int j;

    for (i = 0; i < numOfWindows; i++) {
	int X;
	int Y;

	X = windows[i].X;
	Y = windows[i].Y;

	fillArea(X, Y, windows[i].Width, windows[i].Height);

	for (j = 0; j < windows[i].numOfButtons; j++) {
	    fillArea2(X + windows[i].buttons[j].X, Y + windows[i].buttons[j].Y,
		      windows[i].buttons[j].Width, windows[i].buttons[j].Height);
	    osystem->drawText(X + windows[i].buttons[j].X,
			      Y + windows[i].buttons[j].Y, windows[i].buttons[j].text);
	}
    }
}

void debugger::debugActor(int num)
{
    int i;
    buttonType button;
    int numOfLines;
    int startLineTrack = 0;
    int startLineCom = 0;
    int Y;

   // printf("Debug actor %d\n",num);

    scriptData *comScript;
    scriptData *trackScript;

    comScript = getActorComScript(num);
    trackScript = getActorTrackScript(num);

    do {
	addWin(0, 420, 145, 220, 300);	// track win
	addButton(0, 205, 15, 15, 140, "U", BUTTON_TRACK_UP);
	addButton(0, 205, 156, 15, 140, "D", BUTTON_TRACK_DOWN);

	addWin(1, 0, 145, 418, 300);	// comp win

	addWin(2, 0, 447, 640, 32);	// bottom status win
	addButton(2, 0, 15, 640, 15, "OK", BUTTON_OK);

	drawAll();

	numOfLines = 25;
	Y = 150;

	if (trackScript->numOfLignes < 25)
	    numOfLines = trackScript->numOfLignes;

	for (i = startLineTrack; i < (numOfLines + startLineTrack); i++) {
	    osystem->drawText(425, Y, trackScript->lines[i]);
	    Y += 11;
	}

	numOfLines = 25;
	Y = 150;

	if (comScript->numOfLignes < 25)
	    numOfLines = comScript->numOfLignes;

	for (i = startLineCom; i < (numOfLines + startLineCom); i++) {
	    osystem->drawText(5, Y, comScript->lines[i]);
	    Y += 11;
	}

	osystem->drawBufferToScreen(engine->videoBuffer1);

	button = processInput();

	if (button == BUTTON_TRACK_DOWN && ((startLineTrack + 25) < trackScript->numOfLignes))
	    startLineTrack++;

	if (button == BUTTON_TRACK_UP && (startLineTrack > 0))
	    startLineTrack--;

    }
    while (button != BUTTON_OK);
}

void debugger::addLine(char *buffer, scriptData * script)
{
    script->lines = (char **) realloc(script->lines, sizeof(char *) * (script->numOfLignes + 1));

    script->lines[script->numOfLignes] = (char *) malloc(strlen(buffer) + 1);
    strcpy(script->lines[script->numOfLignes], buffer);
    script->numOfLignes++;
}

scriptData *debugger::getActorTrackScript(int num)
{
    unsigned char *scriptPtr;
    int finish = 0;
    unsigned char opcode;
    scriptData *script;
    char buffer[256];

    script = (scriptData *) malloc(sizeof(scriptData));

    script->lines = 0;
    script->numOfLignes = 0;

    scriptPtr = engine->actors[num].moveScript;

    do {
	opcode = *(scriptPtr++);
	switch (opcode) {
	case 0:
	    {
		sprintf(buffer, "END");
		addLine(buffer, script);
		finish = 1;
		break;
	    }
	case 2:
	    {
		sprintf(buffer, "change body %d", *(scriptPtr++));
		addLine(buffer, script);
		break;
	    }
	case 3:
	    {
		sprintf(buffer, "ANIM %d", *(scriptPtr++));	// bleu
		addLine(buffer, script);
		break;
	    }
	case 4:
	    {
		sprintf(buffer, "GOTO_POINT %d", *(scriptPtr++));
		addLine(buffer, script);
		break;
	    }
	case 5:
	    {
		sprintf(buffer, "WAIT_ANIM");
		addLine(buffer, script);
		break;
	    }
	case 7:
	    {
		short int temp;

		temp = *(short int *) (scriptPtr);

		scriptPtr += 2;

		sprintf(buffer, "ANGLE %d", temp);
		addLine(buffer, script);
		break;
	    }
	case 8:
	    {
		sprintf(buffer, "warp to %d", *(scriptPtr++));
		addLine(buffer, script);
		break;
	    }
	case 9:
	    {
		sprintf(buffer, "LABEL %d", (*scriptPtr++));	// rouge
		addLine(buffer, script);
		break;
	    }
	case 10:
	    {
		short int temp;

		temp = *(short int *) (scriptPtr);

		scriptPtr += 2;

		temp = *(engine->actors[num].moveScript + temp + 1);

		sprintf(buffer, "jump %d", temp);
		addLine(buffer, script);
		break;
	    }
	case 11:
	    {
		sprintf(buffer, "STOP");
		addLine(buffer, script);
		break;
	    }
	case 12:
	    {
		sprintf(buffer, "Walk backward to %d", *(scriptPtr++));
		addLine(buffer, script);
		break;
	    }
	case 13:
	    {
		sprintf(buffer, "opcode 13 %d %d", *(scriptPtr++), *(scriptPtr++));
		addLine(buffer, script);
		break;
	    }
	case 14:
	    {
		short int temp;

		temp = *(short int *) (scriptPtr);

		scriptPtr += 2;

		sprintf(buffer, "SAMPLE %d", temp);	// bleu
		addLine(buffer, script);
		break;
	    }
	case 17:
	    {
		sprintf(buffer, "flag op 17: %d", *(scriptPtr++));
		addLine(buffer, script);
		break;
	    }
	case 18:
	    {
		int temp1;
		int temp2;

		temp1 = *(int *) (scriptPtr);
		temp2 = *(int *) (scriptPtr);

		scriptPtr += 4;
		scriptPtr += 4;

		sprintf(buffer, "wait %d %d", temp1, temp2);
		addLine(buffer, script);
		break;
	    }
	case 19:
	    {
		sprintf(buffer, "destroy body");
		addLine(buffer, script);
		break;
	    }
	case 21:
	    {
		short int temp;

		temp = *(short int *) (scriptPtr);

		scriptPtr += 2;

		sprintf(buffer, "direction 1 - %d", script, temp);
		addLine(buffer, script);
		break;
	    }
	case 22:
	    {
		short int temp;

		temp = *(short int *) (scriptPtr);

		scriptPtr += 2;

		sprintf(buffer, "direction 2 - %d", script, temp);
		addLine(buffer, script);
		break;
	    }
	case 23:
	    {
		short int temp;

		temp = *(short int *) (scriptPtr);

		scriptPtr += 2;

		sprintf(buffer, "direction 3 - %d", script, temp);
		addLine(buffer, script);
		break;
	    }
	case 24:
	    {
		short int temp;

		temp = *(short int *) (scriptPtr);

		scriptPtr += 2;

		sprintf(buffer, "direction 4 - %d", script, temp);
		addLine(buffer, script);
		break;
	    }

	case 25:
	    {
		sprintf(buffer, "Unsuported opcode 25");
		addLine(buffer, script);
		break;
	    }
	case 26:
	    {
		sprintf(buffer, "Unsuported opcode 26");
		addLine(buffer, script);
		break;
	    }
	case 33:
	    {
		sprintf(buffer, "angle toward %d %d", *(scriptPtr++), *(scriptPtr++));
		addLine(buffer, script);
		break;
	    }
	default:
	    {
		sprintf(buffer, "Unknown opcode %d", opcode);
		addLine(buffer, script);
		finish = 1;
		break;
	    }
	}
    }
    while (!finish);

    return (script);
}

scriptData *debugger::getActorComScript(int num)
{
    unsigned char *scriptPtr;
    int finish = 0;
    unsigned char opcode;
    scriptData *script;
    char buffer[256];
    char buffer2[256];

    script = (scriptData *) malloc(sizeof(scriptData));

    script->lines = 0;
    script->numOfLignes = 0;

    scriptPtr = engine->actors[num].actorScript;

    do {
	opcode = *(scriptPtr++);
	switch (opcode) {
	case 0:
	    {
		sprintf(buffer, "END");
		addLine(buffer, script);
		finish = 1;
		break;
	    }
	case 3:
	    {
		short int temp;

		temp = *(short int *) scriptPtr;
		scriptPtr += 2;

		sprintf(buffer, "jump2 %d", temp);
		addLine(buffer, script);
		break;
	    }
	case 4:
	    {
		sprintf(buffer, "comportement: ");
		manipActor(&scriptPtr, buffer);
		doCalc(&scriptPtr, buffer);
		addLine(buffer, script);
		break;
	    }
	case 10:
	    {
		scriptPtr++;
		sprintf(buffer, "10:ignore 1 byte");
		addLine(buffer, script);
		break;
	    }
	case 11:
	    {
		sprintf(buffer, "break once");
		addLine(buffer, script);
		break;
	    }
	case 12:
	    {
		short int temp;

		sprintf(buffer, "if ");
		manipActor(&scriptPtr, buffer);
		doCalc(&scriptPtr, buffer);
		temp = *(short int *) scriptPtr;
		scriptPtr += 2;
		sprintf(buffer2, " goto %d", temp);
		strcat(buffer, buffer2);
		addLine(buffer, script);
		break;
	    }
	case 13:
	    {
		short int temp;

		sprintf(buffer, "if3 ");
		manipActor(&scriptPtr, buffer);
		doCalc(&scriptPtr, buffer);
		temp = *(short int *) scriptPtr;
		scriptPtr += 2;
		sprintf(buffer2, " goto %d", temp);
		strcat(buffer, buffer2);
		addLine(buffer, script);
		break;
	    }
	case 14:
	    {
		short int temp;

		sprintf(buffer, "if2 ");
		manipActor(&scriptPtr, buffer);
		doCalc(&scriptPtr, buffer);
		temp = *(short int *) scriptPtr;
		scriptPtr += 2;
		sprintf(buffer2, " goto %d", temp);
		strcat(buffer, buffer2);
		addLine(buffer, script);
		break;
	    }
	case 15:
	    {
		short int temp;

		temp = *(short int *) scriptPtr;

		scriptPtr += 2;

		sprintf(buffer, "opcode 15 (jump): %d", temp);
		addLine(buffer, script);
		break;
	    }
	case 17:
	    {
		sprintf(buffer, "load costume %d", *(scriptPtr++));
		addLine(buffer, script);
		break;
	    }
	case 18:
	    {
		char temp1;
		char temp2;

		temp1 = *(scriptPtr++);
		temp2 = *(scriptPtr++);

		sprintf(buffer, "load costume %d for actor %d", temp2, temp1);
		addLine(buffer, script);
		break;
	    }
	case 19:
	    {
		sprintf(buffer, "play anim %d", *(scriptPtr++));
		addLine(buffer, script);
		break;
	    }
	case 20:
	    {
		char temp1;
		char temp2;

		temp1 = *(scriptPtr++);
		temp2 = *(scriptPtr++);

		sprintf(buffer, "play anim %d for actor %d", temp2, temp1);
		addLine(buffer, script);
		break;
	    }
	case 23:
	    {
		short int temp;

		temp = *(short int *) scriptPtr;

		scriptPtr += 2;

		temp = *(engine->actors[num].moveScript + temp + 1);

		sprintf(buffer, "SET_TRACK %d", temp);
		addLine(buffer, script);
		break;
	    }
	case 24:
	    {
		char temp1;
		short int temp2;

		temp1 = *(scriptPtr++);
		temp2 = *(short int *) scriptPtr;

		scriptPtr += 2;

		temp2 = *(engine->actors[num].moveScript + temp2 + 1);

		sprintf(buffer, "SET_TRACK %d for actor %d", temp2, temp1);
		addLine(buffer, script);
		break;
	    }
	case 25:
	    {
		short int temp;

		temp = *(short int *) scriptPtr;

		scriptPtr += 2;

		sprintf(buffer, "say dialogue %d", temp);
		addLine(buffer, script);
		break;
	    }
	case 27:
	    {
		char temp;
		char temp2;

		temp = *scriptPtr++;

		if (temp == 2) {
		    temp2 = *scriptPtr++;
		    sprintf(buffer, "SET_COMPORTEMENT %d follow %d", temp, temp2);
		} else {
		    sprintf(buffer, "SET_COMPORTEMENT %d", temp);
		}

		addLine(buffer, script);
		break;
	    }
	case 29:
	    {
		sprintf(buffer, "setActorCamera %d", *(scriptPtr++));
		addLine(buffer, script);
		break;
	    }
	case 31:
	    {
		unsigned char temp1;
		unsigned char temp2;

		temp1 = *(scriptPtr++);
		temp2 = *(scriptPtr++);

		sprintf(buffer, "roomData1[%d]=%d", temp1, temp2);
		addLine(buffer, script);
		break;
	    }
	case 33:
	    {
		short int temp;

		temp = *(short int *) scriptPtr;

		scriptPtr += 2;

		sprintf(buffer, "jump %d", temp);
		addLine(buffer, script);
		break;
	    }
	case 35:
	    {
		addLine("stop", script);
		break;
	    }
	case 38:
	    {
		addLine("remove from room", script);
		break;
	    }
	case 55:
	    {
		short int temp;

		sprintf(buffer, "if4 ");
		manipActor(&scriptPtr, buffer);
		doCalc(&scriptPtr, buffer);
		temp = *(short int *) scriptPtr;
		scriptPtr += 2;
		sprintf(buffer2, " goto %d", temp);
		strcat(buffer, buffer2);
		addLine(buffer, script);
		break;
	    }
	case 57:
	    {
		sprintf(buffer, "opcode 57: %d", *(scriptPtr++));
		addLine(buffer, script);
		break;
	    }
	case 58:
	    {
		sprintf(buffer, "warp at flag %d", *(scriptPtr++));
		addLine(buffer, script);
		break;
	    }
	case 70:
	    {
		short int temp;

		temp = *(short int *) scriptPtr;

		scriptPtr += 2;

		sprintf(buffer, "sayText: %d", temp);
		addLine(buffer, script);
		break;
	    }
	default:
	    {
		sprintf(buffer, "Unknown opcode %d", opcode);
		addLine(buffer, script);
		finish = 1;
		break;
	    }
	}
    }
    while (!finish);

    return (script);
}

void debugger::manipActor(unsigned char **scriptPtr, char *buffer)
{
    unsigned char opcode;
    char buffer2[256];

    opcode = **(scriptPtr);

    *(scriptPtr) = (*(scriptPtr)) + 1;

    manipActorVar1 = 0;

    switch (opcode) {
    case 0:
	{
	    strcat(buffer, "field_56");
	    break;
	}
    case 1:
	char temp;

	temp = **scriptPtr;
	*(scriptPtr) = (*(scriptPtr)) + 1;

	sprintf(buffer2, "actor[%d].field_56", temp);
	strcat(buffer, buffer2);
	break;
    case 2:
	{
	    char temp;

	    temp = **scriptPtr;
	    *(scriptPtr) = (*(scriptPtr)) + 1;

	    sprintf(buffer2, "distance to actor %d", temp);
	    strcat(buffer, buffer2);
	    manipActorVar1 = 1;
	    break;
	}
    case 3:
	{
	    strcat(buffer, "field_54");
	    break;
	}
    case 4:
	{
	    char temp;

	    temp = **scriptPtr;
	    *(scriptPtr) = (*(scriptPtr)) + 1;

	    sprintf(buffer2, "actor[%d].field_5A", temp);
	    strcat(buffer, buffer2);
	    break;
	}
    case 5:
	{
	    strcat(buffer, "field_0");
	    break;
	}
    case 6:
	{
	    char temp;

	    temp = **scriptPtr;
	    *(scriptPtr) = (*(scriptPtr)) + 1;

	    sprintf(buffer2, "actor[%d].field_0", temp);
	    strcat(buffer, buffer2);
	    break;
	}
    case 7:
	{
	    strcat(buffer, "costume");
	    break;
	}
    case 8:
	{
	    char temp;

	    temp = **scriptPtr;
	    *(scriptPtr) = (*(scriptPtr)) + 1;

	    sprintf(buffer2, "actor[%d].costume", temp);
	    strcat(buffer, buffer2);
	    break;
	}
    case 9:
	{
	    strcat(buffer, "label");
	    break;
	}
    case 10:
	{
	    char temp;

	    temp = **scriptPtr;
	    *(scriptPtr) = (*(scriptPtr)) + 1;

	    sprintf(buffer2, "actor[%d].label", temp);
	    strcat(buffer, buffer2);
	    break;
	}
    case 11:
	{
	    char temp;

	    temp = **scriptPtr;
	    *(scriptPtr) = (*(scriptPtr)) + 1;
	    sprintf(buffer2, "roomData1[%d]", temp);
	    strcat(buffer, buffer2);
	    break;
	}
    case 12:
	{
	    char temp;

	    temp = **scriptPtr;
	    *(scriptPtr) = (*(scriptPtr)) + 1;

	    sprintf(buffer2, "angle to actor %d", temp);
	    strcat(buffer, buffer2);
	    manipActorVar1 = 1;
	    break;
	}
    case 13:
	{
	    char temp;

	    temp = **scriptPtr;
	    *(scriptPtr) = (*(scriptPtr)) + 1;

	    sprintf(buffer2, "actor[%d].field_64", temp);
	    strcat(buffer, buffer2);
	    break;
	}
    case 14:
	{
	    strcat(buffer, "updateActorScript");
	    break;
	}
    case 15:
	{
	    sprintf(buffer2, "VAR_GAME %d", **scriptPtr);
	    *(scriptPtr) = (*(scriptPtr)) + 1;
	    strcat(buffer, buffer2);
	    break;
	}
    case 16:
	{
	    strcat(buffer, "life");
	    break;
	}
    case 17:
	{
	    char temp;

	    temp = **scriptPtr;
	    *(scriptPtr) = (*(scriptPtr)) + 1;

	    sprintf(buffer2, "actor[%d].life", temp);
	    strcat(buffer, buffer2);
	    break;
	}
    case 18:
	{
	    strcat(buffer, "numKey");
	    break;
	}
    case 19:
	{
	    strcat(buffer, "numCoin");
	    manipActorVar1 = 1;
	    break;
	}
    case 21:
	{
	    strcat(buffer, "newGameVar2");
	    break;
	}
       // 22 -> manipActorVar1=1;
       // 25 -> manipActorVar1=1;
    default:
	{
	    printf("manip actor %d\n", opcode);
	    exit(1);
	}
    }
}

void debugger::doCalc(unsigned char **scriptPtr, char *buffer)
{
    unsigned char opcode;
    int opcode2;
    char buffer2[256];

    opcode = **(scriptPtr);

    *(scriptPtr) = (*(scriptPtr)) + 1;

    switch (manipActorVar1) {
    case 0:
	{
	    opcode2 = **scriptPtr;
	    *(scriptPtr) = (*(scriptPtr)) + 1;
	    break;
	}
    case 1:
	{
	    opcode2 = *((short int *) (*scriptPtr));
	    *(scriptPtr) = (*(scriptPtr)) + 2;
	    break;
	}
    default:
	{
	    printf("Bad mode in docalc!\n");
	    exit(1);
	    break;
	}
    }

    switch (opcode) {
    case 0:
	{
	    sprintf(buffer2, " == %d", opcode2);
	    strcat(buffer, buffer2);
	    break;
	}
    case 1:
	{
	    sprintf(buffer2, " > %d", opcode2);
	    strcat(buffer, buffer2);
	    break;
	}
    case 2:
	{
	    sprintf(buffer2, " < %d", opcode2);
	    strcat(buffer, buffer2);
	    break;
	}
    case 3:
	{
	    sprintf(buffer2, " >= %d", opcode2);
	    strcat(buffer, buffer2);
	    break;
	}
    case 4:
	{
	    sprintf(buffer2, " <= %d", opcode2);
	    strcat(buffer, buffer2);
	    break;
	}
    case 5:
	{
	    sprintf(buffer2, " != %d", opcode2);
	    strcat(buffer, buffer2);
	    break;
	}
    default:
	{
	    printf("doCalc default: %d\n", opcode);
	    exit(1);
	    break;
	}
    }
}
