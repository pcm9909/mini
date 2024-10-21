/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_unset.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chunpark <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 18:09:26 by chunpark          #+#    #+#             */
/*   Updated: 2024/10/21 18:09:27 by chunpark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static void	ft_unset(char **ptr, char **envp)
{
	int	i;
	int	k;

	i = 1;
	while (ptr[i] != NULL)
	{
		k = search_env(envp, ptr[i], 0);
		if (envp[k])
		{
			free(envp[k]);
			envp[k] = ft_strdup("");
		}
		i++;
	}
}

void	handle_unset_command(t_redir *command, char **envp)
{
	char	**cd;

	cd = ft_strdups(command->cmd->cmd_val);
	ft_unset(cd, envp);
	all_free(cd);
}
