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
    static uint32_t start_time = 0;
    if (!start_time) start_time = NDL_GetTicks() / 1000000;
    uint32_t current = NDL_GetTicks() / 1000000;
    return current - start_time;
}

void SDL_Delay(uint32_t ms)
{
    uint32_t start = SDL_GetTicks();
    while (1)
    {
        uint32_t current = SDL_GetTicks();
        if (current - start > ms)
            return;
    }
    panic("Con't Reach here\n");
}
