#ifndef _LBA_
#define _LBA_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#ifdef WIN32
#define PCLIKE
#endif

#ifdef UNIX
#define PCLIKE
#endif

#ifdef PCLIKE
#include<malloc.h>
#endif // PCLIKE

#ifndef PCLIKE
#include <shinobi.h>    /* Shinobi system routines. */
#include <kamui2.h>     /* Kamui2 low-level graphics HAL. */
#include <sn_fcntl.h>   /* LibCross file types. */
#include <usrsnasm.h>   /* LibCross I/O routines. */
#include <sg_syCbl.h>   /* NTSC/RGB/VGA Cable check interface. */

#define PRELOAD_ALL

#endif

#include<assert.h>

#include"type.h"


#include "HQRLib.h"
#include "HQMLib.h"

#include "streamReader.h"

#ifndef MEM_DEBUG
#define Malloc(size) malloc(size)
#define Free(ptr) free(ptr)
#endif


#ifdef _DEBUG
    #define todo(string) printf("TODO %s:%d %s\n", __FILE__, __LINE__, string)
#else // _DEBUG
    #define todo(string)
#endif // _DEBUG

#ifdef _DEBUG
    #define assert_ptr(ptr) assert((int)ptr!=0xCDCDCDCD)
#else // _DEBUG
    #define assert_ptr(ptr)
#endif // _DEBUG

#ifndef PCLIKE
#define exit(string) printf("Exit required\n")
#endif

class LBA_engine;

struct mouseStatusStruct
    {
	int left;
	int right;
	int X;
	int Y;
    };

struct actorBoxStruct
    {
	int actorNum;
	int left;
	int right;
	int top;
	int bottom;
    };

// memory sizes
#define BUF_SPEAK_SIZE 262176
#define BUF_MEMOSEEK_SIZE 2048
#define BUF_TEXT_SIZE 25000
#define BUF_ORDER_SIZE 1024
#define BUF_ANIM_SIZE 5000
#define BUF_INVENTORY_SIZE 20000

#define BUF_HQM_SIZE 400000

// defines for generic animations
#define ANIM_static 0
#define ANIM_walk 1
#define ANIM_walkBackward 2
#define ANIM_turnLeft 3
#define ANIM_turnRight 4
#define ANIM_hit 5 
#define ANIM_hitBig 6 
#define ANIM_fall 7
#define ANIM_land 8
#define ANIM_landHit 9
#define ANIM_landDeath 10
#define ANIM_activate 11
#define ANIM_climbUpLadder 12
#define ANIM_reachTopOfLadder 13
#define ANIM_jump 14
#define ANIM_throwBall 15
#define ANIM_hide 16
#define ANIM_kick 17
#define ANIM_rightPunch 18
#define ANIM_leftPunch 19
#define ANIM_findObject 20
#define ANIM_drown 21
#define ANIM_hit2 22
#define ANIM_swordAttack 23
#define ANIM_drawSword 24
#define ANIM_fightMode 25
#define ANIM_fightAttack 26
#define ANIM_run 27 // lba2= push
#define ANIM_joinHands 28 // lba2 = talk
#define ANIM_pushMode 29
#define ANIM_pushForward 30 // lba2= climb down ladder

#define ANIM_useSwitch 31
#define ANIM_useUnk 32

#define ANIM_crawl 33
#define ANIM_climbInHole 34
#define ANIM_useUnk2 35
#define ANIM_useSarbacan 36
#define ANIM_dead 37
#define ANIM_sportifTurnLeft 38
#define ANIM_backwardJump 39
#define ANIM_crawl2 40
#define ANIM_usePotion 41

// lba2 48 = use switch
// lba2 49 = crawl

//#include "opcodes.h"
#include "osystem.h"
#include "angle.h"
#include "costume.h"
#include "debug.h"
#include "zones.h"
#include "vars.h"

#endif

