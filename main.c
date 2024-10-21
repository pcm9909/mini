/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chunpark <chunpark@student.42gyeongsan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 18:09:44 by chunpark          #+#    #+#             */
/*   Updated: 2024/10/21 22:13:31 by chunpark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static void	set_local(int *i, t_proc_data **data)
{
	*i = -1;
	*data = malloc(sizeof(t_proc_data));
	if (!*data)
		error_exit("minishell: Error: allocation failed");
}

void print_cmd(t_cmd *cmd, const char *label)
{
    int i = 0;
    if (cmd && cmd->cmd_val)
    {
        while (cmd->cmd_val[i])
        {
            printf("[%s]\n", label);
            printf("%s\n", cmd->cmd_val[i]);
            i++;
        }
    }
}

void print(t_redir *cmd)
{
    print_cmd(cmd->heredoc_redir, "heredoc_redir");
    print_cmd(cmd->input_redir, "input_redir");
    print_cmd(cmd->cmd, "cmd");
    print_cmd(cmd->output_redir, "output_redir");
    print_cmd(cmd->append_redir, "append_redir");
}

static char	*process_input(char *str, char ***envp)
{
	t_proc_data	*data;
	char		*tmp;
	int			i;

	set_local(&i, &data);
	tmp = set_process_data(data, str, envp);
	while (++i < data->cnt)
	{
		print(data->command[i]);
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
	return (tmp);
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
