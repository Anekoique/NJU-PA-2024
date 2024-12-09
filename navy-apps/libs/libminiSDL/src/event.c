#include "debug.h"
#include <NDL.h>
#include <SDL.h>
#include <string.h>

#define keyname(k) #k,

static const char *keyname[] = {"NONE", _KEYS(keyname)};

int SDL_PushEvent(SDL_Event *ev)
{
    panic("Please implement me!");
    return 0;
}

int SDL_PollEvent(SDL_Event *ev)
{
    panic("Please implement me!");
    return 0;
}

int SDL_WaitEvent(SDL_Event *event)
{
    while (1)
    {
        char buf[64];
        if (NDL_PollEvent(buf, sizeof(buf)) == 0) continue;
        else 
        {
            printf("here\n");
            char type;
            char *name;
            sscanf(buf, "k%c AM_%s", &type, name);
            printf("type : %c, name : %s", type, name);
            
            if (type == 'u') event->key.type = SDL_KEYUP;
            else if (type == 'd') event->key.type = SDL_KEYDOWN;
            
            for (int i = 0; i < sizeof(keyname) / sizeof(keyname[0]); i++)
            {
                if (strcmp(name, keyname[i]) == 0)
                {
                    event->key.keysym.sym = i;
                    return 1;
                }
            }

            panic("con't reach hear\n");
        }
    }
}

int SDL_PeepEvents(SDL_Event *ev, int numevents, int action, uint32_t mask)
{
    panic("Please implement me!");
    return 0;
}

uint8_t *SDL_GetKeyState(int *numkeys)
{
    panic("Please implement me!");
    return NULL;
}
