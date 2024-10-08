#include "main.h"

char	*ft_find_single_redirect(char *str, char c)
{
	int	i;

	i = 0;
	if (!str)
		return (NULL);
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
				return (&str[i]);
			}
		}
		i++;
	}
	return (NULL);
}

void	set_order(t_redirection *command, char *str)
{
	int		dual;
	int		single;
	char	*rev;

	dual = ft_strlen(ft_strnstr(str, "<<", sizeof(str)));
	single = ft_strlen(ft_find_single_redirect(str, '<'));
	if (dual > single)
		command->double_left_brace->order = true;
	else if (dual < single)
		command->left_brace->order = true;
	rev = ft_strrev(str);
	dual = ft_strlen(ft_strnstr(rev, ">>", ft_strlen(str)));
	single = ft_strlen(ft_find_single_redirect(rev, '>'));
	if (dual > single)
		command->double_right_brace->order = true;
	else if (dual < single)
		command->right_brace->order = true;
	free(rev);
}

char	*set_command(t_command *command)
{
	char	*result;
	int		i;
	int		total_length;

	if (!command || !command->command)
		return (NULL);
	total_length = 0;
	i = -1;
	while (command->command[++i])
		total_length += ft_strlen(command->command[i]) + 1;
	result = malloc(total_length + 1);
	result[0] = '\0';
	i = -1;
	while (command->command[++i])
	{
		ft_strlcat(result, command->command[i], total_length + 1);
		if (command->command[i + 1])
			ft_strlcat(result, " ", total_length + 1);
	}
	return (result);
}

static void	handle_parse_error(const char *str, int *i, t_redirection *cmd)
{
	ft_putstr_fd("minishell: parse error near `", 2);
	ft_putchar_fd(str[*i], 2);
	ft_putstr_fd("'\n", 2);
	cmd->executable = false;
	while (str[*i] && (str[*i] == '>' || str[*i] == '<' || str[*i] == '\0' || \
			str[*i] == '|' || str[*i] == '&' || str[*i] == ';'))
		(*i)++;
}

static char	*extract_content(const char *str, int *i)
{
	int		j;
	char	*content;

	j = *i;
	while (str[*i] && !is_whitespace(str[*i]) && \
				str[*i] != '>' && str[*i] != '<')
		(*i)++;
	content = ft_substr(str, j, *i - j);
	return (content);
}

static int	handle_double_left(t_redirection *cmd, char *content, \
				const char *str, int *i)
{
	int	j;
	int	check;

	cmd->double_left_brace->exist = true;
	cmd->double_left_brace->command = \
			append_command(&cmd->double_left_brace->command, content);
	j = *i;
	check = 0;
	while (str[j])
	{
		if (str[j] && str[j + 1] && str[j] == '<' && str[j + 1] == '<')
		{
			check = 1;
			break ;
		}
		j++;
	}
	return (check);
}

int	is_upright_vars(int c)
{
	return (c == '>' || c == '<' || c == '\0' || \
			c == '|' || c == '&' || c == ';');
}

static void	handle_single_left(t_redirection *cmd, char *content)
{
	cmd->left_brace->exist = true;
	cmd->left_brace->command = \
		append_command(&cmd->left_brace->command, content);
}

void	parse_left_redirection(const char *str, int *i,
							t_redirection *cmd, char **envp)
{
	int		flag;
	char	*content;

	(*i)++;
	if (str[*i] == '<')
	{
		(*i)++;
		flag = 1;
	}
	while (is_whitespace(str[*i]))
		(*i)++;
	if (is_upright_vars(str[*i]))
	{
		handle_parse_error(str, i, cmd);
		return ;
	}
	content = extract_content(str, i);
	if (flag == 1)
	{
		if (!handle_double_left(cmd, content, str, i))
			handle_double_left_brace(cmd, envp);
	}
	else
		handle_single_left(cmd, content);
	free(content);
}

