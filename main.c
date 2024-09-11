#include "main.h"

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
    cmd->executable = false;
    return cmd;
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

char *complement_cmd(char *str)
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
            tmp = ft_strjoin_with_free(str, readline(">"));
            add_history(tmp);
        }
        else
            return str;
        return umm(tmp);
    }
    return NULL;
}




void process_input(char *str, char ***envp)
{
	static int	exit_code;
	str = *complement_cmd(str);
	char **split = ft_split(str, '|');
	int cnt = cnt_cmd(split);
	t_redirection **command = (malloc(sizeof(t_redirection *) * cnt));
	int input_fd = 0;
	int pipe_fd[2];
	pid_t *pids = malloc(sizeof(pid_t) * cnt);
	struct termios old;

	for (int i = 0; i < cnt; i++)
	{
		initialize_redirection(&command[i]);
		parse_redirection(split[i], command[i], *envp);
	}

	for (int i = 0; i < cnt; i++)
	{
		if (i < cnt - 1)
		{
			if (pipe(pipe_fd) == -1)
			{
				perror("pipe\n");
				exit(EXIT_FAILURE);
			}
		}
		else
		{
			pipe_fd[0] = 0;
			pipe_fd[1] = 1;
		}
		pids[i] = fork();
		if (pids[i] == -1)
		{
			perror("fork\n");
			exit(EXIT_FAILURE);
		}

		if (pids[i] == 0)
		{
			end_sig(&old);
			// Child process
			if (i > 0)
			{
				dup2(input_fd, 0);
				close(input_fd);
			}
			if (i < cnt - 1)
			{
				dup2(pipe_fd[1], 1);
				close(pipe_fd[1]);
			}
			execute_command(command[i], envp, input_fd, pipe_fd[1]);
			exit(EXIT_SUCCESS);
		}
		else
		{
			// Parent process
			if (i > 0)
			{
				close(input_fd);
			}
			if (i < cnt - 1)
			{
				close(pipe_fd[1]);
			}
			input_fd = pipe_fd[0];
		}
	}
	for (int i = 0; i < cnt; i++)
	{
		int statloc;
		waitpid(pids[i], &statloc, 0);
		//printf("statloc : %d\n",WIFEXITED(statloc));
		if (WIFEXITED(statloc))
			exit_code = WEXITSTATUS(statloc);
		if (WIFSIGNALED(statloc))
		{
			exit_code = 128 + WTERMSIG(statloc);
			if (WTERMSIG(statloc) == 2)
				printf("\n");
			else if(WTERMSIG(statloc) == 3)
				printf("Quit (core dumped)\n");
		}
		if (!ft_strncmp(command[i]->full_cmd, "export ", 7) || !ft_strncmp(command[i]->full_cmd, "export", 8))
		{
			char **cd = ft_split(command[i]->full_cmd, ' ');
			if (cd[1] != NULL)
				exit_code = ft_export(cd, envp);
		}
	}
	for (int i = 0; i < cnt; i++)
	{
		free_redirection(command[i]);
	}
	for (int i = 0; split[i]; i++)
	{
		free(split[i]);
	}
	free(command);
	free(split);
	free(str);
	free(pids);
	printf("exit_code : %d\n", exit_code);
}

static void	cleanup(char *str)
{
	struct termios	old;

	end_sig(&old);
	free(str);
	printf("exit\n");
	exit(EXIT_SUCCESS);
}

int	main(int argc, char **argv, char *env[])
{
	char			*str;
	char			**envp;
	struct termios	old;
	char			*cwd;

	envp = initialize_environment(env);
	while (1)
	{
		cwd = build_prompt(envp);
		input_sig(&old);
		str = readline(cwd);
		none_sig(&old);
		if (ft_strlen(str))
			add_history(str);
		if (str)
		{
			process_input(str, &envp);
		}
		else
		{
			cleanup(str);
		}
	}
	return (0);
}
