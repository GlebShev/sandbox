#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
#include "exec.h"

static int exec_command(Tree *node);
static int exec_pipeline(Tree *node);
static int exec_and(Tree *node);
static int exec_or(Tree *node);
static int exec_semicolon(Tree *node);
static int exec_background(Tree *node);

int execute(Tree *root)
{
    if (!root)
        return 0;
    switch (root->type)
    {
    case NODE_COMMAND:
        return exec_command(root);
    case NODE_PIPE:
        return exec_pipeline(root);
    case NODE_AND:
        return exec_and(root);
    case NODE_OR:
        return exec_or(root);
    case NODE_SEMICOLON:
        return exec_semicolon(root);
    case NODE_BACKGROUND:
        return exec_background(root);
    }
    return 0;
}

static int exec_command(Tree *node)
{
    if (node->cmd.is_subshell)
    {
        // подшел, создаем дочерний процесс, чтоб текущий не менялся
        pid_t pid = fork();
        if (pid < 0)
        {
            perror("fork");
            return 1;
        }
        if (pid == 0)
        {
            int status = execute(node->cmd.subshell);
            exit(status);
        }
        int status;
        waitpid(pid, &status, 0);
        return WIFEXITED(status) ? WEXITSTATUS(status) : 1;
    }
    else
    {
        // простая команда
        if (node->cmd.argc == 0)
        {
            // пустая
            return 0;
        }
        // внутрение команды
        if (strcmp(node->cmd.argv[0], "exit") == 0)
        {
            exit(0);
        }
        else if (strcmp(node->cmd.argv[0], "cd") == 0)
        {
            const char *dir = (node->cmd.argc > 1) ? node->cmd.argv[1] : getenv("HOME");
            if (!dir)
            {
                dir = getenv("HOME");
            }
            if (chdir(dir) == -1)
            {
                perror("cd");
                return 1;
            }
        }
        else if (strcmp(node->cmd.argv[0], "pwd") == 0)
        {
            char cwd[4096];
            if (getcwd(cwd, sizeof(cwd)))
            {
                printf("%s\n", cwd);
                return 0;
            }
            else
            {
                perror("pwd");
                return 1;
            }
        }
        // внешняя команда
        pid_t pid = fork();
        if (pid < 0)
        {
            perror("fork");
            return 1;
        }
        if (pid == 0)
        {
            // перенаправления
            if (node->cmd.redirect.input)
            {
                int fd = open(node->cmd.redirect.input, O_RDONLY);
                if (fd < 0)
                {
                    perror("open input");
                    exit(1);
                }
                dup2(fd, STDIN_FILENO);
                close(fd);
            }
            if (node->cmd.redirect.output)
            {
                int flags = O_WRONLY | O_CREAT;
                if (node->cmd.redirect.append)
                {
                    flags |= O_APPEND;
                }
                else
                {
                    flags |= O_TRUNC;
                }
                int fd = open(node->cmd.redirect.output, flags, 0666);
                if (fd < 0)
                {
                    perror("open output");
                    exit(1);
                }
                dup2(fd, STDOUT_FILENO);
                close(fd);
            }
            execvp(node->cmd.argv[0], node->cmd.argv);
            perror("execvp");
            exit(127);
        }
        int status;
        waitpid(pid, &status, 0);
        return WIFEXITED(status) ? WEXITSTATUS(status) : 1;
    }
}

static int exec_pipeline(Tree *node)
{
    int fd[2];
    if (pipe(fd) < 0)
    {
        perror("pipe");
        return 1;
    }
    pid_t pid_left = fork();
    if (pid_left < 0)
    {
        perror("fork left");
        return 1;
    }
    if (pid_left == 0)
    {
        dup2(fd[1], STDOUT_FILENO);
        close(fd[0]);
        close(fd[1]);
        int status = execute(node->left);
        exit(status);
    }
    pid_t pid_right = fork();
    if (pid_right < 0)
    {
        perror("fork right");
        return 1;
    }
    if (pid_right == 0)
    {
        dup2(fd[0], STDIN_FILENO);
        close(fd[0]);
        close(fd[1]);
        int status = execute(node->right);
        exit(status);
    }
    close(fd[0]);
    close(fd[1]);
    int status_left, status_right;
    waitpid(pid_left, &status_left, 0);
    waitpid(pid_right, &status_right, 0);
    return WIFEXITED(status_right) ? WEXITSTATUS(status_right) : 1;
}

static int exec_and(Tree *node)
{
    int left_status = execute(node->left);
    if (left_status == 0)
    {
        return execute(node->right);
    }
    else
    {
        return left_status;
    }
}

static int exec_or(Tree *node)
{
    int left_status = execute(node->left);
    if (left_status != 0)
    {
        return execute(node->right);
    }
    else
    {
        return left_status;
    }
}

static int exec_semicolon(Tree *node)
{
    // левая потом правая
    (void)execute(node->left);
    return execute(node->right);
}

static int exec_background(Tree *node)
{
    pid_t pid = fork();
    if (pid < 0)
    {
        perror("fork background");
        return 1;
    }
    if (pid == 0)
    {
        // перенаправим stdin из /dev/null чтобы точно не было блокировки терминала
        int fd = open("/dev/null", O_RDONLY);
        if (fd >= 0)
        {
            dup2(fd, STDIN_FILENO);
            close(fd);
        }
        int status = execute(node->left);
        exit(status);
    }
    // родитель не ждет а сразу праавую выполянет
    return execute(node->right);
}