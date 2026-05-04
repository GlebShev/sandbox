#include "com.h"

int main()
{
    int semid = semget(SEM_KEY, 1, 0);
    if (semid == -1)
    {
        perror("semget");
        exit(1);
    }

    if (semctl(semid, 0, IPC_RMID) == -1)
    {
        perror("semctl(IPC_RMID)");
        exit(1);
    }

    fprintf(stderr, "Семафор удален.\n");
}