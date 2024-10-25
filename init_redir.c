/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_redir.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chunpark <chunpark@student.42gyeongsan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 18:09:40 by chunpark          #+#    #+#             */
/*   Updated: 2024/10/25 23:42:47 by chunpark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static t_cmd	*create_command(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		error_exit("minishell: Error: allocation failed");
	cmd->cmd_val = NULL;
	cmd->order = false;
	cmd->exist = false;
	return (cmd);
}

void	initialize_redirection(t_redir **redirection)
{
	*redirection = malloc(sizeof(t_redir));
	(*redirection)->heredoc_redir = create_command();
	(*redirection)->append_redir = create_command();
	(*redirection)->cmd = create_command();
	(*redirection)->input_redir = create_command();
	(*redirection)->output_redir = create_command();
	(*redirection)->executable = true;
	(*redirection)->heredoc = ft_strdup("");
}

void	cleanup(char **envp)
{
	struct termios	old;
	int				i;

	i = 0;
	while (envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
	end_sig(&old);
	ft_putstr_fd("exit\n", 2);
	exit(EXIT_SUCCESS);
}

void	set_readline(char **str, char **cwd,
							struct termios *old, char **envp)
{
	*cwd = build_prompt(envp);
	input_sig(old);
	*str = readline(*cwd);
	none_sig();
}
