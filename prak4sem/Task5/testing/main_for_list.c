#include <stdio.h>
#include <stdlib.h>
#include "list.h"

int main()
{
    printf("Enter lines to tokenize:\n");

    while (1)
    {
        printf("> ");
        fflush(stdout);

        char *line = read_line();
        if (!line)
        {
            break;
        }

        if (line[0] == '\0')
        {
            free(line);
            continue;
        }

        List lex_list;
        int count = tokenize_line(line, &lex_list);

        printf("Got %d tokens:\n", count);
        list_print(&lex_list);

        list_clear(&lex_list);
        free(line);
    }

    printf("EOF detected, exiting.\n");
}
