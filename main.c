/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jakim <jakim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 14:56:39 by chunpark          #+#    #+#             */
/*   Updated: 2024/09/12 20:30:33 by jakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

t_command	*create_command(void)
{
	t_command	*cmd;

	cmd = malloc(sizeof(t_command));
	if (!cmd)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	cmd->command = NULL;
	cmd->order = false;
	return (cmd);
}

void	initialize_redirection(t_redirection **redirection)
{
	*redirection = malloc(sizeof(t_redirection));
	if (!*redirection)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	(*redirection)->double_left_brace = create_command();
	(*redirection)->double_right_brace = create_command();
	(*redirection)->command = create_command();
	(*redirection)->left_brace = create_command();
	(*redirection)->right_brace = create_command();
}

char	**allocate_and_copy(char **cmd, int size)
{
	char	**new_cmd;

	new_cmd = malloc(sizeof(char *) * size);
	if (!new_cmd)
	{
		perror("malloc");
		return (NULL);
	}
	for (int j = 0; j < size - 1; j++)
	{
		if (cmd[j] == NULL)
		{
			new_cmd[j] = NULL;
		}
		else
		{
			new_cmd[j] = ft_strdup(cmd[j]);
			if (!new_cmd[j])
			{
				perror("ft_strdup");
				for (int k = 0; k < j; k++)
				{
					free(new_cmd[k]);
				}
				free(new_cmd);
				return (NULL);
			}
		}
	}
	return (new_cmd);
}

char	**append_command(char ***cmd, const char *str)
{
	int		i;
	char	**new_cmd;

	i = 0;
	if (*cmd == NULL)
	{
		new_cmd = malloc(sizeof(char *) * 2);
		if (!new_cmd)
		{
			perror("malloc");
			return (NULL);
		}
		new_cmd[0] = ft_strdup(str);
		new_cmd[1] = NULL;
		return (new_cmd);
	}
	while ((*cmd)[i])
		i++;
	new_cmd = allocate_and_copy(*cmd, i + 2);
	if (!new_cmd)
		return (NULL);
	new_cmd[i] = ft_strdup(str);
	new_cmd[i + 1] = NULL;
	for (int j = 0; j < i; j++)
	{
		free((*cmd)[j]);
	}
	free(*cmd);
	return (new_cmd);
}

char	*complement_cmd(char *str)
{
	char	*tmp;
	int		len;

	if (str)
	{
		len = ft_strlen(str);
		if (len == 0)
			return (str);
		len--;
		while (is_whitespace(str[len]) > 0 && len > 0)
		{
			len--;
		}
		if (is_whitespace(str[len]) && len == 0)
			return (str);
		if (str[len] == '|')
		{
			tmp = ft_strjoin_with_free(str, readline(">"));
			add_history(tmp);
		}
		else
			return (str);
		return (complement_cmd(tmp));
	}
	return (NULL);
}

void	wait_for_children(int cnt, pid_t *pids, t_redirection **command, char ***envp, int *exit_code)
{
	int		statloc;
	int		i;
	char	**cd;

	i = 0;
	while (i < cnt)
	{
		waitpid(pids[i], &statloc, 0);
		if (WIFEXITED(statloc))
			*exit_code = WEXITSTATUS(statloc);
		if (WIFSIGNALED(statloc))
		{
			*exit_code = 128 + WTERMSIG(statloc);
			if (WTERMSIG(statloc) == 2)
				printf("\n");
			else if (WTERMSIG(statloc) == 3)
				printf("Quit (core dumped)\n");
		}
		if (!ft_strncmp(command[i]->full_cmd, "export ", 7) || !ft_strncmp(command[i]->full_cmd, "export", 8))
		{
			cd = ft_split(command[i]->full_cmd, ' ');
			if (cd[1] != NULL)
				*exit_code = ft_export(cd, envp);
		}
		i++;
	}
}

void	cleanup_resources(int cnt, char **split, t_redirection **command, char *str, pid_t *pids)
{
	int	i;

	i = 0;
	while (i < cnt)
	{
		free_redirection(command[i]);
		i++;
	}
	i = 0;
	while (split[i])
	{
		free(split[i]);
		i++;
	}
	free(command);
	free(split);
	free(str);
	free(pids);
}

void	create_pipes(int i, int cnt, int pipe_fd[2])
{
	if (i < cnt - 1)
	{
		if (pipe(pipe_fd) == -1)
		{
			perror("pipe\n");
			exit(EXIT_FAILURE);
		}
	}
	else
	{
		pipe_fd[0] = 0;
		pipe_fd[1] = 1;
	}
}

void	fork_and_execute(int i, int cnt, int input_fd, int pipe_fd[2], pid_t *pids, t_redirection **command, char ***envp, struct termios *old)
{
	pids[i] = fork();
	if (pids[i] == -1)
	{
		perror("fork\n");
		exit(EXIT_FAILURE);
	}
	if (pids[i] == 0)
	{
		end_sig(old);
		if (i > 0)
		{
			dup2(input_fd, 0);
			close(input_fd);
		}
		if (i < cnt - 1)
		{
			dup2(pipe_fd[1], 1);
			close(pipe_fd[1]);
		}
		execute_command(command[i], envp, input_fd, pipe_fd[1]);
		exit(EXIT_SUCCESS);
	}
	else
	{
		if (i > 0)
		{
			close(input_fd);
		}
		if (i < cnt - 1)
		{
			close(pipe_fd[1]);
		}
		input_fd = pipe_fd[0];
	}
}

void	initialize_commands(char **split, int cnt, t_redirection ***command, char ***envp)
{
	int	i;

	i = 0;
	while (i < cnt)
	{
		initialize_redirection(&(*command)[i]);
		parse_redirection(split[i], (*command)[i], *envp);
		i++;
	}
}

void	process_input(char *str, char ***envp)
{
	static int		exit_code;
	char			**split;
	int				cnt;
	t_redirection	**command;
	int				input_fd;
	int				pipe_fd[2];
	pid_t			*pids;
	struct termios	old;
	int				i;

	i = 0;
	str = complement_cmd(str);
	split = ft_split(str, '|');
	cnt = cnt_cmd(split);
	pids = malloc(sizeof(pid_t) * cnt);
	input_fd = 0;
	command = (malloc(sizeof(t_redirection *) * cnt));
	initialize_commands(split, cnt, &command, envp);
	while (i < cnt)
	{
		create_pipes(i, cnt, pipe_fd);
		fork_and_execute(i, cnt, input_fd, pipe_fd, pids, command, envp, &old);
		if (i > 0)
		{
			close(input_fd);
		}
		if (i < cnt - 1)
		{
			close(pipe_fd[1]);
		}
		input_fd = pipe_fd[0];
		i++;
	}
	wait_for_children(cnt, pids, command, envp, &exit_code);
	cleanup_resources(cnt, split, command, str, pids);
	printf("exit_code : %d\n", exit_code);
}

void cleanup(char *str, char *cwd)
{
	struct termios	old;

	end_sig(&old);
    free(str);
	free(cwd);
    printf("exit\n");
    exit(EXIT_SUCCESS);
}

int	main(int argc, char **argv, char *env[])
{
	char			*str;
	char			**envp;
	struct termios	old;
	char			*cwd;

	envp = initialize_environment(env);
	while (1)
	{
		cwd = build_prompt(envp);
		input_sig(&old);
		str = readline(cwd);
		none_sig(&old);
		if (ft_strlen(str))
			add_history(str);
		if (str)
		{
			process_input(str, &envp);
		}
		else
		{
			cleanup(str, cwd);
		}
		free(cwd);
	}
	return (0);
}
