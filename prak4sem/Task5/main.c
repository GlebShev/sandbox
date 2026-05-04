#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "tree.h"
#include "exec.h"

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    while (1)
    {
        printf("=> ");

        char *line = read_line();
        if (!line)
        {
            break;
        }

        char *trim_line = line;
        while (*trim_line == ' ' || *trim_line == '\t')
        {
            trim_line++;
        }

        if (*trim_line == '\0')
        {
            free(line);
            continue;
        }

        // Если введена команда "exit"
        if (strcmp(trim_line, "exit") == 0)
        {
            free(line);
            break;
        }

        // Разбиваем на лексемы
        List lex_list;
        int count = tokenize_line(line, &lex_list);
        free(line);
        if (count <= 0)
        {
            list_clear(&lex_list);
            continue;
        }

        // Парсим в дерево
        Tree *root = parse(&lex_list);
        if (root)
        {
            int status = execute(root);
            free_tree(root);
        }

        list_clear(&lex_list);
    }

    return 0;
}
