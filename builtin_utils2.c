/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils2.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chunpark <chunpark@student.42gyeongsan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 18:09:29 by chunpark          #+#    #+#             */
/*   Updated: 2024/10/23 21:45:35 by chunpark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void	set_dollar(int ptr, char ***envp)
{
	char	*tmp;
	char	*jo;

	tmp = ft_itoa(ptr);
	jo = ft_strjoin_opt("?=", tmp, 2);
	set_env(jo, 1, envp);
}

char	**update_envp(char **envp, int type, char *new)
{
	char	**ptr;
	int		i;

	i = 0;
	ptr = envp;
	while (*ptr != NULL)
	{
		i++;
		ptr++;
	}
	ptr = (char **)malloc((i + 5) * sizeof(char *));
	i = 0;
	while (envp[i] != NULL)
	{
		ptr[i] = ft_strdup(envp[i]);
		i++;
	}
	ptr[i++] = new;
	ptr[i] = NULL;
	if (type == 1)
		all_free(envp);
	return (ptr);
}

static char	*set_src(char *str)
{
	char	*src;
	char	*equal;

	equal = ft_strchr(str, '=');
	if (equal)
		src = ft_substr(str, 0, equal - str);
	else
		src = ft_strdup(str);
	return (src);
}

static char	*set_target(char *name, int flag)
{
	char	*target;

	if (ft_strchr(name, '=') && flag == 1)
		target = ft_substr(name, 0, ft_strchr(name, '=') - name);
	else
		target = ft_strdup(name);
	return (target);
}

int	search_env(char **envp, char *name, int flag)
{
	char	*re;
	char	*tmp;
	int		i;

	i = 0;
	re = set_target(name, flag);
	while (*envp)
	{
		tmp = set_src(*envp);
		if (!ft_strncmp(tmp, re, ft_strlen(re) + 1))
		{
			free(tmp);
			break ;
		}
		envp++;
		i++;
		free(tmp);
	}
	free(re);
	return (i);
}
