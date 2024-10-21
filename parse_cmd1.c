/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chunpark <chunpark@student.42gyeongsan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 18:09:45 by chunpark          #+#    #+#             */
/*   Updated: 2024/10/21 21:08:22 by chunpark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
		if (str[*i] == '<' || str[*i] == '>')
			handle_redirection(str, i, cmd, envp);
		else if (str[*i] == '"' || str[*i] == '\'')
			temp = ft_strjoin_opts(temp, handle_quotes(str, i, cmd, envp), 3);
		else if (str[*i] == '$' && str[*i + 1] && !is_whitespace(str[*i + 1]))
			temp = ft_strjoin(temp, handle_env(i, str, envp));
		else if (str[*i] == '$' && ((str[*i + 1] && is_whitespace(str[*i + 1])) || str[*i + 1] == '\0'))
		{
			temp = ft_strjoin_opts(temp, "$", 1);
			(*i)++;
		}
		else
			temp = ft_strjoin_opts(temp, handle_command(str, i), 3);
	}
	return (temp);
}

char	*handle_single_quotes2(const char *str, int *i)
{
	char	*content;
	int		start;

	start = ++(*i);
	while (str[*i] && str[*i] != '\'')
		(*i)++;
	content = ft_substr(str, start, *i - start);
	(*i)++;
	return (content);
}

char	*handle_double_quotes2(const char *str, int *i)
{
	char	*content;
	int		start;

	start = ++(*i);
	while (str[*i] && str[*i] != '"')
	{
		(*i)++;
	}
		content = ft_strdup("");
		content = ft_strjoin_opts(content, \
									ft_substr(str, start, (*i) - start), 3);
	(*i)++;
	return (content);
}

char	*handle_quotes2(const char *str, int *i)
{
	char	*temp;

	if (str[*i] == '"')
	{
		temp = handle_double_quotes2(str, i);
	}
	else
	{
		temp = handle_single_quotes2(str, i);
	}
	return (temp);
}

char	*handle_command2(const char *str, int *i)
{
	char	*val;
	int		start;

	start = (*i);
	val = ft_strdup("");
	while (str[*i] && str[*i] != '\'' && str[*i] != '"')
	{
		if (str[*i] != ' ')
			(*i)++;
		else
			break ;
	}
	val = ft_strjoin_opts(val, ft_substr(str, start, (*i) - start), 3);
	return (val);
}

static char	*get_parse_value2(char *str, int *i)
{
	char	*temp;

	temp = ft_strdup("");
	while (str[*i])
	{
		if (str[*i] == '"' || str[*i] == '\'')
			temp = ft_strjoin_opts(temp, handle_quotes2(str, i), 3);
		else
			temp = ft_strjoin_opts(temp, handle_command2(str, i), 3);
	}
	return (temp);
}



static void	parse_command(char *str, int *i, t_redir *cmd, char **envp)
{
	char	*temp;
	char	**cmds;
	int 	idx;
	int		adx;
	char	*content;

	while (str[*i] && is_whitespace(str[*i]))
		(*i)++;
	while (str[*i])
	{
		if (str[*i] && str[*i + 1] && ((str[*i] == '"' && str[*i + 1] == '"') \
			|| (str[*i] == '\'' && str[*i + 1] == '\'')))
		{
			if (is_whitespace(str[*i + 2]) || str[*i + 2] == '\0')
				cmd->cmd->cmd_val = append_command(&cmd->cmd->cmd_val, "");
			(*i) += 2;
		}
		temp = get_parse_value(str, i, cmd, envp);
		if (temp)
		{
			if (ft_strlen(temp))
			{
				cmds = split_cmp_quotes(temp, ' ');
				idx = -1;
				while (cmds[++idx])
				{
					printf("before = %s\n", cmds[idx]);
					adx = 0;
					content = get_parse_value2(cmds[idx], &adx);
					printf("after = %s\n", content);
					cmd->cmd->exist = true;
					cmd->cmd->cmd_val = \
						append_command(&cmd->cmd->cmd_val, content);
					free(content);
				}
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