static void	check_flag(t_redirection *command, int flag, char *content)
{
	if (flag == 1)
	{
		command->double_right_brace->exist = true;
		command->double_right_brace->command = \
			append_command(&command->double_right_brace->command, content);
	}
	else
	{
		command->right_brace->exist = true;
		command->right_brace->command = \
			append_command(&command->right_brace->command, content);
	}
}

void	parse_right_redirection(char *str, int *i, \
				char **envp, t_redirection *cmd)
{
	int		j;
	int		flag;
	char	*content;

	flag = 0;
	(*i)++;
	if (str[*i] == '>')
	{
		(*i)++;
		flag = 1;
	}
	while (is_whitespace(str[*i]))
		(*i)++;
	if (is_upright_vars(str[*i]))
	{
		handle_parse_error(str, i, cmd);
	}
	j = *i;
	while (str[*i] && !is_whitespace(str[*i]) && \
			str[*i] != '>' && str[*i] != '<')
		(*i)++;
	content = ft_substr(str, j, *i - j);
	check_flag(cmd, flag, content);
	free(content);
}

void	handle_redirection(char *str, int *i, \
							t_redirection *command, char **envp)
{
	if (str[*i] == '<')
	{
		parse_left_redirection(str, i, command, envp);
	}
	if (str[*i] == '>')
	{
		parse_right_redirection(str, i, envp, command);
	}
	while (str[*i] && is_whitespace(str[*i]))
		(*i)++;
}

char	*handle_quotes(const char *str, int *i, \
				char **envp, t_redirection *command)
{
	char	*temp;

	temp = ft_strdup("");
	if (str[*i] == '"')
	{
		temp = handle_double_quotes(str, i, envp, command);
	}
	else if (str[*i] == '\'')
	{
		temp = handle_single_quotes(str, i, command);
	}
	return (temp);
}

char	*handle_quotes2(const char *str, int *i, \
			char **envp, t_redirection *command)
{
	char	*temp;

	if (str[*i] == '"')
	{
		temp = handle_double_quotes2(str, i, envp, command);
	}
	else if (str[*i] == '\'')
	{
		temp = handle_single_quotes2(str, i, command);
	}
	return (temp);
}

char	*get_parse_value(char *str, int *i, t_redirection *cmd, char **envp)
{
	char	*temp;

	temp = ft_strdup("");
	if (str[*i] && str[*i + 1] && ((str[*i] == '"' && str[*i + 1] == '"') \
							|| (str[*i] == '\'' && str[*i + 1] == '\'')))
	{
		cmd->command->command = append_command(&cmd->command->command, "");
		(*i) += 2;
	}
	else if (str[*i] == '<' || str[*i] == '>')
		handle_redirection(str, i, cmd, envp);
	else if (str[*i] == '"' || str[*i] == '\'')
		temp = ft_strjoin_opts(temp, handle_quotes(str, i, envp, cmd), 3);
	else
		temp = ft_strjoin_opts(temp, handle_command(str, i, envp), 3);
	return (temp);
}

void	parse_command(char *str, int *i, t_redirection *cmd, char **envp)
{
	char	*temp;

	while (str[*i] && is_whitespace(str[*i]))
		(*i)++;
	while (str[*i])
	{
		temp = get_parse_value(str, i, cmd, envp);
		if (str[*i] == 0 || str[*i] == ' ')
			break ;
	}
	if (temp)
	{
		if (ft_strlen(temp))
		{
			cmd->command->exist = true;
			cmd->command->command = \
					append_command(&cmd->command->command, temp);
		}
	}
	free(temp);
}

void	parse_redirection(char *str, t_redirection *command, char **envp)
{
	int	i;

	i = 0;
	while (str[i])
	{
		parse_command(str, &i, command, envp);
		while (is_whitespace(str[i]))
			i++;
	}
	command->full_cmd = set_command(command->command);
	set_order(command, str);
}
