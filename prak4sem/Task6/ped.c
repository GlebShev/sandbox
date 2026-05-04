#include "com.h"

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        perror("Need time");
        exit(1);
    }

    int time_go = atoi(argv[1]); // частато перехода

    int semid = semget(SEM_KEY, 1, 0);
    if (semid == -1)
    {
        perror("semget");
        exit(1);
    }

    struct sembuf wait_green = {0, -1, 0};

    pid_t pid = getppid();
    while (1)
    {
        fprintf(stderr, "Пешеход %d гуляет %d секунд\n", pid, time_go);
        sleep(time_go);

        fprintf(stderr, "Пешеход %d ждет зеленого\n", pid);
        if (semop(semid, &wait_green, 1) == -1)
        {
            // если семафор удален
            if (errno == EIDRM || errno == EINVAL)
            {
                fprintf(stderr, "Семафор пешехода %d удален\n", pid);
                break;
            }
            else
            {
                perror("semop");
                exit(1);
            }
        }

        // если мы тут то уже зеленый
        fprintf(stderr, "Пешеход %d переходит\n", pid);
        sleep(2); // типо переходит
        fprintf(stderr, "Пешеход %d перешел\n", pid);
    }
}