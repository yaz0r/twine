#include "lba.h"

#ifdef PCLIKE
#include "SDL.h"
#endif //PCLIKE

void TimerPause()
{
#ifdef PCLIKE
    SDL_Delay(300);
#endif //PCLIKE
}
