#include "com.h"

int main()
{
    // создаем семафор
    int semid = semget(SEM_KEY, 1, IPC_CREAT | 0666); // 1 - кол-во семафоров
    if (semid == -1)
    {
        perror("semget");
        exit(1);
    }
    semctl(semid, 0, SETVAL, 0); // ставим на 0

    struct sembuf op_red = {0, -1, 0};
    struct sembuf op_green = {0, +1, 0}; // для зеленого увеличиваем на +1

    while (1)
    {
        // красный свет, значение должно быть 0
        int val = semctl(semid, 0, GETVAL);
        if (val == -1)
        {
            perror("semctl(GETVAL)");
            exit(1);
        }

        if (val > 0)
        {
            if (semop(semid, &op_red, 1) == -1)
            {
                perror("semop");
                exit(1);
            }
        }
        // семафор = 0 теперь
        // printf("Semaphore is RED (value = %d)\n", semctl(semid, 0, GETVAL));
        sleep(5);

        // для зеленого ставим семафор в 1
        if (semop(semid, &op_green, 1) == -1)
        {
            perror("semop");
            exit(1);
        }
        // printf("Semaphore is GREEN (value = %d)\n", semctl(semid, 0, GETVAL));
        sleep(3);
    }
}