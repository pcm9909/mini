#include "main.h"

void	set_dollar(int ptr, char ***envp)
{
	char	*tmp;
	char	*jo;

	tmp = ft_itoa(ptr);
	jo = ft_strjoin_opts("?=", tmp, 2);
	set_env(jo, 1, envp);
}

t_cmd	*create_command(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		error_exit("minishell: Error: allocation failed");
	cmd->cmd_val = NULL;
	cmd->order = false;
	cmd->exist = false;
	return (cmd);
}

void	initialize_redirection(t_redir **redirection)
{
	*redirection = malloc(sizeof(t_redir));
	(*redirection)->heredoc_redir = create_command();
	(*redirection)->append_redir = create_command();
	(*redirection)->cmd = create_command();
	(*redirection)->input_redir = create_command();
	(*redirection)->output_redir = create_command();
	(*redirection)->executable = true;
	(*redirection)->heredoc = ft_strdup("");
}

char	**allocate_and_copy(char **cmd, int size)
{
	char	**new_cmd;
	int		j;

	j = 0;
	new_cmd = malloc(sizeof(char *) * size);
	if (!new_cmd)
		error_exit("minishell: Error: allocation failed");
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

void	wait_for_children(t_proc_data *data, char ***envp, int *exit_code)
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

void	cleanup_resources(t_proc_data *data)
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

void	fork_and_execute(t_proc_data *data, \
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
						t_redir ***command, char ***envp)
{
	int	i;

	i = 0;
	while (i < cnt)
	{
		initialize_redirection(&(*command)[i]);
		i++;
	}
	i = 0;
	while (i < cnt)
	{
		parse_redir(split[i], (*command)[i], *envp);
		if (sigcheck(3))
			return ;
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

void	set_single_quotes(const char *str, char **temp, int *i)
{
	char	*content;
	int		start;

	start = (*i)++;
	while (str[*i] && str[*i] != '\'')
		(*i)++;
	if (str[*i] == '\'')
		(*i)++;
	content = ft_substr(str, start, *i - start);
	*temp = ft_strjoin_opts(*temp, content, 3);
}

void	set_double_quotes(const char *str, char **temp, int *i, char **envp)
{
	char	*content;
	int		start;

	start = *i;
	(*i)++;
	while (str[*i] && str[*i] != '"')
	{
		if (str[*i] == '$' && str[*i + 1] && \
			(ft_isalpha(str[*i + 1]) || str[*i + 1] == '?'))
		{
			*temp = ft_strjoin_opts(*temp, \
				ft_substr(str, start, *i - start), 3);
			handle_dollar(i, temp, str, envp);
			start = *i;
		}
		else
			(*i)++;
	}
	if (str[*i] == '"')
		(*i)++;
	*temp = ft_strjoin_opts(*temp, ft_substr(str, start, *i - start), 3);
}

void	set_heredoc_redir(int *i, char **tmp, char *str, int *start)
{
	while (is_whitespace(str[*i]))
		(*i)++;
	while (!is_whitespace(str[*i]) && str[*i])
		(*i)++;
	*tmp = ft_strjoin_opts(*tmp, ft_substr(str, *start, *i - *start), 3);
	*start = *i;
}

void	set_quotes_and_dollar(const char *str, int *i, char **temp, char **envp)
{
	if (str[*i] == '\'')
	{
		set_single_quotes(str, temp, i);
	}
	else if (str[*i] == '"')
	{
		set_double_quotes(str, temp, i, envp);
	}
	else if (str[*i] == '$')
	{
		if (str[*i + 1] && (ft_isalpha(str[*i + 1]) || str[*i + 1] == '?'))
		{
			handle_dollar(i, temp, str, envp);
		}
		else
		{
			(*i)++;
			*temp = ft_strjoin_opts((*temp), "$", 1);
		}
	}
}

void	set_left_redir(int *i, char **tmp, char *str, int *start)
{
	(*i)++;
	if (str[*i] == '<')
	{
		(*i)++;
		set_heredoc_redir(i, tmp, str, start);
	}
	while (str[*i] && is_whitespace(str[*i]))
		(*i)++;
	if (str[*i] == '$')
	{
		while (str[*i] && !is_whitespace(str[*i]))
			(*i)++;
	}
	else
		return ;
}

void	set_right_redir(int *i, char **tmp, char *str, int *start)
{
	(*i)++;
	if (str[*i] == '>')
		(*i)++;
	while (str[*i] && is_whitespace(str[*i]))
		(*i)++;
	if (str[*i] == '$')
	{
		while (str[*i] && !is_whitespace(str[*i]))
			(*i)++;
	}
	else
		return ;
}

void	set_redir(int *i, char **tmp, char *str, int *start)
{
	if (str[*i] == '<')
	{
		set_left_redir(i, tmp, str, start);
	}
	else if (str[*i] == '>')
	{
		set_right_redir(i, tmp, str, start);
	}
}

void	set_str_vars(int *i, int *si, char **tmp)
{
	*i = 0;
	*si = 0;
	*tmp = ft_strdup("");
}

char	*set_str(char *str, char **envp)
{
	char	*tmp;
	int		i;
	int		si;

	set_str_vars(&i, &si, &tmp);
	while (str[i])
	{
		if (str[i] == '\'' || str[i] == '"' || str[i] == '$')
		{
			tmp = ft_strjoin_opts(tmp, ft_substr(str, si, i - si), 3);
			set_quotes_and_dollar(str, &i, &tmp, envp);
			si = i;
		}
		else if (str[i] == '<' || str[i] == '>')
			set_redir(&i, &tmp, str, &si);
		else
			i++;
	}
	tmp = ft_strjoin_opts(tmp, ft_substr(str, si, i - si), 3);
	return (tmp);
}

void	initialize_process_data(t_proc_data *data, char *str, char ***envp)
{
	str = complement_cmd(str);
	str = set_str(str, *envp);
	data->split = ft_splits(str, '|');
	data->cnt = cnt_cmd(data->split);
	data->pids = malloc(sizeof(pid_t) * data->cnt);
	data->input_fd = 0;
	data->command = (malloc(sizeof(t_redir *) * data->cnt));
	initialize_commands(data->split, data->cnt, &data->command, envp);
	free(str);
}

void	handle_builtin(t_proc_data *data, char ***envp, int i)
{
	data->pids[i] = -1;
	data->in = dup(0);
	data->out = dup(1);
	end_sig(&data->old);
	if (i > 0)
		dup2(data->input_fd, 0);
	if (i < data->cnt - 1)
		dup2(data->pipe_fd[1], 1);
	if (!open_redirection_files(data->command[i], *envp) && \
		data->command[i]->executable)
		handle_builtin_command(data->command[i], envp, data->builtin_num);
	if (i > 0)
		close(data->input_fd);
	if (i < data->cnt - 1)
		close(data->pipe_fd[1]);
	data->input_fd = data->pipe_fd[0];
	dup2(data->in, 0);
	dup2(data->out, 1);
}

void	handle_non_builtin(t_proc_data *data, char ***envp, int i)
{
	fork_and_execute(data, i, data->input_fd, envp);
	if (i > 0)
		close(data->input_fd);
	if (i < data->cnt - 1)
		close(data->pipe_fd[1]);
	data->input_fd = data->pipe_fd[0];
}

void	set_local(int *i, t_proc_data **data)
{
	*i = -1;
	*data = malloc(sizeof(t_proc_data));
	if (!*data)
		error_exit("minishell: Error: allocation failed");
}

void	process_input(char *str, char ***envp)
{
	t_proc_data	*data;
	static int	exit_code;
	int			i;

	set_local(&i, &data);
	initialize_process_data(data, str, envp);
	while (++i < data->cnt)
	{
		if (sigcheck(3))
			break ;
		if (data->command[i]->executable == false)
			set_dollar(1, envp);
		else
			set_dollar(0, envp);
		if (data->command[i]->cmd->exist == true)
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
	int				i;

	i = 0;
	while (envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
	end_sig(&old);
	ft_putstr_fd("exit\n", 2);
	exit(EXIT_SUCCESS);
}

void	set_readline(char **str, char **cwd, struct termios *old, char **envp)
{
	*cwd = build_prompt(envp);
	input_sig(old);
	*str = readline(*cwd);
	none_sig(old);
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
		set_readline(&str, &cwd, &old, envp);
		if (ft_strlen(str))
			add_history(str);
		if (str)
		{
			sigcheck(0);
			process_input(str, &envp);
		}
		else
		{
			cleanup(str, envp);
		}
		free(str);
		free(cwd);
	}
	return (0);
}
