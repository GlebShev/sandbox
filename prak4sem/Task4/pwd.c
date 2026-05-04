#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

int main()
{
    char *buffer;
    size_t size = 1024;

    buffer = (char *)malloc(size * sizeof(char));
    if (buffer == NULL)
    {
        perror("Ошибка выделения памяти");
        return 1;
    }

    while (getcwd(buffer, size) == NULL)
    {
        size += 1;
        buffer = realloc(buffer, size * sizeof(char));
    }

    printf("%s\n", buffer);
    free(buffer);
}