#include "lba.h"

streamReader fla_streamReader;

void PlayAnimFla(char *flaName)
{
#ifdef FASTDEBUG
	return;
#endif

#ifndef PCLIKE
	return;
#endif
    printf("PlayAnimFla: %s\n", flaName);

	int var66=0; //quitFla
	int currentFrame;
	int synchTime;
	int esi;
	char buffer[256];

    todo("remove");

	Cls();

	if(useFlaPCX==0)
	{
		printf("Unimplemented useFlaPCX!\n");
		exit(1);
	}
	else
	{
		unsigned int i;
		for(i=0;i<strlen(flaName);i++)
		{
			if(flaName[i]=='.')
				flaName[i]=0;
		}


		stopMusic();
#ifdef PCLIKE
		strcpy(buffer,"fla/");
		strcat(buffer,flaName);
#else
		strcpy(buffer,flaName);
#endif
		AddExt(buffer,".FLA");
		if(InitFla(buffer))
		{
			if(!strcmp(flaHeaderData.version,"V1.3"))
			{
				ExtInitMcga();
				SetBackPal();
				Mcga_Cls();
				Mcga_Flip();
				Mcga_Cls();
				flaVar2=1;
				currentFrame=0;

				if(!var66)
				{
					do
					{
						if(skipIntro==1)
							var66=1;

						esi=time;
						DrawNextFrameFla();
						Mcga_Flip();
						GestionPalette();

						{
							/*char* source=(char*)flaBuffer;
							char* source2;
							char* dest=(char*)frontVideoBuffer;
							
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

							}*/

							osystem->draw320x200BufferToScreen((unsigned char*)flaBuffer);
							
							/*do
							{
								osystem->delay(5);
							}while(time<esi+2);

							
*/
#ifndef USE_GL
							osystem->updateImage();
#endif
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

				FadeToBlack(flaPalette);
				Mcga_Cls();
				Mcga_Flip();
				HQ_StopSample();
				ClearFla();
				ExtInitSvga();
				SetBackPal();
				Cls();
			}
		}

	}

//    Close(dataFileHandle);
}

void AddExt(char* file, char* extention)
{
	// TODO: make real implementation
	strcat(file,extention);
}

int InitFla(char* file)
{
	int i;

	streamReader_open(&fla_streamReader, (const int8*)file);

	workVideoBufferCopy=workVideoBuffer;

	if(samplesLoaded)
	{
		printf("Sample loaded in InitFla\n");
		//exit(1);
	}

	streamReader_get(&fla_streamReader, &flaHeaderData.version, 6);
	streamReader_get(&fla_streamReader, &flaHeaderData.numOfFrames, 4);
	streamReader_get(&fla_streamReader, &flaHeaderData.speed, 1);
	streamReader_get(&fla_streamReader, &flaHeaderData.var1, 1);
	streamReader_get(&fla_streamReader, &flaHeaderData.var2, 2);
	streamReader_get(&fla_streamReader, &flaHeaderData.var3, 2);

	numOfFrameInFLA=flaHeaderData.numOfFrames;

	printf("%d frames\n",numOfFrameInFLA);

	flahVar2=flaHeaderData.var2;
	flahVar3=flaHeaderData.var3;
	flaSpeed=flaHeaderData.speed;

	streamReader_get(&fla_streamReader, &samplesInFla, 4);

	samplesInFla&=0xFFFF; // TODO: remove this cheat

	printf("%d samples in fla\n",samplesInFla);

	for(i=0;i<samplesInFla;i++)
	{
		short int var0;
		short int var1;
		
		streamReader_get(&fla_streamReader, &var0, 2);
		streamReader_get(&fla_streamReader, &var1, 2);
	}

    return(1);
}

void ExtInitMcga()
{
	osystem->set320x200Mode(true);
}

void Mcga_Cls()
{
}

void Mcga_Flip()
{
}

void DrawNextFrameFla()
{
	char* ptr;
	unsigned int currentOpcodeGlob;
	unsigned char currentOpcode;

	int var_C;
	streamReader_get(&fla_streamReader, &frameData.videoSize, 1);
	streamReader_get(&fla_streamReader, &frameData.dummy, 1);
	streamReader_get(&fla_streamReader, &frameData.frameVar0, 4);

	var_C=0;

	runFLAscriptVar0=frameData.videoSize;

	streamReader_get(&fla_streamReader, workVideoBufferCopy, frameData.frameVar0);

	if(var_C>=runFLAscriptVar0)
		return;

	ptr=(char*)workVideoBufferCopy;

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
				short int numOfColor=READ_LE_S16(ptr);
				short int startColor=READ_LE_S16((ptr+2));

				copyStringToString(ptr+4,flaPalette+startColor*3,numOfColor*3);
				
				lastNumOfColor=numOfColor;
				lastStartColor=startColor;

				convertPalToRGBA((byte*)flaPalette, (byte*)flaPaletteRGBA);
				osystem->setPalette320x200((byte*)flaPaletteRGBA);

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
				UpdateFrame(ptr,320);
				break;
			}
		case 7: // draw key frame
			{
				DrawFrame(ptr,320,flahVar3);
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

void DrawFrame(char* ptr, int width, int height)
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

void UpdateFrame(char* ptr, int width)
{
	unsigned short int skip;
	char* destPtr;
	char* startOfLine;
	int height;

	skip=READ_LE_U16(ptr);
	ptr+=2;
	skip*=width;
	startOfLine=destPtr=(char*)flaBuffer+skip;
	height=READ_LE_S16(ptr);
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

void GestionPalette()
{
}

void ClearFla()
{
}

void ExtInitSvga()
{
	osystem->set320x200Mode(false);
}
