#include "main.h"

char *local;

int is_whitespace(int c)
{
    return ((c >= 9 && c <= 13) || c == 32);
}

t_command *create_command(void)
{
    t_command *cmd = malloc(sizeof(t_command));
    if (!cmd)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    cmd->command = NULL;
    cmd->order = false;
    cmd->exist = false;
    return cmd;
}

void free_command(t_command *cmd)
{
    if (cmd)
    {
        if (cmd->command)
        {
            free_command_list(&cmd->command);
        }
        free(cmd);
    }
}

void initialize_redirection(t_redirection **redirection)
{
    *redirection = malloc(sizeof(t_redirection));
    if (!*redirection)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    (*redirection)->double_left_brace = create_command();
    (*redirection)->double_right_brace = create_command();
    (*redirection)->command = create_command();
    (*redirection)->left_brace = create_command();
    (*redirection)->right_brace = create_command();
}

void free_redirection(t_redirection *redirection)
{
    if (redirection)
    {
        free_command(redirection->double_left_brace);
        free_command(redirection->double_right_brace);
        free_command(redirection->command);
        free_command(redirection->left_brace);
        free_command(redirection->right_brace);
        free(redirection);
    }
}

char **allocate_and_copy(char **cmd, int size)
{
    char **new_cmd = malloc(sizeof(char *) * size);
    if (!new_cmd)
    {
        perror("malloc");
        return NULL;
    }
    for (int j = 0; j < size - 1; j++)
    {
        if (cmd[j] == NULL)
        {
            new_cmd[j] = NULL;
        }
        else
        {
            new_cmd[j] = ft_strdup(cmd[j]);
            if (!new_cmd[j])
            {
                perror("ft_strdup");
                for (int k = 0; k < j; k++)
                {
                    free(new_cmd[k]);
                }
                free(new_cmd);
                return NULL;
            }
        }
    }
    return new_cmd;
}

char **append_command(char ***cmd, const char *str)
{
    int i = 0;
    char **new_cmd;

    if (*cmd == NULL)
    {
        new_cmd = malloc(sizeof(char *) * 2);
        if (!new_cmd)
        {
            perror("malloc");
            return NULL;
        }
        new_cmd[0] = ft_strdup(str);
        new_cmd[1] = NULL;
        return new_cmd;
    }
    while ((*cmd)[i])
        i++;
    new_cmd = allocate_and_copy(*cmd, i + 2);
    if (!new_cmd)
        return NULL;
    new_cmd[i] = ft_strdup(str);
    new_cmd[i + 1] = NULL;
    for (int j = 0; j < i; j++)
    {
        free((*cmd)[j]);
    }
    free(*cmd);
    return new_cmd;
}

void free_command_list(char ***command)
{
    if (*command)
    {
        int i = 0;
        while ((*command)[i])
        {
            free((*command)[i]);
            i++;
        }
        free(*command);
        *command = NULL;
    }
}

void parse_left_redirection(const char *str, int *i, t_redirection *command)
{
    int j, flag = 0;
    char *content;

    (*i)++;
    if (str[*i] == '<')
    {
        (*i)++;
        flag = 1;
    }
    while (is_whitespace(str[*i]))
        (*i)++;
    j = *i;
    if (str[*i] == '>' || str[*i] == '<' || str[*i] == '\0' || str[*i] == '|' || str[*i] == '&' || str[*i] == ';')
    {
        printf("zsh: parse error near `%c'\n", str[*i]);
        exit(258);
    }
    while (str[*i] && !is_whitespace(str[*i]) && str[*i] != '>' && str[*i] != '<')
        (*i)++;
    content = ft_substr(str, j, *i - j);
    if (flag == 1)
    {
        command->double_left_brace->exist = true;
        command->double_left_brace->command = append_command(&command->double_left_brace->command, content);
    }
    else
    {
        command->left_brace->exist = true;
        command->left_brace->command = append_command(&command->left_brace->command, content);
    }
    free(content);
}

