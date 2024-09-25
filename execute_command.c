#include "main.h"

static void	exe(t_redirection *command, char **cmd, char **envp)
{
	char	*cmd_path;
	char	*path;

	cmd_path = NULL;
	path = get_path(envp);
	if (cmd)
		cmd_path = get_cmd_path(cmd[0], path);
	if (ft_strlen(*cmd) == 0)
		exit(0);
	if (execve(cmd_path, cmd, envp))
	{
		if (cmd)
		{
			write(2, cmd[0], ft_strlen(cmd[0]));
			write(2, ": command not found\n", ft_strlen(": command not found\n"));
			exit(127);
		}
	}
	else
		exit(2);
	free(path);
}

void	execute_external_command(t_redirection *command, \
									char **envp, int input_fd, int output_fd)
{
	exe(command, command->command->command, envp);
}

static void	open_redirection_files(t_redirection *command)
{
	handle_left_brace(command);
	handle_right_brace(command);
	handle_double_right_brace(command);
}

void	execute_command(t_redirection *cmd, char ***envp, \
							int input_fd, int output_fd)
{
	if (cmd->executable == true)
	{
		open_redirection_files(cmd);
		if (cmd->full_cmd && cmd->full_cmd[0] == 'c' && \
			cmd->full_cmd[1] == 'd' && (cmd->full_cmd[2] == ' ' || \
			cmd->full_cmd[2] == '\0'))
			handle_cd_command(cmd, *envp);
		else if (!ft_strncmp(cmd->full_cmd, "export ", 7) || \
				!ft_strncmp(cmd->full_cmd, "export", 8))
			handle_export_command(cmd, envp);
		else if (!ft_strncmp(cmd->full_cmd, "env", 5) || \
				!ft_strncmp(cmd->full_cmd, "env ", 4))
			handle_env_command(cmd, *envp);
		else if (!ft_strncmp(cmd->full_cmd, "exit", 5) || \
				!ft_strncmp(cmd->full_cmd, "exit ", 5))
			handle_exit_command(cmd);
		else if (!ft_strncmp(cmd->full_cmd, "unset", 6) || \
				!ft_strncmp(cmd->full_cmd, "unset ", 6))
			handle_unset_command(cmd, *envp);
		else if (!ft_strncmp(cmd->full_cmd, "pwd", 6) || \
				!ft_strncmp(cmd->full_cmd, "pwd ", 4))
			handle_pwd_command();
		else if (!ft_strncmp(cmd->full_cmd, "echo", 6) || \
				!ft_strncmp(cmd->full_cmd, "echo ", 5))
			handle_echo_command(cmd, *envp);
		else
			execute_external_command(cmd, *envp, input_fd, output_fd);
	}
	else
		exit(2);
}

