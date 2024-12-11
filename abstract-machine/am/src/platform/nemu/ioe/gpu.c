#include <am.h>
#include <nemu.h>
#include <stdio.h>

#define SYNC_ADDR (VGACTL_ADDR + 4)
#define N 32

static int height = 0;
static int width = 0;

void __am_gpu_init() {
    width = io_read(AM_GPU_CONFIG).width;  // TODO: get the correct width
    height = io_read(AM_GPU_CONFIG).height;  // TODO: get the correct width
}
void __am_gpu_config(AM_GPU_CONFIG_T *cfg)
{
    uint32_t code = inl(VGACTL_ADDR);
    int h= code & 0xffff;
    int w= (code >> 16) & 0xffff;
    *cfg = (AM_GPU_CONFIG_T){.present = true, .has_accel = false, .width = w, .height = h, .vmemsz = width * height};
}

void __am_gpu_fbdraw(AM_GPU_FBDRAW_T *ctl)
{
    if (ctl->sync)
    {
        outl(SYNC_ADDR, 1);
    }
    else 
    {
        uint8_t *fb = (uint8_t *)(uintptr_t)FB_ADDR;
        int i = 0;
        for (int y = ctl->y; y < ctl->h + ctl->y; y++)
        {
            for (int x = ctl->x; x < ctl->w + ctl->x; x++)
            {
                fb[y * width + x] = ((uint8_t *)ctl->pixels)[i++];
                //printf("%p\n", fb + y*width + x);
            }
        }
    }
}

void __am_gpu_status(AM_GPU_STATUS_T *status)
{
    status->ready = true;
}
