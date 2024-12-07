#include <fixedptc.h>
#include <stdio.h>

int main()
{
    float a = 1.2;
    fixedpt c = fixedpt_fromfloat(&a);
    //int d = fixedpt_toint(c);
    printf("a : %d\n", a);
    printf("c : %d\n", c);
}
