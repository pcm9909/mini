/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_prompt.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chunpark <chunpark@student.42gyeongsan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 18:09:59 by chunpark          #+#    #+#             */
/*   Updated: 2024/10/23 21:45:35 by chunpark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static char	*extract_name(char *envp[])
{
	char	*tmp;

	tmp = NULL;
	while (*envp)
	{
		if (!ft_strncmp(*envp, "USER=", 5))
			break ;
		envp++;
	}
	tmp = (*envp);
	if (tmp)
		return (tmp + 5);
	else
		return (tmp);
}

static char	*extract_location(char *envp[])
{
	char	*tmp;

	tmp = NULL;
	while (*envp)
	{
		if (!ft_strncmp(*envp, "SESSION_MANAGER=", 16))
			break ;
		envp++;
	}
	tmp = (*envp);
	if (tmp)
		return (ft_substr(tmp, 22, ft_strchr(tmp, '.') - tmp - 22));
	else
		return (NULL);
}

char	*build_prompt(char **envp)
{
	static char	*front;
	char		*pwd;
	char		*cwd;

	pwd = getcwd(NULL, BUFSIZ);
	if (extract_home(envp) && !ft_strncmp(pwd, extract_home(envp), \
		ft_strlen(extract_home(envp))))
	{
		cwd = pwd + ft_strlen(extract_home(envp));
		cwd = ft_strjoin("~", cwd);
		free(pwd);
	}
	else
	{
		cwd = pwd;
	}
	if (!front)
	{
		front = ft_strjoin_opt(extract_name(envp), "@", 0);
		front = ft_strjoin_opt(front, extract_location(envp), 3);
		front = ft_strjoin_opt(front, ":", 1);
	}
	cwd = ft_strjoin_opt(front, cwd, 2);
	cwd = ft_strjoin_opt(cwd, "$ ", 1);
	return (cwd);
}
