#ifdef MEM_DEBUG

#include<windows.h>
#include<Psapi.h>
#pragma comment(lib,"Psapi.lib")

#endif // MEM_DEBUG

#include "lba.h"

#ifndef PCLIKE
#include <shinobi.h>    /* Shinobi system routines. */
#include <kamui2.h>     /* Kamui2 low-level graphics HAL. */
#include <sn_fcntl.h>   /* LibCross file types. */
#include <usrsnasm.h>   /* LibCross I/O routines. */
#include <sg_syCbl.h>   /* NTSC/RGB/VGA Cable check interface. */

VOID STATIC	PALExtCallbackFunc(PVOID pCallbackArguments)
{
PKMPALEXTINFO	pInfo;

	pInfo = (PKMPALEXTINFO)pCallbackArguments;
	pInfo->nPALExtMode = KM_PALEXT_HEIGHT_RATIO_1_133;	
	return;
}


void DCinit(void)
{
#ifdef __GNUC__
	shinobi_workaround();
#endif

    /* Tell the user about this app. */
    debug_write (SNASM_STDOUT, "LBA_DC", 6);

    /* Check the cable for NTSC/PAL or VGA.. works properly for SCART. */
    switch (syCblCheck())
	{
        /* Initialize the display device and set the frame buffer based on the video mode. */
        case SYE_CBL_NTSC:  /* U.S./North America NTSC (60Hz) and Brazil PAL-M (60Hz). */
            sbInitSystem (KM_DSPMODE_NTSCNI640x480, KM_DSPBPP_RGB565, 1);
            break;
        case SYE_CBL_PAL:   /* Europe PAL (50Hz) and Argentina PAL-N (50Hz). */
			sbInitSystem((int)KM_DSPMODE_PALNI640x480EXT, (int)KM_DSPBPP_RGB565, 1);

			/*	Change the PAL height ratio */
			kmSetPALEXTCallback(PALExtCallbackFunc,NULL);
			kmSetDisplayMode (KM_DSPMODE_PALNI640x480EXT, KM_DSPBPP_RGB565, TRUE, FALSE);
 			break;
        case SYE_CBL_VGA:   /* Standard VGA. */
            sbInitSystem (KM_DSPMODE_VGA, KM_DSPBPP_RGB565, 1);
            break;
        default:
            syBtExit();     /* Unknown video type, return to Dreamcast BootROM. */
	}

    /* Check malloc alignment. */
   // Init32Malloc();
}

#endif

/**
fonction main
*/

extern "C" int main(int argc, char *argv[])
{
    LBA_engine *engine;

#ifdef MEM_DEBUG
	PROCESS_MEMORY_COUNTERS memInfo;
	GetProcessMemoryInfo(GetCurrentProcess(), &memInfo, sizeof(PROCESS_MEMORY_COUNTERS));

	staticMemoryUsage = memInfo.WorkingSetSize;
#endif //MEM_DEBUG

#ifndef PCLIKE
	DCinit();
#endif

	printf("Creation of the system dependant OSystem object\n");
    osystem = new OSystem(argc, argv);
    printf("Success !\n");

/*    printf("LBA_engine object size is: %d\n", sizeof(LBA_engine));
    engine = (LBA_engine *) malloc(sizeof(LBA_engine));	// creation of the LBA_engine

    if (!engine) // if the allocation failed
	{
	    printf("Failed to create the LBA_engine object !!!\n");
	    printf("Please check your free memory ! (how can you be that low on memory ??!!?)\n");
	    exit(1);
	}
    else
	{*/
	//    printf("LBA_engine creation succeed !\n");

	//    memset(engine, 0, sizeof(LBA_engine));

       _debugger.init();

	  //  engine->osystem = osystem;
	  //  engine->_debugger.osystem = osystem;
	  //  engine->_debugger.engine = engine;
	_debugger.osystem = osystem;
	    initVars();	// init the vars (move to the object constructor ?)

#ifdef PCLIKE
	    startThreadTimer(); // the little trouble maker ! This one is responsible for the game timming
#endif

			init();	// startup the game engine !
//	}

//    delete(osystem);

    return(0); // that's all folks !
}

