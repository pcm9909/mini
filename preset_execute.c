/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preset_execute.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chunpark <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 18:09:54 by chunpark          #+#    #+#             */
/*   Updated: 2024/10/21 18:09:54 by chunpark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void	wait_for_children(t_proc_data *data, char ***envp)
{
	int	statloc;
	int	i;

	i = -1;
	while (++i < data->cnt)
	{
		if (data->pids[i] == -1)
			continue ;
		waitpid(data->pids[i], &statloc, 0);
		if (WIFEXITED(statloc))
		{
			set_dollar(WEXITSTATUS(statloc), envp);
		}
		if (WIFSIGNALED(statloc))
		{
			set_dollar(128 + WTERMSIG(statloc), envp);
			if (WTERMSIG(statloc) == 2)
				printf("\n");
			else if (WTERMSIG(statloc) == 3)
				ft_putstr_fd("Quit (core dumped)\n", 2);
		}
	}
}

void	create_pipes(int i, int cnt, int pipe_fd[2])
{
	if (i < cnt - 1)
	{
		pipe(pipe_fd);
	}
	else
	{
		pipe_fd[0] = 0;
		pipe_fd[1] = 1;
	}
}
