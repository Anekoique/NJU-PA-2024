#include "syscall.h"
#include <common.h>

int write(int fd, uint32_t *buf, size_t count)
{
    int num = 0;
    if (fd == 0 || fd == 1)
    {
        for (int i = 0; i < count && buf[i] != '\0'; i++)
        {
            putch(buf[i]);
            num++;
        }
    }

    return num * sizeof(char);
}

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
#endif
    switch (a[0])
    {
    case 0:
        halt(a[1]);
        break;
    case 1:
        yield();
        c->GPRx = 0;
        break;
    case 4:
        c->GPRx = write((int)a[0], (uint32_t *)a[1], (size_t)a[2]);
        break;
    default:
        panic("Unhandled syscall ID = %d", a[0]);
    }
}
