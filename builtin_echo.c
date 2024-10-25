/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jakim <jakim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 18:09:11 by chunpark          #+#    #+#             */
/*   Updated: 2024/10/25 21:14:14 by jakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static void	ft_echo(char **ptr)
{
	int	flag;
	int	i;

	i = 1;
	flag = 0;
	if (ptr[i] && !ft_strncmp(ptr[i], "-n", 2))
	{
		flag = 1;
		i++;
	}
	while (ptr[i])
	{
		printf("%s", ptr[i]);
		if (ptr[i + 1])
			printf(" ");
		i++;
	}
	if (flag == 0)
		printf("\n");
}

void	handle_echo_command(t_redir *command)
{
	char	**cd;

	cd = ft_strdups(command->cmd->cmd_val);
	ft_echo(cd);
	all_free(cd);
}
