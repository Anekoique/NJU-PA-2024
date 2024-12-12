#define SDL_malloc malloc
#define SDL_free free
#define SDL_realloc realloc

#define SDL_STBIMAGE_IMPLEMENTATION
#include "SDL_stbimage.h"

SDL_Surface *IMG_Load_RW(SDL_RWops *src, int freesrc)
{
    assert(src->type == RW_TYPE_MEM);
    assert(freesrc == 0);
    return NULL;
}

SDL_Surface *IMG_Load(const char *filename)
{
    printf("filename : %s\n", filename);
    int fd = open(filename, 0);
    size_t size = lseek(fd, 0, SEEK_END);
    printf("size : %d\n", size);
    void * buf = (void *)malloc(size);
    SDL_Surface *ptr = STBIMG_LoadFromMemory((unsigned char *)buf, size);
    return ptr;
}

int IMG_isPNG(SDL_RWops *src)
{
    return 0;
}

SDL_Surface *IMG_LoadJPG_RW(SDL_RWops *src)
{
    return IMG_Load_RW(src, 0);
}

char *IMG_GetError()
{
    return "Navy does not support IMG_GetError()";
}
