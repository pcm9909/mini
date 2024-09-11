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
	return (tmp+5);
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
	return (tmp+5);
}

char *build_prompt(char **envp)
{
    char *pwd = getcwd(NULL, BUFSIZ);
    char *cwd;
    if (!ft_strncmp(pwd, extract_home(envp), ft_strlen(extract_home(envp)))) {
        cwd = pwd + ft_strlen(extract_home(envp));
        cwd = ft_strjoin("~", cwd);
    }
	else
	{
        cwd = pwd;
    }
    cwd = ft_strjoin(cwd, "$ ");
    cwd = ft_strjoin(":", cwd);
    cwd = ft_strjoin(extract_location(envp), cwd);
    cwd = ft_strjoin("@", cwd);
    cwd = ft_strjoin(extract_name(envp), cwd);
    return cwd;
}