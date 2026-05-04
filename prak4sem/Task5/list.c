#include "list.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>    // для geteuid() и getlogin()
#include <sys/types.h> // для uid_t
#include <pwd.h>       // для получения HOME

void list_init(List *list)
{
    list->head = NULL;
    list->tail = NULL;
    list->lenght = 0;
}

void list_push(List *list, const char *word)
{
    ListNode *new_node = (ListNode *)malloc(sizeof(ListNode));
    if (!new_node)
    {
        fprintf(stderr, "Error list_push memory\n");
        return;
    }
    new_node->str = strdup(word);
    new_node->next = NULL;

    if (list->tail)
    {
        list->tail->next = new_node;
    }
    else
    {
        list->head = new_node;
    }
    list->tail = new_node;
    list->lenght++;
}

void list_print(List *list)
{
    printf("List len: %zu\n", list->lenght);
    ListNode *current = list->head;
    while (current)
    {
        printf("%s\n", current->str);
        current = current->next;
    }
}

void list_clear(List *list)
{
    ListNode *current = list->head;
    while (current)
    {
        ListNode *next = current->next;
        free(current->str);
        free(current);
        current = next;
    }
    list->head = NULL;
    list->tail = NULL;
    list->lenght = 0;
}

// переходим к реализации tokenize_line
// для этого напишем несколько вспомогательных функций

// чтение строки произвольной длины
char *read_line(void)
{
    char *line = NULL;
    size_t len = 0;
    ssize_t nread = getline(&line, &len, stdin);
    if (nread == -1)
    {
        free(line);
        return NULL;
    }
    // если есть символ перевода строки
    if (nread > 0 && line[nread - 1] == '\n')
    {
        line[nread - 1] == '\0';
    }
    return line;
}

// динамически формируем токены(строка мб длинее любой наперед заданной константы)

// динамическое добавление символа в буффер
void append_char(char **buf, size_t *buf_size, size_t *buf_len, char c)
{
    if (*buf_size == 0)
    {
        *buf_size = 1024;
        *buf = malloc(*buf_size);
        if (!*buf)
        {
            fprintf(stderr, "Memory error\n");
            exit(1);
        }
        (*buf)[0] = '\0';
        *buf_len = 0;
    }

    if (*buf_len + 1 >= *buf_size)
    {
        size_t new_size = (*buf_size) * 2;
        char *new_buf = realloc(*buf, new_size);
        if (!new_buf)
        {
            fprintf(stderr, "Memory error\n");
            free(*buf);
            exit(1);
        }
        *buf = new_buf;
        *buf_size = new_size;
    }

    (*buf)[(*buf_len)++] = c;
    (*buf)[*buf_len] = '\0';
}

// динамическое добавление строку в буффер
void append_str(char **buf, size_t *buf_size, size_t *buf_len, const char *str)
{
    size_t slen = strlen(str);
    while (*buf_len + slen >= *buf_size)
    {
        size_t new_size = (*buf_size) * 2;
        char *new_buf = realloc(*buf, new_size);
        if (!new_buf)
        {
            fprintf(stderr, "Memory error\n");
            free(*buf);
            exit(1);
        }
        *buf = new_buf;
        *buf_size = new_size;
    }
    memcpy(*buf + *buf_len, str, slen + 1);
    *buf_len += slen;
}

// добавлять готовый токен в список
void add_token(List *lsit, char **buf, size_t *buf_len)
{
    if (*buf_len > 0)
    {
        list_push(lsit, *buf);
        // очистка буфера
        (*buf)[0] = '\0';
        *buf_len = 0;
    }
}

// вспомогательная фунция для получения значений перем. окружения
static char *replace_variable(const char *var)
{
    if (strcmp(var, "HOME") == 0)
    {
        const char *val = getenv("HOME");
        return var ? val : "";
    }

    if (strcmp(var, "SHELL") == 0)
    {
        const char *val = getenv("SHELL");
        return var ? val : "";
    }

    if (strcmp(var, "EUID") == 0)
    {
        uid_t euid = geteuid();
        char buf[32];
        snprintf(buf, sizeof(buf), "%u", euid);
        return buf;
    }

    return NULL;
}

// вспомогательная функция для подстановки переменных
void substitute_variables_in_list(List *list)
{
    ListNode *current = list->head;
    while (current)
    {
        char *pos = strstr(current->str, "$");
        if (pos)
        {
            char var_name[128];
            int i = 0;
            pos++; // доллар скип
            // ищем строку и меняем
            while (isalnum((unsigned char)*pos) && i < (int)sizeof(var_name) - 1)
            {
                var_name[i++] = *pos++;
            }
            var_name[i] = '\0';

            const char *val = replace_variable(var_name);
            if (val)
            {
                char *old_str = current->str;
                size_t prefix_len = (pos - old_str) - (1 + i);
                size_t new_len = prefix_len + strlen(val) + strlen(pos) + 1;
                char *new_str = malloc(new_len);
                if (new_str)
                {
                    memcpy(new_str, old_str, prefix_len);
                    strcpy(new_str + prefix_len, val);
                    strcat(new_str, pos);
                    free(old_str);
                    current->str = new_str;
                }
            }
        }
        current = current->next;
    }
}

// переходим к реализации основной функции
int tokenize_line(const char *line, List *list)
{
    list_init(list);
    if (!line || *line == '\0')
    {
        return 0;
    }

    int in_single_quot = 0; // 1 кавычки
    int in_double_quot = 0; // 2 кавычки
    int in_comm = 0;        // после #
    int escaped = 0;        // посел /

    const char *p = line;
    char *token_buf = NULL; // динамически обрабатываем символы токена
    size_t buf_size = 0, buf_len = 0;

    while (*p)
    {
        char c = *p;

        if (in_comm)
        {
            p++;
            continue;
        }

        if (escaped)
        {
            append_char(&token_buf, &buf_size, &buf_len, c);
            escaped = 0;
            p++;
            continue;
        }

        if (c == '\\')
        {
            escaped = 1;
            p++;
            continue;
        }

        if (c == '\\')
        {
            escaped = 1;
            p++;
            continue;
        }

        if (!in_single_quot && !in_double_quot && c == '#')
        {
            in_comm = 1;
            p++;
            continue;
        }

        if (c == '\'')
        {
            if (!in_double_quot)
            {
                in_single_quot = !in_single_quot;
            }
            else
            {
                append_char(&token_buf, &buf_size, &buf_len, c);
            }
            p++;
            continue;
        }

        if (c == '"')
        {
            if (!in_single_quot)
            {
                in_double_quot = !in_double_quot;
            }
            else
            {
                append_char(&token_buf, &buf_size, &buf_len, c);
            }
            p++;
            continue;
        }

        if (!in_single_quot && !in_double_quot && isspace((unsigned char)c))
        {
            add_token(list, &token_buf, &buf_len);
            p++;
            continue;
        }

        append_char(&token_buf, &buf_size, &buf_len, c); // если просто символ
        p++;
    }

    add_token(list, &token_buf, &buf_len);

    free(token_buf);

    substitute_variables_in_list(list);

    return (int)list->lenght;
}