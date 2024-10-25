/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chunpark <chunpark@student.42gyeongsan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 18:09:31 by chunpark          #+#    #+#             */
/*   Updated: 2024/10/25 23:45:03 by chunpark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

int	check_builtin_num(t_redir *cmd)
{
	if (cmd->cmd->cmd_val == NULL)
		return (0);
	if (cmd->cmd && cmd->cmd->cmd_val[0][0] == 'c' &&
		cmd->cmd->cmd_val[0][1] == 'd' && (cmd->cmd->cmd_val[0][2] == ' '
		|| cmd->cmd->cmd_val[0][2] == '\0'))
		return (1);
	if (!ft_strncmp(cmd->cmd->cmd_val[0], "export ", 7) || \
		!ft_strncmp(cmd->cmd->cmd_val[0], "export", 8))
		return (2);
	if (!ft_strncmp(cmd->cmd->cmd_val[0], "env", 5) || \
		!ft_strncmp(cmd->cmd->cmd_val[0], "env ", 4))
		return (3);
	if (!ft_strncmp(cmd->cmd->cmd_val[0], "exit", 5) || \
		!ft_strncmp(cmd->cmd->cmd_val[0], "exit ", 5))
		return (4);
	if (!ft_strncmp(cmd->cmd->cmd_val[0], "unset", 6) || \
		!ft_strncmp(cmd->cmd->cmd_val[0], "unset ", 6))
		return (5);
	if (!ft_strncmp(cmd->cmd->cmd_val[0], "pwd", 6) || \
		!ft_strncmp(cmd->cmd->cmd_val[0], "pwd ", 4))
		return (6);
	if (!ft_strncmp(cmd->cmd->cmd_val[0], "echo", 6) || \
		!ft_strncmp(cmd->cmd->cmd_val[0], "echo ", 5))
		return (7);
	return (0);
}

void	handle_builtin_command(t_redir *cmd, char ***envp, \
									int builtin_num, int cnt)
{
	if ((builtin_num) == 1)
		handle_cd_command(cmd, envp);
	else if ((builtin_num) == 2)
		handle_export_command(cmd, envp);
	else if ((builtin_num) == 3)
		handle_env_command(*envp);
	else if ((builtin_num) == 4)
		handle_exit_command(cmd, envp, cnt);
	else if ((builtin_num) == 5)
		handle_unset_command(cmd, *envp);
	else if ((builtin_num) == 6)
		handle_pwd_command();
	else if ((builtin_num) == 7)
		handle_echo_command(cmd);
}

void	handle_builtin(t_proc_data *data, char ***envp, int i)
{
	int	flag;

	flag = 0;
	data->pids[i] = -1;
	data->in = dup(0);
	data->out = dup(1);
	end_sig(&data->old);
	close(data->pipe_fd[i][0]);
	close(data->pipe_fd[i][1]);
	if (i < data->cnt - 1)
	{
		dup2(data->pipe_fd[i + 1][1], 1);
	}
	if (!open_redirection_files(data->command[i])
		&& data->command[i]->executable)
		flag = 1;
	if (flag)
		handle_builtin_command(data->command[i], envp, \
								data->builtin_num, data->cnt);
	dup2(data->in, 0);
	dup2(data->out, 1);
	close(data->pipe_fd[i + 1][0]);
	close(data->pipe_fd[i + 1][1]);
}

void	handle_pipe_set(int i, t_proc_data *data)
{
	if (i > 0)
		dup2(data->pipe_fd[i][0], 0);
	else
		close(data->pipe_fd[i][0]);
	close(data->pipe_fd[i][1]);
	if (i < data->cnt - 1)
		dup2(data->pipe_fd[i + 1][1], 1);
	else
		close(data->pipe_fd[i + 1][1]);
	close(data->pipe_fd[i + 1][0]);
}

void	handle_builtin_fork(t_proc_data *data, char ***envp, int i)
{
	data->pids[i] = fork();
	if (data->pids[i] == 0)
	{
		handle_pipe_set(i, data);
		if (open_redirection_files(data->command[i]))
			exit(1);
		handle_builtin_command(data->command[i], envp, \
								data->builtin_num, data->cnt);
		exit(EXIT_SUCCESS);
	}
	else
	{
		close(data->pipe_fd[i + 1][0]);
		close(data->pipe_fd[i + 1][1]);
	}
}
