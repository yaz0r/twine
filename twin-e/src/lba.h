#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<malloc.h>

typedef unsigned char byte;
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned long uint32;
typedef unsigned int uint;
typedef signed char int8;
typedef signed short int16;
typedef signed long int32;

class LBA_engine;

struct mouseStatusStruct {
    int left;
    int right;
    int X;
    int Y;
};

struct actorBoxStruct {
    int actorNum;
    int left;
    int right;
    int top;
    int bottom;
};

#include"osystem.h"
#include"angle.h"
#include"costume.h"
#include"debug.h"
#include"vars.h"
