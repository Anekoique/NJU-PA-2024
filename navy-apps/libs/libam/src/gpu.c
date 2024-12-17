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
    if (ctl->sync)
        return;
    int fp = open("/dev/fb", 0);
    printf("x %d, y %d, w %d, h %d\n", ctl->x, ctl->y, ctl->w, ctl->h);
    printf("screen_w : %d\n", screen_w);
    for (int i = 0; i < ctl->h; i++)
    {

        lseek(fp, ((ctl->y + i) * screen_w + ctl->x) * sizeof(uint32_t), SEEK_SET);
        write(fp, ((uint32_t *)ctl->pixels) + i * ctl->w, ctl->w * sizeof(uint32_t));
    }
}

void __am_gpu_status(AM_GPU_STATUS_T *status)
{
    status->ready = true;
}
