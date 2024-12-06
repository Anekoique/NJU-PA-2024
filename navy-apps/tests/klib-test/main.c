#include <stdio.h>

int main()
{
    int w = 400, h = 300;
    char buf[64];
    snprintf(buf, sizeof(buf), "WIDTH : %d\nHEIGHT : %d\n", w, h);

    int screen_w, screen_h;
    if (sscanf(buf, "WIDTH : %d\nHEIGHT : %d\n", &screen_w, &screen_h) != 2) {
        printf("Failed to read dimensions\n");
    }
    printf("%d : %d", screen_w, screen_h);
}


