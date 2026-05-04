#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int f_n = 0;
    int strt = 0;

    if (argc > 1 && strcmp(argv[1], "-n") == 0)
    {
        f_n = 1;
        strt = 2;
    }
    for (int i = strt; i < argc; i++)
    {
        printf("%s", argv[i]);
        if (i < argc - 1)
        {
            printf(" ");
        }
    }
    if (!f_n)
    {
        printf("\n");
    }
}