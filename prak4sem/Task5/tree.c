#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"

static int error_flag = 0;
// ошибки
static void error(const char *msg)
{
    fprintf(stderr, "Parse error: %s\n", msg);
    error_flag = 1;
}
// возварает текущйи токен
static const char *peek_token(ListNode *current)
{
    return current ? current->str : NULL;
}
// проверяет на совпданеие текущего токена и строки, преходит к след токену
static int match_token(ListNode **current, const char *str)
{
    if (*current && strcmp((*current)->str, str) == 0)
    {
        *current = (*current)->next;
        return 1;
    }
    return 0;
}

static Tree *new_node(NodeType type)
{
    Tree *node = calloc(1, sizeof(Tree));
    if (!node)
    {
        fprintf(stderr, "Memory error\n");
        exit(1);
    }
    node->type = type;
    return node;
}

static Tree *parse_command_line(ListNode **current);
static Tree *parse_and_or_list(ListNode **current);
static Tree *parse_pipeline(ListNode **current);
static Tree *parse_command(ListNode **current);
static Tree *parse_simple_command(ListNode **current);

Tree *parse(List *tokens)
{
    ListNode *cur = tokens->head;
    error_flag = 0;
    Tree *root = parse_command_line(&cur);
    if (!error_flag && cur != NULL)
    {
        error("Extra tokens");
        free_tree(root);
        return NULL;
    }
    if (error_flag)
    {
        free_tree(root);
        return NULL;
    }
    return root;
}

static Tree *parse_command_line(ListNode **current)
{
    //<строка_команда> ::= <и_или_список> { ';' <и_или_список> }
    //          | <и_или_список> { '&' <и_или_список> }
    //          | <и_или_список>
    Tree *left = parse_and_or_list(current);
    if (!left)
        return NULL;
    while (!error_flag)
    {
        if (match_token(current, ";"))
        {
            Tree *node = new_node(NODE_SEMICOLON);
            node->left = left;
            Tree *right = parse_and_or_list(current);
            if (!right)
            {
                free_tree(node);
                return NULL;
            }
            node->right = right;
            left = node;
        }
        else if (match_token(current, "&"))
        {
            Tree *node = new_node(NODE_BACKGROUND);
            node->left = left;
            Tree *righ = parse_and_or_list(current);
            if (!righ)
            {
                free_tree(node);
                return NULL;
            }
            node->right = righ;
            left = node;
        }
        else
        {
            break;
        }
    }
    return left;
}

static Tree *parse_and_or_list(ListNode **current)
{
    //<и_или_список> ::= <пайп> { '&&' <пайп> | '||' <пайп> }
    Tree *left = parse_pipeline(current);
    if (!left)
        return NULL;
    for (;;)
    {
        const char *tk = peek_token(*current);
        if (tk && strcmp(tk, "&&") == 0)
        {
            (*current) = (*current)->next;
            Tree *node = new_node(NODE_AND);
            node->left = left;
            Tree *righ = parse_pipeline(current);
            if (!righ)
            {
                free_tree(node);
                return NULL;
            }
            node->right = righ;
            left = node;
        }
        else if (tk && strcmp(tk, "||") == 0)
        {
            (*current) = (*current)->next;
            Tree *node = new_node(NODE_OR);
            node->left = left;
            Tree *righ = parse_pipeline(current);
            if (!righ)
            {
                free_tree(node);
                return NULL;
            }
            node->right = righ;
            left = node;
        }
        else
        {
            break;
        }
    }
    return left;
}

static Tree *parse_pipeline(ListNode **current)
{
    //<пайп> ::= <команда> { '|' <команда> }
    Tree *left = parse_command(current);
    if (!left)
        return NULL;
    while (match_token(current, "|"))
    {
        Tree *node = new_node(NODE_PIPE);
        node->left = left;
        Tree *righ = parse_command(current);
        if (!righ)
        {
            free_tree(node);
            return NULL;
        }
        node->right = righ;
        left = node;
    }
    return left;
}

static Tree *parse_command(ListNode **current)
{
    //<команда> ::= <простая_команда>
    //       | '(' <строка_команда> ')' [ перенапрвления ]
    const char *tk = peek_token(*current);
    if (tk && strcmp(tk, "(") == 0)
    {
        // subshell
        (*current) = (*current)->next;
        Tree *sub = parse_command_line(current);
        if (!sub)
            return NULL;
        if (!match_token(current, ")"))
        {
            error("Miss )");
            free_tree(sub);
            return NULL;
        }
        // NODE_COMMAND c is_subshell = 1
        Tree *node = new_node(NODE_COMMAND);
        node->cmd.is_subshell = 1;
        node->cmd.redirect.input = NULL;
        node->cmd.redirect.output = NULL;
        node->cmd.redirect.append = 0;
        node->cmd.argv = NULL;
        node->cmd.argc = 0;
        node->cmd.subshell = sub;
        return node;
    }
    else
    {
        return parse_simple_command(current);
    }
}

