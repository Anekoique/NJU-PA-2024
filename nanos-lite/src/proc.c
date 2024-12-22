#include <proc.h>

#define MAX_NR_PROC 4

static PCB pcb[MAX_NR_PROC] __attribute__((used)) = {};
static PCB pcb_boot = {};
PCB *current = NULL;

static char *const argv[] = {
    "--skip",    
    NULL     
};

uintptr_t naive_uload(PCB *, const char *, char **);


void switch_boot_pcb()
{
    current = &pcb_boot;
}

void hello_fun(void *arg)
{
    printf("here\n");
    //int j = 1;
    while (1)
    {
        //Log("Hello World from Nanos-lite with arg '%c' for the %dth time!", (uintptr_t)arg, j);
        //j++;
        yield();
    }
}

void context_kload(PCB *pcb, void (*entry)(void *), void *arg)
{
    pcb->cp = kcontext((Area){pcb->stack, &(pcb->stack[STACK_SIZE])}, entry, arg);
}

void context_uload(PCB *pcb, const char *filename, char *const argv[], char *const envp[])
{
    uintptr_t entry = naive_uload(pcb, filename, NULL);
    pcb->cp = ucontext(NULL, (Area){pcb->stack, &(pcb->stack[STACK_SIZE])}, (void *)entry);
    
    int argc = 0;
    while (argv[argc] != NULL) argc++;
    void *addr = (void *)0x87800000;
    int *c_ptr = (int *)0x87000000;
    *c_ptr = argc;
    char **v_ptr = (char **)((intptr_t)0x87000000 + sizeof(int));
    for (int i = 0; argv[i] != NULL; i++)
    {
        printf("addr : %p, v_ptr : %p\n", addr, v_ptr);
        memcpy(addr, argv[i], strlen(argv[i]) + 1);
        *v_ptr = addr;
        printf("%s\n", v_ptr[i]);
        addr += sizeof((char *)(argv[i]));
        v_ptr += 1;
    }

    *v_ptr = NULL;
    pcb->cp->GPRx = (uintptr_t)c_ptr;

    // error 2
    //void *addr = (void *)0x87800000;
    //memcpy((void *)addr, (void *)argv, sizeof((void *)argv));
    //int *ptr = (int *)0x87000000;
    //*ptr = argc;
    //pcb->cp->GPRx = (intptr_t)ptr;
    //char **ptr2 = (char **)((intptr_t)0x87000000 + sizeof(int));
    //*ptr2 = addr; 
    //printf("%p\n", ptr2);
    //printf("%s\n", ((char **)(*ptr2))[0]);

    // error 1
    //int *ptr = (int *)((intptr_t)(&argv) - sizeof(int));
    //*ptr = argc;
    //printf("%p\n", heap.start);
    //pcb->cp->GPRx = (intptr_t)(&argv) - sizeof(int);
    //printf("%p\n", (intptr_t)(&argv) - sizeof(int));
    //printf("%x\n", *(int *)((intptr_t)(&argv) - sizeof(int)));
    //uintptr_t address = 0x8220cf98; // 假设这是你想访问的地址
    //int value;
    //value = *(int *)address;
    //printf("Value at address 0x%p: %p\n", (unsigned int)address, value);
}

void init_proc()
{
    context_kload(&pcb[0], hello_fun, (void *)('a'));
    context_uload(&pcb[1], "/bin/pal", argv, NULL);
    printf("hello\n");

    switch_boot_pcb();

    Log("Initializing processes...");

    // load program here
    //naive_uload(NULL, "/bin/nterm", argv);
}

Context *schedule(Context *prev)
{
    //uintptr_t address = 0x87800000; // 假设这是你想访问的地址
    //printf("Value at address 0x%p: %s\n", (unsigned int)address, (char *)0x87800000);
    current->cp = prev;
    current = (current == &pcb[0] ? &pcb[1] : &pcb[0]);
    return current->cp;
}
