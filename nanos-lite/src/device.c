#include "syscall.h"
#include <fs.h>
#include <common.h>

#if defined(MULTIPROGRAM) && !defined(TIME_SHARING)
#define MULTIPROGRAM_YIELD() yield()
#else
#define MULTIPROGRAM_YIELD()
#endif

#define NAME(key) [AM_KEY_##key] = #key,

static const char *keyname[256] __attribute__((used)) = {[AM_KEY_NONE] = "NONE", AM_KEYS(NAME)};
static int fb_w;
static int fb_h;

int gettimeofday(struct _timeval *tv, intptr_t tz)
{
    long int now = io_read(AM_TIMER_UPTIME).us;
    //printf("time : %d\n", now);
    tv->tv_sec = now / 1000000;
    tv->tv_uesc = now % 1000000;
    return 0;
}

size_t serial_write(const void *buf, size_t offset, size_t len)
{
    yield();
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
    yield();
    AM_INPUT_KEYBRD_T ev = io_read(AM_INPUT_KEYBRD);
    if (ev.keycode == AM_KEY_NONE) return 0;
    int writed = 0;
    if (ev.keydown) writed = snprintf(buf, len, "%s %s", "kd", keyname[ev.keycode]);
    else writed = snprintf(buf, len, "%s %s", "ku", keyname[ev.keycode]);
    return writed;
}

size_t dispinfo_read(void *buf, size_t offset, size_t len)
{
    int w = fb_w;
    int h = fb_h;
    int writed;
    writed = snprintf(buf, len, "WIDTH : %d\nHEIGHT : %d\n", w, h);
    //printf("writed : %d", writed);
    //serial_write(buf, 0, len);
    return writed;
}

size_t fb_write(const void *buf, size_t offset, size_t len)
{
    yield();
    uint32_t *buffer = (uint32_t *)buf;
    int x = offset / sizeof(uint32_t) % 400;
    int y = offset / sizeof(uint32_t) / 400;
    //printf("x : %d, y : %d, len : %d\n", x, y, len);
    //printf("buffer position : %p\n", buf);
    io_write(AM_GPU_FBDRAW, x, y, buffer, len / sizeof(uint32_t), 1, false);
    io_write(AM_GPU_FBDRAW, 0, 0, NULL, 0, 0, true);
    //if (*(uint8_t *)check_pos != 66 && check_flag) 
    //{
    //    printf("check_pos : %p\n", check_pos);
    //    printf("check_pos : %d\n", *(uint8_t *)check_pos);
    //    printf("error\n");
    //    panic("error");
    //}
    return len;
}

void init_fb(int *w, int *h)
{
    fb_w = io_read(AM_GPU_CONFIG).width;
    fb_h = io_read(AM_GPU_CONFIG).height;
    *w = fb_w;
    *h = fb_h;
}

void init_device()
{
    Log("Initializing devices...");
    ioe_init();
}
