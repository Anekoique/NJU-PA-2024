#include <stdio.h>
#include <sys/time.h>

int main()
{
    struct timeval now;
    long int current;
    long int last;
    gettimeofday(&now, NULL);
    last = now.tv_sec * 1000000 + now.tv_usec;
    printf("time : %ld\n", now.tv_sec * 1000000 + now.tv_usec);
    int count = 0;
    while (count < 10)
    {
        gettimeofday(&now, NULL);
        current = now.tv_sec * 1000000 + now.tv_usec;
        if (current - last > 5000000)
        {
            last = current;
            printf("after 5 s\n");
        }
    }
    printf("*****PASS*****\n");
}
