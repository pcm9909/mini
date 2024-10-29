/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_right_redir.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chunpark <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 18:09:49 by chunpark          #+#    #+#             */
/*   Updated: 2024/10/21 18:09:51 by chunpark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

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
	char	*content;
	int		flag;

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
