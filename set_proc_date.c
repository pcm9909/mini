/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_proc_date.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chunpark <chunpark@student.42gyeongsan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 18:09:58 by chunpark          #+#    #+#             */
/*   Updated: 2024/10/25 23:43:22 by chunpark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
			if (!append_str(&tmp, str))
				return (NULL);
			return (complement_cmd(tmp));
		}
		else
			return (str);
	}
	free(str);
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

static void	set_vars(int *flag, int *i, int *num, int *fd)
{
	*flag = 0;
	*i = -1;
	*num = 0;
	*fd = 0;
}

char	*set_process_data(t_proc_data *data, char *str, char ***envp)
{
	char	*tmp;
	int		flag;
	int		i;

	set_vars(&flag, &i, &data->builtin_num, &data->input_fd);
	tmp = complement_cmd(ft_strdup(str));
	if (tmp != str)
		flag = 1;
	str = set_str(tmp);
	data->split = split_cmp_quotes(str, '|');
	data->cnt = cnt_cmd(data->split);
	data->pids = malloc(sizeof(pid_t) * data->cnt);
	data->command = (malloc(sizeof(t_redir *) * data->cnt));
	data->pipe_fd = (int **)malloc(sizeof(int *) * (data->cnt + 2));
	while (++i <= data->cnt + 1)
	{
		data->pipe_fd[i] = (int *)malloc(sizeof(int) * 2);
		data->pipe_fd[i][0] = -1;
		data->pipe_fd[i][1] = -1;
	}
	initialize_commands(data->split, data->cnt, &data->command, envp);
	if (flag == 1)
		free(tmp);
	return (str);
}
