#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "tree.h"
#include "exec.h"

int main(void)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;

    printf("Enter a command line:\n");

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
        int count = tokenize_line(line, &lex_list);
        if (count < 0)
        {

            list_clear(&lex_list);
            continue;
        }

        Tree *root = parse(&lex_list);

        if (root)
        {

            print_tree(root);

            // Выполнение
            int status = execute(root);
            printf("Command exited with status: %d\n", status);

            free_tree(root);
        }

        list_clear(&lex_list);
    }

    free(line);
    return 0;
}
