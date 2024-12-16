#include <am.h>
#include <navy.h>

Area heap;

void putch(char ch)
{
    putchar(ch);
}

void halt(int code)
{
    const char *fmt = "Exit code = 40h\n";
    for (const char *p = fmt; *p; p++)
    {
        char ch = *p;
        if (ch == '0' || ch == '4')
        {
            ch = "0123456789abcdef"[(code >> (ch - '0')) & 0xf];
        }
        putch(ch);
    }
    __am_exit_platform(code);
    putstr("Should not reach here!\n");
    while (1)
        ;
}
