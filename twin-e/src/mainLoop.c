/***************************************************************************
                          mainLoop.cpp  -  description
                             -------------------
    begin                : Sun Feb 3 2002
    copyright            : (C) 2002 by 
    email                : 
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "lba.h"

int LBA_engine::mainLoop(void)
{
  int temp;
  int i;
 
  mainLoopVar2=1;
  mainLoopVar3=1;
  mainLoop1(0,-256,5,&mainLoopVar1);

  do
  {
    readKeyboard();
    if(mainLoopVar4>500)
      waitRetrace();

    if(needChangeRoom!=-1)
      changeRoom();
  
    mainLoopVar6=mainLoopVar5;
    key=printTextVar12;
    mainLoopVar5=key1;
    mainLoopVar7=skipIntro;

    if(mainMenuVar4!=0)
    {
      if(getCurrentlyPlayingMusic()!=8)
        playCDtrack(8);
      if(skipIntro!=0)

        return(0);
      if(printTextVar12!=0)
        return(0);
      if(key1!=0)
        return(0);
    }
    else
    {
      if(mainLoopVar3 == 0)
      {
        // debut des inputs

        if(skipIntro == 1 && twinsen->life > 0 && twinsen->costumeIndex != -1 && (twinsen->field_60 & 0x200)) // recheck
        {
          mainLoop2(1);
          freezeTime();
          if(!mainLoop4())
          {
            unfreezeTime();
            fullRedraw(1);
          }
          else
          {
            unfreezeTime();
            fullRedraw(1);
            freezeTime();
            saveGame();
            unfreezeTime();
            return(0);
          }
        }
        if(mainLoopVar7==64) // F6
        {
          temp=languageCD1;
          freezeTime();
          mainLoop2(1);
          soundMenuData[5]=15;
          languageCD1=0;
          loadTextBank(0);
          optionMenu();
          languageCD1=temp;
          loadTextBank(currentTextBank+3);

          if(currentRoom==80)
          {
/*          if(mainLoopVar8==1)
              playCDtrack(8);
            else
              playMusic(roomMusic); */
          }
          else
          {
            playMusic(roomMusic);
          }
          unfreezeTime();
          fullRedraw(1);
        }
        mainLoopVar9=-1;
        if((byte)mainLoopVar5&0x20 && twinsen->costumeIndex != -1 && twinsen->field_40==1) // inventory menu
        {
          freezeTime();
          mainLoop2(1);
          //processInput();
          //todo: implementer ce cas
        }
        if((byte)mainLoopVar5&4 && twinsen->costumeIndex != -1 && twinsen->field_40 == 1) // comportement menu
        {
          freezeTime();
          mainLoop2(1);
          drawInGameMenu();
          unfreezeTime();
          fullRedraw(1);
        }
        if(mainLoopVar7>=59 && mainLoopVar7<= 62 && twinsen->costumeIndex != -1 && twinsen->field_40 == 1) // F1-F4
        {
          freezeTime();
		  mainLoop2(1);
          changeTwinsenComp(mainLoopVar7-59);
          drawInGameMenu();
          unfreezeTime();
          fullRedraw(1);
        }
        if((byte)mainLoopVar5&2 && mainLoopVar10 == 0)     // recenter screen
        {
          changeRoomVar4=actors[reinitVar8].field_1A>>9;
          changeRoomVar5=actors[reinitVar8].field_1C>>8;
          changeRoomVar6=actors[reinitVar8].field_1E>>9;
          mainLoopVar2=1;
          needChangeRoom=currentRoom+1;
        }

/***********************************************/
/*             camera debugger                 */
/***********************************************/      
            
        if(printTextVar12&2) // x--     -> bas
        {
          changeRoomVar6++;
          mainLoopVar2=1;
        }
      
        if(printTextVar12&1) // x++      -> haut
        {
          changeRoomVar6--;
          mainLoopVar2=1;
        }
      
        if(printTextVar12&4) // y--      -> gauche
        {
          changeRoomVar4--;
          mainLoopVar2=1;
        }
      
        if(printTextVar12&8) // y++      -> droite
        {
          changeRoomVar4++;
          mainLoopVar2=1;
        }

