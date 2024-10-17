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
	if (tmp)
		return (tmp + 5);
	else
		return (tmp);
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
	if (tmp)
		return (ft_substr(tmp, 22, ft_strchr(tmp, '.') - tmp - 22));
	else
		return (NULL);
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
	if (tmp)
		return (tmp + 5);
	else
		return (tmp);
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
		front = ft_strjoin_opts(extract_name(envp), "@", 0);
		front = ft_strjoin_opts(front, extract_location(envp), 3);
		front = ft_strjoin_opts(front, ":", 1);
	}
	cwd = ft_strjoin_opts(front, cwd, 2);
	cwd = ft_strjoin_opts(cwd, "$ ", 1);
	return (cwd);
}
