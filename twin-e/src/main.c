#include "lba.h"

int main(int argc, char* argv[])
{
  LBA_engine *engine;
  OSystem* osystem;

  printf("Creation of the system dependant OSystem object\n");
  osystem= new OSystem(argc,argv);
  printf("Success !\n");

  printf("LBA_engine object size is: %d\n",sizeof(LBA_engine));
  engine = (LBA_engine*)malloc(sizeof(LBA_engine)); // creation of the LBA_engine

  if(!engine) // if the allocation failed
  {
    printf("Failed to create the LBA_engine object !!!\n");
    printf("Please check your free memory !\n");
    exit(1);
  }
  else
  {
    printf("LBA_engine creation succeed !\n");

	engine->osystem=osystem;
    engine->initVars(); // init the vars (move to the object creator ?)
	
	startThreadTimer(engine);

    engine->init();     // startup the game engine !
  }

  delete(osystem);

  return 0;
}

void LBA_engine::init(void)
{
	//getPath
	loadCFG();
	//setupMixer
	//setup_lst=loadSetup_lst(setup.lst,Version_US);
	//loadLanguage
/*	if(useSound)
	{
		if(useSB)
			loadMidiResult=load_hqr("midi_sb.hqr",32000,2);
		else
			loadMidiResult=load_hqr("midi_mi.hqr",32000,2);
		
		if(!loadMidiResult)
			useSound=0;
	}*/

	videoBuffer2=(byte*)malloc(307700*sizeof(byte));

	// check screenPtr

	osystem->drawBufferToScreen(videoBuffer1);

    displayAdelineLogo();

	checkCD("CD_LBA");

	// todo: mettre les detection de problemes d'allocations...
	
	bufSpeak=allocateBuf(262176); // Mmm, pas 100% correct je pense

	bufMemoSeek=allocateBuf(2048);

	bufText=(char*)allocateBuf(25000);

	bufOrder=(char*)allocateBuf(1024);



	bufAni1=bufAni2=allocateBuf(5000);

	allocBufCube();

	HQRInventory=load_hqr("invobj.hqr",20000,30);

	allocHQMemory(400000);

	menuPal=loadImageToMemory("ress.hqr",0);
	convertPalToRGBA(menuPal,menuPalRGBA);

	HQRess2=loadImageToMemory("ress.hqr",4);
	HQRess3=loadImageToMemory("ress.hqr",3);
	lbaFont=loadImageToMemory("ress.hqr",1);

	fontInit(lbaFont,2,8);

	setTextColor(14);
 
	setInitVars(136,143,2);

	HQRPtrSpriteExtra=load_hqr("sprites.hqr",4500000,4500000/1000);
	HQRSamples=load_hqr("samples.hqr",4500000,4500000/5000);
	samplesLoaded=1;
	HQRanims=load_hqr("anim.hqr",4500000,4500000/800);

	fadeOut((char*)paletteRGBA);

	if(setup_lst==0) //switch pour les 2 version de l'ecran titre de LBA
		loadImageAndPalette(49);
	else
		loadImageAndPalette(12);

	// now unused since we cross fade the video !
	//waitFor();

	//fadeOut((char*)paletteRGBA);

	loadImageCrossFade(52); // Electronic Arts Logo

	waitFor();

	fadeOut((char*)paletteRGBA);

	playFLA("DRAGON3");

	loadImageToPtr("ress.hqr",videoBuffer2,14);

	copyToBuffer(videoBuffer2,videoBuffer1);

	osystem->drawBufferToScreen(videoBuffer1);

	fadeIn2((char*)menuPalRGBA);

	mainMenu();
}



void LBA_engine::newGame(void)
{
	int flagDisplayTextSave;

	stopMusic();
	if(needChangeRoom)
		return;
	if(newGameVar2)
		return;

	flagDisplayTextSave=flagDisplayText;

	flagDisplayText=1;

	loadImageToPtr("ress.hqr",videoBuffer2,15); // Ecran de Twinsun (ecran 1 de l'intro)
	copyToBuffer(videoBuffer2,videoBuffer1);
	loadImageToPtr("ress.hqr",palette,16);
	convertPalToRGBA(palette,paletteRGBA);
	
	osystem->crossFade((char*)videoBuffer1,(char*)paletteRGBA);
	osystem->setPalette(paletteRGBA);
	//osystem->drawBufferToScreen(videoBuffer1);
	//fadeIn2((char*)paletteRGBA);

	newGameVar4=0;
	newGameVar5=1;
	loadTextBank(2);
	newGame2();
	newGame3(15);


	printTextFullScreen(150);
	readKeyboard();
	
	if(skipIntro!=1)
	{
		//resetPalette();
		loadImageToPtr("ress.hqr",videoBuffer2,17); // Ecran de la citadelle (ecran 2 de l'intro)
		copyToBuffer(videoBuffer2,videoBuffer1);
		loadImageToPtr("ress.hqr",palette,18);
		convertPalToRGBA(palette,paletteRGBA);

		osystem->crossFade((char*)videoBuffer1,(char*)paletteRGBA);
		osystem->setPalette(paletteRGBA);

//		osystem->drawBufferToScreen(videoBuffer1);
//		osystem->setPalette(paletteRGBA);
		printTextFullScreen(151);
		
		readKeyboard();
		if(skipIntro!=1)

		{
		//	resetPalette();
			loadImageToPtr("ress.hqr",videoBuffer2,19); // Ecran du reve de Twisen (ecran 3 de l'intro)
			copyToBuffer(videoBuffer2,videoBuffer1);
			loadImageToPtr("ress.hqr",palette,20);

			convertPalToRGBA(palette,paletteRGBA);

			osystem->crossFade((char*)videoBuffer1,(char*)paletteRGBA);
			osystem->setPalette(paletteRGBA);

//			osystem->drawBufferToScreen(videoBuffer1);
//			osystem->setPalette(paletteRGBA);

			printTextFullScreen(152);
		}
	}

	newGameVar5=0;
	newGame4();
	newGameVar4=1;
	fadeOut((char*)paletteRGBA);
	resetVideoBuffer1();
	osystem->drawBufferToScreen(videoBuffer1);
	playMidi(1);

	playFLA("INTROD");
	resetPalette();
	resetVideoBuffer1();
	osystem->drawBufferToScreen(videoBuffer1);

	flagDisplayText=flagDisplayTextSave; // on remet le flag comme il était au debut

}

void LBA_engine::newGame2(void) /* ok */
{
	dialogueBoxLeft=8;
	dialogueBoxTop=8;
	dialogueBoxRight=631;

	dialogueBoxBottom=471;
	dialogueBoxParam1=11;
	dialogueBoxParam2=607;	
}

void LBA_engine::newGame4(void) /* ok */
{
	dialogueBoxLeft=16;
	dialogueBoxTop=334;
	dialogueBoxRight=623;
	dialogueBoxBottom=463;
	dialogueBoxParam1=3;
	dialogueBoxParam2=591;
}

void LBA_engine::newGame3(short int param)
{
	initVar4=-1;
	initVar5=14;
	initVar2=param<<4;
	initVar3=(param<<4)+12;
}