void parse_right_redirection(char *str, int *i, t_redirection *command)
{
    int j, flag = 0;
    char *content;

    (*i)++;
    if (str[*i] == '>')
    {
        (*i)++;
        flag = 1;
    }
    while (is_whitespace(str[*i]))
        (*i)++;
    if (str[*i] == '>' || str[*i] == '<' || str[*i] == '\0' || str[*i] == '|' || str[*i] == '&' || str[*i] == ';')
    {
        printf("zsh: parse error near `%c'\n", str[*i]);
        exit(258);
    }
    j = *i;
    while (str[*i] && !is_whitespace(str[*i]) && str[*i] != '>' && str[*i] != '<')
        (*i)++;
    content = ft_substr(str, j, *i - j);
    if (flag == 1)
    {
        command->double_right_brace->exist = true;
        command->double_right_brace->command = append_command(&command->double_right_brace->command, content);
    }
    else
    {
        command->right_brace->exist = true;
        command->right_brace->command = append_command(&command->right_brace->command, content);
    }
    free(content);
}

char *handle_single_quotes(const char *str, int *i)
{
    int start = ++(*i);
    while (str[*i] && str[*i] != '\'')
        (*i)++;
    if (str[*i] != '\'')
    {
        fprintf(stderr, "Error: Unmatched single quote\n");
        exit(EXIT_FAILURE);
    }
    char *content = ft_substr(str, start, *i - start);
    (*i)++;
    return content;
}

char *handle_double_quotes(const char *str, int *i)
{
    int start = ++(*i);
    while (str[*i] && str[*i] != '"')
    {
        if (str[*i] == '\\' && (str[*i + 1] == '"' || str[*i + 1] == '\\'))
            (*i)++;
        (*i)++;
    }
    if (str[*i] != '"')
    {
        fprintf(stderr, "Error: Unmatched double quote\n");
        exit(EXIT_FAILURE);
    }
    char *content = ft_substr(str, start, *i - start);
    (*i)++;
    return content;
}

void parse_command(char *str, int *i, t_redirection *command)
{
    int j;
    char *content;

    while (is_whitespace(str[*i]))
        (*i)++;
    if (str[*i] == '"')
    {
        content = handle_double_quotes(str, i);
    }
    else if (str[*i] == '\'')
    {
        content = handle_single_quotes(str, i);
    }
    else
    {
        j = *i;
        while (str[*i] && !is_whitespace(str[*i]) && str[*i] != '>' && str[*i] != '<')
            (*i)++;
        if (j == *i)
            return;
        content = ft_substr(str, j, *i - j);
    }
    command->command->command = append_command(&command->command->command, content);
    free(content);
}

char *ft_find_single_redirect(char *str, char c)
{
    int i = 0;

    if (!str)
        return NULL;
    while (str[i])
    {
        if (str[i] == c)
        {
            if (str[i + 1] == c)
            {
                i++;
            }
            else
            {
                return &str[i];
            }
        }
        i++;
    }
    return NULL;
}

void set_order(t_redirection *command, char *str)
{
    int dual;
    int single;
	char *rev;

    dual = ft_strlen(ft_strnstr(str, "<<", sizeof(str)));
    single = ft_strlen(ft_find_single_redirect(str, '<'));
    if (dual > single)
    {
        command->double_left_brace->order = true;
    }
    else if (dual < single)
    {
        command->left_brace->order = true;
    }
	rev = ft_strrev(str);
    dual = ft_strlen(ft_strnstr(rev, ">>", ft_strlen(str)));
    single = ft_strlen(ft_find_single_redirect(rev, '>'));
    if (dual > single)
    {
        command->double_right_brace->order = true;
    }
    else if (dual < single)
    {
        command->right_brace->order = true;
    }
	free(rev);
}

void parse_redirection(char *str, t_redirection *command)
{
    int i = 0;

    while (str[i])
    {
        while (is_whitespace(str[i]))
            i++;
        if (str[i] == '<')
        {
            parse_left_redirection(str, &i, command);
        }
        else if (str[i] == '>')
        {
            parse_right_redirection(str, &i, command);
        }
        else
        {
            parse_command(str, &i, command);
        }
    }
    set_order(command, str);
}

