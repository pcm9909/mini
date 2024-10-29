/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preset_execute.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chunpark <chunpark@student.42gyeongsan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 18:09:54 by chunpark          #+#    #+#             */
/*   Updated: 2024/10/25 23:39:59 by chunpark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

int	set_exit_val(int statloc, int cnt, char ***envp, int i)
{
	int	exit_val;

	exit_val = 0;
	if (WIFEXITED(statloc))
	{
		if (i == cnt - 1)
			exit_val = WEXITSTATUS(statloc);
		set_dollar(WEXITSTATUS(statloc), envp);
	}
	else if (WIFSIGNALED(statloc))
	{
		if (i == cnt - 1)
			exit_val = 128 + WTERMSIG(statloc);
		set_dollar(128 + WTERMSIG(statloc), envp);
		if (WTERMSIG(statloc) == 2)
			printf("\n");
		else if (WTERMSIG(statloc) == 3)
			ft_putstr_fd("Quit (core dumped)\n", 2);
	}
	return (exit_val);
}

int	wait_for_children(t_proc_data *data, char ***envp)
{
	int	statloc;
	int	ex_code;
	int	i;

	i = data->cnt;
	ex_code = 0;
	while (--i >= 0)
	{
		if (data->pids[i] == -1)
			continue ;
		waitpid(data->pids[i], &statloc, 0);
		if (i == data->cnt -1)
			ex_code = set_exit_val(statloc, data->cnt, envp, i);
		close(data->pipe_fd[i][1]);
		close(data->pipe_fd[i][0]);
	}
	return (ex_code);
}

void	create_pipes(int i, int cnt, int **pipe_fd)
{
	while (i <= cnt)
	{
		pipe(pipe_fd[i]);
		i++;
	}
}
