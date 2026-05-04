# include <stdio.h>
# include <stdlib.h>
//список
typedef struct Node{
    char *word;
    struct Node *next;
} Node;
typedef Node* list;
//создание новоого звена и записи туда слова и возвращения указателя на последнее звено
Node* add_link(char *word){
    //создание звена
    Node* new = (Node*)malloc(sizeof(Node));
    //элемент
    int len = 0;
    while (word[len] != '\0'){
        len++;
    }
    new->word = (char*)malloc((len + 1)*sizeof(char));
    for (int i = 0; i <= len; i++){
        new->word[i] = word[i];
    }
    new->next = NULL;
    return new;
}
//добавляем в конце списка
void add_word(list *lst, char* word){
    Node* new = add_link(word);
    if(*lst == NULL){//пустой изначально
        *lst = new;
    }else{//не пустой, впихиваем в конец
        Node* cur = *lst;
        while(cur->next != NULL){
            cur = cur->next;
        }
        cur->next = new;
    }
}
//сравнения слов
int comp_word(char* s1, char* s2){
    while(*s1 && *s2){
        if(*s1 != *s2){
            return 0;
        }
        s1++;
        s2++;
    }
    return(*s1 == '\0' && *s2 == '\0');
}
//удаление дубликатов
void remove_dubl(list *lst, char* lastWord){
    //костыль чтобы последнее слово тоже не удалить
    Node *prev = NULL;
    Node *cur = *lst;
    Node *last = NULL;
    while (cur != NULL){
        if (comp_word(cur->word, lastWord)){
            last = cur;
        }
        cur = cur->next;
    }
    //удаляем повторы
    prev = NULL;
    cur = *lst;
    while (cur != NULL){
        if (comp_word(cur->word, lastWord) && cur != last){
            if (prev == NULL){
                *lst = cur -> next;
            }else{
                prev->next = cur->next;
            }
            free(cur->word);
            free(cur);
            if (prev == NULL){
                cur = *lst;
            }else{
                cur = prev->next;
            }
        }else{
            prev = cur;
            cur = cur->next;
        }
    }
}

//выделяем из строки слова и запихиваем в список и вызываем удаление дубликатов
void split_add(char *str, list *lst){
    char word[50];
    int len = 0;
    char *lastWord = NULL;
    
    for (int i = 0; ;i++){
        if (str[i] == ' ' || str[i] == '\0'){
            if (len > 0){
                word[len] = '\0';//конец слова
                add_word(lst, word); //добавялем в список
                lastWord = word;
                len = 0;
            }
            if (str[i] == '\0'){
                break;
            }
        }else{
            word[len++] = str[i];
        }
    }
    if (lastWord != NULL){
        remove_dubl(lst, lastWord);
    }
}
//очищаем список
void free_lst(list *lst){
    Node *cur = *lst;
    while (cur != NULL){
        Node *temp = cur;
        cur = cur->next;
        free(temp->word);
        free(temp);
    }
    *lst = NULL;
}
//вывод списка
void print(list *lst){
    Node *cur = *lst;
    while(cur != NULL){
        printf("%s", cur->word);
        if(cur->next != NULL){
            printf(" ");
        }
        cur = cur->next;
    }
    printf("\n");
}

int main() {
    list lst = NULL;
    char str[1000];
    int len = 0;
    int i = 0;
    printf("Введите строку: ");
    fgets(str, sizeof(str), stdin);
    while (str[i] != '\0'){
        len++;
        i++;
    }
    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
    }
    split_add(str, &lst);
    print(&lst);
    free_lst(&lst);
    return 0;
}