/**********************************************/      
           
        if(mainLoopVar7=='h' && vars[0] == 1 && vars[28] == 0) // draw holomap (vars[0] = have holomap) (vars[28]=holomapMode)
        {
          freezeTime();
          mainLoop2(1);
//        processHolomap();
          mainLoopVar3=1;






          unfreezeTime();
          fullRedraw(1);
        }
    
        if(mainLoopVar7=='p')
        {
//        pauseSound();
          freezeTime();
          if(!drawInGameTransBox)
          {
            setTextColor(15);
            printStringSimple(5,446,"Pause");
            osystem->refresh(videoBuffer1,5,446,100,479);
          }
          readKeyboard();
          while(!skipIntro)
          {
            readKeyboard();
          };
          do
          {
            readKeyboard();
          }while(!skipIntro && !printTextVar12 && key1);
          if(!drawInGameTransBox)
          {
            drawBoxTrans(5,446,100,479,(char*)videoBuffer2,5,446,(char*)videoBuffer1);
            osystem->refresh(videoBuffer1,5,446,100,479);
          }
          unfreezeTime();
//        resumeSound();
        }

     // il manque un process des input là pour F5

     // fin des inputs
      }
    }
//  mainLoopVar17=mainLoopSub17(&mainLoopVar1);
//  if(!mainLoopVar17)
//    mainLoopVar17=1;

    mainLoop1(0,-256,5,&mainLoopVar1);
    mainLoopVar10=0;
//  mainLoopSub18();

    for(i=0;i<numActorInRoom;i++)
    {
     actors[i].field_64=-1;
    }

//  mainLoopSub19();

   //2nd boucle de process des acteurs

    for(i=0;i<numActorInRoom;i++)
    {
      if(!(actors[i].field_62 & 0x20))
      {
//      printf("Processing actor %d...\n",i);
        if(actors[i].life == 0)
        {
 //       printf("Unsuported actor[i].field_68\n");
        }
//      updateActors(i);

        actors[i].field_20=actors[i].field_1A;
        actors[i].field_22=actors[i].field_1C;
        actors[i].field_24=actors[i].field_1E;

        if(actors[i].field_46 != -1)
        {
//        printf("Unsuported actor field 46\n");
        }

//      processActor(i);

        if(actors[i].field_60 & 4)
        {
//        runSceneScript(&actors[i],i);
        }
        if(actors[i].positionInActorScript!=-1)
        {
          runActorScript(i);
        }

     // implementer la suite...

      }
    }

    if(mainLoopVar10)
    {
    // implement this
    }
    fullRedraw(mainLoopVar2);
    mainLoopVar2=0;
    counter++;
  }while(1);
}

void LBA_engine::reinitAll(int save)
{
  maximizeTextWindow();
  mainTab[6]=896;
  mainTab[7]=950;
  reinitAll1();
  reinitVars();
  reinitAll3();
  GV9=0x2000;
  GV10=0x1800;
  GV11=0x2000;
  currentRoom=-1;
  reinitVar3=-1;
  numClover=2;
  numCloverBox=2;
  reinitVar4=-1;
  needChangeRoom=0;
  GV6=0;
  GV7=0;
  numCoin=0;
  numKey=0;
  newGameVar2=0;
  GV18=0;
  currentTextBank=0;
  GV15=0;
  reinitVar7=0;
  reinitVar8=0;
  reinitVar9=0;
  comportement=0;
  reinitVar10=0;
/*  if(save==-1)
  {
    loadSaveGame();
    if(GV9==-1)
    {
      reinitVar11=0;
    }
  } */
  fadeOut((char*)menuPal);
  resetVideoBuffer1();
  osystem->drawBufferToScreen(videoBuffer1);
}

void LBA_engine::reinitAll1(void)
{
  setSomething(311,240,512);
  setSomething2(0,0,0);
  setSomething3(0,0,0);
  setSomething4(mainTab[7],mainTab[6],0);
}


void LBA_engine::setSomething(int a, int b, int c)
{
  setSomethingVar1=a;
  setSomethingVar2=b;
  setSomethingVar3=c;
  setSomethingVar4=1;
}

