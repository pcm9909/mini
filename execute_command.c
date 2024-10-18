#include "main.h"

static void	check_executable(char *path)
{
	struct stat	st;

	if (lstat(path, &st) < 0)
	{
		return ;
	}
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

static void	exe(t_redir *command, char **cmd, char **envp)
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
			write(2, "minishell: ", ft_strlen("minishell: " ));
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

void	execute_external_command(t_redir *command, \
									char **envp, int input_fd, int output_fd)
{
	exe(command, command->cmd->cmd_val, envp);
}

void	handle_double_left_brace_fork(t_redir *command, int pipe_fd[2])
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

int	open_redirection_files(t_redir *command, char **envp)
{
	int	pipe_fd[2];

	if (handle_input_redir(command, envp, -1) \
		|| handle_output_redir(command, envp, -1) \
		|| handle_append_redir(command, envp, -1))
		return (EXIT_FAILURE);
	if (command->heredoc_redir->exist)
	{
		if (pipe(pipe_fd) == -1)
			perror_exit("pipe");
		handle_double_left_brace_fork(command, pipe_fd);
	}
	return (EXIT_SUCCESS);
}

int	check_builtin_num(t_redir *cmd)
{
	if (cmd->cmd->cmd_val == NULL)
		return (0);
	if (cmd->cmd && cmd->cmd->cmd_val[0][0] == 'c' && \
		cmd->cmd->cmd_val[0][1] == 'd' && (cmd->cmd->cmd_val[0][2] == ' ' || \
		cmd->cmd->cmd_val[0][2] == '\0'))
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

void	handle_builtin_command(t_redir *cmd, \
								char ***envp, int builtin_num)
{
	if ((builtin_num) == 1)
		handle_cd_command(cmd, envp);
	else if ((builtin_num) == 2)
		handle_export_command(cmd, envp);
	else if ((builtin_num) == 3)
		handle_env_command(cmd, *envp);
	else if ((builtin_num) == 4)
		handle_exit_command(cmd, envp);
	else if ((builtin_num) == 5)
		handle_unset_command(cmd, *envp);
	else if ((builtin_num) == 6)
		handle_pwd_command();
	else if ((builtin_num) == 7)
		handle_echo_command(cmd, *envp);
}

void	execute_command(t_redir *cmd, char ***envp, \
						int input_fd, int output_fd)
{
	int	builtin_num;

	builtin_num = check_builtin_num(cmd);
	if (cmd->executable == true)
	{
		if (open_redirection_files(cmd, *envp))
			exit(2);
		execute_external_command(cmd, *envp, input_fd, output_fd);
	}
	else
		exit(2);
}
