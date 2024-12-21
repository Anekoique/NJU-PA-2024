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
    printf("argc : %d\n", argc);
    environ = (char **)envp;
    exit(main(argc, (char **)argv, (char **)envp));
    assert(0);
}
