#include "main.h"

void	print_error(char *target)
{
	ft_putstr_fd("minishell: ", 2);
	perror(target);
}

int	sigcheck(int type)
{
	static int	flag;

	if (type == 1)
		flag = 1;
	if (type == 0)
		flag = 0;
	return (flag);
}

char	*check_input(const char *str, char **envp)
{
	int		i;
	int		start;
	char	*val;

	i = 0;
	start = i;
	val = ft_strdup("");
	while (str[i])
		i++;
	val = ft_strjoin_opts(val, ft_substr(str, start, i - start), 3);
	return (val);
}

char	*handle_quotes4(const char *str, \
						t_redir *command, char param, int *idx)
{
	int		start;
	char	*content;

	(*idx)++;
	start = (*idx);
	content = ft_strdup("");
	while (str[(*idx)] && str[(*idx)] != param)
		(*idx)++;
	if (str[(*idx)] != param)
	{
		return (print_qutoes_error(str, command, &content));
	}
	else
	{
		free(content);
		content = ft_substr(str, start, (*idx) - start);
	}
	return (content);
}

char	*handle_quotes3(const char *str, t_redir *cmd, int *j)
{
	if (str[*j] == '"')
		return (handle_quotes4(str, cmd, '"', j));
	else
		return (handle_quotes4(str, cmd, '\'', j));
}

char	*handle_quotes_and_join(char *str,
			t_redir *cmd, int *j, int start)
{
	char	*sub;
	char	*quote_content;
	char	*new_content;

	sub = ft_substr(str, start, *j - start);
	quote_content = handle_quotes3(str, cmd, j);
	new_content = ft_strjoin_opts(sub, quote_content, 3);
	return (new_content);
}

char	*join_remaining_content(char *command, char *content, int start, int j)
{
	char	*sub;
	char	*new_content;

	sub = ft_substr(command, start, j - start);
	new_content = ft_strjoin_opts(content, sub, 1);
	free(sub);
	return (new_content);
}

char	*process_command(char *command, t_redir *cmd)
{
	char			*content;
	char			*sub;
	int				j;
	int				start;
	int				len;

	j = 0;
	len = ft_strlen(command);
	content = ft_strdup("");
	start = 0;
	while (j < len)
	{
		if (command[j] == '"' || command[j] == '\'')
		{
			sub = handle_quotes_and_join(command, cmd, &j, start);
			content = ft_strjoin_opts(content, sub, 3);
			start = ++j;
		}
		else
			j++;
	}
	content = join_remaining_content(command, content, start, j);
	return (content);
}

void	append_until_dollar(char **proc_read, const char *read, int *j)
{
	int		start;
	char	*temp;

	start = *j;
	while (read[*j] && read[*j] != '$')
	{
		(*j)++;
	}
	temp = ft_substr(read, start, *j - start);
	*proc_read = ft_strjoin_opts(*proc_read, temp, 3);
}

void	proc_read_input(char *read, int pipe_fd[2], char **envp)
{
	char	*proc_read;
	int		j;

	add_history(read);
	proc_read = ft_strdup("");
	j = 0;
	while (read[j])
	{
		if (read[j] == '$')
			handle_dollar(&j, &proc_read, read, envp);
		else
			append_until_dollar(&proc_read, read, &j);
	}
	proc_read = ft_strjoin_opts(proc_read, "\n", 1);
	write(pipe_fd[1], proc_read, ft_strlen(proc_read));
	free(proc_read);
	free(read);
}

void	proc_heredoc_child(t_redir *cmd, int i, int pipe_fd[2], char **envp)
{
	struct termios	old;
	char			*read;
	char			*proc;
	int				j;

	close(pipe_fd[0]);
	while (cmd->executable)
	{
		heredoc_sig(&old);
		read = readline(">");
		none_sig(&old);
		if (!read || (ft_strncmp(read, cmd->heredoc_redir->cmd_val[i], \
			ft_strlen(cmd->heredoc_redir->cmd_val[i])) == 0 && \
			ft_strlen(read) == ft_strlen(cmd->heredoc_redir->cmd_val[i])))
		{
			free(read);
			break ;
		}
		if (cmd->heredoc_redir->cmd_val[i + 1] == NULL)
			proc_read_input(read, pipe_fd, envp);
	}
	close(pipe_fd[1]);
	exit(EXIT_SUCCESS);
}