void LBA_engine::setSomething2(int a, int b, int c)
{
  mainTab[0]=a;
  mainTab[1]=b;
  mainTab[2]=c;
}

void LBA_engine::setSomething3(int a,int b,int c)
{

  int var1;
  int var2;
  int var3;
  int var4;
  int var5;
  int var6;
  int var7;

  mainTab[9]=a&0x3FF;

  var1=tab1[mainTab[9]];


  var2=tab1[(mainTab[9]+256)&0x3FF];





  mainTab[11]=c&0x3FF;

  var3=tab1[mainTab[11]];
  mainTab[39]=tab1[(mainTab[11]+256)&0x3FF];

  mainTab[40]=-var3;


  mainTab[42]=(var3*var2)>>14;
  mainTab[43]=(mainTab[39]*var2)>>14;
  mainTab[45]=(var3*var1)>>14;
  mainTab[46]=(mainTab[39]*var1)>>14;


  mainTab[10]=b&0x3FF;


  var4=tab1[mainTab[10]];

  var5=tab1[(mainTab[10]+256)&0x3FF];

  mainTab[39]=(mainTab[39]*var5)>>14;
  mainTab[41]=(var4*mainTab[39])>>14;


  var6=mainTab[42];

  mainTab[42]=((var6*var5)+(var4*var1))>>14;
  mainTab[44]=((var5*var1)+(var6*var4))>>14;



  var7=mainTab[45];



  mainTab[45]=((var2*var4)+(var5*var7))>>14;

  mainTab[47]=((var2*var5)+(var4*var7))>>14;

//  setSomething3Sub(setSomething2Var3,setSomething2Var2,setSomething2Var1);

  mainTab[2]=mainTab[18];
  mainTab[3]=mainTab[19];
  mainTab[4]=mainTab[20];

}

void LBA_engine::setSomething4(int a, int b, int c)
{
  mainTab[6]=a;
  mainTab[12]=a;
  mainTab[7]=b;
  mainTab[13]=b;
  mainTab[8]=c;

  mainTab[14]=c;


  renderS1S1(&mainTab[21],&mainTab[39]);
  renderS2S2(0,0,59);

  mainTab[15]=mainTab[18];
  mainTab[16]=mainTab[19];
  mainTab[17]=mainTab[20];
}

void LBA_engine::renderS2S2(short int ax, short int bx, short int cx)
{
  int ebp;
  int ebx;
  int ecx;
  int eax;
  int edi;

  ebp=ax;
  ebx=bx;
  ecx=cx;

  edi=mainTab[21];
  eax=mainTab[22];
  edi*=ebp;
  eax*=ebx;
  edi+=eax;
  eax=mainTab[23];
  eax*=ecx;
  eax+=edi;
  eax>>=14;

  mainTab[18]=eax;

  edi=mainTab[24];
  eax=mainTab[25];
  edi*=ebp;
  eax*=ebx;
  edi+=eax;
  eax=mainTab[26];
  eax*=ecx;
  eax+=edi;
  eax>>=14;
  mainTab[19]=eax;



  ebp*=mainTab[27];
  ebx*=mainTab[28];
  ecx*=mainTab[29];
  ebx+=ebp;
  ebx+=ecx;
  ebx>>=14;
  mainTab[20]=eax;

}

void LBA_engine::mainLoop2(int arg_0)
{
  if(!drawInGameTransBox)
   return;

 if(useSamples)
 {
  // code de son non géré...
 }

 mainLoop2sub1();

 if(isMenuDisplayed)
   osystem->setPalette(menuPal);
 else
   osystem->setPalette(palette);

 drawInGameTransBox=0;

 // code son non gégé...


 if(!arg_0)
   fullRedraw(1);


}

void LBA_engine::mainLoop2sub1(void)


{
  //code dpmi non géré.

 initVideoVar1=-1;
}

void LBA_engine::mainLoop1(short int arg0, short int arg4, short int arg8, timeStruct *ptr)
{
  ptr->var1=arg0;
  ptr->var2=arg4;
  ptr->var3=arg8;
  ptr->var4=time;
}