void open_redirection_files(t_redirection *command)
{
    int i = 0;
    int fd;

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
                str = ft_strjoin(str, input);
                str = ft_strjoin(str, "\n");
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
    i = 0;
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

    i = 0;
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

    i = 0;
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

int cnt_cmd(char **split)
{
    int i = 0;

    if (!split)
    {
        return 0;
    }
    while (split[i])
    {
        i++;
    }
    return i;
}

void exe(t_redirection *command, char **cmd, char **envp)
{
    char *cmd_path = NULL;
    char *path = get_path(envp);
    if (cmd)
    {
        cmd_path = get_cmd_path(cmd[0], path);
    }
    if (execve(cmd_path, cmd, envp))
    {
        if (cmd)
            printf("%s: command not found\n", cmd[0]);
    }
    free(path);
}

void execute_command(t_redirection *command, char **envp, int input_fd, int output_fd)
{
    pid_t pid;

    pid = fork();
    if (pid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0)
    {
        // Child process
        if (input_fd != 0)
        {
            dup2(input_fd, 0);
            close(input_fd);
        }
        if (output_fd != 1)
        {
            dup2(output_fd, 1);
            close(output_fd);
        }
        open_redirection_files(command);
        exe(command, command->command->command, envp);
        exit(EXIT_FAILURE); // execve가 실패한 경우
    }
}

char *umm(char *str)
{
    if (str)
    {
        int len = ft_strlen(str);
        char *tmp;

        if (len == 0) // 문자열이 비어 있는 경우
            return str;

        len--;
        while (is_whitespace(str[len]) > 0 && len > 0)
        {
            len--;
        }
        if (is_whitespace(str[len]) && len == 0) // 문자열이 공백만 포함하는 경우
            return str;

        if (str[len] == '|')
        {
            tmp = ft_strjoin(str, readline(">"));
            add_history(tmp);
        }
        else
            return str;
        return umm(tmp);
    }
    return NULL;
}

int main(int argc, char **argv, char **envp)
{
    char *str;
    while (1)
    {
        pid_t pid;
        pid = fork();
        if (pid == 0)
        {
            str = readline("command : ");
            if (ft_strlen(str))
                add_history(str);
            if (str)
            {
                str = umm(str);
                char **split = ft_split(str, '|');
                int cnt = cnt_cmd(split);
                int i = 0;
                t_redirection **command = (malloc(sizeof(t_redirection *) * cnt));
                int input_fd = 0;
                int pipe_fd[2];

                if (split)
                {
                    while (split[i])
                    {
                        initialize_redirection(&command[i]);
                        if (str == NULL)
                            exit(EXIT_FAILURE);
                        parse_redirection(split[i], command[i]);
                        i++;
                    }
                }

                for (i = 0; i < cnt; i++)
                {
                    if (i < cnt - 1)
                    {
                        if (pipe(pipe_fd) == -1)
                        {
                            perror("pipe");
                            exit(EXIT_FAILURE);
                        }
                    }
                    else
                    {
                        pipe_fd[0] = 0;
                        pipe_fd[1] = 1;
                    }
                    execute_command(command[i], envp, input_fd, pipe_fd[1]);

                    if (input_fd != 0)
                        close(input_fd);
                    if (pipe_fd[1] != 1)
                        close(pipe_fd[1]);

                    input_fd = pipe_fd[0];
                }

                for (i = 0; i < cnt; i++)
                {
                    wait(NULL);
                }

                for (i = 0; i < cnt; i++)
                {
                    free_redirection(command[i]);
                }

                if (split)
                {
					i = 0;
                    while (split[i])
                    {
                        free(split[i]);
                        i++;
                    }
                }
                free(command);
                free(split);
                free(str);
                exit(EXIT_SUCCESS);
            }
            else
            {
                free(str);
                exit(EXIT_SUCCESS);
            }
        }
        else
            waitpid(pid, NULL, 0);
    }
    return 0;
}