/*
Copyright (C) 2002-2010 The TwinE team

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at youoption) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*/

#ifndef _LBA_
#define _LBA_

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "type.h"

#ifdef WIN32
# define PCLIKE
#endif

#ifdef UNIX
# define PCLIKE
#endif

#ifdef PCLIKE
# ifdef MACOSX
#  include <malloc/malloc.h>
# else
#  include <malloc.h>
# endif
#endif

#ifndef PCLIKE
# ifdef DREAMCAST
#  include <shinobi.h>    /* Shinobi system routines. */
#  include <kamui2.h>     /* Kamui2 low-level graphics HAL. */
#  include <sn_fcntl.h>   /* LibCross file types. */
#  include <usrsnasm.h>   /* LibCross I/O routines. */
#  include <sg_syCbl.h>   /* NTSC/RGB/VGA Cable check interface. */
#  define PRELOAD_ALL
# endif
#endif

#include <assert.h>

#define todo(string) printf("TODO %s:%d %s\n", __FILE__, __LINE__, string)
#define assert_ptr(ptr) /*assert((int)ptr != 0xCDCDCDCD && ptr != NULL)*/

#ifdef MACOSX
# define convertDWFromLE(x) ((x >> 24) | ((x >> 8) & 0x0000ff00) | ((x << 8) & 0x00ff0000) | (x << 24))
# define convertWFromLE(x) (((x>>8)&0xFF)|((x<<8)&0xFF00))
#else
# define convertDWFromLE(x) (x)
# define convertWFromLE(x) (x)
#endif

typedef struct
{
	int left;
	int right;
	int X;
	int Y;
} mouseStatus;

typedef struct
{
	int actorNum;
	int left;
	int right;
	int top;
	int bottom;
} actorBox;


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

typedef enum
{
	holomap,
	ball,
	saber,
	horn,
	tunic,
	book,
	medaillon,
	water,
	redCard,
	blueCard,
	idCard,
	pass,
	protopack,
	snowboard,
	penguin,
	gas,
	flag,
	flute,
	guitar,
	dryer,
	key,
	syrupBottle,
	emptyBottle,
	ticket,
	keypad,
	coffee,
	bonusesList,
	clover
} inventory;

#define SPRITE_coin 3
#define SPRITE_heart 4
#define SPRITE_manabox 5
#define SPRITE_key 6
#define SPRITE_clover 7
#define SPRITE_cloverbox 41

#define NORMAL 0
#define ATHLETIC 1
#define AGRESSIVE 2
#define DISCRETE 3

#define NO_MOVE 0
#define MOVE_MANUAL 1
#define MOVE_FOLLOW 2
#define MOVE_TRACK 3
#define MOVE_FOLLOW_2 4
#define MOVE_TRACK_ATTACK 5
#define MOVE_SAME_XZ 6
#define MOVE_RANDOM 7

#include "osystem.h"
#include "angle.h"
#include "costume.h"
#include "zones.h"
#include "angleTable.h"
#include "overlay.h"

#ifndef MAX_PATH
#define MAX_PATH 256
#endif

#endif
