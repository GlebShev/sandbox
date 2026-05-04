#ifndef TREE_H
#define TREE_H

typedef enum
{
    NODE_COMMAND,    // простая командаили поддерво в скобках
    NODE_PIPE,       // конвей
    NODE_AND,        // &&
    NODE_OR,         // ||
    NODE_SEMICOLON,  // ;
    NODE_BACKGROUND, // &
    // NODE_SUBSHELL    // ( ... )
} NodeType;

typedef struct Redirect
{
    char *input;  // имя для <
    char *output; // имя для > или >>
    int append;   // 1 если >>, иначе 0
} Redirect;

typedef struct Command
{
    char **argv; // массив аргументов
    int argc;
    Redirect redirect; // перенаправления
    int is_subshell;
    struct Tree *subshell; // если флаг 1 то поддерево
} Command;

typedef struct Tree
{
    NodeType type;
    struct Tree *left;
    struct Tree *right;
    struct Command cmd; // для NODE_COMMAND
} Tree;

// построение дерева по списку лексем
#include "list.h"
Tree *parse(List *tokens);

// освобождение памяти дерева
void free_tree(Tree *tree);

// печать для отладки
void print_tree(const Tree *tree);

#endif