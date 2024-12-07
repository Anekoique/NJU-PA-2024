#include <fixedptc.h>
#include <stdio.h>

int main()
{
    fixedpt a = fixedpt_rconst(-1.2);
    fixedpt c = fixedpt_abs(a);
    //int d = fixedpt_toint(c);
    printf("a : %d\n", a);
    printf("c : %d\n", c);
}