void LBA_engine::waitRetrace(void)
{
}

void LBA_engine::freezeTime(void)

{
  if(!time1)

   time3=time;

  time1++;

}

void LBA_engine::unfreezeTime(void)
{

  if(--time<=0)
   time=time3;
}

int LBA_engine::mainLoop4(void)        //process le menu "continuer ou abandonner"
{
  int saveLangue;
 int temp;

  copyToBuffer(videoBuffer1,videoBuffer2);
 mainMenu2();

 do
 {
   saveLangue=languageCD1;
  languageCD1=0;
  loadTextBank(0);
  temp=processMenu(subMenu2Data);
  languageCD1=saveLangue;
  loadTextBank(currentTextBank+3);
 }while(temp!=27 && temp!=28);

  if(temp==27)
    return(1);
 if(temp==28)
    return(0);

 return(0);
}

void LBA_engine::reinitVars(void)
{
  int i;

  //reinitData();

 for(i=0;i<10;i++)
   roomData2[i].field_0=-1;

 for(i=0;i<80;i++)
   roomData1[i]=0;

 for(i=0;i<255;i++)
   vars[i]=0;

 for(i=0;i<28;i++)
   GV16[i]=0;

 sceneVar2.field_0=-1;
 sceneVar2.field_2=-1;
 sceneVar2.field_4=-1;
 sceneVar2.field_6=-1;

 sceneVar3.field_0=1;
 sceneVar3.field_2=1;
 sceneVar3.field_4=1;
 sceneVar3.field_6=1;

 sceneVar4.field_0=1;
 sceneVar4.field_2=1;
 sceneVar4.field_4=1;
 sceneVar4.field_6=1;

 for(i=0;i<150;i++)
   GV14[i]=0;

 numActorInRoom=0;
 reinitAll2Var3=0;
 reinitAll2Var4=0;

 reinitAll2Var5=0;

}

void LBA_engine::reinitAll3(void)
{
  resetActor(0);
 changeRoomVar7=-1;
 numCloverBox=2;
 numClover=2;
 numCoin=0;
 numKey=0;
 GV7=0;
 GV18=0;
 twinsen->field_0=0;
 twinsen->life=50;

 twinsen->field_16=4;

}

void LBA_engine::drawInGameMenu(void)
{

 int languageCD1temp;
 int var_4;
 int savedLevel;

  freezeTime();

 if(comportement==4)
 {
  mainMenu2();
  changeTwinsenComp(0);
 }

 currentCostume=bodyPtrTab[twinsen->costumeIndex];

 TCOS[0]=TCos0Init;
 TCOS[1]=TCos1Init;
 TCOS[2]=TCos2Init;
 TCOS[3]=TCos3Init;

 setActorTime(twinsen->field_32,twinsen->field_32-256,50,&timeVar);


 copyToBuffer(videoBuffer1,videoBuffer2);

 languageCD1temp=languageCD1;
 languageCD1=0;
 var_4=currentTextBank;
 currentTextBank=-1;

 loadTextBank(0);

 drawMenuWin(twinsen->field_32);

 savedLevel=comportement;

 drawMenuWin1(winTab[comportement],getHqrdataPtr(HQRanims,TCOS[comportement]),currentCostume);

 while(key1&4 || (skipIntro>59 && skipIntro<62))
 {
  readKeyboard();

  key=printTextVar12;

  if(key&8)
    comportement++;

  if(key&4)
    comportement--;

  if(comportement<0)          // J'ai reecrit ca à ma sauce
    comportement=3;

  if(comportement>=4)
    comportement=0;

  if(savedLevel!=comportement)
  {
   drawInGameMenuEntry(comportement,twinsen->field_32,1);
   savedLevel=comportement;
   setActorTime(twinsen->field_32,twinsen->field_32-256,50,&timeVar);
   drawMenuWin1(winTab[comportement],getHqrdataPtr(HQRanims,TCOS[comportement]),currentCostume);

   while(printTextVar12)
   {
    readKeyboard();
    drawInGameMenuEntry(comportement,-1,1);
   }
  }
  drawInGameMenuEntry(comportement,-1,1);
 }

 changeTwinsenComp(comportement);

 reinitAll1();

 currentTextBank=var_4;
 loadTextBank(currentTextBank+3);

 languageCD1=languageCD1temp;


 unfreezeTime();

}

