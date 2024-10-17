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

void	set_order(t_redir *command, char *str)
{
	int		dual;
	int		single;
	char	*rev;

	dual = ft_strlen(ft_strnstr(str, "<<", sizeof(str)));
	single = ft_strlen(ft_find_single_redirect(str, '<'));
	if (dual > single)
		command->heredoc_redir->order = true;
	else if (dual < single)
		command->input_redir->order = true;
	rev = ft_strrev(str);
	dual = ft_strlen(ft_strnstr(rev, ">>", ft_strlen(str)));
	single = ft_strlen(ft_find_single_redirect(rev, '>'));
	if (dual > single)
		command->append_redir->order = true;
	else if (dual < single)
		command->output_redir->order = true;
	free(rev);
}

static void	handle_parse_error(const char *str, int *i, t_redir *cmd)
{
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	ft_putchar_fd(str[*i], 2);
	ft_putstr_fd("'\n", 2);
	cmd->executable = false;
	while (str[*i] && (str[*i] == '>' || str[*i] == '<' || str[*i] == '\0' || \
			str[*i] == '|' || str[*i] == '&' || str[*i] == ';'))
		(*i)++;
}

int	handle_dollar1(int *i, char **content, const char *str, char **envp)
{
	char	*temp;
	char	*envp_var;
	char	*envp_val;
	int		idx;
	int		start;

	(*i)++;
	start = (*i);
	while (str[*i] && !is_whitespace(str[*i]))
		(*i)++;
	envp_var = ft_substr(str, start, (*i) - start);
	idx = ft_strlen(envp_var) + 1;
	envp_val = ft_strdup(envp[search_env(envp, envp_var, 1)]);
	if (envp_val)
		(*content) = ft_strjoin_opts((*content), &envp_val[idx], 1);
	if (ft_strlen(envp_val) == 0)
	{
		printf("minishell: $%s: ambiguous redirect\n", envp_var);
		free(envp_var);
		free(envp_val);
		return (1);
	}
	free(envp_var);
	free(envp_val);
	return (0);
}

static char	*extract_content(const char *str, int *i, char **envp, t_redir *cmd)
{
	int		j;
	char	*content;
	char	*temp;

	j = *i;
	content = ft_strdup("");
	while (str[*i] && !is_whitespace(str[*i]) && \
				str[*i] != '>' && str[*i] != '<' && str[*i] != '$')
		(*i)++;
	if (str[*i] == '$')
	{
		temp = ft_substr(str, j, *i - j);
		content = ft_strjoin_opts(content, temp, 3);
		if (handle_dollar1(i, &content, str, envp))
			cmd->executable = false;
		j = *i;
	}
	temp = ft_substr(str, j, *i - j);
	content = ft_strjoin_opts(content, temp, 3);
	return (content);
}

static char	*extract_heredoc_content(const char *str, int *i)
{
	char	*content;
	int		j;

	j = *i;
	while (str[*i] && !is_whitespace(str[*i]) \
			&& str[*i] != '>' && str[*i] != '<')
		(*i)++;
	content = ft_substr(str, j, *i - j);
	return (content);
}

static int	handle_double_left(t_redir *cmd, char *content, \
				const char *str, int *i)
{
	int	j;
	int	check;

	cmd->heredoc_redir->exist = true;
	cmd->heredoc_redir->cmd_val = \
			append_command(&cmd->heredoc_redir->cmd_val, content);
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

static void	handle_single_left(t_redir *cmd, char *content)
{
	cmd->input_redir->exist = true;
	cmd->input_redir->cmd_val = \
		append_command(&cmd->input_redir->cmd_val, content);
}

void	parse_left_redir(const char *str, int *i,
							t_redir *cmd, char **envp)
{
	int		flag;
	char	*content;

	(*i)++;
	flag = 0;
	if (str[*i] == '<')
	{
		(*i)++;
		flag = 1;
	}
	while (is_whitespace(str[*i]))
		(*i)++;
	if (is_upright_vars(str[*i]))
		handle_parse_error(str, i, cmd);
	if (flag == 0)
		content = extract_content(str, i, envp, cmd);
	if (flag == 1)
	{
		content = extract_heredoc_content(str, i);
		if (!handle_double_left(cmd, content, str, i))
			handle_heredoc_redir(cmd, envp);
	}
	else
		handle_single_left(cmd, content);
	free(content);
}

static void	check_flag(t_redir *command, int flag, char *content)
{
	if (flag == 1)
	{
		command->append_redir->exist = true;
		command->append_redir->cmd_val = \
			append_command(&command->append_redir->cmd_val, content);
	}
	else
	{
		command->output_redir->exist = true;
		command->output_redir->cmd_val = \
			append_command(&command->output_redir->cmd_val, content);
	}
}

void	parse_right_redir(char *str, int *i, \
				char **envp, t_redir *cmd)
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
	content = extract_content(str, i, envp, cmd);
	check_flag(cmd, flag, content);
	free(content);
}

void	handle_redirection(char *str, int *i, \
							t_redir *command, char **envp)
{
	if (str[*i] == '<')
	{
		parse_left_redir(str, i, command, envp);
	}
	if (str[*i] == '>')
	{
		parse_right_redir(str, i, envp, command);
	}
	while (str[*i] && is_whitespace(str[*i]))
		(*i)++;
}

char	*handle_quotes(const char *str, int *i, \
				char **envp, t_redir *command)
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

char *get_parse_value(char *str, int *i, t_redir *cmd, char **envp)
{
	char *temp = ft_strdup("");
	while (str[*i] && !is_whitespace(str[*i]))
	{
		if (str[*i] && str[*i + 1] && ((str[*i] == '"' && str[*i + 1] == '"') || \
			(str[*i] == '\'' && str[*i + 1] == '\'')))
		{
			if (is_whitespace(str[*i + 2]) || str[*i + 2] == '\0')
			{
				cmd->cmd->cmd_val = append_command(&cmd->cmd->cmd_val, "");
				(*i) += 2;
			}
			else
				(*i)+=2;
		}
		else if (str[*i] == '<' || str[*i] == '>')
			handle_redirection(str, i, cmd, envp);
		else if (str[*i] == '"' || str[*i] == '\'')
			temp = ft_strjoin_opts(temp, handle_quotes(str, i, envp, cmd), 3);
		else
			temp = ft_strjoin_opts(temp, handle_command(str, i, envp), 3);
	}
	return temp;
}

void	parse_command(char *str, int *i, t_redir *cmd, char **envp)
{
	char	*temp;

	while (str[*i] && is_whitespace(str[*i]))
		(*i)++;
	while (str[*i])
	{
		temp = get_parse_value(str, i, cmd, envp);
		if (temp)
		{
			if (ft_strlen(temp))
			{
				cmd->cmd->exist = true;
				cmd->cmd->cmd_val = \
						append_command(&cmd->cmd->cmd_val, temp);
			}
			free(temp);
			while (str[*i] && is_whitespace(str[*i]))
				(*i)++;
		}
		else
			free(temp);
	}
}

void	parse_redir(char *str, t_redir *cmd, char **envp)
{
	int		i;

	i = 0;
	while (str[i])
	{
		parse_command(str, &i, cmd, envp);
		while (is_whitespace(str[i]))
			i++;
	}
	set_order(cmd, str);
}
