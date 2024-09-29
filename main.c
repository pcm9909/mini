#include "main.h"

void	set_dollar(int ptr, char ***envp)
{
	char	*tmp;
	char	*jo;

	tmp = ft_itoa(ptr);
	jo = ft_strjoin("?=",tmp);
	//printf("%s\n",jo);
	set_env(jo, 1, envp);
}
t_command	*create_command(void)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
	{
		ft_putstr_fd("minishell: Error: allocation failed", 2);
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
	(*redirection)->double_left_brace = create_command();
	(*redirection)->double_right_brace = create_command();
	(*redirection)->command = create_command();
	(*redirection)->left_brace = create_command();
	(*redirection)->right_brace = create_command();
	(*redirection)->executable = true;
	(*redirection)->here_doc = ft_strdup("");
}

char	**allocate_and_copy(char **cmd, int size)
{
	char	**new_cmd;
	int		j;

	j = 0;
	new_cmd = malloc(sizeof(char *) * size);
	if (!new_cmd)
	{
		ft_putstr_fd("minishell: Error: allocation failed", 2);
		return (NULL);
	}
	while (j < size - 1)
	{
		if (cmd[j] == NULL)
			new_cmd[j] = NULL;
		else
			new_cmd[j] = ft_strdup(cmd[j]);
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
	j = -1;
	while (++j < i)
		free((*cmd)[j]);
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
			len--;
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
	char	*tmp;
	char	**cd;

	i = -1;
	while (++i < cnt)
	{
		waitpid(pids[i], &statloc, 0);
		if (WIFEXITED(statloc))
		{
			set_dollar(WEXITSTATUS(statloc),envp);
		}
		if (WIFSIGNALED(statloc))
		{
			set_dollar(128 + WTERMSIG(statloc),envp);
			if (WTERMSIG(statloc) == 2)
				printf("\n");
			else if (WTERMSIG(statloc) == 3)
				ft_putstr_fd("Quit (core dumped)\n", 2);
		}
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
		pipe(pipe_fd);
	}
	else
	{
		pipe_fd[0] = 0;
		pipe_fd[1] = 1;
	}
}
void fork_and_execute(int i, int cnt, int input_fd, int pipe_fd[2], \
                            pid_t *pids, t_redirection **command, \
                            char ***envp, struct termios *old)
{
	pids[i] = fork();
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
			close(input_fd);
		if (i < cnt - 1)
			close(pipe_fd[1]);
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

void handle_dollar1(int *i, char **content, const char *str, char **envp)
{
	char	*temp;
	char	*envp_var;
	char	*envp_val;
	int		idx;
	int		start;

	(*i)++;
	start = (*i);
	while (is_envp_vars(str[*i]))
		(*i)++;
	envp_var = ft_substr(str, start, (*i) - start);
	idx = ft_strlen(envp_var) + 1;
	envp_val = ft_strdup(envp[search_env(envp, envp_var, 1)]);
	free(envp_var);
	if (envp_val)
		(*content) = ft_strjoin_with_free((*content), &envp_val[idx]);
	free(envp_val);
}

char *set_str(char *str, char **envp)
{
	int 	i;
	int		start;
	char	*content;

	i = 0;
	start = 0;
	content = ft_strdup("");
	while(str[i])
	{
		if(str[i] == '\'')
		{
			i++;
			while(str[i] && str[i] != '\'')
			{
				i++;
			}
			if(str[i] == '\'')
				i++;
		}
		else if(str[i] == '$')
		{
			content = ft_strjoin_with_free2(content, ft_substr(str, start, i - start));
			handle_dollar(&i, &content, str, envp);
			start = i;
		}
		else
			i++;
	}
	content = ft_strjoin_with_free2(content, ft_substr(str, start, i - start));
	free(str);
	return content;
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
	int				builtin_num;
	int	in;
	int out;

	i = 0;
	str = complement_cmd(str);
	str = set_str(str, *envp);
	printf("\n\nstr = %s\n\n", str);
	split = ft_splits(str, '|');
	cnt = cnt_cmd(split);
	pids = malloc(sizeof(pid_t) * cnt);
	input_fd = 0;
	command = (malloc(sizeof(t_redirection *) * cnt));
	initialize_commands(split, cnt, &command, envp);
	while (i < cnt)
	{
		set_dollar(0 ,envp);
		builtin_num = check_builtin_num(command[i]);
		if (builtin_num)
		{
			create_pipes(i, cnt, pipe_fd);
			in = dup(0);
			out = dup(1);
			end_sig(&old);
			if (i > 0)
			{
				dup2(input_fd, 0);
				//close(input_fd);
			}
			if (i < cnt - 1)
			{
				dup2(pipe_fd[1], 1);
				//close(pipe_fd[1]);
			}
			if(!open_redirection_files(command[i]))
				handle_builtin_command(command[i], envp, builtin_num);
			if (i > 0)
				close(input_fd);
			if (i < cnt - 1)
				close(pipe_fd[1]);
			input_fd = pipe_fd[0];
			dup2(in, 0);
			dup2(out, 1);
		}
		else
		{
			create_pipes(i, cnt, pipe_fd);
			fork_and_execute(i, cnt, input_fd, pipe_fd, pids, command, envp, &old);
			if (i > 0)
				close(input_fd);
			if (i < cnt - 1)
				close(pipe_fd[1]);
			input_fd = pipe_fd[0];
		}
		i++;
	}
	wait_for_children(cnt, pids, command, envp, &exit_code);
	cleanup_resources(cnt, split, command, str, pids);
}

static void	cleanup(char *str)
{
	struct termios	old;

	end_sig(&old);
	free(str);
	ft_putstr_fd("exit\n", 2);
	exit(EXIT_SUCCESS);
}

int	main(int argc, char **argv, char *env[])
{
	char			*str;
	char			**envp;
	struct termios	old;
	char			*cwd;

	envp = update_envp(env,0,ft_strdup("?=0"));
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
