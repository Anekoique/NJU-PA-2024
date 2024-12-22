#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[], char *envp[]);
extern char **environ;
void call_main(uintptr_t *args)
{
    int argc = *((int *)(args));
    uintptr_t argv = (uintptr_t)((int *)(args) + 1);
    uintptr_t envp = (uintptr_t)((char **)argv + argc);
    environ = (char **)envp;
    printf("args address: %p\n", args);
    printf("args value: 0x%lx\n", *(uintptr_t *)args);
    printf("%p\n", (void *)argv);
    exit(main(argc, (char **)argv, (char **)envp));
    assert(0);
}
