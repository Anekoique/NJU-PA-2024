#include <proc.h>

#define MAX_NR_PROC 16

static PCB pcb[MAX_NR_PROC] __attribute__((used)) = {};
static PCB pcb_boot = {};
PCB *current = NULL;
PCB *pre = NULL;
static int pcb_num = 0;

static char *const argv[] = {
    NULL     
};

uintptr_t naive_uload(PCB *, const char *, char **);

void switch_boot_pcb()
{
    current = &pcb_boot;
}

PCB* new_pcb()
{
    pcb_num++;
    if (pcb_num > MAX_NR_PROC)
        panic("Too much proc to be called!");
    return &pcb[pcb_num-1];
}

PCB free_pcb(int num)
{
    return pcb_boot;
}

void hello_fun(void *arg)
{
//#define TOPRINT
    printf("here\n");
#ifdef TOPRINT
    int j = 1;
#endif
    while (1)
    {
#ifdef TOPRINT
        Log("Hello World from Nanos-lite with arg '%c' for the %dth time!", (uintptr_t)arg, j);
        j++;
#endif
        yield();
    }
}

void context_kload(PCB *pcb, void (*entry)(void *), void *arg)
{
    pcb->cp = kcontext((Area){pcb->stack, &(pcb->stack[STACK_SIZE])}, entry, arg);
}

void context_uload(PCB *pcb, const char *filename, char *const argv[], char *const envp[])
{
    //implement : placed mm maloc
    int argc = 0;
    size_t string_tab = 0;
    int offset = 0x20;
    if (argv != NULL)
        while (argv[argc] != NULL) {
            string_tab += strlen(argv[argc]);
            argc++;
        }
    if (envp != NULL)
        for (int i = 0; envp[i] != NULL; i++)
            string_tab += strlen(envp[i]);

    void *addr = (void *)(ROUNDUP((uintptr_t)new_page(4) + 4 * PGSIZE, PGSIZE) - string_tab);
    int *c_ptr = (int *)(addr - sizeof(argv[0]) - sizeof(envp[0]) - sizeof(int) - offset);
    *c_ptr = argc;
    char **v_ptr = (char **)((intptr_t)c_ptr + sizeof(int));
    for (int i = 0; argv[i] != NULL; i++)
    {
        memcpy(addr, argv[i], strlen(argv[i]) + 1);
        *v_ptr = addr;
        addr += strlen(argv[i]) + 1;
        v_ptr += 1;
    }

    uintptr_t entry = naive_uload(pcb, filename, NULL);
    pcb->cp = ucontext(NULL, (Area){pcb->stack, &(pcb->stack[STACK_SIZE])}, (void *)entry);

    *v_ptr = NULL;
    pcb->cp->GPRx = (uintptr_t)c_ptr;
    printf("gethere\n");


    // implement : placed heap.end
    //int argc = 0;
    //while (argv[argc] != NULL) argc++;
    //void *addr = (void *)0x87800000;
    //int *c_ptr = (int *)0x87000000;
    //*c_ptr = argc;
    //char **v_ptr = (char **)((intptr_t)0x87000000 + sizeof(int));
    //for (int i = 0; argv[i] != NULL; i++)
    //{
    //    memcpy(addr, argv[i], strlen(argv[i]) + 1);
    //    *v_ptr = addr;
    //    addr += sizeof((char *)(argv[i]));
    //    v_ptr += 1;
    //}

    //*v_ptr = NULL;
    //pcb->cp->GPRx = (uintptr_t)c_ptr;

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
    context_kload(new_pcb(), hello_fun, (void *)('a'));
    context_uload(new_pcb(), "/bin/nterm", argv, NULL);
    switch_boot_pcb();

    Log("Initializing processes...");

    // load program here
    //naive_uload(NULL, "/bin/nterm", argv);
}

Context *schedule(Context *prev)
{
    if (pcb_num == 0 || pcb_num == 1) return prev;
    if (current == &pcb_boot)
    {
        current = (pre == NULL ? &pcb[pcb_num-1] : pre);
        pre = (pre == NULL ? &pcb[0] : &pcb[pcb_num-1]);
    }
    else 
    {
        current->cp = prev;
        PCB* temp = current;
        current = pre;
        pre = temp;
    }
    return current->cp;
}
