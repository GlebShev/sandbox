#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 16
#define N 64

// Глобальные переменные
char **lst = NULL; // список слов
int sizelist = 0;  // размер списка
int curlist = 0;   // индекс текущего слова в списке
char *buf = NULL;  // буфер для накопления текущего слова
int sizebuf = 0;   // размер буфера
int curbuf = 0;    // индекс текущего символа в буфере

// Прототипы функций
void null_list();
void clearlist();
void nullbuf();
void addsym(char c);
void addword();
void termlist();
void printlist();
int check(const char *str1, const char *str2);
void sortlist();

int main() {
    char *str = NULL;
    size_t str_size = 0;
    null_list(); // Инициализация списка слов

    // Чтение строк из stdin, пока не достигнут конец файла (EOF)
    while (getline(&str, &str_size, stdin) != -1) {
        int len = strlen(str);

        // Обработка каждого символа в строке
        for (int i = 0; i < len; i++) {
            char c = str[i];
            if (c == ' ' || c == '\t' || c == '\n') { // Если символ - пробел, табуляция или новая строка
                if (curbuf > 0) { // Если в буфере накопилось слово
                    addword(); // Добавляем слово в список
                    nullbuf(); // Очищаем буфер для следующего слова
                }
            } else if (c == '|' || c == '&' || c == ';' || c == '>' || c == '<' || c == '(' || c == ')') { // Если символ - специальный
                if (curbuf > 0) { // Если в буфере накопилось слово
                    addword(); // Добавляем слово в список
                    nullbuf(); // Очищаем буфер
                }
                addsym(c); // Добавляем специальный символ в буфер
                // Проверка на двойные специальные символы (||, &&, >>)
                if ((c == '|' && str[i + 1] == '|') || (c == '&' && str[i + 1] == '&') || (c == '>' && str[i + 1] == '>')) {
                    addsym(str[++i]); // Добавляем второй символ и продвигаем индекс
                }
                addword(); // Добавляем специальное слово в список
                nullbuf(); // Очищаем буфер
            } else { // Если символ - часть обычного слова
                addsym(c); // Добавляем символ в буфер
            }
        }

        // Добавление последнего слова, если оно есть в буфере
        if (curbuf > 0) {
            addword();
        }

        termlist(); // Завершаем список, добавляя NULL в конец
        printlist(); // Печатаем список слов в исходном порядке
        sortlist(); // Сортируем список слов в лексикографическом порядке
        printlist(); // Печатаем отсортированный список
        clearlist(); // Очищаем список для обработки следующей строки
    }
    free(str);
}

// Инициализация списка слов
void null_list() {
    sizelist = 0;
    curlist = 0;
    lst = NULL;
}

// Очистка списка слов и освобождение памяти
void clearlist() {
    if (lst != NULL) {
        for (int i = 0; i < curlist; i++) {
            free(lst[i]); // Освобождаем память для каждого слова
        }
        free(lst); // Освобождаем память для списка
        lst = NULL;
    }
    sizelist = 0;
    curlist = 0;
}

// Инициализация буфера для текущего слова
void nullbuf() {
    if (buf != NULL) {
        free(buf); // Освобождаем память, если буфер уже был выделен
    }
    buf = NULL;
    sizebuf = 0;
    curbuf = 0;
}

// Добавление символа в буфер
void addsym(char c) {
    if (curbuf >= sizebuf) { // Если буфер заполнен, увеличиваем его размер
        char *new_buf = realloc(buf, sizebuf + SIZE);
        if (new_buf == NULL) { // Проверка на успешное выделение памяти
            fprintf(stderr, "Ошибка выделения памяти\n");
            exit(1);
        }
        buf = new_buf;
        sizebuf += SIZE;
    }
    buf[curbuf++] = c; // Добавляем символ в буфер и увеличиваем индекс
}

// Добавление слова в список слов
void addword() {
    if (curbuf >= sizebuf) { // Если в буфере нет места для '\0', увеличиваем размер
        char *new_buf = realloc(buf, sizebuf + 1);
        if (new_buf == NULL) { // Проверка на успешное выделение памяти
            fprintf(stderr, "Ошибка выделения памяти\n");
            exit(1);
        }
        buf = new_buf;
        sizebuf += 1;
    }
    buf[curbuf++] = '\0'; // Завершаем слово нулевым символом
    char *new_buf = realloc(buf, curbuf); // Обрезаем буфер до точного размера слова
    if (new_buf == NULL) { // Проверка на успешное выделение памяти
        fprintf(stderr, "Ошибка выделения памяти\n");
        exit(1);
    }
    buf = new_buf;

    if (curlist >= sizelist) { // Если список заполнен, увеличиваем его размер
        char **new_lst = realloc(lst, (sizelist + SIZE) * sizeof(*lst));
        if (new_lst == NULL) { // Проверка на успешное выделение памяти
            fprintf(stderr, "Ошибка выделения памяти\n");
            exit(1);
        }
        lst = new_lst;
        sizelist += SIZE;
    }
    lst[curlist++] = buf;
    buf = NULL; // Обнуляем буфер, чтобы не потерять указатель после добавления слова
}

// Завершение списка слов, добавление NULL в конец
void termlist() {
    if (lst == NULL) return;
    if (curlist >= sizelist) {
        char **new_lst = realloc(lst, (sizelist + 1) * sizeof(*lst));
        if (new_lst == NULL) {
            fprintf(stderr, "Ошибка выделения памяти\n");
            exit(1);
        }
        lst = new_lst;
        sizelist += 1;
    }
    lst[curlist] = NULL; // Завершаем список NULL-указателем
}

// Печать списка слов
void printlist() {
    if (lst == NULL || curlist == 0) { // Проверяем, пуст ли список
        printf("Список пуст\n");
        return;
    }
    printf("Длина списка: %d\n", curlist); // Печатаем длину списка
    for (int i = 0; i < curlist; i++) {
        printf("%s\n", lst[i]); // Печатаем каждое слово
    }
}

// Функция для сравнения строк
int check(const char *str1, const char *str2) {
    while (*str1 == *str2) {
        if (*str1 == '\0') {
            return 0; // Строки равны
        }
        str1++;
        str2++;
    }
    return (*str1 > *str2) ? 1 : -1; // Возвращаем 1, если str1 больше, и -1, если str2 больше
}

// Сортировка списка слов в лексикографическом порядке
void sortlist() {
    if (lst == NULL) return;
    // Используем пузырек
    for (int i = 0; i < curlist - 1; i++) {
        for (int j = i + 1; j < curlist; j++) {
            if (check(lst[i], lst[j]) > 0) { // Сравнение строк
                char *temp = lst[i];
                lst[i] = lst[j];
                lst[j] = temp; // Обмен слов местами
            }
        }
    }
}