void init(void)
{
#ifdef MEM_DEBUG
	initMemorySystem();
#endif

	soundInit();

   // GetDiskEnv
    InitProgram();
   // ReadVolumeSettings
   // setup_lst=Def_ReadValue(setup.lst,Version_US);
   // InitLanguage

   /*
    * if(useSound) { if(useSB) HQR_Midi=HQR_Init_Ressource("midi_sb.hqr",32000,2); else
    * HQR_Midi=HQR_Init_Ressource("midi_mi.hqr",32000,2);
    * 
    * if(!HQR_Midi) useSound=0; }
    */

    workVideoBuffer = (byte *) Malloc(307700 * sizeof(byte));

   // check screenPtr

    osystem->Flip(frontVideoBuffer);

#ifndef FASTDEBUG

    AdelineLogo();

#endif

    InitCDR("CD_LBA");

   // todo: mettre les detection de problemes d'allocations...

    bufSpeak = (byte*)Malloc( BUF_SPEAK_SIZE );	// was allocated in dos memory

    bufMemoSeek = (byte*)Malloc( BUF_MEMOSEEK_SIZE );

    bufText = (char *) Malloc( BUF_TEXT_SIZE );

    bufOrder = (char *) Malloc( BUF_ORDER_SIZE );

    bufAni1 = bufAni2 = (byte*)Malloc( BUF_ANIM_SIZE );

    InitBufferCube();

//#ifndef PRELOAD_ALL
  //  HQM_Init_Memory( BUF_HQM_SIZE );
//#endif

    menuPal = LoadMalloc_HQR("ress.hqr", 0);
    convertPalToRGBA(menuPal, menuPalRGBA);

    shadowSprite = LoadMalloc_HQR("ress.hqr", 4);
    spriteActorData = LoadMalloc_HQR("ress.hqr", 3);

    lbaFont = LoadMalloc_HQR("ress.hqr", 1);
    SetFont(lbaFont, 2, 8);
    CoulFont(14);
    CoulDial(136, 143, 2);

#ifdef PRELOAD_ALL
	HQR_Fic =			HQR_Init_RessourcePreload("file3d.hqr");
	HQR_Inventory =		HQR_Init_RessourcePreload("invobj.hqr");
//	HQR_Text =			HQR_Init_RessourcePreload("text.hqr");
	HQR_Scenes =		HQR_Init_RessourcePreload("scene.hqr");
	HQR_Sprites =		HQR_Init_RessourcePreload("sprites.hqr");
	HQR_Anims =			HQR_Init_RessourcePreload("anim.hqr");
	HQR_Bodies =		HQR_Init_RessourcePreload("body.hqr");
	HQR_Samples =		HQR_Init_RessourcePreload("samples.hqr");
	/*HQR_Grids =			HQR_Init_RessourcePreload("lba_gri.hqr");
	HQR_Bll =			HQR_Init_RessourcePreload("lba_bll.hqr");*/
#else
	HQR_Inventory =		HQR_Init_Ressource("invobj.hqr", BUF_INVENTORY_SIZE, 30);
    HQR_Sprites =		HQR_Init_Ressource("sprites.hqr", 300000, 120); // enough to hold all the sprites in mem
    HQR_Samples =		HQR_Init_Ressource("samples.hqr", 4500000, 4500000 / 5000);
    HQR_Anims =			HQR_Init_Ressource("anim.hqr", 450000, 600); // should be able to hold all the anims of the game
#endif //PRELOAD_ALL

#ifdef _DEBUG
#ifdef PRELOAD_ALL
	flagModelPtr = HQR_GetCopy( HQR_Bodies, 121 );
	HQRM_Load("body.hqr", 121, &flagModelPtr); // load the flag model used to display tracks
#else
    HQRM_Load("body.hqr", 121, &flagModelPtr); // load the flag model used to display tracks
#endif
    loadGfxSub(flagModelPtr);
#endif

	samplesLoaded = 1;

    FadeToBlack((char *) paletteRGBA);

    if (setup_lst == 0)		// switch pour les 2 version de l'ecran titre de LBA
		RessPict(49);
    else
		RessPict(12);

   // now unused since we cross fade the video !
   // TimerPause();

   // FadeToBlack((char*)paletteRGBA);

    loadImageCrossFade(52);	// Electronic Arts Logo

    TimerPause();

    FadeToBlack((char *) paletteRGBA);

    PlayAnimFla("DRAGON3");

    Load_HQR("ress.hqr", workVideoBuffer, 14);

    CopyScreen(workVideoBuffer, frontVideoBuffer);

    osystem->Flip(frontVideoBuffer);

    FadeToPal((char *) menuPalRGBA);

    MainGameMenu();
}

