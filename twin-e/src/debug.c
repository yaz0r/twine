#include "lba.h"

int debugger::processDebug(void)
{
	mouseStatusStruct mouseData;
	int actorNum;

	osystem->getMouseStatus(&mouseData);

	//printf("Mouse: %d %d - %d %d\n",mouseData.X,mouseData.Y,mouseData.left,mouseData.right);
//	printf("Num Actors: %d\n",numOfActorOnScreen);

	actorNum=findActor(mouseData.X,mouseData.Y);

	if(actorNum!=-1 && mouseData.left)
	{
		debugActor(actorNum);
	}


	return(0);
}

int debugger::findActor(int X,int Y)
{
	int i;

	for(i=numOfActorOnScreen;i>=0;i--)
	{
		if(inBox(X,Y,actorBox[i].top,actorBox[i].left,actorBox[i].bottom,actorBox[i].right))
		{
			return(actorBox[i].actorNum);
		}
	}

	return(-1);
}

int debugger::inBox(int X,int Y,int top, int left, int bottom, int right)
{
	if(X>left && X<right)
		if(Y>top && Y<bottom)
			return(1);

	return(0);
}

void debugger::debugActor(int num)
{
	int i;
	printf("Debug actor %d\n",num);

	scriptData* comScript;
	scriptData* trackScript;

	trackScript=getActorTrackScript(num);

	for(i=0;i<trackScript->numOfLignes;i++)
	{
		printf("%s\n",trackScript->lines[i]);
	}
}

void debugger::addLine(char* buffer,scriptData* script)
{
	script->lines=(char**)realloc(script->lines,sizeof(char*)*(script->numOfLignes+1));

	script->lines[script->numOfLignes]=(char*)malloc(strlen(buffer)+1);
	strcpy(script->lines[script->numOfLignes],buffer);
	script->numOfLignes++;
}

scriptData* debugger::getActorTrackScript(int num)
{
	unsigned char* scriptPtr;
	int finish=0;
	unsigned char opcode;
	scriptData* script;
	char buffer[256];

	script=(scriptData*)malloc(sizeof(scriptData));

	script->lines=0;
	script->numOfLignes=0;

	scriptPtr=engine->actors[num].moveScript;

	do
	{
		opcode=*(scriptPtr++);
		switch(opcode)
		{
		case 0:
			{
				sprintf(buffer,"END");
				addLine(buffer,script);
				finish=1;
				break;
			}
		case 2:
			{
				sprintf(buffer,"change body %d",*(scriptPtr++));
				addLine(buffer,script);
				break;
			}
		case 3:
			{
				sprintf(buffer,"ANIM %d",*(scriptPtr++)); // bleu
				addLine(buffer,script);
				break;
			}
		case 4:
			{
				sprintf(buffer,"GOTO_POINT %d",*(scriptPtr++));
				addLine(buffer,script);
				break;
			}
		case 5:
			{
				sprintf(buffer,"WAIT_ANIM");
				addLine(buffer,script);
				break;
			}
		case 7:
			{
				short int temp;

				temp=*(short int*)(scriptPtr);

				scriptPtr+=2;

				sprintf(buffer,"ANGLE %d",temp);
				addLine(buffer,script);
				break;
			}
		case 9:
			{
				sprintf(buffer,"LABEL %d",(*scriptPtr++)); // rouge
				addLine(buffer,script);
				break;
			}
		case 10:
			{
				short int temp;

				temp=*(short int*)(scriptPtr);

				scriptPtr+=2;

				sprintf(buffer,"jump %d",temp);
				addLine(buffer,script);
				break;
			}
		case 11:
			{
				sprintf(buffer,"STOP");
				addLine(buffer,script);
				break;
			}
		case 12:
			{
				sprintf(buffer,"Walk backward to %d",*(scriptPtr++));
				addLine(buffer,script);
				break;
			}
		case 13:
			{
				sprintf(buffer,"opcode 13 %d %d",*(scriptPtr++),*(scriptPtr++));
				addLine(buffer,script);
				break;
			}
		case 14:
			{
				short int temp;

				temp=*(short int*)(scriptPtr);

				scriptPtr+=2;

				sprintf(buffer,"SAMPLE %d",temp); //bleu
				addLine(buffer,script);
				break;
			}
		case 18:
			{
				int temp1;
				int temp2;

				temp1=*(int*)(scriptPtr);
				temp2=*(int*)(scriptPtr);

				scriptPtr+=4;
				scriptPtr+=4;

				sprintf(buffer,"wait %d %d",temp1,temp2);
				addLine(buffer,script);
				break;
			}
		case 33:
			{
				sprintf(buffer,"angle toward %d %d",*(scriptPtr++),*(scriptPtr++));
				addLine(buffer,script);
				break;
			}
		default:
			{
				sprintf(buffer,"Unknown opcode %d",opcode);
				addLine(buffer,script);
				finish=1;
			}
		}
	}while(!finish);

	return(script);
}


