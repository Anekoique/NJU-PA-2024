#include <am.h>
#include <navy.h>

Area heap;
extern int _syscall_(int, uintptr_t, uintptr_t, uintptr_t);

void putch(char ch)
{
    printf("%c", ch);
    return;
}

void halt(int code)
{
    _syscall_(SYS_exit, code, 0, 0);
    return;
}
