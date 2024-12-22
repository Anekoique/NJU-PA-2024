#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[], char *envp[]);
extern char **environ;
void call_main(uintptr_t *args)
{
    int argc = *((int *)(args));
    char ** argv = (char **)((int *)(args) + 1);
    char ** envp = (char **)((char **)argv + argc);
    environ = (char **)envp;
    printf("args address: %p\n", argv);
    printf("args value: 0x%lx\n", *(uintptr_t *)argv);
    printf("%d\n", argc);
    printf("%s\n", argv[0]);
    exit(main(argc, (char **)argv, (char **)envp));
    assert(0);
}
