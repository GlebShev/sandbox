#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>
#include <unistd.h>

// печать атрибутов файла
void print_file_attributes(struct stat fileStat, char *filename, int f_g)
{
    // тип файла
    if (S_ISREG(fileStat.st_mode))
        printf("-");
    else if (S_ISDIR(fileStat.st_mode))
        printf("d");
    else if (S_ISLNK(fileStat.st_mode))
        printf("l");
    else
        printf("?");

    // права доступа к файлу
    printf((fileStat.st_mode & S_IRUSR) ? "r" : "-"); // владелец чтение итд
    printf((fileStat.st_mode & S_IWUSR) ? "w" : "-");
    printf((fileStat.st_mode & S_IXUSR) ? "x" : "-");

    printf((fileStat.st_mode & S_IRGRP) ? "r" : "-"); // группы чтение итд
    printf((fileStat.st_mode & S_IWGRP) ? "w" : "-");
    printf((fileStat.st_mode & S_IXGRP) ? "x" : "-");

    printf((fileStat.st_mode & S_IROTH) ? "r" : "-"); // другие чтение итд
    printf((fileStat.st_mode & S_IWOTH) ? "w" : "-");
    printf((fileStat.st_mode & S_IXOTH) ? "x" : "-");

    // владельцы и доступ
    struct passwd *pw = getpwuid(fileStat.st_uid);
    struct group *gr = getgrgid(fileStat.st_gid);
    printf(" %s", pw->pw_name);

    // обработка -g
    if (f_g)
    {
        printf(" %s", gr->gr_name);
    }
    // размер файла
    printf(" %lld", fileStat.st_size);

    // имя
    printf(" %s", filename);

    // символическая ссылка, если есть
    if (S_ISLNK(fileStat.st_mode))
    {
        char link[1024];
        ssize_t l = readlink(filename, link, sizeof(link) - 1);
        if (l != -1)
        {
            link[l] = '\0';
            printf(" -> %s", link);
        }
    }

    printf("\n");
}

// содержимое директорий (основная функция)
void list_dir(const char *f_p, int f_R, int f_l, int f_g)
{
    DIR *dir;
    struct dirent *content;

    if (!(dir = opendir(f_p)))
    {
        perror("Ошибка открытия директории");
        return;
    }

    // чтение содержимого
    while ((content = readdir(dir)) != NULL)
    { // пропускаем текущую и род. директории
        if (strcmp(content->d_name, ".") == 0 || strcmp(content->d_name, "..") == 0)
            continue;

        // путь к файлу
        char filePath[1024];
        snprintf(filePath, sizeof(filePath), "%s/%s", f_p, content->d_name);

        struct stat fileStat;
        // инфа о файле
        if (lstat(filePath, &fileStat) == -1)
        {
            perror("Ошибка получения информации о файле");
            continue;
        }
        // Если -l
        if (f_l)
        {
            print_file_attributes(fileStat, content->d_name, f_g);
        }
        else
        {
            printf("%s\n", content->d_name);
        }
        // обработка флага -R
        if (f_R && S_ISDIR(fileStat.st_mode))
        {
            printf("\n%s:\n", filePath);
            list_dir(filePath, f_R, f_l, f_g); // рекурсивно обходим
        }
    }
    closedir(dir);
}

int main(int argc, char *argv[])
{
    int f_R = 0;
    int f_l = 0;
    int f_g = 0;

    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-R") == 0)
        {
            f_R = 1;
        }
        else if (strcmp(argv[i], "-l") == 0)
        {
            f_l = 1;
        }
        else if (strcmp(argv[i], "-g") == 0)
        {
            f_g = 1;
        }
    }
    list_dir(".", f_R, f_l, f_g);
}
