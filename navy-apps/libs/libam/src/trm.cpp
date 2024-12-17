#include <am.h>
#include <navy.h>

Area heap;
extern void _exit(int);

void putch(char ch)
{
    printf("%c", ch);
}

void halt(int code)
{
    _exit(code);
}
