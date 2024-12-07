#include "debug.h"
#include <NDL.h>
#include <SDL.h>

int SDL_OpenAudio(SDL_AudioSpec *desired, SDL_AudioSpec *obtained)
{
    panic("Please implement me!");
    return 0;
}

void SDL_CloseAudio()
{
    panic("Please implement me!");
}

void SDL_PauseAudio(int pause_on)
{
    panic("Please implement me!");
}

void SDL_MixAudio(uint8_t *dst, uint8_t *src, uint32_t len, int volume)
{
    panic("Please implement me!");
}

SDL_AudioSpec *SDL_LoadWAV(const char *file, SDL_AudioSpec *spec, uint8_t **audio_buf, uint32_t *audio_len)
{
    panic("Please implement me!");
    return NULL;
}

void SDL_FreeWAV(uint8_t *audio_buf)
{
    panic("Please implement me!");
}

void SDL_LockAudio()
{
    panic("Please implement me!");
}

void SDL_UnlockAudio()
{
    panic("Please implement me!");
}
