/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_command_path.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chunpark <chunpark@student.42gyeongsan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 18:09:39 by chunpark          #+#    #+#             */
/*   Updated: 2024/10/23 21:45:35 by chunpark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static char	*check_path(char *cmd)
{
	if (!cmd)
		return (NULL);
	if (cmd[0] == '.' && cmd[1] == '/')
	{
		return (cmd);
	}
	if (ft_strchr(cmd, '/'))
	{
		if (access(cmd, X_OK) != -1)
			return (ft_strdup(cmd));
		else
			return (NULL);
	}
	return (NULL);
}

static char	*search_path(char *cmd, char *path)
{
	char	**paths;
	char	*cmd_path;
	char	*tmp;
	int		i;

	if (!path)
		return (ft_strdup(cmd));
	paths = ft_split(path, ':');
	cmd_path = NULL;
	i = 0;
	while (paths[i])
	{
		tmp = ft_strjoin(paths[i], "/");
		cmd_path = ft_strjoin_opt(tmp, cmd, 1);
		if (access(cmd_path, X_OK) != -1)
			break ;
		free(cmd_path);
		cmd_path = NULL;
		i++;
	}
	i = -1;
	while (paths[++i])
		free(paths[i]);
	free(paths);
	return (cmd_path);
}

char	*get_path(char **envp)
{
	int		i;
	char	*path;

	i = 0;
	while (envp[i])
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			path = ft_strdup(envp[i] + 5);
			return (path);
		}
		i++;
	}
	return (NULL);
}

char	*get_cmd_path(char *cmd, char *path)
{
	char	*cmd_path;

	cmd_path = check_path(cmd);
	if (cmd_path != NULL)
		return (cmd_path);
	return (search_path(cmd, path));
}
