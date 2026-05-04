#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>

#define BUF 1024

// глобальная атомарная для изменения переменная, при этом всегда обращаемся к памяти
volatile sig_atomic_t cl_stop_fl = 0;

void handle_signal()
{
    cl_stop_fl = 1;
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

int main(int argc, char *argv[])
{
    int port;
    int ip;
    int sock;
    char buf[BUF];
    // char *mess = "Client 1 say hi";
    fd_set master_set, read_set;
    int fd_max;
    int bite_rec;

    struct sockaddr_in addr;
    // считываем айпи и порта
    if (argc != 3)
    {
        perror("incorrect number of arguments");
        exit(1);
    }

    sig_control();
    // ip = atoi(argv[1]);
    port = atoi(argv[2]);

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("socket fail");
        exit(1);
    }
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = inet_addr(argv[1]);
    if (connect(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("connect fail");
        exit(1);
    }

    FD_ZERO(&master_set);
    FD_SET(sock, &master_set);
    FD_SET(STDIN_FILENO, &master_set);
    fd_max = (sock > STDIN_FILENO) ? sock : STDIN_FILENO;
    while (1)
    {
        if (cl_stop_fl == 1)
        {
            char mess[] = "\\qiut Client shut down";
            if (send(sock, mess, strlen(mess), 0) < 0)
            {
                perror("send fail");
            }
            printf("###Client shut down\n");
            break;
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

        if (FD_ISSET(sock, &read_set))
        {
            // данные от сервера
            bite_rec = recv(sock, buf, sizeof(buf) - 1, 0);
            if (bite_rec < 0)
            {
                perror("recv fail");
                // close(i);
                break;
            }
            else if (bite_rec == 0)
            {
                printf("### Server stop\n");
                // close(i);
                break;
            }
            else
            {
                buf[bite_rec] = '\0';
                printf("%s\n", buf);
            }
        }
        // ввели строку
        if (FD_ISSET(STDIN_FILENO, &read_set))
        {
            if (fgets(buf, sizeof(buf), stdin) == NULL)
            {
                break;
            }
            buf[strcspn(buf, "\r\n")] = '\0';
            if (send(sock, buf, strlen(buf), 0) < 0)
            {
                perror("send fail");
            }
        }
    }
    close(sock);
}
