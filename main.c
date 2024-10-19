#include "main.h"

static void	set_local(int *i, t_proc_data **data)
{
	*i = -1;
	*data = malloc(sizeof(t_proc_data));
	if (!*data)
		error_exit("minishell: Error: allocation failed");
}

static void	process_input(char *str, char ***envp)
{
	t_proc_data	*data;
	int			i;

	set_local(&i, &data);
	set_process_data(data, str, envp);
	while (++i < data->cnt)
	{
		if (sigcheck(3))
			break ;
		if (data->command[i]->executable == false)
			set_dollar(1, envp);
		else
			set_dollar(0, envp);
		if (data->command[i]->cmd->exist == true)
			data->builtin_num = check_builtin_num(data->command[i]);
		create_pipes(i, data->cnt, data->pipe_fd);
		if (data->builtin_num)
			handle_builtin(data, envp, i);
		else
			handle_non_builtin(data, envp, i);
	}
	wait_for_children(data, envp);
	cleanup_resources(data);
	free(data);
}

static void	cleanup(char **envp)
{
	struct termios	old;
	int				i;

	i = 0;
	while (envp[i])
	{
		free(envp[i]);
		i++;
	}
	free(envp);
	end_sig(&old);
	ft_putstr_fd("exit\n", 2);
	exit(EXIT_SUCCESS);
}

static void	set_readline(char **str, char **cwd, \
							struct termios *old, char **envp)
{
	*cwd = build_prompt(envp);
	input_sig(old);
	*str = readline(*cwd);
	none_sig();
}

int	main(int argc, char **argv, char *env[])
{
	char			*str;
	char			**envp;
	struct termios	old;
	char			*cwd;

	if (argc == 999)
		argv[0] = NULL; // Werror 방지용
	envp = update_envp(env, 0, ft_strdup("?=0"));
	while (1)
	{
		set_readline(&str, &cwd, &old, envp);
		if (ft_strlen(str))
			add_history(str);
		if (str)
		{
			sigcheck(0);
			process_input(str, &envp);
		}
		else
		{
			cleanup(envp);
		}
		free(str);
		free(cwd);
	}
	return (0);
}
