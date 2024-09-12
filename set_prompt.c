/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_prompt.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chunpark <chunpark@student.42gyeongsan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 14:36:50 by chunpark          #+#    #+#             */
/*   Updated: 2024/09/12 21:06:50 by chunpark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

char	**extract_path(char *envp[])
{
	char	**tmp;
	char	**re;

	tmp = NULL;
	while (*envp)
	{
		if (!ft_strncmp(*envp, "PATH=", 5))
			break ;
		envp++;
	}
	if (*envp)
	{
		tmp = ft_split((*envp) + 5, ':');
		re = tmp;
		while (*re)
		{
			ft_strlcat(*re, "/", 999);
			re++;
		}
	}
	return (tmp);
}

char	*extract_name(char *envp[])
{
	char	*tmp;
	char	**re;

	tmp = NULL;
	while (*envp)
	{
		if (!ft_strncmp(*envp, "USER=", 5))
			break ;
		envp++;
	}
	tmp = (*envp);
	return (tmp + 5);
}

char	*extract_location(char *envp[])
{
	char	*tmp;
	char	**re;

	tmp = NULL;
	while (*envp)
	{
		if (!ft_strncmp(*envp, "SESSION_MANAGER=", 16))
			break ;
		envp++;
	}
	tmp = (*envp);
	return (ft_substr(tmp, 22, ft_strchr(tmp, '.') - tmp - 22));
}

char	*extract_home(char *envp[])
{
	char	*tmp;
	char	**re;

	tmp = NULL;
	while (*envp)
	{
		if (!ft_strncmp(*envp, "HOME=", 5))
			break ;
		envp++;
	}
	tmp = (*envp);
	return (tmp + 5);
}

char	*build_prompt(char **envp)
{
	char	*pwd;
	char	*cwd;
	char	*tmp;

	pwd = getcwd(NULL, BUFSIZ);
	if (!ft_strncmp(pwd, extract_home(envp), ft_strlen(extract_home(envp))))
	{
		cwd = pwd + ft_strlen(extract_home(envp));
		tmp = ft_strjoin("~", cwd);
	}
	else
		tmp = ft_strdup(pwd);
	cwd = ft_strjoin(tmp, "$ ");
	free(tmp);
	free(pwd);
	tmp = ft_strjoin(":", cwd);
	free(cwd);
	pwd = extract_location(envp);
	cwd = ft_strjoin(pwd, tmp);
	free(pwd);
	free(tmp);
	tmp = ft_strjoin("@", cwd);
	free(cwd);
	cwd = ft_strjoin(extract_name(envp), tmp);
	free(tmp);
	return (cwd);
}
