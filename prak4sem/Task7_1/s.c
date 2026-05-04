#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <ctype.h>
#include <signal.h>

#define BUF 1024
#define NICK 16

// глобальная атомарная для изменения переменная, при этом всегда обращаемся к памяти
volatile sig_atomic_t serv_stop_fl = 0;

void handle_signal()
{
    serv_stop_fl = 1;
}

void sig_control()
{
    struct sigaction s;
    s.sa_handler = handle_signal;
    sigemptyset(&s.sa_mask);
    s.sa_flags = 0;
    if (sigaction(SIGINT, &s, NULL) == -1)
    {
        perror("SIGINT");
        exit(1);
    }
    /*
    if (sigaction(SIGSTOP, &s, NULL) == -1)
    {
        perror("SIGSTOP");
        exit(1);
    }
    */
    if (sigaction(SIGTERM, &s, NULL) == -1)
    {
        perror("SIGTERM");
        exit(1);
    }
}

// структура для хранения клиентов
typedef struct Client
{
    int socket;
    char nick[NICK];
    struct Client *next;
} Client;

Client *add_new_cl(Client **head, int sock, char *nick)
{
    Client *new_cl = malloc(sizeof(Client));
    if (!new_cl)
    {
        perror("malloc fail");
        exit(1);
    }
    new_cl->socket = sock;
    strncpy(new_cl->nick, nick, NICK - 1);
    new_cl->nick[NICK - 1] = '\0';
    new_cl->next = *head;
    *head = new_cl;
    return new_cl;
}

void del_cl(Client **head, int sock)
{
    Client *cur = *head;
    Client *prev = NULL;
    while (cur != NULL)
    {
        if (cur->socket == sock)
        {
            if (prev == NULL)
            {
                *head = cur->next;
            }
            else
            {
                prev->next = cur->next;
            }
            free(cur);
            return;
        }
        prev = cur;
        cur = cur->next;
    }
}

char *get_nick(Client *head, int sock)
{
    while (head != NULL)
    {
        if (head->socket == sock)
        {
            return head->nick;
        }
        head = head->next;
    }
    return "Dont know nick";
}

int uniq_nick(Client *head, char *nick)
{
    while (head != NULL)
    {
        if (strcmp(head->nick, nick) == 0)
        {
            return 0; // уже есть
        }
        head = head->next;
    }
    return 1; // уникален
}

void no_space(char *s)
{
    char *start = s;
    // пропускаем начальные пробелы и табы
    while (*start && isspace((unsigned char)*start))
        start++;
    // Если есть начальные пробелы сдвигаем строку влево
    if (start != s)
        memmove(s, start, strlen(start) + 1);
    // удаляем пробелы с конца
    int len = strlen(s);
    while (len > 0 && isspace((unsigned char)s[len - 1]))
    {
        s[len - 1] = '\0';
        len--;
    }
}

