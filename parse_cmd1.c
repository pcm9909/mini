#include "main.h"

static char	*ft_find_single_redirect(char *str, char c)
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

static void	set_order(t_redir *command, char *str)
{
	char	*rev;
	int		dual;
	int		single;

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

static char	*get_parse_value(char *str, int *i, t_redir *cmd, char **envp)
{
	char	*temp;

	temp = ft_strdup("");
	while (str[*i] && !is_whitespace(str[*i]))
	{
		if (str[*i] && str[*i + 1] && ((str[*i] == '"' && str[*i + 1] == '"') \
			|| (str[*i] == '\'' && str[*i + 1] == '\'')))
		{
			if (is_whitespace(str[*i + 2]) || str[*i + 2] == '\0')
				cmd->cmd->cmd_val = append_command(&cmd->cmd->cmd_val, "");
			(*i) += 2;
		}
		else if (str[*i] == '<' || str[*i] == '>')
			handle_redirection(str, i, cmd, envp);
		else if (str[*i] == '"' || str[*i] == '\'')
			temp = ft_strjoin_opts(temp, handle_quotes(str, i, cmd), 3);
		else if (str[*i] == '$' && str[*i + 1] && !is_whitespace(str[*i + 1]))
			handle_env(i, str, envp, cmd);
		else
			temp = ft_strjoin_opts(temp, handle_command(str, i), 3);
	}
	return (temp);
}

static void	parse_command(char *str, int *i, t_redir *cmd, char **envp)
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
			while (str[*i] && is_whitespace(str[*i]))
				(*i)++;
		}
		free(temp);
	}
}

void	parse_redir(char *str, t_redir *cmd, char **envp)
{
	int	i;

	i = 0;
	while (str[i])
	{
		parse_command(str, &i, cmd, envp);
		while (is_whitespace(str[i]))
			i++;
	}
	set_order(cmd, str);
}
