#include "lba.h"
#include "SDL.h"
#include "SDL_thread.h"

LBA_engine *tempEngine;

int threadTimer(void *test)	// LBA time is 50 frames per seconds ie,  a frame every 20 miliseconds
{
    while (1)
	{
	    SDL_Delay(15); // granularity restriction
	    tempEngine->musicPosition++;
	    tempEngine->time++;
	}
    return (0);
}

void startThreadTimer(LBA_engine * lengine)
{
    tempEngine = lengine;
    SDL_CreateThread(threadTimer, NULL);
}