int main(int argc, char *argv[])
{
    Client *cl_l = NULL;
    int port;
    int sock;
    char buf[BUF];
    char *mess = "Hi, enter your nick on next string ↓";
    // char *mess2 = "%s enter the server";
    int bite_rec;
    fd_set master_set, read_set; // для работы с select, полная копия и для работы внутри select
    int fd_max;
    struct sockaddr_in addr;

    // считываем номер порта
    if (argc != 2)
    {
        perror("incorrect number of arguments");
        exit(1);
    }
    port = atoi(argv[1]);

    // обработчик сигналов
    sig_control();

    // создаем сокет
    int ser_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (ser_fd == -1)
    {
        perror("socket fail");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    // установка соединения
    if (bind(ser_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("blind fail");
        exit(1);
    }

    if (listen(ser_fd, 1) < 0)
    {
        perror("listen fail");
        exit(1);
    }
    // подключаемся

    FD_ZERO(&master_set);
    FD_SET(ser_fd, &master_set);
    fd_max = ser_fd;

    while (1)
    {
        if (serv_stop_fl == 1)
        {
            char shutdown_mess[] = "### Server shutdown by signal\n";
            for (int j = 0; j <= fd_max; j++)
            {
                if (FD_ISSET(j, &master_set))
                {
                    send(j, shutdown_mess, strlen(shutdown_mess), 0);
                    close(j);
                }
            }
            printf("Server stop\n");
            close(ser_fd);
            exit(0);
        }
        read_set = master_set;
        int sel = select(fd_max + 1, &read_set, NULL, NULL, NULL);
        if (sel < 0)
        {
            if (errno == EINTR)
            {
                continue;
            }
            perror("select fail");
            exit(1);
        }
        // перебираем сокеты
        for (int i = 0; i <= fd_max; i++)
        {
            if (FD_ISSET(i, &read_set))
            {
                if (i == ser_fd)
                {
                    // если дошли до слушующего сокеты то подключаемся
                    sock = accept(ser_fd, NULL, NULL);
                    if (sock < 0)
                    {
                        perror("accept fail");
                        // exit(1);
                    }
                    else
                    {
                        // собираем ник
                        char nick[NICK];
                        if (send(sock, mess, strlen(mess), 0) < 0)
                        {
                            perror("send fail");
                        }
                        int n = recv(sock, nick, sizeof(nick) - 1, 0);
                        if (n < 0)
                        {
                            close(sock);
                        }
                        else
                        {
                            nick[n] = '\0';
                            no_space(nick);
                            if (strlen(nick) == 0)
                            {
                                perror("incorrect nick");
                                close(sock);
                            }
                            else if (!uniq_nick(cl_l, nick))
                            {
                                char not_uniq_nick_mess[] = "### Nick is not uniq. Choose another\n";
                                send(sock, not_uniq_nick_mess, strlen(not_uniq_nick_mess), 0);
                                close(sock);
                            }
                            else
                            {
                                add_new_cl(&cl_l, sock, nick);
                                FD_SET(sock, &master_set);
                                printf("%d join\n", sock);
                                if (fd_max < sock)
                                {
                                    fd_max = sock;
                                }
                                char join_mes[BUF];
                                snprintf(join_mes, sizeof(join_mes), "*** %s  join our chat\n", nick);
                                for (int j = 0; j <= fd_max; j++)
                                {
                                    if (FD_ISSET(j, &master_set))
                                    {
                                        // слушающему не надо
                                        if (j != ser_fd)
                                        {
                                            if (send(j, join_mes, strlen(join_mes), 0) < 0)
                                            {
                                                perror("send fail");
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
                // данные от клиентов
                else
                {
                    // получаем сообщение(также обрабатываем ошибки и отключение соединения)
                    bite_rec = recv(i, buf, sizeof(buf) - 1, 0);
                    if (bite_rec < 0)
                    {
                        perror("recv fail");
                        close(i);
                        FD_CLR(i, &master_set);
                        del_cl(&cl_l, i);
                    }
                    else if (bite_rec == 0)
                    {
                        printf("Connecttion stop, socket: %d\n", i);
                        close(i);
                        FD_CLR(i, &master_set);
                        del_cl(&cl_l, i);
                    }
                    else
                    {
                        // обработка сообщения
                        buf[bite_rec] = '\0';
                        // проверка длины сообщения
                        if (bite_rec == sizeof(BUF) - 1 && strchr(buf, '\n') == NULL)
                        {
                            char too_long_mess[] = "### Too long message, it will be ignor\n";
                            if (send(i, too_long_mess, strlen(too_long_mess), 0) < 0)
                            {
                                perror("send fail");
                            }
                            continue;
                        }
                        // проверка пробелов
                        no_space(buf);
                        if (strlen(buf) == 0)
                        {
                            perror("empty mess");
                            continue;
                        }
                        // проверка на команды
                        if (buf[0] == '\\')
                        {
                            if (strncmp(buf, "\\users", 6) == 0 && (buf[6] == '\0' || isspace((unsigned char)buf[6])))
                            {
                                char user_l[BUF];
                                user_l[0] = '\0';
                                Client *cur = cl_l;
                                while (cur != NULL)
                                {
                                    strcat(user_l, cur->nick);
                                    strcat(user_l, "\n");
                                    cur = cur->next;
                                }
                                if (send(i, user_l, strlen(user_l), 0) < 0)
                                {
                                    perror("send fail");
                                }
                            }
                            else if (strncmp(buf, "\\quit", 5) == 0 && (buf[5] == '\0' || isspace((unsigned char)buf[5])))
                            {
                                char *bye_mess = buf + 5;
                                while (*bye_mess == ' ')
                                {
                                    bye_mess++;
                                }
                                char bye[BUF];
                                snprintf(bye, sizeof(bye), "*** %s left chat: %s", get_nick(cl_l, i), bye_mess);
                                for (int j = 0; j <= fd_max; j++)
                                {
                                    if (FD_ISSET(j, &master_set))
                                    {
                                        // слушающему не надо
                                        if (j != ser_fd)
                                        {
                                            if (send(j, bye, strlen(bye), 0) < 0)
                                            {
                                                perror("send fail");
                                            }
                                        }
                                    }
                                }
                                printf("Connecttion stop, socket: %d\n", i);
                                close(i);
                                FD_CLR(i, &master_set);
                                del_cl(&cl_l, i);
                            }
                        }
                        else
                        {
                            char out_mes[BUF];
                            char *nick = get_nick(cl_l, i);
                            snprintf(out_mes, sizeof(out_mes), "%s: %s", nick, buf);
                            printf("%d send: %s\n", i, buf);
                            // теперт отпраляем остальным клиентом
                            for (int j = 0; j <= fd_max; j++)
                            {
                                if (FD_ISSET(j, &master_set))
                                {
                                    // слушающему не надо
                                    if (j != ser_fd)
                                    {
                                        if (send(j, out_mes, strlen(out_mes), 0) < 0)
                                        {
                                            perror("send fail");
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}