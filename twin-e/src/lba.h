#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<malloc.h>

#include"type.h"

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
#define ANIM_climbDownLadder 13
#define ANIM_jump 14
#define ANIM_throwBall 15
#define ANIM_hide 16
#define ANIM_kick 17
#define ANIM_rightPunch 18
#define ANIM_leftPunch 19
#define ANIM_findObject 20
#define ANIM_drown 21
#define ANIM_hit2 22
#define ANIM_useSword 23
#define ANIM_drawSword 24
#define ANIM_fightMode 25
#define ANIM_fightAttack 26
#define ANIM_run 27
#define ANIM_joinHands 28
#define ANIM_pushMode 29
#define ANIM_pushForward 30

#define ANIM_crawn 31

#define ANIM_useCard 33
#define ANIM_useSarbacan 34
#define ANIM_dead 35
#define ANIM_sporifRightTurn 36
#define ANIM_backWardJump 37
#define ANIM_crawn2 38
#define ANIM_useWater 39

//#include "opcodes.h"
#include "osystem.h"
#include "angle.h"
#include "costume.h"
#include "debug.h"
#include "vars.h"
