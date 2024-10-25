/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preset_execute.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jakim <jakim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 18:09:54 by chunpark          #+#    #+#             */
/*   Updated: 2024/10/25 19:25:15 by jakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

int	wait_for_children(t_proc_data *data, char ***envp)
{
	int	statloc;
	int	i;
	int exit_status;

	i = data->cnt;
	while (--i >= 0)
	{
		if (data->pids[i] == -1)
			continue ;
		waitpid(data->pids[i], &statloc, 0);
		close(data->pipe_fd[i][1]);
		close(data->pipe_fd[i][0]);
		if (WIFEXITED(statloc))
		{
			exit_status = WEXITSTATUS(statloc);
			set_dollar(WEXITSTATUS(statloc), envp);
		}
		if (WIFSIGNALED(statloc))
		{
			exit_status = 128 + WTERMSIG(statloc);
			set_dollar(128 + WTERMSIG(statloc), envp);
			if (WTERMSIG(statloc) == 2)
				printf("\n");
			else if (WTERMSIG(statloc) == 3)
				ft_putstr_fd("Quit (core dumped)\n", 2);
		}
	}
	return (exit_status);
}

void	create_pipes(int i, int cnt, int **pipe_fd)
{
	while (i <= cnt)
	{
		pipe(pipe_fd[i]);
		i++;
	}
}
