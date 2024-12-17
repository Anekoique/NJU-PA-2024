#include <am.h>
#include <navy.h>

static int screen_w = 0;
static int screen_h = 0;

void __am_gpu_init()
{
    int ret = open("/proc/dispinfo", 0);
    char buffer[64];
    read(ret, buffer, 64);
    sscanf(buffer, "WIDTH : %d\nHEIGHT : %d\n", &screen_w, &screen_h);
}

void __am_gpu_config(AM_GPU_CONFIG_T *cfg)
{
    *cfg = (AM_GPU_CONFIG_T) {.present = true, .has_accel = false, .width = screen_w, .height = screen_h, .vmemsz = screen_h * screen_w};
}

void __am_gpu_fbdraw(AM_GPU_FBDRAW_T *ctl)
{
    NDL_OpenCanvas(&ctl->w, &ctl->h);
    NDL_DrawRect((void *)ctl->pixels, ctl->x, ctl->y, ctl->w, ctl->h);
}

void __am_gpu_status(AM_GPU_STATUS_T *status)
{
    status->ready = true;
}
