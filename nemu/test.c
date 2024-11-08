#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef __uint32_t uint32_t;
typedef __uint64_t uint64_t;
typedef uint32_t word_t;

void set(int *a)
{
#define macro(i) \
    if (i > 1) *a = 2;  \
    else *a = 1;
#undef macro
}

int main() {
    int a = 0;
    set(&a);
    printf("%d", a);
    return 0;
}
