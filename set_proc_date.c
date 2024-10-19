#include "main.h"

static void	initialize_commands(char **split, int cnt, \
							t_redir ***command, char ***envp)
{
	int	i;

	i = 0;
	while (i < cnt)
	{
		initialize_redirection(&(*command)[i]);
		i++;
	}
	i = 0;
	while (i < cnt)
	{
		parse_redir(split[i], (*command)[i], *envp);
		if (sigcheck(3))
			return ;
		i++;
	}
}

static char	*complement_cmd(char *str)
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
			len--;
		if (is_whitespace(str[len]) && len == 0)
			return (str);
		if (str[len] == '|')
		{
			tmp = ft_strjoin_opts(str, readline(">"), 3);
			add_history(tmp);
			str = tmp;
		}
		else
			return (str);
		return (complement_cmd(tmp));
	}
	return (NULL);
}

static int	cnt_cmd(char **split)
{
	int	i;

	i = 0;
	if (!split)
	{
		return (0);
	}
	while (split[i])
	{
		i++;
	}
	return (i);
}

void	set_process_data(t_proc_data *data, char *str, char ***envp)
{
	str = complement_cmd(str);
	str = set_str(str, *envp);
	data->split = split_cmp_quotes(str, '|');
	data->cnt = cnt_cmd(data->split);
	data->pids = malloc(sizeof(pid_t) * data->cnt);
	data->input_fd = 0;
	data->command = (malloc(sizeof(t_redir *) * data->cnt));
	initialize_commands(data->split, data->cnt, &data->command, envp);
	free(str);
}
