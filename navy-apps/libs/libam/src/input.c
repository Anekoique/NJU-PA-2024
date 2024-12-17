#include <am.h>
#include <navy.h>

#define NAME(key) [AM_KEY_##key] = #key,
static const char *keyname[256] __attribute__((used)) = {[AM_KEY_NONE] = "NONE", AM_KEYS(NAME)};
#define KEYNUM (sizeof(keyname) / sizeof(keyname[0]))

void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd)
{
    kbd->keydown = 0;
    kbd->keycode = AM_KEY_NONE;

    char buf[64];
    if (NDL_PollEvent(buf, sizeof(buf)))
    {
        char type, name[64];
        sscanf(buf, "k%c %s", &type, name);
        printf("%c , %s\n", type, name);
        kbd->keydown = ((type == 'u') ? false : true);
        for (int i = 0; i < KEYNUM; i++)
        {
            printf("%s\n", keyname[i]);
            if (strcmp(name, keyname[i]) == 0)
            {
                kbd->keycode = i;
                break;
            }
        }
    }
}
