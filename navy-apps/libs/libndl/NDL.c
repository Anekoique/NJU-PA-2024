#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>

static int evtdev = -1;
static int fbdev = -1;
static int screen_w = 0, screen_h = 0;

typedef size_t (*ReadFn)(void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn)(const void *buf, size_t offset, size_t len);

ReadFn get_read_func(int fd);
WriteFn get_write_func(int fd);

uint32_t NDL_GetTicks()
{
    struct timeval now;
    gettimeofday(&now, NULL);
    return now.tv_sec * 1000000 + now.tv_usec;
}

int NDL_PollEvent(char *buf, int len)
{
    FILE *fp = fopen("/dev/events", "r+");
    if (fgets(buf, len, fp) != NULL) 
        return 1;
    else return 0;

}

void NDL_OpenCanvas(int *w, int *h)
{
    FILE *fp = fopen("/proc/dispinfo", "r+");
    char line1[64];
    char line2[64];
    fgets(line1, sizeof(line1), fp);
        printf("%s", line1);
    for (int i = 0; i < 64; i++)
    {
        printf("%c", line1[i]);
    }
    //printf("\n");
    //printf("***************\n");
    //printf("***************\n");
    fgets(line2, sizeof(line2), fp);
    printf("%s", line2); // 检查文件内容

    *w = screen_w;
    *h = screen_h;
    if (getenv("NWM_APP"))
    {
        printf("here ***********\n");
        printf("here ***********\n");
        printf("here ***********\n");
        printf("here ***********\n");
        int fbctl = 4;
        fbdev = 5;
        screen_w = *w;
        screen_h = *h;
        char buf[64];
        int len = sprintf(buf, "%d %d", screen_w, screen_h);
        // let NWM resize the window and create the frame buffer
        write(fbctl, buf, len);
        while (1)
        {
            // 3 = evtdev
            int nread = read(3, buf, sizeof(buf) - 1);
            if (nread <= 0)
                continue;
            buf[nread] = '\0';
            if (strcmp(buf, "mmap ok") == 0)
                break;
        }
        close(fbctl);
    }
}

void NDL_DrawRect(uint32_t *pixels, int x, int y, int w, int h)
{
}

void NDL_OpenAudio(int freq, int channels, int samples)
{
}

void NDL_CloseAudio()
{
}

int NDL_PlayAudio(void *buf, int len)
{
    return 0;
}

int NDL_QueryAudio()
{
    return 0;
}

int NDL_Init(uint32_t flags)
{
    if (getenv("NWM_APP"))
    {
        evtdev = 3;
    }
    return 0;
}

void NDL_Quit()
{
}
