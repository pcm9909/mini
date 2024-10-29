/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_redir1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jakim <jakim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 18:09:36 by chunpark          #+#    #+#             */
/*   Updated: 2024/10/25 20:12:11 by jakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static void	handle_heredoc_fork(t_redir *command, int pipe_fd[2])
{
	pid_t	pid;

	pid = fork();
	if (pid == -1)
		perror_exit("fork");
	if (pid == 0)
	{
		close(pipe_fd[0]);
		dup2(pipe_fd[1], STDOUT_FILENO);
		write(pipe_fd[1], command->heredoc, ft_strlen(command->heredoc));
		close(pipe_fd[1]);
		exit(EXIT_SUCCESS);
	}
	else
	{
		close(pipe_fd[1]);
		wait(NULL);
		dup2(pipe_fd[0], STDIN_FILENO);
		close(pipe_fd[0]);
	}
}

static int	handle_append_redir(t_redir *cmd, int i)
{
	char	*proc_cmd;
	int		fd;

	while (cmd->append_redir->cmd_val \
			&& cmd->append_redir->cmd_val[++i])
	{
		proc_cmd = \
			process_command(cmd->append_redir->cmd_val[i], cmd);
		free(cmd->append_redir->cmd_val[i]);
		cmd->append_redir->cmd_val[i] = ft_strdup(proc_cmd);
		free(proc_cmd);
		if (cmd->executable == false)
			break ;
		fd = open(cmd->append_redir->cmd_val[i], \
					O_CREAT | O_APPEND | O_WRONLY, 0644);
		if (fd == -1)
		{
			print_error(cmd->append_redir->cmd_val[i]);
			return (EXIT_FAILURE);
		}
		if (cmd->append_redir->order == true)
			dup2(fd, 1);
		close(fd);
	}
	return (EXIT_SUCCESS);
}

static int	handle_output_redir(t_redir *cmd, int i)
{
	char	*proc_cmd;
	int		fd;

	while (cmd->output_redir->cmd_val && cmd->output_redir->cmd_val[++i])
	{
		proc_cmd = process_command(cmd->output_redir->cmd_val[i], cmd);
		free(cmd->output_redir->cmd_val[i]);
		cmd->output_redir->cmd_val[i] = ft_strdup(proc_cmd);
		free(proc_cmd);
		if (cmd->executable == false)
			break ;
		fd = open(cmd->output_redir->cmd_val[i], \
					O_CREAT | O_TRUNC | O_WRONLY, 0644);
		if (fd == -1)
		{
			print_error(cmd->output_redir->cmd_val[i]);
			return (EXIT_FAILURE);
		}
		if (cmd->output_redir->order == true)
			dup2(fd, 1);
		close(fd);
	}
	return (EXIT_SUCCESS);
}

static int	handle_input_redir(t_redir *cmd, int i)
{
	char	*proc_cmd;
	int		fd;

	while (cmd->input_redir->cmd_val && cmd->input_redir->cmd_val[++i])
	{
		proc_cmd = process_command(cmd->input_redir->cmd_val[i], cmd);
		free(cmd->input_redir->cmd_val[i]);
		cmd->input_redir->cmd_val[i] = ft_strdup(proc_cmd);
		free(proc_cmd);
		if (cmd->executable == false)
			break ;
		fd = open(cmd->input_redir->cmd_val[i], O_RDONLY);
		if (fd == -1)
		{
			print_error(cmd->input_redir->cmd_val[i]);
			return (EXIT_FAILURE);
		}
		if (cmd->input_redir->cmd_val[i + 1] == NULL && cmd->input_redir->order)
		{
			dup2(fd, 0);
			break ;
		}
		close(fd);
	}
	return (EXIT_SUCCESS);
}

int	open_redirection_files(t_redir *command)
{
	int	pipe_fd[2];

	if (handle_input_redir(command, -1)
		|| handle_output_redir(command, -1) || handle_append_redir(command, -1))
		return (EXIT_FAILURE);
	if (command->heredoc_redir->exist)
	{
		if (pipe(pipe_fd) == -1)
			perror_exit("pipe");
		handle_heredoc_fork(command, pipe_fd);
	}
	return (EXIT_SUCCESS);
}
