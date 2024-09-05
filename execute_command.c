#include "main.h"

void execute_external_command(t_redirection *command, char **envp, int input_fd, int output_fd);
static void open_redirection_files(t_redirection *command);

void execute_command(t_redirection *command, char ***envp, int input_fd, int output_fd)
{
	open_redirection_files(command);
    if (command->full_cmd && command->full_cmd[0] == 'c' && command->full_cmd[1] == 'd' && (command->full_cmd[2] == ' ' || command->full_cmd[2] == '\0'))
    {
        handle_cd_command(command, *envp);
    }
    else if (!ft_strncmp(command->full_cmd, "export ", 7) || !ft_strncmp(command->full_cmd, "export", 8))
    {
        handle_export_command(command, envp);
    }
    else if (!ft_strncmp(command->full_cmd, "env", 5) || !ft_strncmp(command->full_cmd, "env ", 4))
    {
        handle_env_command(command, *envp);
    }
    else if (!ft_strncmp(command->full_cmd, "exit", 5) || !ft_strncmp(command->full_cmd, "exit ", 5))
    {
        handle_exit_command(command);
    }
    else if (!ft_strncmp(command->full_cmd, "unset", 6) || !ft_strncmp(command->full_cmd, "unset ", 6))
    {
        handle_unset_command(command, *envp);
    }
    else if (!ft_strncmp(command->full_cmd, "pwd", 6) || !ft_strncmp(command->full_cmd, "pwd ", 4))
    {
        handle_pwd_command();
    }
    else if (!ft_strncmp(command->full_cmd, "echo", 6) || !ft_strncmp(command->full_cmd, "echo ", 5))
    {
        handle_echo_command(command, *envp);
    }
    else
    {
        execute_external_command(command, *envp, input_fd, output_fd);
    }
}

static void open_redirection_files(t_redirection *command)
{
    handle_double_left_brace(command);
    handle_left_brace(command);
    handle_right_brace(command);
    handle_double_right_brace(command);
}

static void exe(t_redirection *command, char **cmd, char **envp)
{
    char *cmd_path = NULL;
    char *path = get_path(envp);
    if (cmd)
    {
        cmd_path = get_cmd_path(cmd[0], path);
    }
    if (execve(cmd_path, cmd, envp))
    {
        if (cmd)
        {
			write(2, cmd[0], ft_strlen(cmd[0]));
			write(2, ": command not found\n", ft_strlen(": command not found\n"));
		}
    }
    free(path);
}

void execute_external_command(t_redirection *command, char **envp, int input_fd, int output_fd)
{
    /*pid_t pid = fork();
    if (pid == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }*/

    //if (pid == 0)
    //{
        // Child process
        // if (input_fd != 0)
        // {
        //     dup2(input_fd, 0);
        //     close(input_fd);
        // }
        // if (output_fd != 1)
        // {
        //     dup2(output_fd, 1);
        //     close(output_fd);
        // }
		open_redirection_files(command);
        exe(command, command->command->command, envp);
    /*}
    else
    {
        waitpid(pid, NULL, 0);
    }*/
}