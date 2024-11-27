/***************************************************************************************
 * Copyright (c) 2014-2024 Zihao Yu, Nanjing University
 *
 * NEMU is licensed under Mulan PSL v2.
 * You can use this software according to the terms and conditions of the Mulan PSL v2.
 * You may obtain a copy of Mulan PSL v2 at:
 *          http://license.coscl.org.cn/MulanPSL2
 *
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
 * EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
 * MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
 *
 * See the Mulan PSL v2 for more details.
 ***************************************************************************************/

#include <SDL2/SDL.h>
#include <common.h>
#include <device/map.h>

enum
{
    reg_freq,
    reg_channels,
    reg_samples,
    reg_sbuf_size,
    reg_init,
    reg_count,
    nr_reg
};

static uint8_t *sbuf = NULL;
static uint32_t *audio_base = NULL;
//static SDL_AudioSpec s = {};

static void audio_io_handler(uint32_t offset, int len, bool is_write)
{
}

//void audio_callback(void *userdata, Uint8 *stream, int len)
//{
//
//}
//
//static void open_audio()
//{
//    s.format = AUDIO_S16SYS;
//    s.userdata = NULL;
//    s.freq = 0;
//    s.samples = 8192;
//    s.channels = 0;
//    s.callback = audio_callback;
//
//    SDL_InitSubSystem(SDL_INIT_AUDIO);
//    Assert(SDL_OpenAudio(&s, NULL) < 0, "Unable to open audio subsystem!\n");
//    SDL_PauseAudio(0);
//}

void init_audio()
{
    uint32_t space_size = sizeof(uint32_t) * nr_reg;
    audio_base = (uint32_t *)new_space(space_size);
#ifdef CONFIG_HAS_PORT_IO
    add_pio_map("audio", CONFIG_AUDIO_CTL_PORT, audio_base, space_size, audio_io_handler);
#else
    add_mmio_map("audio", CONFIG_AUDIO_CTL_MMIO, audio_base, space_size, audio_io_handler);
#endif

    sbuf = (uint8_t *)new_space(CONFIG_SB_SIZE);
    add_mmio_map("audio-sbuf", CONFIG_SB_ADDR, sbuf, CONFIG_SB_SIZE, NULL);
}
