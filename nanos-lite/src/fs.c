#include <fs.h>

extern uint8_t ramdisk_start;
extern uint8_t ramdisk_end;

typedef size_t (*ReadFn)(void *buf, size_t offset, size_t len);
typedef size_t (*WriteFn)(const void *buf, size_t offset, size_t len);

size_t serial_write(const void *buf, size_t offset, size_t len);
size_t events_read(void *buf, size_t offset, size_t len);

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
#include "files.h"
};

void init_fs()
{
    // TODO: initialize the size of /dev/fb
}

int fs_open(const char *pathname, int flags, int mode)
{
    int len = sizeof(file_table) / sizeof(Finfo);
    for (int i = 3; i < len; i++)
    {
        if (!strcmp(file_table[i].name, pathname)) return i;
    }
    printf("Invalid filename !\n");
    assert(0);
}

size_t fs_read(int fd, void *buf, size_t len)
{
    if (fd == 3) return events_read(buf, 0, len);
    if (file_table[fd].open_offset + len > file_table[fd].size)
    {
        len = file_table[fd].size - file_table[fd].open_offset;
        printf("Con't Reach here\n");
    }
    uint8_t *pos = &ramdisk_start + file_table[fd].disk_offset + file_table[fd].open_offset;
    memcpy(buf, pos, len);
    file_table[fd].open_offset += len;
    return len;
}

size_t fs_write(int fd, const void *buf, size_t len)
{
    if (fd == 2 || fd == 1)
    {
        file_table[fd].write(buf, 0, len);
    }

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
