#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void cat(const char *filename, int pr_n, int *line_n)
{
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        perror("Error open file");
        return;
    }

    char line[1024];
    while (fgets(line, sizeof(line), file) != NULL)
    {
        if (pr_n)
        {
            printf("%d  %s", (*line_n)++, line);
        }
        else
        {
            printf("%s", line);
        }
    }
    fclose(file);
}

int main(int argc, char *argv[])
{
    int pr_n = 0;
    int index_strt = 1;

    if (argc < 2)
    {
        perror("Error count of arg");
        return 1;
    }

    if (strcmp(argv[1], "-n") == 0)
    {
        pr_n = 1;
        index_strt = 2;
    }

    int line_n = 1;
    for (int i = index_strt; i < argc; i++)
    {
        cat(argv[i], pr_n, &line_n);
    }
}