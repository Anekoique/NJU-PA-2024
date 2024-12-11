#include <fs.h>

extern uint8_t ramdisk_start;
extern uint8_t ramdisk_end;

typedef size_t (*ReadFn)(void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn)(const void *buf, size_t offset, size_t len);

size_t serial_write(const void *buf, size_t offset, size_t len);
size_t events_read(void *buf, size_t offset, size_t len);
size_t dispinfo_read(void *buf, size_t ooffset, size_t len);
size_t fb_write(const void *buf, size_t offset, size_t len);
void init_fb(int *w, int *h);

intptr_t check_pos = 0x80064a37;
bool check_flag = false;

typedef struct
{
    char *name;
    size_t size;
    size_t disk_offset;
    size_t open_offset;
    ReadFn read;
    WriteFn write;
} Finfo;

enum
{
    FD_STDIN,
    FD_STDOUT,
    FD_STDERR,
    FD_EVENT,
    FD_DISP,
    FD_FB
};

size_t invalid_read(void *buf, size_t offset, size_t len)
{
    panic("should not reach here");
    return 0;
}

size_t invalid_write(const void *buf, size_t offset, size_t len)
{
    panic("should not reach here");
    return 0;
}

/* This is the information about all files in disk. */
static Finfo file_table[] __attribute__((used)) = {
    [FD_STDIN] = {"stdin",  0, 0, 0, invalid_read, invalid_write},
    [FD_STDOUT] = {"stdout", 0, 0, 0, invalid_read, serial_write},
    [FD_STDERR] = {"stderr", 0, 0, 0, invalid_read, serial_write},
    [FD_EVENT] = {"/dev/events", 0, 0, 0, events_read, invalid_write},
    [FD_DISP] = {"/proc/dispinfo", 0, 0, 0, dispinfo_read, invalid_write},
    [FD_FB] = {"/dev/fb", 0, 0, 0, invalid_read, fb_write},
#include "files.h"
};

void init_fs()
{
    // TODO: initialize the size of /dev/fb
    int w, h;
    init_fb(&w, &h);
    file_table[FD_FB].size = w * h * sizeof(uint32_t);
}

int fs_open(const char *pathname, int flags, int mode)
{
    int len = sizeof(file_table) / sizeof(Finfo);
    for (int i = 3; i < len; i++)
    {
        if (!strcmp(file_table[i].name, pathname)) return i;
    }
    printf("Invalid file !\n");
    return -1;
}

size_t fs_read(int fd, void *buf, size_t len)
{
    //if (*(uint8_t *)check_pos != 66 && check_flag) 
    //{
    //    printf("fd : %d\n", fd);
    //    printf("check_pos : %p\n", check_pos);
    //    printf("check_pos_value : %d\n", *(uint8_t *)check_pos);
    //    printf("error\n");
    //    panic("error");
    //}
    if (fd == 3) return events_read(buf, 0, len);
    if (fd == 4) return dispinfo_read(buf, 0, len);
    if (file_table[fd].open_offset + len > file_table[fd].size)
    {
        printf("open_offset : %d, len : %d\n", file_table[fd].open_offset, len);
        len = file_table[fd].size - file_table[fd].open_offset;
        printf("fd : %d\n", fd);
        
        printf("Con't Reach here\n");
    }
    uint8_t *pos = &ramdisk_start + file_table[fd].disk_offset + file_table[fd].open_offset;
    memcpy(buf, pos, len);

    //for (int i = 0; i < 1; i++)
    //    printf("%d", pos[i]);
    //printf("\n");
    //if (file_table[fd].disk_offset == 0x61b0f && len == 1024 && !check_flag)
    //{
    //    check_flag = true;
    //    check_pos = (intptr_t)pos;
    //}
    ////
    file_table[fd].open_offset += len;
    return len;
}

size_t fs_write(int fd, const void *buf, size_t len)
{
    if (fd == 2 || fd == 1)
    {
        return file_table[fd].write(buf, 0, len);
    }
    else if (fd == 5) return file_table[fd].write(buf, file_table[fd].open_offset, len);

    if (file_table[fd].open_offset + len > file_table[fd].size)
    {
        len = file_table[fd].size - file_table[fd].open_offset;
    }
    uint8_t *pos = &ramdisk_start + file_table[fd].disk_offset + file_table[fd].open_offset;
    memcpy(pos, buf, len);
    file_table[fd].open_offset += len;
    return len;
}

size_t fs_lseek(int fd, size_t offset, int whence)
{

    switch(whence)
    {
        case SEEK_SET:
            if (offset > file_table[fd].size) return -1;
            file_table[fd].open_offset = offset;
            break;
        case SEEK_CUR:
            if (offset + file_table[fd].open_offset > file_table[fd].size) return -1;
            file_table[fd].open_offset += offset;
            break;
        case SEEK_END:
            if (offset > 0) return -1;
            file_table[fd].open_offset = file_table[fd].size;
            break;
        default:
            printf("Invalid whence !\n");
            assert(0);
    }
    return file_table[fd].open_offset;
}

int fs_close(int fd)
{
    file_table[fd].open_offset = 0;
    return 0;
}

size_t get_disk_offset(int fd)
{
    return file_table[fd].disk_offset;
}

ReadFn get_read_func(int fd)
{
    return file_table[fd].read;
}

WriteFn get_write_func(int fd)
{
    return file_table[fd].write;
}
