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
    _syscall_((intptr_t)SYS_exit, (intptr_t)code, (intptr_t)0, (intptr_t)0);
}