static Tree *parse_simple_command(ListNode **current)
{
    //<простая_команда> ::= <имя_файла> {<аргумент>} [ перенапрвления ]
    const char *tk = peek_token(*current);
    if (!tk)
    {
        error("Command expect");
        return NULL;
    }
    // сбор аргументов
    char **argv = NULL;
    int argc = 0;
    while (tk)
    {
        if (strcmp(tk, ">") == 0 || strcmp(tk, ">>") == 0 || strcmp(tk, "<") == 0)
        {
            break;
        }
        // спецсимволы чек и не учитываем
        if (strcmp(tk, "|") == 0 || strcmp(tk, "&&") == 0 || strcmp(tk, "||") == 0 || strcmp(tk, ";") == 0 || strcmp(tk, "&") == 0 || strcmp(tk, ")") == 0)
        {
            break;
        }
        (*current) = (*current)->next;
        argv = realloc(argv, sizeof(char *) * (argc + 2));
        argv[argc] = strdup(tk);
        argc++;
        argv[argc] = NULL;
        tk = peek_token(*current);
    }
    if (argc == 0)
    {
        error("Comman expect");
        free(argv);
        return NULL;
    }

    Tree *node = new_node(NODE_COMMAND);
    node->cmd.argv = argv;
    node->cmd.argc = argc;
    node->cmd.is_subshell = 0;
    node->cmd.redirect.input = NULL;
    node->cmd.redirect.output = NULL;
    node->cmd.redirect.append = 0;

    // перенаправления
    while (*current)
    {
        tk = peek_token(*current);
        if (!tk)
            break;
        if (strcmp(tk, "<") == 0)
        {
            (*current) = (*current)->next;
            const char *fn = peek_token(*current);
            if (!fn)
            {
                error("Need file");
                free_tree(node);
                return NULL;
            }
            (*current) = (*current)->next;
            node->cmd.redirect.input = strdup(fn);
        }
        else if (strcmp(tk, ">") == 0)
        {
            (*current) = (*current)->next;
            const char *fn = peek_token(*current);
            if (!fn)
            {
                error("Need file");
                free_tree(node);
                return NULL;
            }
            (*current) = (*current)->next;
            node->cmd.redirect.output = strdup(fn);
            node->cmd.redirect.append = 0;
        }
        else if (strcmp(tk, ">>") == 0)
        {
            (*current) = (*current)->next;
            const char *fn = peek_token(*current);
            if (!fn)
            {
                error("Need file");
                free_tree(node);
                return NULL;
            }
            (*current) = (*current)->next;
            node->cmd.redirect.output = strdup(fn);
            node->cmd.redirect.append = 1;
        }
        else
        {
            break;
        }
    }
    return node;
}

// очистка дерева
void free_tree(Tree *tree)
{
    if (!tree)
        return;
    free_tree(tree->left);
    free_tree(tree->right);
    if (tree->type == NODE_COMMAND)
    {
        if (tree->cmd.argv)
        {
            for (int i = 0; i < tree->cmd.argc; i++)
            {
                free(tree->cmd.argv[i]);
            }
            free(tree->cmd.argv);
        }
        free(tree->cmd.redirect.input);
        free(tree->cmd.redirect.output);
    }
    free(tree);
}

// печать
static void print_spc(int depth)
{
    for (int i = 0; i < depth; i++)
        printf("  ");
}
void print_tree(const Tree *tree)
{
    static int depth = 0;
    if (!tree)
        return;
    depth++;
    print_spc(depth - 1);
    switch (tree->type)
    {
    case NODE_COMMAND:
        if (tree->cmd.is_subshell)
        {
            printf("(subshell)\n");
            print_tree(tree->cmd.subshell);
        }
        else
        {
            printf("COMMAND: ");
            for (int i = 0; i < tree->cmd.argc; i++)
            {
                printf("%s ", tree->cmd.argv[i]);
            }
            if (tree->cmd.redirect.input)
            {
                printf("< %s ", tree->cmd.redirect.input);
            }
            if (tree->cmd.redirect.output)
            {
                printf("%s %s ", tree->cmd.redirect.append ? ">>" : ">", tree->cmd.redirect.output);
            }
            printf("\n");
        }
        break;

    case NODE_PIPE:
        printf("PIPE\n");
        print_tree(tree->left);
        print_tree(tree->right);
        break;

    case NODE_AND:
        printf("AND\n");
        print_tree(tree->left);
        print_tree(tree->right);
        break;

    case NODE_OR:
        printf("OR\n");
        print_tree(tree->left);
        print_tree(tree->right);
        break;

    case NODE_SEMICOLON:
        printf("SEMICOLON\n");
        print_tree(tree->left);
        print_tree(tree->right);
        break;

    case NODE_BACKGROUND:
        printf("BACKGROUND\n");
        print_tree(tree->left);
        print_tree(tree->right);
        break;
    }
    depth--;
}