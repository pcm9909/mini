/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_heredoc1.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chunpark <chunpark@student.42gyeongsan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 18:09:32 by chunpark          #+#    #+#             */
/*   Updated: 2024/10/23 21:45:35 by chunpark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static void	write_heredoc_warning(char *cmd_val)
{
	write(2, "minishell: warning: here-document", 33);
	write(2, " delimited by end-of-file (wanted '", 34);
	write(2, cmd_val, ft_strlen(cmd_val));
	write(2, "')\n", 3);
}

static void	proc_heredoc_child(t_redir *cmd, int i, int pipe_fd[2], char **envp)
{
	struct termios	old;
	char			*read;

	close(pipe_fd[0]);
	while (cmd->executable)
	{
		heredoc_sig(&old);
		read = readline(">");
		none_sig();
		if (!read || (ft_strncmp(read, cmd->heredoc_redir->cmd_val[i], \
				ft_strlen(cmd->heredoc_redir->cmd_val[i])) == 0 && \
				ft_strlen(read) == ft_strlen(cmd->heredoc_redir->cmd_val[i])))
		{
			if (!read)
				write_heredoc_warning(cmd->heredoc_redir->cmd_val[i]);
			free(read);
			break ;
		}
		if (cmd->heredoc_redir->cmd_val[i + 1] == NULL)
			proc_read_input(read, pipe_fd, envp);
	}
	close(pipe_fd[1]);
	exit(EXIT_SUCCESS);
}

static void	proc_heredoc_parent(int pipe_fd[2], t_redir *cmd)
{
	char	buffer[1024];
	ssize_t	bytes_read;
	int		status;

	close(pipe_fd[1]);
	waitpid(-1, &status, 0);
	if (WIFEXITED(status) && WEXITSTATUS(status) == EXIT_SUCCESS)
	{
		bytes_read = read(pipe_fd[0], buffer, sizeof(buffer) - 1);
		while (bytes_read > 0)
		{
			buffer[bytes_read] = '\0';
			add_history(buffer);
			cmd->heredoc = ft_strjoin_opt(cmd->heredoc, buffer, 1);
			bytes_read = read(pipe_fd[0], buffer, sizeof(buffer) - 1);
		}
	}
	else
	{
		cmd->executable = false;
		sigcheck(1);
	}
	close(pipe_fd[0]);
}

static void	handle_heredoc(t_redir *cmd, int i, char **envp)
{
	int		pipe_fd[2];
	pid_t	pid;

	pipe(pipe_fd);
	pid = fork();
	if (pid == 0)
	{
		proc_heredoc_child(cmd, i, pipe_fd, envp);
	}
	else
	{
		proc_heredoc_parent(pipe_fd, cmd);
	}
}

void	handle_heredoc_redir(t_redir *cmd, char **envp)
{
	char	*processed_command;
	int		i;

	i = -1;
	while (cmd->heredoc_redir->cmd_val && \
			cmd->heredoc_redir->cmd_val[++i])
	{
		processed_command = \
			process_command(cmd->heredoc_redir->cmd_val[i], cmd);
		free(cmd->heredoc_redir->cmd_val[i]);
		cmd->heredoc_redir->cmd_val[i] = ft_strdup(processed_command);
		free(processed_command);
		handle_heredoc(cmd, i, envp);
	}
}
