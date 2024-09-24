#include "main.h"

t_command	*create_command(void)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	cmd->command = NULL;
	cmd->order = false;
	cmd->exist = false;
	return (cmd);
}

void	initialize_redirection(t_redirection **redirection)
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
	(*redirection)->executable = true;
}

char	**allocate_and_copy(char **cmd, int size)
{
	char	**new_cmd;
	int		j;
	int		k;

	j = 0;
	new_cmd = malloc(sizeof(char *) * size);
	if (!new_cmd)
	{
		perror("malloc");
		return (NULL);
	}
	while (j < size - 1)
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
				while (k < j)
				{
					free(new_cmd[k]);
					k++;
				}
				free(new_cmd);
				return (NULL);
			}
		}
		j++;
	}
	return (new_cmd);
}

char	**append_command(char ***cmd, char *str)
{
	int		i;
	char	**new_cmd;
	int		j;

	i = 0;
	if (*cmd == NULL)
	{
		new_cmd = malloc(sizeof(char *) * 2);
		if (!new_cmd)
		{
			perror("malloc");
			return (NULL);
		}
		new_cmd[0] = ft_strdup(str);
		new_cmd[1] = NULL;
		return (new_cmd);
	}
	while ((*cmd)[i])
		i++;
	new_cmd = allocate_and_copy(*cmd, i + 2);
	if (!new_cmd)
		return (NULL);
	new_cmd[i] = ft_strdup(str);
	new_cmd[i + 1] = NULL;
	j = 0;
	while (j < i)
	{
		free((*cmd)[j]);
		j++;
	}
	free(*cmd);
	return (new_cmd);
}

char	*complement_cmd(char *str)
{
	char	*tmp;
	int		len;

	if (str)
	{
		len = ft_strlen(str);
		if (len == 0)
			return (str);
		len--;
		while (is_whitespace(str[len]) > 0 && len > 0)
		{
			len--;
		}
		if (is_whitespace(str[len]) && len == 0)
			return (str);
		if (str[len] == '|')
		{
			tmp = ft_strjoin_with_free(str, readline(">"));
			add_history(tmp);
			str = tmp;
		}
		else
			return (str);
		return (complement_cmd(tmp));
	}
	return (NULL);
}

void	wait_for_children(int cnt, pid_t *pids, t_redirection **command, \
							char ***envp, int *exit_code)
{
	int		statloc;
	int		i;
	char	**cd;

	i = 0;
	while (i < cnt)
	{
		waitpid(pids[i], &statloc, 0);
		if (WIFEXITED(statloc))
			*exit_code = WEXITSTATUS(statloc);
		if (WIFSIGNALED(statloc))
		{
			*exit_code = 128 + WTERMSIG(statloc);
			if (WTERMSIG(statloc) == 2)
				printf("\n");
			else if (WTERMSIG(statloc) == 3)
				printf("Quit (core dumped)\n");
		}
		if (!ft_strncmp(command[i]->full_cmd, "export ", 7) || \
			!ft_strncmp(command[i]->full_cmd, "export", 8))
		{
			cd = ft_split(command[i]->full_cmd, ' ');
			if (cd[1] != NULL)
				*exit_code = ft_export(cd, envp);
		}
		i++;
	}
}

void	cleanup_resources(int cnt, char **split, t_redirection **command, \
							char *str, pid_t *pids)
{
	int	i;

	i = 0;
	while (i < cnt)
	{
		free_redirection(command[i]);
		i++;
	}
	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(command);
	free(split);
	free(str);
	free(pids);
}

void	create_pipes(int i, int cnt, int pipe_fd[2])
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
}
void handle_double_left_brace(t_redirection *cmd)
{
    int     i;
    char    *input;
    char    *str;
    int     pipe_fd[2];
    pid_t   pid;

    i = 0;
    while (cmd->double_left_brace->command && \
            cmd->double_left_brace->command[i])
    {
        if (pipe(pipe_fd) == -1)
        {
            perror("pipe");
            exit(EXIT_FAILURE);
        }

        pid = fork();
        if (pid == -1)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) // 자식 프로세스
        {
            close(pipe_fd[0]); // 읽기 끝을 닫음
            str = NULL;
            while (1)
            {
                input = readline(">");
                if (!input)
                    break;
                if (ft_strncmp(input, cmd->double_left_brace->command[i], \
                    ft_strlen(cmd->double_left_brace->command[i])) == 0 && \
                ft_strlen(input) == ft_strlen(cmd->double_left_brace->command[i]))
                {
                    free(input);
                    break;
                }
                str = ft_strjoin_with_free(str, input);
                str = ft_strjoin_with_free(str, "\n");
                add_history(input);
                free(input);
            }
            if (str)
            {
                write(pipe_fd[1], str, ft_strlen(str));
                free(str);
            }
            close(pipe_fd[1]);
            exit(EXIT_SUCCESS);
        }
        else // 부모 프로세스
        {
            close(pipe_fd[1]); // 쓰기 끝을 닫음
            waitpid(pid, NULL, 0); // 자식 프로세스가 끝날 때까지 기다림
            if (cmd->double_left_brace->command[i + 1] == NULL)
            {
                dup2(pipe_fd[0], 0); // 파이프의 읽기 끝을 표준 입력으로 복제
            }
            close(pipe_fd[0]);
        }
        i++;
    }
}

