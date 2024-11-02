#include <string.h>
#include <stdio.h>

int main()
{
	char str[]= "a b c";
    char *a = strtok(str, " ");
    char *b = strtok(NULL, " ");
    char *c = strtok(NULL, " ");
    printf("%s, %s, %s", a, b, c);
}
