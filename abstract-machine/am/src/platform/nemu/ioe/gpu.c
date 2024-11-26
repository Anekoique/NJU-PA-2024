#include <am.h>
#include <nemu.h>
#include <stdio.h>

#define SYNC_ADDR (VGACTL_ADDR + 4)
#define N 32

void __am_gpu_init() {
    int i;
    int w = io_read(AM_GPU_CONFIG).width / N;  // TODO: get the correct width
    int h = io_read(AM_GPU_CONFIG).height / N;  // TODO: get the correct width
    uint32_t *fb = (uint32_t *)(uintptr_t)FB_ADDR;
    for (i = 0; i < w * h; i ++) fb[i] = i;
    outl(SYNC_ADDR, 1);
}
void __am_gpu_config(AM_GPU_CONFIG_T *cfg)
{
    uint32_t code = inl(VGACTL_ADDR);
    int height = code & 0xffff;
    int width = (code >> 16) & 0xffff;
    *cfg = (AM_GPU_CONFIG_T){.present = true, .has_accel = false, .width = width, .height = height, .vmemsz = width * height};
}

void __am_gpu_fbdraw(AM_GPU_FBDRAW_T *ctl)
{
    if (ctl->sync)
    {
        outl(SYNC_ADDR, 1);
    }
    else 
    {
        for (int y = 0; y < N; y++)
        {
            for (int x = 0; x < N; x++)
            {
                outl(FB_ADDR + y * ctl->w + x, ((uint32_t *)ctl->pixels)[y * N + x]);
            }
        }
    }
}

void __am_gpu_status(AM_GPU_STATUS_T *status)
{
    status->ready = true;
}