void fork_and_execute(int i, int cnt, int input_fd, int pipe_fd[2], \
                            pid_t *pids, t_redirection **command, \
                            char ***envp, struct termios *old)
{
    if (command[i]->double_left_brace && command[i]->double_left_brace->exist)
    {
        handle_double_left_brace(command[i]);
    }
    pids[i] = fork();
    if (pids[i] == -1)
    {
        perror("fork\n");
        exit(EXIT_FAILURE);
    }
    if (pids[i] == 0)
    {
        end_sig(old);
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


void	initialize_commands(char **split, int cnt, \
			t_redirection ***command, char ***envp)
{
	int	i;

	i = 0;
	while (i < cnt)
	{
		initialize_redirection(&(*command)[i]);
		parse_redirection(split[i], (*command)[i], *envp);
		i++;
	}
}

int	ft_count_wordss(const char *s, char c)
{
    int	i;
    int	count;
    int	in_quotes;

    i = 0;
    count = 0;
    in_quotes = 0;
    while (s[i])
    {
        if (s[i] == '"' || s[i] == '\'')
            in_quotes = !in_quotes;
        if (s[i] != c || in_quotes)
        {
            count++;
            while (s[i] && (s[i] != c || in_quotes))
            {
                if (s[i] == '"' || s[i] == '\'')
                    in_quotes = !in_quotes;
                i++;
            }
        }
        else
            i++;
    }
    return (count);
}

char	**ft_free_arrs(char **arr, size_t i)
{
    while (i > 0)
    {
        free(arr[i]);
        i--;
    }
    free(arr);
    return (NULL);
}

char	**ft_splits(char const *s, char c)
{
    char	**arr;
    int		i;
    int		j;
    int		k;
    int		in_quotes;

    if (!s)
        return (NULL);
    arr = (char **)malloc(sizeof(char *) * (ft_count_wordss(s, c) + 1));
    if (!arr)
        return (NULL);
    i = -1;
    j = 0;
    in_quotes = 0;
    while (++i < ft_count_wordss(s, c))
    {
        while (s[j] == c && !in_quotes)
            j++;
        k = j;
        while (s[j] && (s[j] != c || in_quotes))
        {
            if (s[j] == '"' || s[j] == '\'')
                in_quotes = !in_quotes;
            j++;
        }
        arr[i] = ft_substr(s, k, j - k);
        if (!arr[i])
            return (ft_free_arrs(arr, i));
    }
    arr[i] = NULL;
    return (arr);
}

void	process_input(char *str, char ***envp)
{
    static int		exit_code;
    char			**split;
    int				cnt;
    t_redirection	**command;
    int				input_fd;
    int				pipe_fd[2];
    pid_t			*pids;
    struct termios	old;
    int				i;

    i = 0;
    str = complement_cmd(str);
    split = ft_splits(str, '|');
    cnt = cnt_cmd(split);
    pids = malloc(sizeof(pid_t) * cnt);
    input_fd = 0;
    command = (malloc(sizeof(t_redirection *) * cnt));
    initialize_commands(split, cnt, &command, envp);
    while (i < cnt)
    {
        create_pipes(i, cnt, pipe_fd);
        fork_and_execute(i, cnt, input_fd, pipe_fd, pids, command, envp, &old);
        if (i > 0)
        {
            close(input_fd);
        }
        if (i < cnt - 1)
        {
            close(pipe_fd[1]);
        }
        input_fd = pipe_fd[0];
        i++;
    }
    wait_for_children(cnt, pids, command, envp, &exit_code);
    cleanup_resources(cnt, split, command, str, pids);
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
