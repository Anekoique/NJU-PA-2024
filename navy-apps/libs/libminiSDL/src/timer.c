#include "debug.h"
#include <NDL.h>
#include <sdl-timer.h>
#include <stdio.h>

SDL_TimerID SDL_AddTimer(uint32_t interval, SDL_NewTimerCallback callback, void *param)
{
    panic("Please implement me!");
    return NULL;
}

int SDL_RemoveTimer(SDL_TimerID id)
{
    panic("Please implement me!");
    return 1;
}

uint32_t SDL_GetTicks()
{
    panic("Please implement me!");
    return 0;
}

void SDL_Delay(uint32_t ms)
{
    panic("Please implement me!");
}
