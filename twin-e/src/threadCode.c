#include "lba.h"
#include "SDL.h"
#include "SDL_thread.h"

LBA_engine* tempEngine;

int threadTimer(void* test)
{
	while(1)
	{
		SDL_Delay(10);
		tempEngine->musicPosition++;
		tempEngine->time++;
	}
	return(0);
}

void startThreadTimer(LBA_engine* lengine)
{
	tempEngine=lengine;
	SDL_CreateThread(threadTimer,NULL);
}
