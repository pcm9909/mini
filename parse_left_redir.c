/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_left_redir.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chunpark <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 18:09:48 by chunpark          #+#    #+#             */
/*   Updated: 2024/10/21 18:09:49 by chunpark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static void	handle_single_left(t_redir *cmd, char *content)
{
	cmd->input_redir->exist = true;
	cmd->input_redir->cmd_val = \
		append_command(&cmd->input_redir->cmd_val, content);
}

static int	handle_double_left(t_redir *cmd, char *content,
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

static char	*extract_heredoc_content(const char *str, int *i)
{
	char	*content;
	int		j;

	j = *i;
	while (str[*i] && !is_whitespace(str[*i]) && \
			str[*i] != '>' && str[*i] != '<')
		(*i)++;
	content = ft_substr(str, j, *i - j);
	return (content);
}

void	parse_left_redir(const char *str, int *i,
						t_redir *cmd, char **envp)
{
	char	*content;
	int		flag;

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
