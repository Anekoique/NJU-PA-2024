#include <stdio.h>
#include <sys/time.h>

__uint32_t NDL_GetTicks();

int main()
{
    long int current;
    long int last;
    current = NDL_GetTicks();
    last = current;
    printf("time : %ld\n", current);
    int count = 0;
    while (count < 10)
    {
        current = NDL_GetTicks();
        if (current - last > 5000000)
        {
            last = current;
            printf("after 5 s\n");
            count++;
        }
    }
    printf("*****PASS*****\n");
}
