#include "syscall.h"
#include <common.h>

int fs_open(const char *pathname, int flags, int mode);
size_t fs_read(int fd, void *buf, size_t len);
size_t fs_write(int fd, const void *buf, size_t len);
size_t fs_lseek(int fd, size_t offset, int whence);
int fs_close(int fd);

//#define STRACE
#ifdef STRACE
const char* syscall_names[] = {
    "SYS_exit",
    "SYS_yield",
    "SYS_open",
    "SYS_read",
    "SYS_write",
    "SYS_kill",
    "SYS_getpid",
    "SYS_close",
    "SYS_lseek",
    "SYS_brk",
    "SYS_fstat",
    "SYS_time",
    "SYS_signal",
    "SYS_execve",
    "SYS_fork",
    "SYS_link",
    "SYS_unlink",
    "SYS_wait",
    "SYS_times",
    "SYS_gettimeofday"
};
#endif

int write(int fd, uint8_t *buf, size_t count)
{
    int num = 0;
    if (fd == 0 || fd == 1)
    {
        for (int i = 0; i < count && *buf != '\0'; i++)
        {
            putch(*buf);
            buf++;
            num++;
        }
    }

    return num;
}

int gettimeofday(struct timeval *tv, intptr_t tz);

uintptr_t sbrk(intptr_t address)
{
    return 0;
}

void do_syscall(Context *c)
{
    uintptr_t a[4];
    a[0] = c->GPR1;
    a[1] = c->GPR2;
    a[2] = c->GPR3;
    a[3] = c->GPR4;
#ifdef STRACE
    printf("syscall\ntype : %s\n", syscall_names[a[0]]);
    printf("a0 : %d, a1 : %d, a2 : %d\n", a[1], a[2], a[3]);
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
    case 2:
        c->GPRx = fs_open((char *)a[1], a[2], a[3]);
        break;
    case 3:
        c->GPRx = fs_read(a[1], (void*)a[2], a[3]);
        break;
    case 4:
        c->GPRx = fs_write((int)a[1], (uint8_t *)a[2], (size_t)a[3]);
        break;
    case 7:
        c->GPRx = fs_close(a[1]);
        break;
    case 8:
        c->GPRx = fs_lseek(a[1], a[2], a[3]);
        break;
    case 9:
        c->GPRx = sbrk(a[1]);
        break;
    case 11:
        c->GPRx = gettimeofday((struct timeval *)a[1], a[2]);
        break;
    default:
        panic("Unhandled syscall ID = %d", a[0]);
    }
}
