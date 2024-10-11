#include "main.h"

void print(t_redirection *cmd)
{
	int i = 0;
	if(cmd->left_brace->command)
	{
		while(cmd->left_brace->command[i])
		{
			printf("[lb]\n");
			printf("%s\n", cmd->left_brace->command[i]);
			i++;
		}
	}
	i = 0;
	if(cmd->double_left_brace->command)
	{
		while(cmd->double_left_brace->command[i])
		{
			printf("[dlb]\n");
			printf("%s\n", cmd->double_left_brace->command[i]);
			i++;
		}
	}
	i = 0;
	if(cmd->command->command)
	{
		while(cmd->command->command[i])
		{
			printf("[cmd]\n");
			printf("%s\n", cmd->command->command[i]);
			i++;
		}
	}
	i = 0;
	if(cmd->right_brace->command)
	{
		while(cmd->right_brace->command[i])
		{
			printf("[rb]\n");
			printf("%s\n", cmd->right_brace->command[i]);
			i++;
		}
	}
	i = 0;
	if(cmd->double_right_brace->command)
	{
		while(cmd->double_right_brace->command[i])
		{
			printf("[drb]\n");
			printf("%s\n", cmd->double_right_brace->command[i]);
			i++;
		}
	}
}

void	set_dollar(int ptr, char ***envp)
{
	char	*tmp;
	char	*jo;

	tmp = ft_itoa(ptr);
	jo = ft_strjoin_opts("?=", tmp, 2);
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
			tmp = ft_strjoin_opts(str, readline(">"), 3);
			add_history(tmp);
			str = tmp;
		}
		else
			return (str);
		return (complement_cmd(tmp));
	}
	return (NULL);
}

void	wait_for_children(t_process_data *data, char ***envp, int *exit_code)
{
	int		statloc;
	int		i;
	char	*tmp;
	char	**cd;

	i = -1;
	while (++i < data->cnt)
	{
		if (data->pids[i] == -1)
			continue ;
		waitpid(data->pids[i], &statloc, 0);
		if (WIFEXITED(statloc))
		{
			set_dollar(WEXITSTATUS(statloc), envp);
		}
		if (WIFSIGNALED(statloc))
		{
			set_dollar(128 + WTERMSIG(statloc), envp);
			if (WTERMSIG(statloc) == 2)
				printf("\n");
			else if (WTERMSIG(statloc) == 3)
				ft_putstr_fd("Quit (core dumped)\n", 2);
		}
	}
}

