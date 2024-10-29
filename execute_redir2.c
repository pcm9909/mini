/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redir2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jakim <jakim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 18:09:37 by chunpark          #+#    #+#             */
/*   Updated: 2024/10/25 01:49:06 by jakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static char	*get_quotes_val(const char *str, \
						t_redir *command, char param, int *idx)
{
	char	*content;
	int		start;

	start = ++(*idx);
	content = ft_strdup("");
	while (str[(*idx)] && str[(*idx)] != param)
	{
		(*idx)++;
	}
	if (str[(*idx)] != param)
	{
		return (print_qutoes_error(command));
	}
	else
	{
		free(content);
		content = ft_substr(str, start, (*idx) - start);
	}
	return (content);
}

static char	*get_quotes(const char *str, t_redir *cmd, int *j)
{
	if (str[*j] == '"')
		return (get_quotes_val(str, cmd, '"', j));
	else
		return (get_quotes_val(str, cmd, '\'', j));
}

static char	*handle_quotes_and_join(char *str,
							t_redir *cmd, int *j, int start)
{
	char	*sub;
	char	*quote_content;
	char	*new_content;

	sub = ft_substr(str, start, *j - start);
	quote_content = get_quotes(str, cmd, j);
	new_content = ft_strjoin_opt(sub, quote_content, 3);
	return (new_content);
}

static char	*join_remaining_content(char *command,
							char *content, int start, int j)
{
	char	*new_content;
	char	*sub;

	sub = ft_substr(command, start, j - start);
	new_content = ft_strjoin_opt(content, sub, 1);
	free(sub);
	return (new_content);
}

char	*process_command(char *command, t_redir *cmd)
{
	char	*content;
	char	*sub;
	int		j;
	int		start;
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
			content = ft_strjoin_opt(content, sub, 3);
			start = ++j;
		}
		else
			j++;
	}
	content = join_remaining_content(command, content, start, j);
	return (content);
}
