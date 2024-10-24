/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd1.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jakim <jakim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 18:09:45 by chunpark          #+#    #+#             */
/*   Updated: 2024/10/24 16:21:30 by jakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static void	handle_dollar_sign(int *i, char **temp, \
							const char *str, char **envp)
{
	if (str[*i + 1] && str[*i + 1] != '$' && !is_whitespace(str[*i + 1]))
	{
		handle_dollar(i, temp, str, envp);
	}
	else
	{
		while (str[*i] && str[*i] == '$')
		{
			*temp = ft_strjoin_opt(*temp, "$", 1);
			(*i)++;
		}
	}
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
			temp = ft_strjoin_opt(temp, handle_quotes(str, i, cmd, envp), 3);
		else if (str[*i] == '$')
			handle_dollar_sign(i, &temp, str, envp);
		else
			temp = ft_strjoin_opt(temp, handle_command(str, i), 3);
	}
	return (temp);
}

static void	process_temp(char *temp, int *i, t_redir *cmd, char *str)
{
	char	**cmds;
	int		idx;
	int		adx;
	char	*content;

	if (ft_strlen(temp))
	{
		cmds = split_cmp_quotes(temp, ' ');
		idx = -1;
		while (cmds[++idx])
		{
			adx = 0;
			content = get_cmd_value(cmds[idx], &adx);
			cmd->cmd->exist = true;
			if (ft_strlen(content))
				cmd->cmd->cmd_val = append_command(&cmd->cmd->cmd_val, content);
			free(content);
		}
		all_free(cmds);
	}
	while (str[*i] && is_whitespace(str[*i]))
		(*i)++;
}

static void	parse_command(char *str, int *i, t_redir *cmd, char **envp)
{
	char	*temp;

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
			process_temp(temp, i, cmd, str);
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
