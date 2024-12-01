#include "syscall.h"
#include <common.h>
void do_syscall(Context *c)
{
    uintptr_t a[4];
    a[0] = c->GPR1;
    a[1] = c->GPR2;
    a[2] = c->GPR3;
    a[3] = c->GPR4;
#ifdef STRACE
    printf("syscall\ntype : %s\n", syscall_type[a[0]]);
    printf("a0 : %d, a1 : %d, a2 : %d, a3 : %d\n", a[1], a[2], a[3]);
    printf("mcause : %d, mepc : %d\n", c->mcause, c->mepc);
#endif
    switch (a[0])
    {
    case 1:
        yield();
        c->GPRx = 0;
        break;
    case 0:
        printf("here\n");
        halt(a[1]);
        break;
    default:
        panic("Unhandled syscall ID = %d", a[0]);
    }
}
