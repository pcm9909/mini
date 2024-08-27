#include "main.h"

void handle_double_left_brace(t_redirection *command)
{
    int i = 0;
    while (command->double_left_brace->command && command->double_left_brace->command[i])
    {
        int pipe_fd[2];
        if (pipe(pipe_fd) == -1)
        {
            perror("pipe");
            exit(EXIT_FAILURE);
        }

        pid_t pid = fork();
        if (pid == -1)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pid == 0)
        {
            // Child process
            close(pipe_fd[0]);
            char *str = ft_strdup("");
            char *input;
            input = readline(">");
            while (input)
            {
                if (ft_strncmp(input, command->double_left_brace->command[i], ft_strlen(input)) == 0 && ft_strlen(input) != 0 && command->double_left_brace->order == true)
                {
                    write(pipe_fd[1], str, ft_strlen(str));
                    free(input);
                    input = NULL;
                    break;
                }
                str = ft_strjoin_with_free(str, input);
                str = ft_strjoin_with_free(str, "\n");
                add_history(str);
                free(input);
                input = readline(">");
            }
            free(str);
            close(pipe_fd[1]);
            exit(EXIT_SUCCESS);
        }
        else
        {
            // Parent process
            close(pipe_fd[1]);
            wait(NULL);
            if (command->double_left_brace->command[i + 1] == NULL) // 마지막 <<의 경우
            {
                dup2(pipe_fd[0], 0);
            }
            close(pipe_fd[0]);
        }
        i++;
    }
}

void handle_left_brace(t_redirection *command)
{
    int i = 0;
    int fd;
    while (command->left_brace->command && command->left_brace->command[i])
    {
        int pipe_fd[2];
        if (pipe(pipe_fd) == -1)
        {
            perror("pipe");
            exit(EXIT_FAILURE);
        }

        pid_t pid = fork();
        if (pid == -1)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pid == 0)
        {
            // Child process
            close(pipe_fd[0]);
            fd = open(command->left_brace->command[i], O_RDONLY);
            if (fd == -1)
            {
                perror(command->left_brace->command[i]);
                exit(EXIT_FAILURE);
            }
            char buffer[1024];
            ssize_t bytes_read;
            while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0)
            {
                if (write(pipe_fd[1], buffer, bytes_read) == -1)
                {
                    perror("write");
                    exit(EXIT_FAILURE);
                }
            }
            close(fd);
            close(pipe_fd[1]);
            exit(EXIT_SUCCESS);
        }
        else
        {
            // Parent process
            close(pipe_fd[1]);
            wait(NULL);
            if (command->left_brace->command[i + 1] == NULL && command->left_brace->order) // 마지막 <의 경우
            {
                dup2(pipe_fd[0], 0);
            }
            close(pipe_fd[0]);
        }
        i++;
    }
}

void handle_right_brace(t_redirection *command)
{
    int i = 0;
    int fd;
    while (command->right_brace->command && command->right_brace->command[i])
    {
        fd = open(command->right_brace->command[i], O_CREAT | O_TRUNC | O_WRONLY, 0644);
        if (fd == -1)
        {
            perror(command->right_brace->command[i]);
            exit(EXIT_FAILURE);
        }
        if (command->right_brace->order == true)
        {
            dup2(fd, 1);
        }
        i++;
    }
}

void handle_double_right_brace(t_redirection *command)
{
    int i = 0;
    int fd;
    while (command->double_right_brace->command && command->double_right_brace->command[i])
    {
        fd = open(command->double_right_brace->command[i], O_CREAT | O_APPEND | O_WRONLY, 0644);
        if (fd == -1)
        {
            perror(command->double_right_brace->command[i]);
            exit(EXIT_FAILURE);
        }
        if (command->double_right_brace->order == true)
        {
            dup2(fd, 1);
        }
        i++;
    }
}