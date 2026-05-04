#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "tree.h"

int main(void)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;

    printf("Enter a command line (Ctrl+D to exit):\n");

    while (1)
    {
        printf("> ");
        fflush(stdout);

        nread = getline(&line, &len, stdin);
        if (nread == -1)
        {

            break;
        }

        if (nread > 0 && line[nread - 1] == '\n')
        {
            line[nread - 1] = '\0';
        }

        if (line[0] == '\0')
        {
            continue;
        }

        List lex_list;

        if (tokenize_line(line, &lex_list) < 0)
        {

            list_clear(&lex_list);
            continue;
        }

        Tree *root = parse(&lex_list);

        if (root)
        {
            print_tree(root);
            free_tree(root);
        }

        list_clear(&lex_list);
    }

    free(line);
    return 0;
}
