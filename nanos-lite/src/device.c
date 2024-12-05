#include "syscall.h"
#include <common.h>

#if defined(MULTIPROGRAM) && !defined(TIME_SHARING)
#define MULTIPROGRAM_YIELD() yield()
#else
#define MULTIPROGRAM_YIELD()
#endif

#define NAME(key) [AM_KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {[AM_KEY_NONE] = "NONE", AM_KEYS(NAME)};

int gettimeofday(struct timeval *tv, intptr_t tz)
{
    long int now = io_read(AM_TIMER_UPTIME).us;
    printf("time : %ld\n", now);
    tv->tv_sec = now / 1000000;
    tv->tv_uesc = now % 1000000;
    return 0;
}

size_t serial_write(const void *buf, size_t offset, size_t len)
{
    int num = 0;
    for (int i = 0; i < len && *(uint8_t *)buf != '\0'; i++)
    {
        putch(*(uint8_t *)buf);
        buf++;
        num++;
    }
    return num;
}

size_t events_read(void *buf, size_t offset, size_t len)
{
    return 0;
}

size_t dispinfo_read(void *buf, size_t offset, size_t len)
{
    return 0;
}

size_t fb_write(const void *buf, size_t offset, size_t len)
{
    return 0;
}

void init_device()
{
    Log("Initializing devices...");
    ioe_init();
}
