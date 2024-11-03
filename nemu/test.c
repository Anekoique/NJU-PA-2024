#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef __uint32_t uint32_t;
typedef __uint64_t uint64_t;

#define base 0x80000000
int main() {
    int a[5]={1, 2, 3, 4, 5};
    printf("%p\n%p", a, a + 1);
    return 0;
}
