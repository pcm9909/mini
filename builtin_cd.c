/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chunpark <chunpark@student.42gyeongsan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 18:09:02 by chunpark          #+#    #+#             */
/*   Updated: 2024/10/23 21:45:35 by chunpark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static void	dir_err(char **cd, char ***envp)
{
	ft_putstr_fd("minishell: cd: ", 2);
	ft_putstr_fd(cd[1], 2);
	ft_putstr_fd(": No such file or directory\n", 2);
	set_dollar(1, envp);
}

static void	dir_move(char *old, char *tmp_pwd, char ***envp, int flag)
{
	if (flag == 1)
	{
		set_env(ft_strjoin("OLDPWD=", old), 1, envp);
		set_env(ft_strjoin("PWD=", tmp_pwd), 1, envp);
	}
	else
	{
		free(old);
		free(tmp_pwd);
	}
}

static void	make_cd_path(char **cd_path, char **tmp_pwd, int i)
{
	if (!ft_strncmp(cd_path[i], "..", 5))
	{
		if (ft_strrchr(*tmp_pwd, '/') == *tmp_pwd)
			*(ft_strrchr(*tmp_pwd, '/') + 1) = '\0';
		else
			*(ft_strrchr(*tmp_pwd, '/')) = '\0';
	}
	else
	{
		*tmp_pwd = ft_strjoin_opt(*tmp_pwd, "/", 1);
		*tmp_pwd = ft_strjoin_opt(*tmp_pwd, cd_path[i], 1);
	}
}

static void	cd_check(char **cd, char **tmp_pwd, char ***envp)
{
	char	**cd_path;
	int		i;

	if (!ft_strncmp(cd[1], "/", 1))
	{
		free(*tmp_pwd);
		*tmp_pwd = ft_strdup("/");
	}
	i = 0;
	cd_path = ft_split(cd[1], '/');
	while (cd_path[i] != NULL)
	{
		if (i == 0 && !ft_strncmp(cd_path[i], "~", 4))
		{
			free(*tmp_pwd);
			*tmp_pwd = ft_strdup(extract_home(*envp));
		}
		else
			make_cd_path(cd_path, tmp_pwd, i);
		i++;
	}
	all_free(cd_path);
}

void	handle_cd_command(t_redir *command, char ***envp)
{
	char	*tmp_pwd;
	char	**cd;
	char	*old;

	tmp_pwd = getcwd(NULL, BUFSIZ);
	old = getcwd(NULL, BUFSIZ);
	cd = ft_strdups(command->cmd->cmd_val);
	if (cd[1] == NULL)
	{
		free(tmp_pwd);
		tmp_pwd = ft_strdup(extract_home(*envp));
	}
	else if (cd[2] != NULL)
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		set_dollar(1, envp);
	}
	else
		cd_check(cd, &tmp_pwd, envp);
	if (chdir(tmp_pwd) == -1)
		dir_err(cd, envp);
	else
		dir_move(old, tmp_pwd, envp, 1);
	dir_move(old, tmp_pwd, envp, 0);
	all_free(cd);
}
