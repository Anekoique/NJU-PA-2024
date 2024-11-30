#include "syscall.h"
#include <common.h>
void do_syscall(Context *c)
{
    uintptr_t a[4];
    a[0] = c->GPR1;
    printf("here\n");
    switch (a[0])
    {
    default:
        panic("Unhandled syscall ID = %d", a[0]);
    }
}
