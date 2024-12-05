#include <stdio.h>
#include <sys/time.h>

int main()
{
    struct timeval now;
    long int current;
    long int last;
    gettimeofday(&now, NULL);
    last = now.tv_sec * 100000 + now.tv_usec;
    printf("time : %d\n", now.tv_sec * 100000 + now.tv_usec);
    int count = 0;
    while (count < 10)
    {
        gettimeofday(&now, NULL);
        current = now.tv_sec * 100000 + now.tv_usec;
        if (current - last > 500000)
        {
            last = current;
            printf("after 5 s\n");
            count++;
        }
    }
    printf("*****PASS*****\n");
}