void LBA_engine::drawMenuWin(short int var)
{

  drawBoxOutLine(100,100,550,290);
  drawBoxInsideTrans(101,101,549,289,2);

  drawMenuWin1(winTab[0],getHqrdataPtr(HQRanims,TCOS[0]),currentCostume);
  drawInGameMenuEntry(0,var,0);

  drawMenuWin1(winTab[1],getHqrdataPtr(HQRanims,TCOS[1]),currentCostume);
  drawInGameMenuEntry(1,var,0);

  drawMenuWin1(winTab[2],getHqrdataPtr(HQRanims,TCOS[2]),currentCostume);
  drawInGameMenuEntry(2,var,0);

  drawMenuWin1(winTab[3],getHqrdataPtr(HQRanims,TCOS[3]),currentCostume);
  drawInGameMenuEntry(3,var,0);

  drawMenuWin2(100,300);

  osystem->refresh(videoBuffer1,100,100,550,290);
}

void LBA_engine::drawInGameMenuEntry(int anim, int arg, int costume)
{
  short int temp;
  unsigned char * temp2;
  int temp3;
  int var_4;
  int var_C;
  int var_8;
  int var_10;

 temp=anim*110+110;
 var_4=temp+99;

 temp2=getHqrdataPtr(HQRanims,TCOS[anim]);

 var_C=110;
 var_8=229;

 temp3=winTab[anim];

/* if(draw3D1(temp3,temp2,currentCostume))
 {
  temp3++;
  if(temp3==draw3D2(temp2))
  {
   temp3=draw3D3(temp2);
  }
  winTab[anim]=temp3;
 }   */

 if(costume==0)
   drawBoxOutLine(temp-1,var_C-1,var_4+1,var_8+1);

 saveTextWindow();
 maximizeTextWindow();

 if(anim!=comportement)
 {
  drawBlackBox(temp,var_C,var_4,var_8,0);
 }
 else
 {
  drawBlackBox(temp,var_C,var_4,var_8,68);
  drawBlackBox(110,239,540,279,0);
  drawBoxOutLine(110,239,540,279);

  setTextColor(15);
  if(comportement== 2 && agressivity == 1)
  {
   printString(4,dataString);       // pour afficher le "auto" ou non
  }
  else
    printString(comportement,dataString);

  printStringSimple((650-getStringLength(dataString))/2,240,dataString); // ecrit le nom du comportement dans lequel on se trouve
 }

 var_10=var_C;

 draw3D4(temp,var_C,var_4,var_8,-600,arg,currentCostume);   // dessine le model

 osystem->refresh(videoBuffer1,temp,var_10,var_4,var_8);
 osystem->refresh(videoBuffer1,110,239,540,279);

 loadSavedTextWindow();

}

void LBA_engine::draw3D4(short int arg_0, short int arg_4, short int arg_8, short int arg_C, short int arg_10, short int arg_14, unsigned char * costumePtr)
{
 int var_4;
  int temp1;
  int temp2;
 int temp,tempbis;

  var_4=arg_8;


  temp1=arg_C+arg_4;
  temp1>>=1;

  temp2=arg_8+arg_0;
  temp2>>=1;

  setSomething(temp2,temp1,0);
  setTextWindowSize(arg_0,arg_4,var_4,arg_C);


 if(arg_14==-1)
 {
/*  temp=draw3D4sub1(&timeVar);

  if(timeVar.var3==0)
  {
    tempbis=temp;
    setActorTime(tempbis,temp-256,50,&timeVar);
  }
  startRenderer(0,arg_10,0,0,1,0,costumePtr); */
 }
 else
   startRenderer(0,arg_10,0,0,arg_14,0,costumePtr);

}

int LBA_engine::draw3D4sub1(timeStruct* arg_0)
{
  // implement this
  return(0);
}