void newGame(void)
{
    int flagDisplayTextSave;

    stopMusic();
    if (needChangeRoom)
	return;
    if (chapter)
	return;

    flagDisplayTextSave = flagDisplayText;

    flagDisplayText = 1;

    Load_HQR("ress.hqr", workVideoBuffer, 15);	// Ecran de Twinsun (ecran 1 de l'intro)
    CopyScreen(workVideoBuffer, frontVideoBuffer);
    Load_HQR("ress.hqr", palette, 16);
    convertPalToRGBA(palette, paletteRGBA);

    osystem->crossFade((char *) frontVideoBuffer, (char *) paletteRGBA);
    osystem->setPalette(paletteRGBA);
   // osystem->Flip(frontVideoBuffer);
   // FadeToPal((char*)paletteRGBA);

    newGameVar4 = 0;
    newGameVar5 = 1;
    InitDial(2);
    newGame2();
    TestCoulDial(15);
/*
    printTextFullScreen(150);
    readKeyboard();

    if (skipIntro != 1)
	{
	   // SetBackPal();
	    Load_HQR("ress.hqr", workVideoBuffer, 17);	// Ecran de la citadelle (ecran 2 de
	   // l'intro)
	    CopyScreen(workVideoBuffer, frontVideoBuffer);
	    Load_HQR("ress.hqr", palette, 18);
	    convertPalToRGBA(palette, paletteRGBA);

	    osystem->crossFade((char *) frontVideoBuffer, (char *) paletteRGBA);
	    osystem->setPalette(paletteRGBA);

	   // osystem->Flip(frontVideoBuffer);
	   // osystem->setPalette(paletteRGBA);
	    printTextFullScreen(151);

	    readKeyboard();
	    if (skipIntro != 1)
		{
		   // SetBackPal();
		    Load_HQR("ress.hqr", workVideoBuffer, 19);	// Ecran du reve de Twisen (ecran 3 de l'intro)
		    CopyScreen(workVideoBuffer, frontVideoBuffer);
		    Load_HQR("ress.hqr", palette, 20);

		    convertPalToRGBA(palette, paletteRGBA);

		    osystem->crossFade((char *) frontVideoBuffer, (char *) paletteRGBA);
		    osystem->setPalette(paletteRGBA);

		   // osystem->Flip(frontVideoBuffer);
		   // osystem->setPalette(paletteRGBA);

		    printTextFullScreen(152);
		}
	}*/

    newGameVar5 = 0;
    newGame4();
    newGameVar4 = 1;
    FadeToBlack((char *) paletteRGBA);
    Cls();
    osystem->Flip(frontVideoBuffer);
    playMidi(1);

    PlayAnimFla("INTROD");
    SetBackPal();
    Cls();
    osystem->Flip(frontVideoBuffer);

    flagDisplayText = flagDisplayTextSave;	// on remet le flag comme il était au debut

}

void newGame2(void)
{				/* ok */
    dialogueBoxLeft = 8;
    dialogueBoxTop = 8;
    dialogueBoxRight = 631;

    dialogueBoxBottom = 471;
    dialogueBoxParam1 = 11;
    dialogueBoxParam2 = 607;
}

void newGame4(void)
{				/* ok */
    dialogueBoxLeft = 16;
    dialogueBoxTop = 334;
    dialogueBoxRight = 623;
    dialogueBoxBottom = 463;
    dialogueBoxParam1 = 3;
    dialogueBoxParam2 = 591;
}

void TestCoulDial(short int param)
{
    progressiveTextStepSize = -1;
    progressiveTextBufferSize = 14;
    progressiveTextStartColor = param << 4;
    progressiveTextStopColor = (param << 4) + 12;
}
