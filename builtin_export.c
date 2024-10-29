/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_export.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chunpark <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 18:09:21 by chunpark          #+#    #+#             */
/*   Updated: 2024/10/21 18:09:23 by chunpark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static int	ft_export(char **ptr, char ***envp)
{
	int		i;
	int		flag;
	char	*rax;
	char	*tmp;

	i = 1;
	flag = 0;
	while (ptr[i] != NULL)
	{
		tmp = ft_strdup(ptr[i]);
		rax = set_env(tmp, env_validate(ptr[i]), envp);
		if (flag == 0 && rax != NULL)
		{
			ft_putstr_fd("minishell: export: `", 2);
			ft_putstr_fd(rax, 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			flag = 1;
		}
		if (rax != NULL)
			free(tmp);
		i++;
	}
	return (flag);
}

void	handle_export_command(t_redir *command, char ***envp)
{
	char	**cd;

	cd = ft_strdups(command->cmd->cmd_val);
	if (cd[1] == NULL)
	{
		print_envp(*envp, 1);
	}
	else
		set_dollar(ft_export(cd, envp), envp);
	all_free(cd);
}