void LBA_engine::setTextWindowSize(int left, int top, int right, int bottom)
{
  if(left<0)
    left=0;

  textWindowLeft=left;

  if(top<0)
    top=0;
  textWindowTop=top;


  if(right>largeurEcran)
    right=largeurEcran-1;
  textWindowRight=right;

  if(bottom>hauteurEcran)
    bottom=hauteurEcran-1;
  textWindowBottom=bottom;
}




int LBA_engine::drawMenuWin1(int arg_0, unsigned char * ptr, unsigned char * arg_8)
{

  unsigned char * lptr2;
  short int* lptr;
// short int temp;


 lptr=(short int*) ptr;

  if(arg_0<*(lptr++))
   return(0);

 lptr2=*(lptr)*8+ptr+8;

 if(*(short int*)arg_8 & 2)
   return(0);

 printf("unimplemented drawMenuWin1\n");


 exit(1);

  // implementer
  return(0);
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
  drawBoxOutLine(arg_0,arg_4,arg_0+450,arg_4+80);
  drawBlackBox(arg_0+1,arg_4+1,arg_0+449,arg_4+79,0);
  var_14=arg_0+9;
 drawSprite(0,var_14,arg_4+13,getHqrdataPtr(HQRPtrSpriteExtra,4));
 var_10=arg_0+325;
 temp=arg_0+25;
 var_4=remapComposante(temp,var_10,50,twinsen->life);

 var_8=arg_4+10;
 var_C=arg_4+25;
  drawBlackBox(temp,var_8,var_4,var_C,91);
  drawBlackBox(var_4,var_8,var_10,var_C,0);

 drawBoxOutLine(arg_0+25,arg_4+10,arg_0+325,arg_4+10+15);


 if(!vars[28] && vars[4])                           // ca doit etre le mana

 {
  drawSprite(0,var_14,arg_4+36,getHqrdataPtr(HQRPtrSpriteExtra,5));
  printf("Unimplemented drawMenuWin2\n");
  exit(1);
  // implementer la suite

 }

 var_4=arg_0+340;

 drawSprite(0,var_4,arg_4+15,getHqrdataPtr(HQRPtrSpriteExtra,3));                  // draw coins
 setTextColor(155);
  printStringSimple(arg_0+370,arg_4+5,getStringFromNum(numCoin)); // amount of coins

 drawSprite(0,var_4,arg_4+55,getHqrdataPtr(HQRPtrSpriteExtra,6));       // draw key
 setTextColor(155);
 printStringSimple(arg_0+370,arg_4+40,getStringFromNum(numKey));

 for(i=0;i<numCloverBox;i++) // boites à trefles
 {
  drawSprite(0,remapComposante(arg_0+25,arg_0+325,10,i),arg_4+58,getHqrdataPtr(HQRPtrSpriteExtra,41));
 }

 for(i=0;i<numClover;i++) // trefles
 {
  drawSprite(0,remapComposante(arg_0+25,arg_0+325,10,i)+2,arg_4+60,getHqrdataPtr(HQRPtrSpriteExtra,7));
 }

 osystem->refresh(videoBuffer1,arg_0,arg_4,arg_0+450,arg_4+80);

}


void LBA_engine::drawBlackBox(int left,int top,int right,int bottom, unsigned char e)
{

  unsigned char *ptr;

 int offset;

 int x;
 int y;

  if(left>textWindowRight)
   return;
 if(right<textWindowLeft)

   return;
 if(top>textWindowBottom)
   return;
 if(bottom<textWindowTop)
   return;

 // cropping

 offset=-((right-left)-largeurEcran);


 ptr=videoBuffer1+screenLockupTable[top]+left;

 for(x=top;x<bottom;x++)
 {
  for(y=left;y<right;y++)
  {

   *(ptr++)=e;
  }
  ptr+=offset;
 }

}

