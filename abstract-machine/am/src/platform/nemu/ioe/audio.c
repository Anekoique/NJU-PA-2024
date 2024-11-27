#include <am.h>
#include <nemu.h>
#include <stdio.h>

#define AUDIO_FREQ_ADDR (AUDIO_ADDR + 0x00)
#define AUDIO_CHANNELS_ADDR (AUDIO_ADDR + 0x04)
#define AUDIO_SAMPLES_ADDR (AUDIO_ADDR + 0x08)
#define AUDIO_SBUF_SIZE_ADDR (AUDIO_ADDR + 0x0c)
#define AUDIO_INIT_ADDR (AUDIO_ADDR + 0x10)
#define AUDIO_COUNT_ADDR (AUDIO_ADDR + 0x14)

static uint8_t *sb = (uint8_t *)(uintptr_t)AUDIO_SBUF_ADDR;
static int bufsize;

void __am_audio_init()
{
    bufsize = io_read(AM_AUDIO_CONFIG).bufsize;
}

void __am_audio_config(AM_AUDIO_CONFIG_T *cfg)
{
    cfg->present = true;
    cfg->bufsize = AUDIO_SBUF_ADDR;
}

void __am_audio_ctrl(AM_AUDIO_CTRL_T *ctrl)
{
    outl(AUDIO_FREQ_ADDR, ctrl->freq);
    outl(AUDIO_CHANNELS_ADDR, ctrl->channels);
    outl(AUDIO_SAMPLES_ADDR, ctrl->samples);
    outl(AUDIO_INIT_ADDR, 1);
}

void __am_audio_status(AM_AUDIO_STATUS_T *stat)
{
    stat->count = inl(AUDIO_COUNT_ADDR);
}

void __am_audio_play(AM_AUDIO_PLAY_T *ctl)
{
    uint32_t len = ctl->buf.end - ctl->buf.start;
    int count = io_read(AM_AUDIO_STATUS).count;

    while (len > AUDIO_SBUF_SIZE - count)
    {
        count = io_read(AM_AUDIO_STATUS).count;
    }

    sb = (uint8_t *)((uintptr_t)AUDIO_SBUF_ADDR + io_read(AM_AUDIO_STATUS).count);
    for (uint8_t *i = ctl->buf.start; i < (uint8_t *)(ctl->buf.end); i++)
    {
        *sb = *i;
        sb++;
        if (sb > (uint8_t *)((uintptr_t)AUDIO_SBUF_ADDR + AUDIO_SBUF_SIZE))
            sb = (uint8_t *)(uintptr_t)AUDIO_SBUF_ADDR;
    }
    printf("%p\n", AUDIO_SBUF_ADDR + AUDIO_SBUF_SIZE);

    outl(AUDIO_COUNT_ADDR, count + len);
}
