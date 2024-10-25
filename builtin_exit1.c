/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jakim <jakim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 18:09:18 by chunpark          #+#    #+#             */
/*   Updated: 2024/10/25 19:28:47 by jakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static int	only_digit(char *ptr)
{
	if (*ptr == '-' || *ptr == '+')
		ptr++;
	while (*ptr)
	{
		if (!ft_isdigit(*ptr))
			return (0);
		ptr++;
	}
	return (1);
}

static void	ft_exit(char **ptr, char ***envp, int cnt)
{
	if (cnt == 1)
		ft_putstr_fd("exit\n", 2);
	if (ptr[1] == NULL)
		exit(0);
	else if (!only_digit(ptr[1]) || exit_range(ptr[1]))
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(ptr[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		exit(2);
	}
	else if (ptr[2] != NULL)
	{
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
		set_dollar(1, envp);
	}
	else
		exit((unsigned char)ft_atoi(ptr[1]));
}

void	handle_exit_command(t_redir *command, char ***envp, int cnt)
{
	char	**cd;

	cd = ft_strdups(command->cmd->cmd_val);
	ft_exit(cd, envp, cnt);
	all_free(cd);
}
