#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef __uint32_t uint32_t;
typedef __uint64_t uint64_t;
typedef uint32_t word_t;

int main() {
    word_t base = 0x80000000;
    word_t value = (word_t)strtoul("16", NULL, 10);
    base += value;
    printf("%x\n", base);
    return 0;
}
