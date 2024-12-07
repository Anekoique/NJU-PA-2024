#include <fixedptc.h>
#include <stdio.h>

int main()
{
    fixedpt a = fixedpt_rconst(1.2);
    fixedpt c = fixedpt_ceil(a);
    //int d = fixedpt_toint(c);
    printf("%d\n", c);
}
