#include "debug.h"
#include <NDL.h>
#include <SDL.h>
#include <string.h>

#define keyname(k) #k,

static const char *keyname[] = {"NONE", _KEYS(keyname)};
#define KEYNUM (sizeof(keyname) / sizeof(keyname[0]))
static uint8_t keystate[KEYNUM] = { 0 };


int SDL_PushEvent(SDL_Event *ev)
{
    panic("Please implement me!");
    return 0;
}

int SDL_PollEvent(SDL_Event *ev)
{
    char buf[64];
    if (NDL_PollEvent(buf, sizeof(buf)) == 0) {
        return 0;
    } 
    else 
    {
        char type;
        char name[64];
        sscanf(buf, "k%c %s", &type, name);
        
        if (type == 'u') ev->key.type = SDL_KEYUP;
        else if (type == 'd') ev->key.type = SDL_KEYDOWN;
        
        for (int i = 0; i < sizeof(keyname) / sizeof(keyname[0]); i++)
        {
            if (strcmp(name, keyname[i]) == 0)
            {
                if (ev->key.type == SDL_KEYDOWN) 
                    keystate[i] = 1;
                else 
                    keystate[i] = 0;
                ev->key.keysym.sym = i;
                return 1;
            }
        }

    }
    return 0;
}

int SDL_WaitEvent(SDL_Event *event)
{
    while (1)
    {
        char buf[64];
        if (NDL_PollEvent(buf, sizeof(buf)) == 0) {
            continue;
        } 
        else 
        {
            printf("here\n");
            char type;
            char name[64];
            sscanf(buf, "k%c %s", &type, name);
            
            if (type == 'u') event->type = SDL_KEYUP;
            else if (type == 'd') event->type = SDL_KEYDOWN;
            
            for (int i = 0; i < KEYNUM; i++)
            {
                if (strcmp(name, keyname[i]) == 0)
                {
                    event->key.keysym.sym = i;
                    printf("i : %d\n", i);
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
    if (numkeys)
        *numkeys = KEYNUM;
    
    return keystate;
}
