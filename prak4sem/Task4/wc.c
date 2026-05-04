#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

void wc(const char *file_name)
{
    FILE *file = fopen(file_name, "r");
    if (file == NULL)
    {
        perror("Error open file");
        return;
    }

    int line_c = 0;
    int word_c = 0;
    int symb_c = 0;
    int fl_in_word = 0;
    char c;
    while ((c = fgetc(file)) != EOF)
    {
        symb_c++;
        if (c == '\n')
        {
            line_c++;
        }
        if (isspace(c))
        {
            fl_in_word = 0;
        }
        else if (!fl_in_word)
        {
            word_c++;
            fl_in_word = 1;
        }
    }
    fclose(file);
    printf("%d %d %d %s\n", line_c, word_c, symb_c, file_name);
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        perror("Error count of arg");
        return 1;
    }
    for (int i = 1; i < argc; i++)
    {
        wc(argv[i]);
    }
}