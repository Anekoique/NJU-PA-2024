#include <am.h>
#include <navy.h>

Area heap;
extern intptr_t _syscall_(intptr_t, intptr_t, intptr_t, intptr_t);

void putch(char ch)
{
    printf("%c", ch);
}

void halt(int code)
{
    _syscall_(SYS_exit, code, 0, 0);
}
