#include "main.h"

char **update_envp(char **envp, int type, char *new)
{
	int	i;
	char	**ptr;

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

char **initialize_environment(char *env[])
{
    return update_envp(env, 0, NULL);
}

int	double_ptr_size(char **ptr)
{
	int	i;

	i = 0;
	while (ptr[i] != NULL)
		i++;
	return (i);
}

int	search_env(char **envp, char *name, int flag) // free
{
	int	i;
	char	*re;
	char	*tmp;

	i = 0;
	if (ft_strchr(name, '=') && flag == 1)
		re = ft_substr(name, 0, ft_strchr(name, '=') - name);
	else
		re = name;
	while (*envp)
	{
		if (ft_strchr(*envp, '='))
			tmp = ft_substr(*envp, 0, ft_strchr(*envp, '=') - *envp);
		else
			tmp = *envp;
		if (!ft_strncmp(tmp, re, ft_strlen(re)))
			break ;
		envp++;
		i++;
	}
	return (i);
}

int	is_validname(char *ptr)
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
	if ((ft_isalpha(*ptr) || *ptr == '_') && is_validname(ptr))
	{
		if (ft_strchr(ptr, '='))
		{
			if ((ft_strchr(ptr, '=') + 1)[0] == '\0')
				return (1);
			if (ft_strchr(ptr, '=') != ft_strrchr(ptr, '='))
				return (1);
			return (1);
		}
		else
			return (0);
	}
	else
		return (-1);
}

char *set_env(char *name, int flag, char ***envp)
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
			(*envp)[i] =  name;
		}
		else
			*envp = update_envp(*envp, 0, name);
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
	//exit(0);
}

void	ft_export(char **ptr, char ***envp)
{
	int	i;
	int	flag;
	char	*rax;

	i = 1;
	flag = 0;
	while (ptr[i] != NULL)
	{
		rax = set_env(ptr[i], env_validate(ptr[i]), envp);
		if (flag == 0 && rax != NULL)
		{
			printf("minishell: export: `%s': not a valid identifier\n", rax);
			flag = 1;
		}
		i++;
	}
	//exit(flag);
}

int	only_digit(char *ptr)
{
	while (*ptr)
	{
		if (!ft_isdigit(*ptr))
			return (0);
		ptr++;
	}
	return (1);
}

void	ft_exit(char **ptr)
{
	printf("exit\n");
	if (ptr[1] == NULL)
		exit(0);
	else if (!only_digit(ptr[1]))
	{
		printf("minishell: exit: %s: numeric argument required\n", ptr[1]);
		exit(2);
	}
	else if (ptr[2] != NULL)
		printf("minishell: exit: too many arguments\n");
	else
		exit((unsigned char)ft_atoi(ptr[1]));
}

void	ft_unset(char **ptr, char **envp)
{
	int	i;
	int	k;

	i = 1;
	while (ptr[i] != NULL)
	{
		k = search_env(envp, ptr[i], 0);
		free(envp[k]);
		envp[k] = ft_strdup("");
		i++;
	}
}

void	ft_echo(char *ptr, char **envp)
{
	int	flag;
	int	i;
	int	k;
	char	*tmp;

	i = 1;
	flag = 0;
	ptr = ptr + 4;
	while (*ptr == ' ')
		ptr++;
	while (!ft_strncmp(ptr, "-n ", 3))
	{
		flag = 1;
		ptr+=3;
	}
	while (*ptr)
	{
		if (*ptr == '$')
		{
			ptr++;
			if (*ptr == '?')
				printf("%d", 0); // 종료상태 넣어야함
			else
			{
				i = 0;
				while (ptr[i] != ' ' && ptr[i] != '\0')
					i++;
				k = search_env(envp, ft_substr(ptr, 0, i), 0);
				ptr = &ptr[i];
				if (envp[k] != NULL)
					printf("%s", ft_strchr(envp[k], '=') + 1);
			}
		}
		else
		{
			printf("%c", *ptr);
			ptr++;
		}
	}
	if (flag == 0)
		printf("\n");
}

void handle_cd_command(t_redirection *command, char **envp)
{
    char *tmp_pwd = getcwd(NULL, BUFSIZ);
    char **cd = ft_split(command->full_cmd, ' ');
    char **cd_path;
    int i;

    if (cd[1] == NULL)
    {
        free(tmp_pwd);
        tmp_pwd = ft_strdup(extract_home(envp));
    }
    else if (cd[2] != NULL)
    {
        printf("minishell: cd: too many arguments\n");
    }
    else
    {
        if (!ft_strncmp(cd[1], "/", 1))
        {
            free(tmp_pwd);
            tmp_pwd = ft_strdup("/");
        }
        i = 0;
        cd_path = ft_split(cd[1], '/');
        while (cd_path[i] != NULL)
        {
            if (i == 0 && !ft_strncmp(cd_path[i], "~", 4))
            {
                free(tmp_pwd);
                tmp_pwd = ft_strdup(extract_home(envp));
            }
            else
            {
                if (!ft_strncmp(cd_path[i], "..", 5))
                {
                    if (ft_strrchr(tmp_pwd, '/') == tmp_pwd)
                        *(ft_strrchr(tmp_pwd, '/') + 1) = '\0';
                    else
                        *(ft_strrchr(tmp_pwd, '/')) = '\0';
                }
                else
                {
                    tmp_pwd = ft_strjoin(tmp_pwd, "/");
                    tmp_pwd = ft_strjoin(tmp_pwd, cd_path[i]);
                }
            }
            i++;
        }
    }
    if (chdir(tmp_pwd) == -1)
    {
        printf("minishell: cd: %s: No such file or directory\n", cd[1]);
    }
}

void handle_export_command(t_redirection *command, char **envp)
{
    char **cd = ft_split(command->full_cmd, ' ');
    if (cd[1] == NULL)
        print_envp(envp, 1);
    else
        ft_export(cd, &envp);
}

void handle_env_command(t_redirection *command, char **envp)
{
    print_envp(envp, 0);
}

void handle_exit_command(t_redirection *command)
{
    char **cd = ft_split(command->full_cmd, ' ');
    ft_exit(cd);
}

void handle_unset_command(t_redirection *command, char **envp)
{
    char **cd = ft_split(command->full_cmd, ' ');
    ft_unset(cd, envp);
}

void handle_pwd_command()
{
    char *tmp_pwd = getcwd(NULL, BUFSIZ);
    printf("%s\n", tmp_pwd);
    free(tmp_pwd);
}

void handle_echo_command(t_redirection *command, char **envp)
{
    ft_echo(command->full_cmd, envp);
}