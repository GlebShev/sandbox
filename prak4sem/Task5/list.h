#ifndef LIST_H
#define LIST_H

#include <stddef.h>

// струкртура списка
typedef struct ListNode
{
    char *str;
    struct ListNode *next;
} ListNode;

typedef struct
{
    ListNode *head;
    ListNode *tail;
    size_t lenght;
} List;

// иницилизация пустого списка
void list_init(List *list);

// добавить слова в конец списка
void list_push(List *list, const char *word);

// печать, для отладки
void list_print(List *list);

// очистка списка
void list_clear(List *list);

// создание списка лексем и подстановка переменных окружения
int tokenize_line(const char *line, List *list);

// функция для чтения динамически
char *read_line(void);

#endif