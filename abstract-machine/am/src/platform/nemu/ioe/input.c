#include <am.h>
#include <nemu.h>

#define KEYDOWN_MASK 0x8000

void __am_input_keybrd(AM_INPUT_KEYBRD_T *kbd)
{
    kbd->keydown = 0;
    kbd->keycode = AM_KEY_NONE;
    uint32_t am_scancode = inl(KBD_ADDR);
    if (am_scancode != 0)
    {
        kbd->keydown = (am_scancode & KEYDOWN_MASK ? true : false);
        kbd->keycode = am_scancode & ~KEYDOWN_MASK;
    }
}
