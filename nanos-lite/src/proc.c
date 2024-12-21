#include <proc.h>

#define MAX_NR_PROC 4

static PCB pcb[MAX_NR_PROC] __attribute__((used)) = {};
static PCB pcb_boot = {};
PCB *current = NULL;

void naive_uload(PCB *, const char *, char **);

void switch_boot_pcb()
{
    current = &pcb_boot;
}

void hello_fun(void *arg)
{
    printf("here\n");
    int j = 1;
    while (1)
    {
        Log("Hello World from Nanos-lite with arg '%c' for the %dth time!", (uintptr_t)arg, j);
        j++;
        yield();
    }
}

void context_kload(PCB *pcb, void (*entry)(void *), void *arg)
{
    pcb->cp = kcontext((Area){pcb->stack, &(pcb->stack[STACK_SIZE])}, entry, arg);
    printf("%p\n", pcb->cp);
}

void init_proc(char *argv[])
{
    context_kload(&pcb[0], hello_fun, (void *)('a'));
    context_kload(&pcb[1], hello_fun, (void *)('b'));

    switch_boot_pcb();

    Log("Initializing processes...");

    // load program here
    //naive_uload(NULL, "/bin/nterm", argv);
}

Context *schedule(Context *prev)
{
    current->cp = prev;
    current = (current == &pcb[0] ? &pcb[1] : &pcb[0]);
    printf("%p\n", current->cp);
    return current->cp;
}
