#include "lba.h"


int manipActorVar1 = 0;

int debugger::processDebug(void)
{
    mouseStatusStruct mouseData;
    int actorNum;

    osystem->getMouseStatus(&mouseData);

   // printf("Mouse: %d %d - %d %d\n",mouseData.X,mouseData.Y,mouseData.left,mouseData.right);
   // printf("Num Actors: %d\n",numOfActorOnScreen);

    actorNum = findActor(mouseData.X, mouseData.Y);

    numOfWindows = 0;

    if (actorNum != -1 && mouseData.left)
	{
	    debugActor(actorNum);
	    engine->fullRedraw(1);
	}

    return (0);
}

int debugger::findActor(int X, int Y)
{
    int i;

    for (i = numOfActorOnScreen; i >= 0; i--)
	{
	    if (inBox
		(X, Y, actorBox[i].top, actorBox[i].left, actorBox[i].bottom, actorBox[i].right))
		{
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

    for (i = 0; i < height; i++)
	{
	    for (j = 0; j < width; j++)
		{
		    *(ptr++) = 72;
		}
	    ptr += offset;
	}

   // osystem->drawBufferToScreen(engine->videoBuffer1);
   // osystem->refresh(engine->videoBuffer1,0,0,640,480);

}

void debugger::fillArea2(int X, int Y, int width, int height, char color)
{
    int i, j;
    unsigned char *ptr;
    int offset;

    ptr = engine->videoBuffer1 + engine->screenLockupTable[Y] + X;
    offset = 640 - (width);

    for (i = 0; i < height; i++)
	{
	    for (j = 0; j < width; j++)
		{
		    *(ptr++) = color;
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

void debugger::addButton(int winIndex, int X, int Y, int width, int height, char *text,
			 buttonType type)
{
    windows[winIndex].buttons[windows[winIndex].numOfButtons].X = X;
    windows[winIndex].buttons[windows[winIndex].numOfButtons].Y = Y;
    windows[winIndex].buttons[windows[winIndex].numOfButtons].Width = width;
    windows[winIndex].buttons[windows[winIndex].numOfButtons].Height = height;
    windows[winIndex].buttons[windows[winIndex].numOfButtons].type = type;
	windows[winIndex].buttons[windows[winIndex].numOfButtons].color = 70;

    windows[winIndex].buttons[windows[winIndex].numOfButtons].text =
	(char *) malloc(strlen(text) + 1);

    strcpy(windows[winIndex].buttons[windows[winIndex].numOfButtons].text, text);

    windows[winIndex].numOfButtons++;
}

void debugger::addButton(int winIndex, int X, int Y, int width, int height, char *text,
			 buttonType type, char boolVar)
{
    windows[winIndex].buttons[windows[winIndex].numOfButtons].X = X;
    windows[winIndex].buttons[windows[winIndex].numOfButtons].Y = Y;
    windows[winIndex].buttons[windows[winIndex].numOfButtons].Width = width;
    windows[winIndex].buttons[windows[winIndex].numOfButtons].Height = height;
    windows[winIndex].buttons[windows[winIndex].numOfButtons].type = type;
	if(boolVar)
		windows[winIndex].buttons[windows[winIndex].numOfButtons].color = 100;
	else
		windows[winIndex].buttons[windows[winIndex].numOfButtons].color = 70;

    windows[winIndex].buttons[windows[winIndex].numOfButtons].text =
	(char *) malloc(strlen(text) + 1);

    strcpy(windows[winIndex].buttons[windows[winIndex].numOfButtons].text, text);

    windows[winIndex].numOfButtons++;
}


buttonType debugger::findButton(int X, int Y)
{
    int i;
    int j;

    for (i = 0; i < numOfWindows; i++)
	{
	    for (j = 0; j < windows[i].numOfButtons; j++)
		{
		    if (X > (windows[i].X + windows[i].buttons[j].X)
			&& X <
			(windows[i].X + windows[i].buttons[j].X + windows[i].buttons[j].Width)
			&& Y > (windows[i].Y + windows[i].buttons[j].Y)
			&& Y <
			(windows[i].Y + windows[i].buttons[j].Y + windows[i].buttons[j].Height))
			{
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

    do
	{
	    engine->readKeyboard();
	    osystem->getMouseStatus(&mouseData);

	    if (mouseData.left)
		{
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

    for (i = 0; i < numOfWindows; i++)
	{
	    int X;
	    int Y;

	    X = windows[i].X;
	    Y = windows[i].Y;

	    fillArea(X, Y, windows[i].Width, windows[i].Height);

	    for (j = 0; j < windows[i].numOfButtons; j++)
		{
			if(windows[i].buttons[j].type!=BUTTON_INFO)
				fillArea2(X + windows[i].buttons[j].X, Y + windows[i].buttons[j].Y,windows[i].buttons[j].Width, windows[i].buttons[j].Height,windows[i].buttons[j].color);
		    osystem->drawText(X + windows[i].buttons[j].X, Y + windows[i].buttons[j].Y,windows[i].buttons[j].text);
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
	char string[256];

   // printf("Debug actor %d\n",num);

    scriptData *comScript;
    scriptData *trackScript;

    comScript = getActorComScript(num);
    trackScript = getActorTrackScript(num);

    do
	{
		char fallable;
		char noshado;
		char backgrd;
		char carrier;
		char zonable;
		char objcol;
		char brick;
		char no_col;
		char clip;
		char no_aff;
		char minizy;
		char pushable;
		char codejeu;
		char no_choc;
		char noclip;

		objcol=		engine->actors[num].field_60&0x0001;
		zonable=	engine->actors[num].field_60&0x0004;
		no_aff=		engine->actors[num].field_60&0x0200;
		fallable=	engine->actors[num].field_60&0x0800;
		noshado=	engine->actors[num].field_60&0x1000;
		backgrd=	engine->actors[num].field_60&0x2000;

		

	    numOfWindows = 0;
	    addWin(0, 420, 145, 220, 300);	// track win
	    addButton(0, 205, 15, 15, 140, "U", BUTTON_TRACK_UP);
	    addButton(0, 205, 156, 15, 140, "D", BUTTON_TRACK_DOWN);

	    addWin(1, 0, 145, 418, 300);	// comp win
	    addButton(1, 403, 15, 15, 140, "U", BUTTON_COMP_UP);
	    addButton(1, 403, 156, 15, 140, "D", BUTTON_COMP_DOWN);

	    addWin(2, 0, 447, 640, 32);	// bottom status win
	    addButton(2, 0, 15, 640, 15, "OK", BUTTON_OK);

		addWin(3, 0, 0, 527, 80); // status win
		addButton(3, 0, 0, 90, 15, "Name", BUTTON_GEN);

		addButton(3, 0, 16, 15, 15, "-", BUTTON_GEN);
		sprintf(string,"X: %d",engine->actors[num].X);
		addButton(3, 16, 16, 70, 15, string, BUTTON_GEN);
		addButton(3, 87, 16, 15, 15, "+", BUTTON_GEN);

		addButton(3, 0, 32, 15, 15, "-", BUTTON_GEN);
		sprintf(string,"Y: %d",engine->actors[num].Z); // TODO: inverse Y/Z
		addButton(3, 16, 32, 70, 15, string, BUTTON_GEN);
		addButton(3, 87, 32, 15, 15, "+", BUTTON_GEN);

		addButton(3, 0, 48, 15, 15, "-", BUTTON_GEN);
		sprintf(string,"Z: %d",engine->actors[num].Y);
		addButton(3, 16, 48, 70, 15, string, BUTTON_GEN); // TODO: inverse Y/Z
		addButton(3, 87, 48, 15, 15, "+", BUTTON_GEN);

		addButton(3, 103, 16, 40, 15, "Angl", BUTTON_INFO);
		addButton(3, 144, 16, 15, 15, "-", BUTTON_GEN);
		sprintf(string,"%d",engine->actors[num].angle);
		addButton(3, 160, 16, 35, 15, string, BUTTON_GEN);
		addButton(3, 196, 16, 15, 15, "+", BUTTON_GEN);

		addButton(3, 103, 32, 40, 15, "Vite", BUTTON_INFO);
		addButton(3, 144, 32, 15, 15, "-", BUTTON_GEN);
		sprintf(string,"?");
		addButton(3, 160, 32, 35, 15, string, BUTTON_GEN);
		addButton(3, 196, 32, 15, 15, "+", BUTTON_GEN);

		addButton(3, 103, 48, 40, 15, "Pav", BUTTON_INFO);
		addButton(3, 144, 48, 15, 15, "-", BUTTON_GEN);
		sprintf(string,"?");
		addButton(3, 160, 48, 35, 15, string, BUTTON_GEN);
		addButton(3, 196, 48, 15, 15, "+", BUTTON_GEN);

		addButton(3, 95, 64, 48, 15, "Armure", BUTTON_INFO);
		addButton(3, 144, 64, 15, 15, "-", BUTTON_GEN);
		sprintf(string,"?");
		addButton(3, 160, 64, 35, 15, string, BUTTON_GEN);
		addButton(3, 196, 64, 15, 15, "+", BUTTON_GEN);

		addButton(3, 212,  0, 50, 15, "MONEY", BUTTON_GEN);
		addButton(3, 212, 16, 50, 15, "LIFE", BUTTON_GEN);
		addButton(3, 212, 32, 50, 15, "MAGIC", BUTTON_GEN);
		addButton(3, 212, 48, 50, 15, "KEY", BUTTON_GEN);
		addButton(3, 212, 64, 50, 15, "CLOVE", BUTTON_GEN);

		addButton(3, 263 , 0, 65, 15, "FALLABLE", BUTTON_GEN,fallable);
		addButton(3, 263, 16, 65, 15, "NOSHADO", BUTTON_GEN,noshado);
		addButton(3, 263, 32, 65, 15, "BACKGRD", BUTTON_GEN,backgrd);
		addButton(3, 263, 48, 65, 15, "CARRIER", BUTTON_GEN);

		addButton(3, 329,  0, 65, 15, "ZONABLE", BUTTON_GEN,zonable);
		addButton(3, 329, 16, 65, 15, "OBJCOL", BUTTON_GEN,objcol);
		addButton(3, 329, 32, 65, 15, "BRICK", BUTTON_GEN);
		addButton(3, 329, 48, 65, 15, "NO_COL", BUTTON_GEN);
		addButton(3, 329, 64, 65, 15, "CLIP", BUTTON_GEN);

		addButton(3, 395,  0, 65, 15, "NO_AFF", BUTTON_GEN,no_aff);
		addButton(3, 395, 16, 65, 15, "MINIZY", BUTTON_GEN);
		addButton(3, 395, 32, 65, 15, "PUSHABLE", BUTTON_GEN);
		addButton(3, 395, 48, 65, 15, "CODE JEU", BUTTON_GEN);
		addButton(3, 395, 64, 65, 15, "NO_CHOC", BUTTON_GEN);

		addButton(3, 461, 16, 65, 15, "NOCLIP", BUTTON_GEN);
		addButton(3, 461, 32, 65, 15, "ZBUFFER", BUTTON_GEN);
		addButton(3, 461, 48, 65, 15, "MESSAGES", BUTTON_GEN);
		addButton(3, 461, 64, 65, 15, "Free", BUTTON_GEN);


		addWin(4, 0, 82, 200, 61); // file win

	    drawAll();

	    numOfLines = 25;
	    Y = 150;

	    if (trackScript->numOfLignes < 25)
		numOfLines = trackScript->numOfLignes;

	    for (i = startLineTrack; i < (numOfLines + startLineTrack); i++)
		{
		    osystem->drawText(425, Y, trackScript->lines[i]);
		    Y += 11;
		}

	    numOfLines = 25;
	    Y = 150;

	    numOfLines = comScript->numOfLignes - startLineCom;
	    if (numOfLines > 25)
		numOfLines = 25;

	    for (i = startLineCom; i < (numOfLines + startLineCom); i++)
		{
		    osystem->drawText(5, Y, comScript->lines[i]);
		    Y += 11;
		}

	    osystem->drawBufferToScreen(engine->videoBuffer1);

	    button = processInput();

	    if (button == BUTTON_TRACK_DOWN && ((startLineTrack + 25) < trackScript->numOfLignes))
		startLineTrack++;

	    if (button == BUTTON_TRACK_UP && (startLineTrack > 0))
		startLineTrack--;

	    if (button == BUTTON_COMP_DOWN && ((startLineCom + 25) < comScript->numOfLignes))
		{
		    startLineCom += 25;
		    if (startLineCom > comScript->numOfLignes)
			startLineCom = comScript->numOfLignes;
		}

	    if (button == BUTTON_COMP_UP && (startLineCom > 0))
		startLineCom -= 25;

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

    do
	{
	    opcode = *(scriptPtr++);
	    switch (opcode)
		{
		case 0:
		   {
		       sprintf(buffer, "END");
		       addLine(buffer, script);
		       finish = 1;
		       break;
		   }
		case 1:
		   {
		       addLine("NOP", script);
		       break;
		   }
		case 2:
		   {
		       sprintf(buffer, "BODY %d", *(scriptPtr++));
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
		case 6:
		   {
		       addLine("LOOP", script);
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
		       sprintf(buffer, "POS_POINT %d", *(scriptPtr++));
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

		       sprintf(buffer, "GOTO %d", temp);
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
		       sprintf(buffer, "GOTO_SYM_POINT %d", *(scriptPtr++));
		       addLine(buffer, script);
		       break;
		   }
		case 13:
		   {
		       sprintf(buffer, "WAIT_NB_ANIM %d %d", *(scriptPtr++), *(scriptPtr++));
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
		case 15:
		   {
		       sprintf(buffer, "GOTO_POINT_3D %d", *(scriptPtr++));
		       addLine(buffer, script);
		       break;
		   }
		case 16:
		   {
		       short int temp;

		       temp = *(short int *) scriptPtr;
		       scriptPtr += 2;

		       sprintf(buffer, "SPEED %d", temp);
		       addLine(buffer, script);
		       break;
		   }
		case 17:
		   {
		       sprintf(buffer, "BACKGROUND %d", *(scriptPtr++));
		       addLine(buffer, script);
		       break;
		   }
		case 18:
		   {
		       short int temp1;
		       short int temp2;
			   char temp3;

				temp3 =*(scriptPtr++);

		       temp1 = *(int *) (scriptPtr);
			   scriptPtr += 2;

		       temp2 = *(int *) (scriptPtr);
			   scriptPtr+=2;

		       sprintf(buffer, "WAIT_NB_SECOND %d %d %d", temp1, temp2,temp3);
		       addLine(buffer, script);
		       break;
		   }
		case 19:
		   {
		       sprintf(buffer, "NO_BODY");
		       addLine(buffer, script);
		       break;
		   }
		case 20:
		   {
		       short int temp;

		       temp = *(short int *) scriptPtr;

		       scriptPtr += 2;

		       sprintf(buffer, "BETA %d", temp);
		       addLine(buffer, script);
		       break;
		   }
		case 21:
		   {
		       short int temp;

		       temp = *(short int *) (scriptPtr);

		       scriptPtr += 2;

		       sprintf(buffer, "OPEN_LEFT %d", script, temp);
		       addLine(buffer, script);
		       break;
		   }
		case 22:
		   {
		       short int temp;

		       temp = *(short int *) (scriptPtr);

		       scriptPtr += 2;

		       sprintf(buffer, "OPEN_RIGHT %d", script, temp);
		       addLine(buffer, script);
		       break;
		   }
		case 23:
		   {
		       short int temp;

		       temp = *(short int *) (scriptPtr);

		       scriptPtr += 2;

		       sprintf(buffer, "OPEN_UP %d", script, temp);
		       addLine(buffer, script);
		       break;
		   }
		case 24:
		   {
		       short int temp;

		       temp = *(short int *) (scriptPtr);

		       scriptPtr += 2;

		       sprintf(buffer, "OPEN_DOWN %d", script, temp);
		       addLine(buffer, script);
		       break;
		   }

		case 25:
		   {
		       sprintf(buffer, "CLOSE");
		       addLine(buffer, script);
		       break;
		   }
		case 26:
		   {
		       sprintf(buffer, "WAIT_DOOR");
		       addLine(buffer, script);
		       break;
		   }
		case 27:
		   {
		       short int temp;

		       temp = *(short int *) scriptPtr;
		       scriptPtr += 2;
		       sprintf(buffer, "SAMPLE_RND %d", temp);
		       addLine(buffer, script);
		       break;
		   }
		case 28:
		   {
		       short int temp;

		       temp = *(short int *) scriptPtr;
		       scriptPtr += 2;
		       sprintf(buffer, "SAMPLE_ALWAYS %d", temp);
		       addLine(buffer, script);
		       break;
		   }
		case 29:
		   {
		       short int temp;

		       temp = *(short int *) scriptPtr;
		       scriptPtr += 2;
		       sprintf(buffer, "SAMPLE_STOP %d", temp);
		       addLine(buffer, script);
			   break;
		   }
		case 30:
			{
				sprintf(buffer,"PLAY_FLA %s",scriptPtr);
				scriptPtr+=strlen((char*)scriptPtr)+1;
				addLine(buffer,script);
				break;
			}
		case 31:
			{
				short int temp;

				temp= *(short int*)scriptPtr;
				scriptPtr+=2;
				sprintf(buffer,"REPEAT_SAMPLE %d",temp);
				addLine(buffer,script);
				break;
			}
		case 32:
			{
				short int temp;

				temp=*(short int*)scriptPtr;
				scriptPtr+=2;
				sprintf(buffer,"SIMPLE_SAMPLE %d",temp);
				addLine(buffer,script);
				break;
			}
		case 33:
		   {
			   short int temp;

			   temp = *(short int*) scriptPtr;
			   scriptPtr+=2;
		       sprintf(buffer, "FACE_TWINKEL %d", temp);
		       addLine(buffer, script);
		       break;
		   }
		case 34:
			{
				short int temp1;
				short int temp2;

				temp1=*(short int*)scriptPtr;
				scriptPtr+=2;
				temp2=*(short int*)scriptPtr;
				scriptPtr+=2;
				sprintf(buffer, "ANGLE_RND %d %d",temp1,temp2);
				addLine(buffer,script);
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

    do
	{
	    sprintf(buffer, "%5d:    ", scriptPtr - engine->actors[num].actorScript);
	    opcode = *(scriptPtr++);
	    switch (opcode)
		{
		case 0:
		   {
		       strcat(buffer, "END");
		       addLine(buffer, script);
		       finish = 1;
		       break;
		   }
		case 1:
			{
				strcat(buffer, "NOP");
				addLine(buffer,script);
				break;
			}
		case 2:
		   {
		       short int temp;

		       strcat(buffer, "SNIF ");
		       manipActor(&scriptPtr, buffer);
		       doCalc(&scriptPtr, buffer);
		       temp = *(short int *) scriptPtr;
		       scriptPtr += 2;
		       sprintf(buffer2, " goto %d", temp);
		       strcat(buffer, buffer2);
		       addLine(buffer, script);
		       break;
		   }
		case 3:
		   {
		       short int temp;

		       temp = *(short int *) scriptPtr;
		       scriptPtr += 2;

		       sprintf(buffer2, "OFFSET %d", temp);
		       strcat(buffer, buffer2);
		       addLine(buffer, script);
		       break;
		   }
		case 4:
		   {
		       short int temp;

		       strcat(buffer, "NEVERIF ");
		       manipActor(&scriptPtr, buffer);
		       doCalc(&scriptPtr, buffer);

		       temp = *(short int *) scriptPtr;
		       scriptPtr += 2;

		       sprintf(buffer2, " goto %d", temp);
		       strcat(buffer, buffer2);

		       addLine(buffer, script);
		       break;
		   }
		case 10:
		   {
		       sprintf(buffer2, "LABEL %d",*(scriptPtr++));
		       strcat(buffer, buffer2);
		       addLine(buffer, script);
		       break;
		   }
		case 11:
		   {
		       strcat(buffer, "RETURN");
		       addLine(buffer, script);
		       break;
		   }
		case 12:
		   {
		       short int temp;

		       strcat(buffer, "IF ");
		       manipActor(&scriptPtr, buffer);
		       doCalc(&scriptPtr, buffer);
		       temp = *(short int *) scriptPtr;
		       scriptPtr += 2;
		       sprintf(buffer2, " else goto %d", temp);
		       strcat(buffer, buffer2);
		       addLine(buffer, script);
		       break;
		   }
		case 13:
		   {
		       short int temp;

		       strcat(buffer, "SWIF ");
		       manipActor(&scriptPtr, buffer);
		       doCalc(&scriptPtr, buffer);
		       temp = *(short int *) scriptPtr;
		       scriptPtr += 2;
		       sprintf(buffer2, " else goto %d", temp);
		       strcat(buffer, buffer2);
		       addLine(buffer, script);
		       break;
		   }
		case 14:
		   {
		       short int temp;

		       strcat(buffer, "ONEIF ");
		       manipActor(&scriptPtr, buffer);
		       doCalc(&scriptPtr, buffer);
		       temp = *(short int *) scriptPtr;
		       scriptPtr += 2;
		       sprintf(buffer2, " else goto %d", temp);
		       strcat(buffer, buffer2);
		       addLine(buffer, script);
		       break;
		   }
		case 15:
		   {
		       short int temp;

		       temp = *(short int *) scriptPtr;

		       scriptPtr += 2;

		       sprintf(buffer2, "ELSE %d", temp);
		       strcat(buffer, buffer2);
		       addLine(buffer, script);
		       break;
		   }
		case 17:
		   {
		       sprintf(buffer2, "BODY %d", *(scriptPtr++));
		       strcat(buffer, buffer2);
		       addLine(buffer, script);
		       break;
		   }
		case 18:
		   {
		       char temp1;
		       char temp2;

		       temp1 = *(scriptPtr++);
		       temp2 = *(scriptPtr++);

		       sprintf(buffer2, "BODY_OBJ %d %d", temp2, temp1);
		       strcat(buffer, buffer2);
		       addLine(buffer, script);
		       break;
		   }
		case 19:
		   {
		       sprintf(buffer2, "ANIM %d", *(scriptPtr++));
		       strcat(buffer, buffer2);
		       addLine(buffer, script);
		       break;
		   }
		case 20:
		   {
		       char temp1;
		       char temp2;

		       temp1 = *(scriptPtr++);
		       temp2 = *(scriptPtr++);

		       sprintf(buffer2, "ANIM_OBJ %d %d", temp2, temp1);
		       strcat(buffer, buffer2);
		       addLine(buffer, script);
		       break;
		   }
		case 21:
		   {
		       short int temp;

		       temp = *(short int *) scriptPtr;
		       scriptPtr += 2;

		       sprintf(buffer2, "SET_LIFE %d", temp);
		       strcat(buffer, buffer2);
		       addLine(buffer, script);
		       break;
		   }
		case 22:
		   {
		       char temp1;
		       short int temp2;

		       temp1 = *(scriptPtr++);
		       temp2 = *(short int *) scriptPtr;
		       scriptPtr += 2;

		       sprintf(buffer2, "SET_LIFE_OBJ %d %d", temp1, temp2);
		       strcat(buffer, buffer2);
		       addLine(buffer, script);
		       break;
		   }
		case 23:
		   {
		       short int temp;

		       temp = *(short int *) scriptPtr;

		       scriptPtr += 2;

		       temp = *(engine->actors[num].moveScript + temp + 1);

		       sprintf(buffer2, "SET_TRACK %d", temp);
		       strcat(buffer, buffer2);
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

		       sprintf(buffer2, "SET_TRACK_OBJ %d %d", temp2, temp1);
		       strcat(buffer, buffer2);
		       addLine(buffer, script);
		       break;
		   }
		case 25:
		   {
		       short int temp;

		       temp = *(short int *) scriptPtr;

		       scriptPtr += 2;

		       sprintf(buffer2, "MESSAGE %d", temp);
		       strcat(buffer, buffer2);
		       addLine(buffer, script);
		       break;
		   }
		case 26:
		   {
		       sprintf(buffer2, "FALLABLE %d", *(scriptPtr++));
		       strcat(buffer, buffer2);
		       addLine(buffer, script);
		       break;
		   }
		case 27:
		   {
		       char temp;
		       char temp2;

		       temp = *scriptPtr++;

		       if (temp == 2)
			   {
			       temp2 = *scriptPtr++;
			       sprintf(buffer2, "SET_DIR %d follow %d", temp, temp2);
			   }
		       else
			   {
			       sprintf(buffer2, "SET_DIR %d", temp);
			   }
		       strcat(buffer, buffer2);
		       addLine(buffer, script);
		       break;
		   }
		case 28:
		   {
		       char temp;
		       char temp2;
		       char temp3;

		       temp3 = *scriptPtr++;
		       temp = *scriptPtr++;

		       if (temp == 2)
			   {
			       temp2 = *scriptPtr++;
			       sprintf(buffer2, "SET_DIR_OBJ (actor %d) %d follow %d", temp3,
				       temp, temp2);
			   }
		       else
			   {
			       sprintf(buffer2, "SET_DIR_OBJ (actor %d) %d", temp3, temp);
			   }
		       strcat(buffer, buffer2);
		       addLine(buffer, script);
		       break;

		   }
		case 29:
		   {
		       sprintf(buffer2, "CAM_FOLLOW %d", *(scriptPtr++));
		       strcat(buffer, buffer2);
		       addLine(buffer, script);
		       break;
		   }
		case 30:
		   {
		       sprintf(buffer2, "COMPORTEMENT_HERO %d", *(scriptPtr++));
		       strcat(buffer, buffer2);
		       addLine(buffer, script);
		       break;
		   }
		case 31:
		   {
		       unsigned char temp1;
		       unsigned char temp2;

		       temp1 = *(scriptPtr++);
		       temp2 = *(scriptPtr++);

		       sprintf(buffer2, "SET_FLAG_CUBE %d %d", temp1, temp2);
		       strcat(buffer, buffer2);
		       addLine(buffer, script);
		       break;
		   }
		case 32:
		   {
		       scriptPtr++;
		       strcat(buffer, "COMPORTEMENT %d");
		       addLine(buffer, script);
		       break;
		   }
		case 33:
		   {
		       short int temp;

		       temp = *(short int *) scriptPtr;

		       scriptPtr += 2;

		       sprintf(buffer2, "SET_COMPORTEMENT %d", temp);
		       strcat(buffer, buffer2);
		       addLine(buffer, script);
		       break;
		   }
		case 34:
		   {
		       char temp1;
		       short int temp2;

		       temp1 = *(scriptPtr++);
		       temp2 = *(short int *) scriptPtr;

		       scriptPtr += 2;

		       sprintf(buffer2, "SET_COMPORTEMENT_OBJ %d %d", temp1, temp2);
		       strcat(buffer, buffer2);
		       addLine(buffer, script);
		       break;
		   }
		case 35:
		   {
		       strcat(buffer, "END_COMPORTEMENT");
		       strcat(buffer, buffer2);
		       break;
		   }
		case 36:
		   {
		       unsigned char temp1;
		       char temp2;

		       temp1 = *(scriptPtr++);
		       temp2 = *(scriptPtr++);

		       sprintf(buffer2, "SET_FLAG_GAME %d %d", temp1, temp2);
		       strcat(buffer, buffer2);
		       addLine(buffer, script);
		       break;
		   }
		case 37:
		   {
		       sprintf(buffer2, "KILL_OBJ %d", *(scriptPtr++));
		       strcat(buffer, buffer2);
		       addLine(buffer, script);
		       break;
		   }
		case 38:
		   {
		       strcat(buffer, "SUICIDE");
		       addLine(buffer, script);
		       break;
		   }
		case 39:
		   {
		       strcat(buffer, "USE_ONE_LITTLE_KEY");
		       addLine(buffer, script);
		       break;
		   }
		case 40:
		   {
		       short int temp;

		       temp = *(short int *) scriptPtr;
		       scriptPtr += 2;

		       sprintf(buffer2, "GIVE_GOLD_PIECES %d", temp);
		       strcat(buffer, buffer2);
		       addLine(buffer, script);
		       break;
		   }
		case 41:
		   {
		       strcat(buffer, "END_LIFE");
		       addLine(buffer, script);
		       break;
		   }
		case 42:
		   {
		       strcat(buffer, "STOP_L_TRACK");
		       addLine(buffer, script);
		       break;
		   }
		case 43:
		   {
		       strcat(buffer, "RESTORE_L_TRACK");
		       addLine(buffer, script);
		       break;
		   }
		case 44:
		   {
		       char temp1;
		       short int temp2;

		       temp1 = *(scriptPtr++);
		       temp2 = *(short int *) (scriptPtr);
		       scriptPtr += 2;

		       sprintf(buffer2, "MESSAGE_OBJ %d %d", temp1, temp2);
		       strcat(buffer, buffer2);
		       addLine(buffer, script);
		       break;
		   }
		case 45:
		   {
		       strcat(buffer, "INC_CHAPTER");
		       addLine(buffer, script);
		       break;
		   }
		case 46:
		   {
		       sprintf(buffer2, "FOUND_OBJECT %d", *(scriptPtr++));
		       strcat(buffer, buffer2);
		       addLine(buffer, script);
		       break;
		   }
		case 47:
		   {
		       short int temp;

		       temp = *(short int *) (scriptPtr++);
		       scriptPtr += 2;

		       sprintf(buffer2, "SET_DOOR_LEFT %d", temp);
		       strcat(buffer, buffer2);
		       addLine(buffer, script);
		       break;
		   }
		case 48:
		   {
		       short int temp;

		       temp = *(short int *) (scriptPtr++);
		       scriptPtr += 2;

		       sprintf(buffer2, "SET_DOOR_RIGHT %d", temp);
		       strcat(buffer, buffer2);
		       addLine(buffer, script);
		       break;
		   }
		case 49:
		   {
		       short int temp;

		       temp = *(short int *) (scriptPtr++);
		       scriptPtr += 2;

		       sprintf(buffer2, "SET_DOOR_UP %d", temp);
		       strcat(buffer, buffer2);
		       addLine(buffer, script);
		       break;
		   }
		case 50:
		   {
		       short int temp;

		       temp = *(short int *) (scriptPtr++);
		       scriptPtr += 2;

		       sprintf(buffer2, "SET_DOOR_DOWN %d", temp);
		       strcat(buffer, buffer2);
		       addLine(buffer, script);
		       break;
		   }
		case 51:
		   {
		       sprintf(buffer2, "GIVE_BONUS %d", *(scriptPtr++));
		       strcat(buffer, buffer2);
		       addLine(buffer, script);
		       break;
		   }
		case 52:
		   {
		       sprintf(buffer2, "CHANGE_CUBE %d", *(scriptPtr++));
		       strcat(buffer, buffer2);
		       addLine(buffer, script);
		       break;
		   }
		case 53:
		   {
		       sprintf(buffer2, "OBJ_COL %d", *(scriptPtr++));
		       strcat(buffer, buffer2);
		       addLine(buffer, script);
		       break;
		   }
		case 54:
		   {
		       sprintf(buffer2, "BRICK_COL %d", *(scriptPtr++));
		       strcat(buffer, buffer2);
		       addLine(buffer, script);
		       break;
		   }
		case 55:
		   {
		       short int temp;

		       strcat(buffer, "OR_IF ");
		       manipActor(&scriptPtr, buffer);
		       doCalc(&scriptPtr, buffer);
		       temp = *(short int *) scriptPtr;
		       scriptPtr += 2;
		       sprintf(buffer2, " goto %d", temp);
		       strcat(buffer, buffer2);
		       addLine(buffer, script);
		       break;
		   }
		case 56:
		   {
		       sprintf(buffer2, "INVISIBLE %d", *(scriptPtr++));
		       strcat(buffer, buffer2);
		       addLine(buffer, script);
		       break;
		   }
		case 57:
		   {
		       sprintf(buffer2, "ZOOM %d", *(scriptPtr++));
		       strcat(buffer, buffer2);
		       addLine(buffer, script);
		       break;
		   }
		case 58:
		   {
		       sprintf(buffer2, "POS_POINT %d", *(scriptPtr++));
		       strcat(buffer, buffer2);
		       addLine(buffer, script);
		       break;
		   }
		case 59:
		   {
		       sprintf(buffer2, "SET_MAGIC_LEVEL %d", *(scriptPtr++));
		       strcat(buffer, buffer2);
		       addLine(buffer, script);
		       break;
		   }
		case 60:
		   {
		       sprintf(buffer2, "SUB_MAGIC_POINT %d", *(scriptPtr++));
		       strcat(buffer, buffer2);
		       addLine(buffer, script);
		       break;
		   }
		case 61:
		   {
		       char temp1;
		       char temp2;

		       temp1 = *(scriptPtr++);
		       temp2 = *(scriptPtr++);

		       sprintf(buffer2, "SET_LIFE_POINT_OBJ %d %d", temp1, temp2);
		       strcat(buffer, buffer2);
		       addLine(buffer, script);
		       break;
		   }
		case 62:
		   {
		       char temp1;
		       char temp2;

		       temp1 = *(scriptPtr++);
		       temp2 = *(scriptPtr++);

		       sprintf(buffer2, "SUB_LIFE_POINT_OBJ %d %d", temp1, temp2);
		       strcat(buffer, buffer2);
		       addLine(buffer, script);
		       break;
		   }
		case 63:
		   {
		       char temp1;
		       char temp2;

		       temp1 = *(scriptPtr++);
		       temp2 = *(scriptPtr++);

		       sprintf(buffer2, "HIT_OBJ %d %d", temp1, temp2);
		       strcat(buffer, buffer2);
		       addLine(buffer, script);
		       break;
		   }
		case 64:
		   {
		       int temp = strlen((char *) scriptPtr);

		       sprintf(buffer2, "PLAY_FLA %s", (char *) scriptPtr);
		       strcat(buffer, buffer2);
		       addLine(buffer, script);
		       scriptPtr += temp + 1;
		       break;
		   }
		case 65:
		   {
		       sprintf(buffer2, "PLAY_MIDI %d", *(scriptPtr++));
		       strcat(buffer, buffer2);
		       addLine(buffer, script);
		       break;
		   }
		case 66:
		   {
		       strcat(buffer, "INC_CLOVER_BOX");
		       addLine(buffer, script);
		       break;
		   }
		case 67:
		   {
		       sprintf(buffer2, "SET_USED_INVENTORY %d", *(scriptPtr++));
		       strcat(buffer, buffer2);
		       addLine(buffer, script);
		       break;
		   }
		case 68:
		   {
		       short int temp;

		       temp = *(short int *) (scriptPtr);

		       scriptPtr += 2;

		       sprintf(buffer2, "ADD_CHOICE %d", temp);
		       strcat(buffer, buffer2);
		       addLine(buffer, script);
		       break;
		   }
		case 69:
		   {
		       strcat(buffer, "ASK_CHOICE");
		       addLine(buffer, script);
		       break;
		   }
		case 70:
		   {
		       short int temp;

		       temp = *(short int *) scriptPtr;

		       scriptPtr += 2;

		       sprintf(buffer2, "BIG_MESSAGE %d", temp);
		       strcat(buffer, buffer2);
		       addLine(buffer, script);
		       break;
		   }
		case 71:
		   {
		       sprintf(buffer2, "INIT_PINGOUIN %d", *(scriptPtr++));
		       strcat(buffer, buffer2);
		       addLine(buffer, script);
		       break;
		   }
		case 72:
		   {
		       sprintf(buffer2, "SET_HOLO_POS %d", *(scriptPtr++));
		       strcat(buffer, buffer2);
		       addLine(buffer, script);
		       break;
		   }
		case 73:
		   {
		       sprintf(buffer2, "CLR_HOLO_POS %d", *(scriptPtr++));
		       strcat(buffer, buffer2);
		       addLine(buffer, script);
		       break;
		   }
		case 74:
		   {
		       sprintf(buffer2, "ADD_FUEL %d", *(scriptPtr++));
		       strcat(buffer, buffer2);
		       addLine(buffer, script);
		       break;
		   }
		case 75:
		   {
		       sprintf(buffer2, "SUB_FUEL%d", *(scriptPtr++));
		       strcat(buffer, buffer2);
		       addLine(buffer, script);
		       break;
		   }
		case 76:
		   {
		       sprintf(buffer2, "SET_GRM %d", *(scriptPtr++));
		       strcat(buffer, buffer2);
		       addLine(buffer, script);
		       break;
		   }
		case 77:
		   {
		       short int temp;

		       temp = *(short int *) scriptPtr;

		       scriptPtr += 2;

		       sprintf(buffer2, "SAY_MESSAGE %d", temp);
		       strcat(buffer, buffer2);
		       addLine(buffer, script);
		       break;
		   }
		case 78:
		   {
		       sprintf(buffer2, "SAY_MESSAGE_OBJ %d", *(scriptPtr++)); // recheck !
		       strcat(buffer, buffer2);
		       addLine(buffer, script);
		       break;
		   }
		case 79:
		   {
		       strcat(buffer, "FULL_POINT");
		       addLine(buffer, script);
		       break;
		   }
		case 80:
		   {
		       short int angle;

		       angle = *(short int *) scriptPtr;

		       scriptPtr += 2;

		       sprintf(buffer2, "BETA %d", angle);
		       strcat(buffer, buffer2);
		       addLine(buffer, script);
		       break;
		   }
		case 81:
		   {
		       strcat(buffer, "GRM_OFF");
		       addLine(buffer, script);
		       break;
		   }
		case 82:
		   {
		       strcat(buffer, "FADE_PAL_RED");
		       addLine(buffer, script);
		       break;
		   }
		case 89:
		   {
		       strcat(buffer, "BULLE_ON");
		       addLine(buffer, script);
		       break;
		   }
		case 90:
		   {
		       strcat(buffer, "BULLE_OFF");
		       addLine(buffer, script);
		       break;
		   }
		case 91:
		   {
		       char temp1;
		       short int temp2;

		       temp1 = *(scriptPtr++);
		       temp2 = *(short int *) scriptPtr;

		       scriptPtr += 2;

		       sprintf(buffer2, "ASK_CHOICE_OBJ %d %d", temp1, temp2);
		       strcat(buffer, buffer2);
		       addLine(buffer, script);
		       break;
		   }
		case 101:
		   {
		       strcat(buffer, "PROJ_ISO");
		       addLine(buffer, script);
		       break;
		   }
		case 102:
		   {
		       strcat(buffer, "PROJ_3D");
		       addLine(buffer, script);
		       break;
		   }
		case 103:
		   {
		       short int temp;

		       temp = *(short int *) scriptPtr;
		       scriptPtr += 2;

		       sprintf(buffer2, "TEXT %d", temp);
		       strcat(buffer, buffer2);
		       addLine(buffer, script);
		       break;
		   }
		case 104:
		   {
		       strcat(buffer, "CLEAR_TEXT");
		       addLine(buffer, script);
		       break;
		   }
		default:
		   {
		       sprintf(buffer2, "Unknown opcode %d", opcode);
		       strcat(buffer, buffer2);
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

    switch (opcode)
	{
	case 0:
	   {
	       strcat(buffer, "COL");
	       break;
	   }
	case 1:
	   {
	       char temp;

	       temp = **scriptPtr;
	       *(scriptPtr) = (*(scriptPtr)) + 1;

	       sprintf(buffer2, "COL_OBJ %d", temp);
	       strcat(buffer, buffer2);
	       break;
	   }
	case 2:
	   {
	       char temp;

	       temp = **scriptPtr;
	       *(scriptPtr) = (*(scriptPtr)) + 1;

	       sprintf(buffer2, "DISTANCE %d", temp);
	       strcat(buffer, buffer2);
	       manipActorVar1 = 1;
	       break;
	   }
	case 3:
	   {
	       strcat(buffer, "ZONE");
	       break;
	   }
	case 4:
	   {
	       char temp;

	       temp = **scriptPtr;
	       *(scriptPtr) = (*(scriptPtr)) + 1;

	       sprintf(buffer2, "ZONE_OBJ %d", temp);
	       strcat(buffer, buffer2);
	       break;
	   }
	case 5:
	   {
	       strcat(buffer, "BODY");
	       break;
	   }
	case 6:
	   {
	       char temp;

	       temp = **scriptPtr;
	       *(scriptPtr) = (*(scriptPtr)) + 1;

	       sprintf(buffer2, "BODY_OBJ %d", temp);
	       strcat(buffer, buffer2);
	       break;
	   }
	case 7:
	   {
	       strcat(buffer, "ANIM");
	       break;
	   }
	case 8:
	   {
	       char temp;

	       temp = **scriptPtr;
	       *(scriptPtr) = (*(scriptPtr)) + 1;

	       sprintf(buffer2, "ANIM_OBJ %d", temp);
	       strcat(buffer, buffer2);
	       break;
	   }
	case 9:
	   {
	       strcat(buffer, "L_TRACK");
	       break;
	   }
	case 10:
	   {
	       char temp;

	       temp = **scriptPtr;
	       *(scriptPtr) = (*(scriptPtr)) + 1;

	       sprintf(buffer2, "L_TRACK %d", temp);
	       strcat(buffer, buffer2);
	       break;
	   }
	case 11:
	   {
	       char temp;

	       temp = **scriptPtr;
	       *(scriptPtr) = (*(scriptPtr)) + 1;
	       sprintf(buffer2, "FLAG_CUBE %d", temp);
	       strcat(buffer, buffer2);
	       break;
	   }
	case 12:
	   {
	       char temp;

	       temp = **scriptPtr;
	       *(scriptPtr) = (*(scriptPtr)) + 1;

	       sprintf(buffer2, "CONE_VIEW %d", temp);
	       strcat(buffer, buffer2);
	       manipActorVar1 = 1;
	       break;
	   }
	case 13:
	   {
	       sprintf(buffer2, "HIT_BY");
	       strcat(buffer, buffer2);
	       break;
	   }
	case 14:
	   {
	       strcat(buffer, "ACTION");
	       break;
	   }
	case 15:
	   {
	       sprintf(buffer2, "FLAG_GAME %d", **scriptPtr);
	       *(scriptPtr) = (*(scriptPtr)) + 1;
	       strcat(buffer, buffer2);
	       break;
	   }
	case 16:
	   {
	       strcat(buffer, "LIFE_POINT");
	       break;
	   }
	case 17:
	   {
	       char temp;

	       temp = **scriptPtr;
	       *(scriptPtr) = (*(scriptPtr)) + 1;

	       sprintf(buffer2, "LIFE_POINT_OBJ %d", temp);
	       strcat(buffer, buffer2);
	       break;
	   }
	case 18:
	   {
	       strcat(buffer, "NB_LITTLE_KEYS");
	       break;
	   }
	case 19:
	   {
	       strcat(buffer, "NB_GOLD_PIECES");
	       manipActorVar1 = 1;
	       break;
	   }
	case 20:
	   {
	       strcat(buffer, "COMPORTEMENT_HERO");
	       break;
	   }
	case 21:
	   {
	       strcat(buffer, "CHAPTER");
	       break;
	   }
	case 22:
	   {
	       char temp;

	       temp = **(scriptPtr);

	       *(scriptPtr) = (*(scriptPtr)) + 1;

	       sprintf(buffer2, "DISTANCE_3D %d", temp);
	       strcat(buffer, buffer2);
	       manipActorVar1 = 1;
	       break;
	   }
	case 25:
	   {
	       char temp;

	       temp = **(scriptPtr);

	       *(scriptPtr) = (*(scriptPtr)) + 1;

	       sprintf(buffer2, "USE_INVENTORY %d", temp);
	       strcat(buffer, buffer2);
	       break;

	   }
	case 26:
	   {
	       strcat(buffer, "CHOICE");
	       manipActorVar1 = 1;
	       break;
	   }
	case 27:
	   {
	       strcat(buffer, "FUEL");
	       break;
	   }
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

    switch (manipActorVar1)
	{
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

    switch (opcode)
	{
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
