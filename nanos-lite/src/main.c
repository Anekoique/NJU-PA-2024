#include <common.h>

void init_mm(void);
void init_device(void);
void init_ramdisk(void);
void init_irq(void);
void init_fs(void);
void init_proc();

int main(int argc, char *argv[])
{
    printf("%s\n", argv);
    extern const char logo[];
    printf("%s", logo);
    Log("'Hello World!' from Nanos-lite");
    Log("Build time: %s, %s", __TIME__, __DATE__);

    init_mm();

    init_device();

    init_ramdisk();

#ifdef HAS_CTE
    init_irq();
#endif

    init_fs();

    init_proc();

    Log("Finish initialization");
    uintptr_t address = 0x8220cf98; // 假设这是你想访问的地址
    int value;
    value = *(int *)address;
    printf("Value at address 0x%p: %p\n", (unsigned int)address, value);
    printf("hello\n");
#ifdef HAS_CTE
    yield();
#endif

    panic("Should not reach here");
}
