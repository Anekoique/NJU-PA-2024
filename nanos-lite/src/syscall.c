#include "syscall.h"
#include <common.h>
void do_syscall(Context *c)
{
    uintptr_t a[4];
    a[0] = c->GPR1;
    a[1] = c->GPR2;
    a[2] = c->GPR3;
    a[3] = c->GPR4;
#ifdef CONFIG_STRACE
    printf("here\n");
#endif
    switch (a[0])
    {
    case 1:
        yield();
        c->GPRx = 0;
        break;
    case 0:
        halt(a[1]);
        break;
    default:
        panic("Unhandled syscall ID = %d", a[0]);
    }
}
