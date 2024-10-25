/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_non_builtin.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chunpark <chunpark@student.42gyeongsan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 18:09:35 by chunpark          #+#    #+#             */
/*   Updated: 2024/10/25 23:25:21 by chunpark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static void	fork_and_execute(t_proc_data *data,
					int i, char ***envp)
{
	data->pids[i] = fork();
	if (data->pids[i] == 0)
	{
		end_sig(&data->old);
		handle_pipe_set(i, data);
		if (open_redirection_files(data->command[i]))
			exit(1);
		execute_command(data->command[i], envp);
		exit(EXIT_SUCCESS);
	}
	else
	{
		close(data->pipe_fd[i + 1][0]);
		close(data->pipe_fd[i + 1][1]);
	}
}

void	handle_non_builtin(t_proc_data *data, char ***envp, int i)
{
	fork_and_execute(data, i, envp);
}

void	execute_command(t_redir *cmd, char ***envp)
{
	if (cmd->executable == true)
	{
		execute_external_command(cmd, cmd->cmd->cmd_val, *envp);
	}
	else
		exit(2);
}

void	check_executable(char *path)
{
	struct stat	st;

	if (lstat(path, &st) < 0)
		return ;
	if (S_ISDIR(st.st_mode))
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": Is a directory\n", 2);
		exit(126);
	}
	if ((S_IXUSR & st.st_mode) != S_IXUSR)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(path, 2);
		ft_putstr_fd(": Permission denied\n", 2);
		exit(126);
	}
}

void	execute_external_command(t_redir *command, char **cmd, char **envp)
{
	char	*cmd_path;
	char	*path;

	cmd_path = NULL;
	path = get_path(envp);
	if (cmd)
		cmd_path = get_cmd_path(cmd[0], path);
	if (!command->cmd->cmd_val)
		exit(0);
	if (execve(cmd_path, cmd, envp))
	{
		if (cmd_path != NULL)
			check_executable(cmd[0]);
		if (cmd)
		{
			write(2, "minishell: ", ft_strlen("minishell: "));
			write(2, cmd[0], ft_strlen(cmd[0]));
			write(2, ": command not found\n", \
					ft_strlen(": command not found\n"));
			exit(127);
		}
	}
	else
		exit(2);
	free(path);
}
