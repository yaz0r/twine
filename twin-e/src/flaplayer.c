#include "lba.h"

void LBA_engine::playFLA(char *flaName)
{
    printf("playFLA: %s\n", flaName);

	int var66=0; //quitFla
	int currentFrame;
	int synchTime;
	int esi;
	char buffer[256];

	//TODO: remove

	resetVideoBuffer1();

	if(useFlaPCX==0)
	{
		printf("Unimplemented useFlaPCX!\n");
		exit(1);
	}
	else
	{
		int i;
		for(i=0;i<strlen(flaName);i++)
		{
			if(flaName[i]=='.')
				flaName[i]=0;
		}


		stopMusic();
		strcpy(buffer,"fla/");
		strcat(buffer,flaName);
		makeExtention(buffer,".FLA");
		if(loadFlaSample(buffer))
		{
			if(!strcmp(flaHeaderData.version,"V1.3"))
			{
				enterZoom();
				resetPalette();
				flaResetVideoBuffer1();
				copyToFlaVideoBuffer();
				flaResetVideoBuffer1();
				flaVar2=1;
				currentFrame=0;

				if(!var66)
				{
					do
					{
						if(skipIntro==1)
							var66=1;

						esi=time;
						runFLAscript();
						copyToFlaVideoBuffer();
						updateFlaPalette();

						{
							int i;
							int j;

							char* source=(char*)flaBuffer;
							char* source2;
							char* dest=(char*)videoBuffer1;
							
							for(i=0;i<200;i++)
							{
								for(j=0;j<320;j++)
								{
									*(dest++)=*(source);
									*(dest++)=*(source++);
								}

								source2=dest-640;

								for(j=0;j<640;j++)
								{
									*(dest++)=*(source2++);
								}

							}

							convertPalToRGBA((byte*)flaPalette, (byte*)flaPaletteRGBA);
							osystem->setPalette((byte*)flaPaletteRGBA);
							osystem->drawBufferToScreen((unsigned char*)flaBuffer);
							
							do
							{
								osystem->delay(5);
							}while(time<esi+2);

							readKeyboard();
						}

						//TODO: time sync code
						synchTime=time;

						currentFrame++;

						if(currentFrame==numOfFrameInFLA)
							var66=1;

						flaTime=synchTime;
					}while(!var66);
				}

				fadeOut(flaPalette);
				flaResetVideoBuffer1();
				copyToFlaVideoBuffer();
				mainMenu2();
				FLAsamples();
				exitZoom();
				resetPalette();
				resetVideoBuffer1();
			}
		}

	}
}

void LBA_engine::makeExtention(char* file, char* extention)
{
	// TODO: make real implementation
	strcat(file,extention);
}

int LBA_engine::loadFlaSample(char* file)
{
	int i;

	videoBuffer2Copy=videoBuffer2;
	dataFileHandle=openResource(file);

	if(samplesLoaded)
	{
		printf("Sample loaded in loadFlaSample\n");
		//exit(1);
	}

	readResourceData(dataFileHandle,(char*)&(flaHeaderData.version),6);
	readResourceData(dataFileHandle,(char*)&(flaHeaderData.numOfFrames),4);
	readResourceData(dataFileHandle,(char*)&(flaHeaderData.speed),1);
	readResourceData(dataFileHandle,(char*)&(flaHeaderData.var1),1);
	readResourceData(dataFileHandle,(char*)&(flaHeaderData.var2),2);
	readResourceData(dataFileHandle,(char*)&(flaHeaderData.var3),2);

	numOfFrameInFLA=flaHeaderData.numOfFrames;

	printf("%d frames\n",numOfFrameInFLA);

	flahVar2=flaHeaderData.var2;
	flahVar3=flaHeaderData.var3;
	flaSpeed=flaHeaderData.speed;

	readResourceData(dataFileHandle,(char*)&samplesInFla,4);

	samplesInFla&=0xFFFF; // TODO: remove this cheat

	printf("%d samples in fla\n",samplesInFla);

	for(i=0;i<samplesInFla;i++)
	{
		short int var0;
		short int var1;
		
		readResourceData(dataFileHandle,(char*)&var0,2);
		readResourceData(dataFileHandle,(char*)&var1,2);
	}

    return(1);
}

