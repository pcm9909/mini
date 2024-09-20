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

char	*set_command(t_command *command)
{
	char	*result;
	int		i;
	int		total_length;

	if (!command || !command->command)
		return (NULL);
	total_length = 0;
	i = 0;
	while (command->command[i])
	{
		total_length += ft_strlen(command->command[i]) + 1;
		i++;
	}
	result = malloc(total_length + 1);
	if (!result)
	{
		perror("malloc");
		return (NULL);
	}
	result[0] = '\0';
	i = 0;
	while (command->command[i])
	{
		ft_strlcat(result, command->command[i], total_length + 1);
		if (command->command[i + 1])
			ft_strlcat(result, " ", total_length + 1);
		i++;
	}
	return (result);
}

void	parse_left_redirection(const char *str, int *i, t_redirection *command)
{
	int		j;
	int		flag;
	char	*content;

	flag = 0;
	(*i)++;
	if (str[*i] == '<')
	{
		(*i)++;
		flag = 1;
	}
	while (is_whitespace(str[*i]))
		(*i)++;
	j = *i;
	if (str[*i] == '>' || str[*i] == '<' || str[*i] == '\0'
		|| str[*i] == '|' || str[*i] == '&' || str[*i] == ';')
	{
		printf("minishell: parse error near `%c'\n", str[*i]);
		command->executable = false;
		while (str[*i] == '>' || str[*i] == '<' || str[*i] == '\0' || \
				str[*i] == '|' || str[*i] == '&' || str[*i] == ';')
			(*i)++;
	}
	while (str[*i] && !is_whitespace(str[*i]) && \
		str[*i] != '>' && str[*i] != '<')
		(*i)++;
	content = ft_substr(str, j, *i - j);
	if (flag == 1)
	{
		command->double_left_brace->exist = true;
		command->double_left_brace->command = \
			append_command(&command->double_left_brace->command, content);
	}
	else
	{
		command->left_brace->exist = true;
		command->left_brace->command = \
			append_command(&command->left_brace->command, content);
	}
	free(content);
}

void	parse_right_redirection(char *str, int *i, t_redirection *command)
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
	if (str[*i] == '>' || str[*i] == '<' || str[*i] == '\0' || \
		str[*i] == '|' || str[*i] == '&' || str[*i] == ';')
	{
		printf("minishell: parse error near `%c'\n", str[*i]);
		command->executable = false;
		while (str[*i] == '>' || str[*i] == '<' || str[*i] == '\0' || \
			str[*i] == '|' || str[*i] == '&' || str[*i] == ';')
			(*i)++;
	}
	j = *i;
	while (str[*i] && !is_whitespace(str[*i]) && \
			str[*i] != '>' && str[*i] != '<')
		(*i)++;
	content = ft_substr(str, j, *i - j);
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
	free(content);
}

void	parse_command(char *str, int *i, t_redirection *command, char **envp)
{
	int		j;
	char	*content;

	content = ft_strdup("");
	while (is_whitespace(str[*i]))
		(*i)++;
	if (str[*i] == '"')
		content = handle_double_quotes(str, i, envp, command);
	else if (str[*i] == '\'')
		content = handle_single_quotes(str, i, command);
	else if (str[*i] == '<')
		parse_left_redirection(str, i, command);
	else if (str[*i] == '>')
		parse_right_redirection(str, i, command);
	content = ft_strjoin_with_free(content, handle_command(str, i, envp));
	if (ft_strlen(content))
		command->command->command = \
			append_command(&command->command->command, content);
	free(content);
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
