/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chunpark <chunpark@student.42gyeongsan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 18:09:44 by chunpark          #+#    #+#             */
/*   Updated: 2024/10/25 23:42:14 by chunpark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static int	process_commands(t_proc_data *data, char ***envp)
{
	int	i;

	i = data->cnt;
	while (--i >= 0)
	{
		if (sigcheck(3))
			break ;
		if (data->command[i]->executable == false)
			set_dollar(1, envp);
		else
			set_dollar(0, envp);
		if (data->command[i]->cmd->exist == true)
			data->builtin_num = check_builtin_num(data->command[i]);
		if (data->builtin_num)
		{
			if (data->cnt > 1)
				handle_builtin_fork(data, envp, i);
			else
				handle_builtin(data, envp, i);
		}
		else
			handle_non_builtin(data, envp, i);
	}
	return (wait_for_children(data, envp));
}

void	proc_child(t_proc_data *data, char ***envp)
{
	int	status;

	create_pipes(0, data->cnt, data->pipe_fd);
	status = process_commands(data, envp);
	exit(status);
}

void	proc_parent(pid_t pid, char ***envp)
{
	int	status;

	waitpid(pid, &status, 0);
	if (WIFEXITED(status))
	{
		status = WEXITSTATUS(status);
		set_dollar(status, envp);
	}
}

static char	*process_input(char *str, char ***envp)
{
	t_proc_data		*data;
	char			*tmp;
	pid_t			pid;

	data = malloc(sizeof(t_proc_data));
	if (!data)
		error_exit("minishell: Error: allocation failed");
	tmp = set_process_data(data, str, envp);
	if (data->cnt > 1)
	{
		pid = fork();
		if (pid == 0)
			proc_child(data, envp);
		else if (pid > 0)
			proc_parent(pid, envp);
		else
			error_exit("minishell: Error: fork failed\n");
	}
	else
		process_commands(data, envp);
	cleanup_resources(data);
	return (tmp);
}

int	main(int argc, char **argv, char *env[])
{
	struct termios	old;
	char			*str;
	char			**envp;
	char			*cwd;
	char			*tmp;

	if (argc != 1 || argv[1] != NULL)
		error_exit("minishell : too many arguments\n");
	envp = update_envp(env, 0, ft_strdup("?=0"));
	while (1)
	{
		tmp = NULL;
		set_readline(&str, &cwd, &old, envp);
		if (ft_strlen(str))
			add_history(str);
		if (str)
		{
			sigcheck(0);
			tmp = process_input(str, &envp);
		}
		else
			cleanup(envp);
		free_readline(cwd, tmp, str);
	}
}