void LBA_engine::enterZoom()
{
}

void LBA_engine::flaResetVideoBuffer1()
{
}

void LBA_engine::copyToFlaVideoBuffer()
{
}

void LBA_engine::runFLAscript()
{
	char* ptr;
	unsigned int currentOpcodeGlob;
	unsigned char currentOpcode;

	int var_C;
	readResourceData(dataFileHandle,(char*)&(frameData.videoSize),1);
	readResourceData(dataFileHandle,(char*)&(frameData.dummy),1);
	readResourceData(dataFileHandle,(char*)&(frameData.frameVar0),4);

	var_C=0;

	runFLAscriptVar0=frameData.videoSize;

	readResourceData(dataFileHandle,(char*)videoBuffer2Copy,frameData.frameVar0);

	if(var_C>=runFLAscriptVar0)
		return;

	ptr=(char*)videoBuffer2Copy;

	do
	{
		currentOpcode=*(unsigned char*)ptr;
		ptr+=2;
		currentOpcodeGlob=*(unsigned short int*)ptr;
		ptr+=2;

		/*printf("Size=%d\n",currentOpcodeGlob);

		printf("Opcode: %d\n",currentOpcode-1);*/

		switch(currentOpcode-1)
		{
		case 0: // load palette
			{
				short int numOfColor=*(short int*)ptr;
				short int startColor=*(short int*)(ptr+2);

				copyStringToString(ptr+4,flaPalette+startColor*3,numOfColor*3);
				
				lastNumOfColor=numOfColor;
				lastStartColor=startColor;

				break;
			}
		case 1:
			{
				printf("Fade/music opcode in fla\n");
				break;
			}
		case 2: // play sample
			{
				break;
			}
		case 4: // play sample
			{
				break;
			}
		case 5: // draw delat frame
			{
				flaUnpackFrame2(ptr,320);
				break;
			}
		case 7: // draw key frame
			{
				flaUnpackFrame1(ptr,320,flahVar3);
				break;
			}
		default:
			{
				printf("Unhandled fla opcode %d!\n",currentOpcode-1);
				//exit(1);
				break;
			}
		}

		var_C++;
		ptr+=currentOpcodeGlob;

	}while(var_C<runFLAscriptVar0);

}

void LBA_engine::flaUnpackFrame1(char* ptr, int width, int height)
{
	char* destPtr=(char*)flaBuffer;
	char* startOfLine=destPtr;
	char flag1;
	char flag2;

	int i;
	int j;

	do
	{
		flag1=*(ptr++);

		for(i=0;i<flag1;i++)
		{
			flag2=*(ptr++);

			if(flag2<0)
			{
				flag2=-flag2;
				for(j=0;j<flag2;j++)
				{
					*(destPtr++)=*(ptr++);
				}
			}
			else
			{
				char colorFill;

				colorFill=*(ptr++);

				for(j=0;j<flag2;j++)
				{
					*(destPtr++)=colorFill;
				}
			}
		}

		startOfLine=destPtr=startOfLine+width;
	}while(--height);
}

void LBA_engine::flaUnpackFrame2(char* ptr, int width)
{
	unsigned short int skip;
	char* destPtr;
	char* startOfLine;
	int height;

	skip=*(unsigned short int*)ptr;
	ptr+=2;
	skip*=width;
	startOfLine=destPtr=(char*)flaBuffer+skip;
	height=*(short int*)ptr;
	ptr+=2;

	char flag1;
	char flag2;

	int i;
	int j;

	do
	{
		flag1=*(ptr++);

		for(i=0;i<flag1;i++)
		{
			destPtr+=(unsigned char)*(ptr++);
			flag2=*(ptr++);

			if(flag2>0)
			{
				
				for(j=0;j<flag2;j++)
				{
					*(destPtr++)=*(ptr++);
				}
			}
			else
			{
				flag2=-flag2;
				char colorFill;

				colorFill=*(ptr++);

				for(j=0;j<flag2;j++)
				{
					*(destPtr++)=colorFill;
				}
			}
		}

		startOfLine=destPtr=startOfLine+width;
	}while(--height);
}

void LBA_engine::updateFlaPalette()
{
}

void LBA_engine::FLAsamples()
{
}

void LBA_engine::exitZoom()
{
}