unsigned char* LBA_engine::getHqrdataPtr(hqr_entry* hqrPtr, short int arg_4)
{

  FILE* fin;
  int headerSize;
  int offToData;
  int dataSize;
  int compressedSize;
  short int mode;

 short int var_4;
 int ltime;
 int temp2;
 unsigned char* ptr;

 int i;

 int dataSize2;
 subHqr* hqrdata;
 subHqr* hqrdataPtr;

  if(arg_4 < 0)
   return(0);

 hqrdata=(subHqr*)((unsigned char*)hqrPtr+sizeof(hqr_entry));

 hqrdataPtr = findSubHqr(arg_4,hqrPtr->unk,hqrdata);

 if(hqrdataPtr)
 {
    hqrdataPtr->lastAccessedTime=time;
  drawInventoryVar=0;
  return(hqrdataPtr->offFromPtr+hqrPtr->ptr);
 }

 prepareResource(hqrPtr->fileName,arg_4);

 fin=openResource(hqrPtr->fileName);

  if(!fin)
    return(0);

  readResourceData(fin,(char*)&headerSize,4);

  if(arg_4>=headerSize/4)
  {
    closeResource(fin);
    return(0);
  }

  fseek(fin,arg_4*4,SEEK_SET);
  readResourceData(fin,(char*)&offToData,4);

  fseek(fin,offToData,SEEK_SET);
  readResourceData(fin,(char*)&dataSize,4);
  readResourceData(fin,(char*)&compressedSize,4);
  readResourceData(fin,(char*)&mode,2);

 dataSize2=dataSize;

 // ici, test sur la taille de dataSize

 ltime=time;

 while(dataSize2 >= hqrPtr->remainingSize || hqrPtr->unk>=hqrPtr->b)  // pour retirer les elements les plus vieux jusqu'a ce qu'on ai de la place

 {
  var_4=0;
  temp2=0;

  for(i=0;i<hqrPtr->unk;i++)
  {
   if(temp2<=ltime-hqrdata[i].lastAccessedTime)
   {
    temp2=ltime-hqrdata[var_4].lastAccessedTime;
    var_4=i;
   }
  }

  drawInventory2(hqrPtr,var_4);

 }

 ptr=hqrPtr->ptr+hqrPtr->size1-hqrPtr->remainingSize;


  if(mode<=0) // uncompressed
  {
    readResourceData(fin,(char*)ptr,dataSize);
  }
  else
  {
    if(mode==1) // compressed
    {
      readResourceData(fin,(char*)(ptr+dataSize-compressedSize+500),compressedSize);
      decompress(dataSize,ptr,(ptr+dataSize-compressedSize+500));
    }

    else
    {
      closeResource(fin);
   return(0);
    }
  }

 closeResource(fin);


 hqrdataPtr=&hqrdata[hqrPtr->unk];

 hqrdataPtr->index=arg_4;
 drawInventoryVar=1;
 hqrdataPtr->lastAccessedTime=time;
 hqrdataPtr->offFromPtr=hqrPtr->size1-hqrPtr->remainingSize;
 hqrdataPtr->size=dataSize2;


 hqrPtr->unk++;
 hqrPtr->remainingSize-=dataSize2;

 return(ptr);



}

subHqr* LBA_engine::findSubHqr(int arg_0, int arg_4, subHqr * arg_8)
{
 subHqr * temp;
 int i;

  if(arg_4 == 0)
   return(0);

 temp=arg_8;

 for(i=0;i<arg_4;i++)
 {
  if(temp->index==arg_0)
    return(temp);

  temp++;
 }

 return(0);

}

int LBA_engine::drawInventory2(hqr_entry *hqrPtr, int var)
{
  subHqr* subPtr;

  int lvar;

 unsigned char * source;
 unsigned char * dest;
 int size;
 int retVal;


 lvar=var;
 
 subPtr=(subHqr*)(hqrPtr+sizeof(hqr_entry)+var*sizeof(subHqr)); 
 
 retVal=subPtr->size;
 
 if(hqrPtr->unk -1 > lvar)
 {
  source=hqrPtr->ptr+subPtr->offFromPtr;

  dest=source+retVal;
  size=hqrPtr->ptr+hqrPtr->size1-dest;
  
  memmove(source,dest,size);
  
  printf("Unsupported drawInventory2\n");
  exit(1);
  // implementer la suite
 }
 
 hqrPtr->unk--;
 
 hqrPtr->remainingSize=retVal;
 
 return(retVal);
}