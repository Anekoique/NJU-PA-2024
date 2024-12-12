#include "debug.h"
#include <NDL.h>
uint32_t SDL_GetTicks();

int SDL_Init(uint32_t flags)
{
    SDL_GetTicks();
    return NDL_Init(flags);
}

void SDL_Quit()
{
    NDL_Quit();
}

char *SDL_GetError()
{
    panic("Please implement me!");
    return "Navy does not support SDL_GetError()";
}

int SDL_SetError(const char *fmt, ...)
{
    //panic("Please implement me!");
    return -1;
}

int SDL_ShowCursor(int toggle)
{
    panic("Please implement me!");
    return 0;
}

void SDL_WM_SetCaption(const char *title, const char *icon)
{
    panic("Please implement me!");
}