void	proc_heredoc_parent(int pipe_fd[2], t_redir *cmd)
{
	char	buffer[1024];
	ssize_t	bytes_read;
	int		status;

	close(pipe_fd[1]);
	waitpid(-1, &status, 0);
	if (WIFEXITED(status) && WEXITSTATUS(status) == EXIT_SUCCESS)
	{
		bytes_read = read(pipe_fd[0], buffer, sizeof(buffer) - 1);
		while (bytes_read > 0)
		{
			buffer[bytes_read] = '\0';
			add_history(buffer);
			cmd->heredoc = ft_strjoin_opts(cmd->heredoc, buffer, 1);
			bytes_read = read(pipe_fd[0], buffer, sizeof(buffer) - 1);
		}
	}
	else
	{
		cmd->executable = false;
		sigcheck(1);
	}
	close(pipe_fd[0]);
}

void	handle_heredoc(t_redir *cmd, int i, int flag, char **envp)
{
	int		pipe_fd[2];
	pid_t	pid;

	pipe(pipe_fd);
	pid = fork();
	if (pid == 0)
	{
		proc_heredoc_child(cmd, i, pipe_fd, envp);
	}
	else
	{
		proc_heredoc_parent(pipe_fd, cmd);
	}
}

void	handle_heredoc_redir(t_redir *cmd, char **envp)
{
	char	*read;
	int		i;
	int		flag;
	char	*processed_command;

	i = -1;
	flag = 0;
	while (cmd->heredoc_redir->cmd_val && \
			cmd->heredoc_redir->cmd_val[++i])
	{
		processed_command = \
			process_command(cmd->heredoc_redir->cmd_val[i], cmd);
		free(cmd->heredoc_redir->cmd_val[i]);
		cmd->heredoc_redir->cmd_val[i] = ft_strdup(processed_command);
		free(processed_command);
		handle_heredoc(cmd, i, flag, envp);
	}
}

int	handle_left_brace(t_redir *cmd)
{
	int		i;
	int		fd;
	char	*processed_command;

	i = -1;
	while (cmd->input_redir->cmd_val && cmd->input_redir->cmd_val[++i])
	{
		processed_command = process_command(cmd->input_redir->cmd_val[i], cmd);
		free(cmd->input_redir->cmd_val[i]);
		cmd->input_redir->cmd_val[i] = ft_strdup(processed_command);
		free(processed_command);
		fd = open(cmd->input_redir->cmd_val[i], O_RDONLY);
		if (fd == -1)
		{
			print_error(cmd->input_redir->cmd_val[i]);
			return (EXIT_FAILURE);
		}
		if (cmd->input_redir->cmd_val[i + 1] == NULL && cmd->input_redir->order)
		{
			dup2(fd, 0);
			break ;
		}
		close(fd);
	}
	return (EXIT_SUCCESS);
}

int	handle_right_brace(t_redir *cmd)
{
	int		i;
	int		fd;
	char	*processed_command;

	i = 0;
	while (cmd->output_redir->cmd_val && cmd->output_redir->cmd_val[i])
	{
		processed_command = process_command(cmd->output_redir->cmd_val[i], cmd);
		free(cmd->output_redir->cmd_val[i]);
		cmd->output_redir->cmd_val[i] = ft_strdup(processed_command);
		free(processed_command);
		fd = open(cmd->output_redir->cmd_val[i], \
				O_CREAT | O_TRUNC | O_WRONLY, 0644);
		if (fd == -1)
		{
			print_error(cmd->output_redir->cmd_val[i]);
			return (EXIT_FAILURE);
		}
		if (cmd->output_redir->order == true)
			dup2(fd, 1);
		close(fd);
		i++;
	}
	return (EXIT_SUCCESS);
}

int	handle_double_right_brace(t_redir *cmd)
{
	int		i;
	int		fd;
	char	*proc_command;

	i = -1;
	while (cmd->append_redir->cmd_val && \
				cmd->append_redir->cmd_val[++i])
	{
		proc_command = \
			process_command(cmd->append_redir->cmd_val[i], cmd);
		free(cmd->append_redir->cmd_val[i]);
		cmd->append_redir->cmd_val[i] = ft_strdup(proc_command);
		free(proc_command);
		fd = open(cmd->append_redir->cmd_val[i], \
					O_CREAT | O_APPEND | O_WRONLY, 0644);
		if (fd == -1)
		{
			print_error(cmd->heredoc_redir->cmd_val[i]);
			return (EXIT_FAILURE);
		}
		if (cmd->append_redir->order == true)
			dup2(fd, 1);
		close(fd);
	}
	return (EXIT_SUCCESS);
}
