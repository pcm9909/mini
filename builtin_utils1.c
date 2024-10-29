/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chunpark <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 18:09:28 by chunpark          #+#    #+#             */
/*   Updated: 2024/10/21 18:09:29 by chunpark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static int	is_validname(char *ptr)
{
	while (*ptr && *ptr != '=')
	{
		if (!ft_isalnum(*ptr) && *ptr != '_')
			return (0);
		ptr++;
	}
	return (1);
}

int	env_validate(char *ptr)
{
	char	*dup_ptr;
	int		ret;

	dup_ptr = ft_strdup(ptr);
	if ((ft_isalpha(*dup_ptr) || *dup_ptr == '_') && is_validname(dup_ptr))
	{
		if (ft_strchr(dup_ptr, '='))
			ret = 1;
		else
			ret = 0;
	}
	else
		ret = -1;
	free(dup_ptr);
	return (ret);
}

char	*extract_home(char *envp[])
{
	char	*tmp;

	tmp = NULL;
	while (*envp)
	{
		if (!ft_strncmp(*envp, "HOME=", 5))
			break ;
		envp++;
	}
	tmp = (*envp);
	if (tmp)
		return (tmp + 5);
	else
		return (tmp);
}

char	*set_env(char *name, int flag, char ***envp)
{
	int	i;

	if (flag < 0)
		return (name);
	else
	{
		i = search_env(*envp, name, 1);
		if ((*envp)[i] != NULL)
		{
			if (!ft_strchr(name, '='))
				return (NULL);
			free((*envp)[i]);
			(*envp)[i] = name;
		}
		else
			*envp = update_envp(*envp, 1, name);
	}
	return (NULL);
}

void	print_envp(char **envp, int flag)
{
	while (*envp != NULL)
	{
		if (flag == 1)
		{
			if (env_validate(*envp) == 1)
			{
				ft_putstr_fd("declare -x ", 1);
				write(1, *envp, ft_strchr(*envp, '=') - *envp);
				write(1, "=\"", 2);
				ft_putstr_fd(ft_strchr(*envp, '=') + 1, 1);
				write(1, "\"\n", 2);
			}
			else if (env_validate(*envp) == 0)
				printf("declare -x %s\n", *envp);
		}
		else
		{
			if (env_validate(*envp) == 1)
				printf("%s\n", *envp);
		}
		envp++;
	}
}
