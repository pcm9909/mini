#include "main.h"

void	print_error(char *target)
{
	ft_putstr_fd("minishell: ", 2);
	perror(target);
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
						t_redirection *command, char param, int *idx)
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

char	*handle_quotes3(const char *str, t_redirection *command, int *j)
{
	if (str[*j] == '"')
		return (handle_quotes4(str, command, '"', j));
	else
		return (handle_quotes4(str, command, '\'', j));
}

char	*handle_quotes_and_join(char *command,
			t_redirection *cmd, int *j, int start)
{
	char	*sub;
	char	*quote_content;
	char	*new_content;

	sub = ft_substr(command, start, *j - start);
	quote_content = handle_quotes3(command, cmd, j);
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

char	*process_command(char *command, t_redirection *cmd)
{
	int		j;
	int		start;
	char	*content;
	struct termios	old;
	char	*sub;
	int		len;

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

void	handle_readline(t_redirection *cmd, int i, int flag, char **envp)
{
	char	*read;

	while (cmd->executable)
	{
		read = readline(">");
		if (!read || (ft_strncmp(read, cmd->double_left_brace->command[i], \
			ft_strlen(cmd->double_left_brace->command[i])) == 0 && \
			ft_strlen(read) == ft_strlen(cmd->double_left_brace->command[i])))
		{
			free(read);
			break ;
		}
		if (cmd->double_left_brace->command[i + 1] == NULL)
		{
			add_history(read);
			if (flag)
				read = ft_strjoin_opts(read, "\n", 1);
			else
				read = ft_strjoin_opts(check_input(read, envp), "\n", 1);
			cmd->here_doc = ft_strjoin_opts(cmd->here_doc, read, 3);
		}
	}
}

void	handle_double_left_brace(t_redirection *cmd, char **envp)
{
	char	*read;
	int		i;
	int		flag;
	char	*processed_command;

	i = -1;
	flag = 0;
	while (cmd->double_left_brace->command && \
			cmd->double_left_brace->command[++i])
	{
		processed_command = \
			process_command(cmd->double_left_brace->command[i], cmd);
		free(cmd->double_left_brace->command[i]);
		cmd->double_left_brace->command[i] = ft_strdup(processed_command);
		free(processed_command);
		handle_readline(cmd, i, flag, envp);
	}
}

int	handle_left_brace(t_redirection *cmd)
{
	int		i;
	int		fd;
	char	*processed_command;

	i = -1;
	while (cmd->left_brace->command && cmd->left_brace->command[++i])
	{
		processed_command = process_command(cmd->left_brace->command[i], cmd);
		free(cmd->left_brace->command[i]);
		cmd->left_brace->command[i] = ft_strdup(processed_command);
		free(processed_command);
		fd = open(cmd->left_brace->command[i], O_RDONLY);
		if (fd == -1)
		{
			print_error(cmd->left_brace->command[i]);
			return (EXIT_FAILURE);
		}
		if (cmd->left_brace->command[i + 1] == NULL && cmd->left_brace->order)
		{
			dup2(fd, 0);
			break ;
		}
		close(fd);
	}
	return (EXIT_SUCCESS);
}

int	handle_right_brace(t_redirection *cmd)
{
	int		i;
	int		fd;
	char	*processed_command;

	i = 0;
	while (cmd->right_brace->command && cmd->right_brace->command[i])
	{
		processed_command = process_command(cmd->right_brace->command[i], cmd);
		free(cmd->right_brace->command[i]);
		cmd->right_brace->command[i] = ft_strdup(processed_command);
		free(processed_command);
		fd = open(cmd->right_brace->command[i], \
				O_CREAT | O_TRUNC | O_WRONLY, 0644);
		if (fd == -1)
		{
			print_error(cmd->right_brace->command[i]);
			return (EXIT_FAILURE);
		}
		if (cmd->right_brace->order == true)
			dup2(fd, 1);
		close(fd);
		i++;
	}
	return (EXIT_SUCCESS);
}

int	handle_double_right_brace(t_redirection *cmd)
{
	int		i;
	int		fd;
	char	*proc_command;

	i = -1;
	while (cmd->double_right_brace->command && \
				cmd->double_right_brace->command[++i])
	{
		proc_command = \
			process_command(cmd->double_right_brace->command[i], cmd);
		free(cmd->double_right_brace->command[i]);
		cmd->double_right_brace->command[i] = ft_strdup(proc_command);
		free(proc_command);
		fd = open(cmd->double_right_brace->command[i], \
					O_CREAT | O_APPEND | O_WRONLY, 0644);
		if (fd == -1)
		{
			print_error(cmd->double_left_brace->command[i]);
			return (EXIT_FAILURE);
		}
		if (cmd->double_right_brace->order == true)
			dup2(fd, 1);
		close(fd);
	}
	return (EXIT_SUCCESS);
}
