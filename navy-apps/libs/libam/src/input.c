#include <am.h>
#include <navy.h>

#define NAME(key) [AM_KEY_##key] = #key,
static const char *keyname[256] __attribute__((used)) = {[AM_KEY_NONE] = "NONE", AM_KEYS(NAME)};

void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd)
{
    kbd->keydown = 0;
    kbd->keycode = AM_KEY_NONE;

    char buf[64];
    if (!NDL_PollEvent(buf, sizeof(buf)))
    {
        char type, name[64];
        sscanf(buf, "k%c %s", &type, name);
        kbd->keydown = type == 'u' ? false : true;
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
}