void	cleanup_resources(t_process_data *data)
{
	int	i;

	i = 0;
	while (i < data->cnt)
	{
		free_redirection(data->command[i]);
		i++;
	}
	i = 0;
	while (data->split[i])
	{
		free(data->split[i]);
		i++;
	}
	free(data->command);
	free(data->split);
	free(data->pids);
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

void	fork_and_execute(t_process_data *data, \
						int i, int input_fd, char ***envp)
{
	data->pids[i] = fork();
	if (data->pids[i] == 0)
	{
		end_sig(&data->old);
		if (i > 0)
		{
			dup2(input_fd, 0);
			close(input_fd);
		}
		if (i < data->cnt - 1)
		{
			dup2(data->pipe_fd[1], 1);
			close(data->pipe_fd[1]);
		}
		execute_command(data->command[i], envp, input_fd, data->pipe_fd[1]);
		exit(EXIT_SUCCESS);
	}
	else
	{
		if (i > 0)
			close(input_fd);
		if (i < data->cnt - 1)
			close(data->pipe_fd[1]);
		input_fd = data->pipe_fd[0];
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

void	set_splits_vars(int *i, int *j, int *in_quotes)
{
	(*i) = -1;
	(*j) = 0;
	(*in_quotes) = 0;
}

void	set_in_quotes(const char *s, int *j, char c, int in_quotes)
{
	while (s[*j] && (s[*j] != c || in_quotes))
	{
		if (s[*j] == '"' || s[*j] == '\'')
			in_quotes = !in_quotes;
		(*j)++;
	}
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
	set_splits_vars(&i, &j, &in_quotes);
	while (++i < ft_count_wordss(s, c))
	{
		while (s[j] == c && !in_quotes)
			j++;
		k = j;
		set_in_quotes(s, &j, c, in_quotes);
		arr[i] = ft_substr(s, k, j - k);
		if (!arr[i])
			return (ft_free_arrs(arr, i));
	}
	arr[i] = NULL;
	return (arr);
}

void	set_single_quotes(int *i, char *str)
{
	(*i)++;
	while (str[*i] && str[*i] != '\'')
		(*i)++;
	if (str[*i] == '\'')
		(*i)++;
}

char	*set_str(char *str, char **envp)
{
	int		i;
	int		start;
	char	*content;

	i = 0;
	start = 0;
	content = ft_strdup("");
	while (str[i])
	{
		if (str[i] == '\'')
			set_single_quotes(&i, str);
		else if (str[i] == '$')
		{
			content = ft_strjoin_opts(content, \
							ft_substr(str, start, i - start), 3);
			handle_dollar(&i, &content, str, envp);
			start = i;
		}
		else
			i++;
	}
	content = ft_strjoin_opts(content, ft_substr(str, start, i - start), 3);
	free(str);
	return (content);
}

void	initialize_process_data(t_process_data *data, char *str, char ***envp)
{
	str = complement_cmd(str);
	str = set_str(str, *envp);
	data->split = ft_splits(str, '|');
	data->cnt = cnt_cmd(data->split);
	data->pids = malloc(sizeof(pid_t) * data->cnt);
	data->input_fd = 0;
	data->command = (malloc(sizeof(t_redirection *) * data->cnt));
	initialize_commands(data->split, data->cnt, &data->command, envp);
	set_dollar(0, envp);
	free(str);
}

void	handle_builtin(t_process_data *data, char ***envp, int i)
{
	data->pids[i] = -1;
	data->in = dup(0);
	data->out = dup(1);
	end_sig(&data->old);
	if (i > 0)
		dup2(data->input_fd, 0);
	if (i < data->cnt - 1)
		dup2(data->pipe_fd[1], 1);
	if (!open_redirection_files(data->command[i]))
		handle_builtin_command(data->command[i], envp, data->builtin_num);
	if (i > 0)
		close(data->input_fd);
	if (i < data->cnt - 1)
		close(data->pipe_fd[1]);
	data->input_fd = data->pipe_fd[0];
	dup2(data->in, 0);
	dup2(data->out, 1);
}

void	handle_non_builtin(t_process_data *data, char ***envp, int i)
{
	fork_and_execute(data, i, data->input_fd, envp);
	if (i > 0)
		close(data->input_fd);
	if (i < data->cnt - 1)
		close(data->pipe_fd[1]);
	data->input_fd = data->pipe_fd[0];
}

void	process_input(char *str, char ***envp)
{
	static int		exit_code;
	t_process_data	*data;
	int				i;

	data = malloc(sizeof(t_process_data));
	if (!data)
	{
		ft_putstr_fd("minishell: Error: allocation failed", 2);
		exit(EXIT_FAILURE);
	}
	initialize_process_data(data, str, envp);
	i = -1;
	while (++i < data->cnt)
	{
		print(data->command[i]);
		set_dollar(0, envp);
		data->builtin_num = check_builtin_num(data->command[i]);
		create_pipes(i, data->cnt, data->pipe_fd);
		if (data->builtin_num)
			handle_builtin(data, envp, i);
		else
			handle_non_builtin(data, envp, i);
	}
	wait_for_children(data, envp, &exit_code);
	cleanup_resources(data);
	free(data);
}

static void	cleanup(char *str, char **envp)
{
	struct termios	old;
	int i = 0;
	while(envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
	end_sig(&old);
	ft_putstr_fd("exit\n", 2);
	exit(EXIT_SUCCESS);
}

int	main(int argc, char **argv, char *env[])
{
	char			*str;
	char			**envp;
	struct termios	old;
	char			*cwd;

	envp = update_envp(env, 0, ft_strdup("?=0"));
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
			cleanup(str, envp);
		}
		free(cwd);
	}
	return (0);
}
