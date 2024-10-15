#include "main.h"

char	**update_envp(char **envp, int type, char *new)
{
	int		i;
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

char	**initialize_environment(char *env[])
{
	return (update_envp(env, 0, NULL));
}

int	double_ptr_size(char **ptr)
{
	int	i;

	i = 0;
	while (ptr[i] != NULL)
		i++;
	return (i);
}

int	search_env(char **envp, char *name, int flag)
{
	int		i;
	char	*re;
	char	*tmp;
	char	*dup_name;

	i = 0;
	if (ft_strchr(name, '=') && flag == 1)
		re = ft_substr(name, 0, ft_strchr(name, '=') - name);
	else
		re = ft_strdup(name);
	while (*envp)
	{
		if (ft_strchr(*envp, '='))
			tmp = ft_substr(*envp, 0, ft_strchr(*envp, '=') - *envp);
		else
			tmp = ft_strdup(*envp);
		if (!ft_strncmp(tmp, re, ft_strlen(re) + 1))
		{
			break ;
		}
		envp++;
		i++;
		free(tmp);
	}
	free(re);
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
	char	*dup_ptr;

	dup_ptr = ft_strdup(ptr);
	if ((ft_isalpha(*dup_ptr) || *dup_ptr == '_') && is_validname(dup_ptr))
	{
		if (ft_strchr(dup_ptr, '='))
		{
			if ((ft_strchr(dup_ptr, '=') + 1)[0] == '\0')
			{
				free(dup_ptr);
				return (1);
			}
			if (ft_strchr(dup_ptr, '=') != ft_strrchr(dup_ptr, '='))
			{
				free(dup_ptr);
				return (1);
			}
			free(dup_ptr);
			return (1);
		}
		else
		{
			free(dup_ptr);
			return (0);
		}
	}
	else
	{
		free(dup_ptr);
		return (-1);
	}
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
}

int	ft_export(char **ptr, char ***envp)
{
	int		i;
	int		flag;
	char	*rax;

	i = 1;
	flag = 0;
	while (ptr[i] != NULL)
	{
		rax = set_env(ptr[i], env_validate(ptr[i]), envp);
		if (flag == 0 && rax != NULL)
		{
			ft_putstr_fd("minishell: export: `", 2);
			ft_putstr_fd(rax, 2);
			ft_putstr_fd("': not a valid identifier\n", 2);
			flag = 1;
		}
		i++;
	}
	return (flag);
}

int	only_digit(char *ptr)
{
	if (*ptr == '-' || *ptr == '+')
		ptr++;
	while (*ptr)
	{
		if (!ft_isdigit(*ptr))
			return (0);
		ptr++;
	}
	return (1);
}

static int	ft_isspace1(int c)
{
	return (c == ' ' || (c >= '\t' && c <= '\r'));
}

static long long	ft_atoll(const char *str)
{
	int			i;
	long long	sign;
	long long	result;

	i = 0;
	sign = 1;
	result = 0;
	while (ft_isspace1((unsigned char)str[i]))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * sign);
}

static int	ft_count_len1(long long n)
{
	long long	temp;
	int			len;

	if (n == 0)
		return (1);
	temp = n;
	len = 0;
	while (temp)
	{
		len++;
		temp /= 10;
	}
	return (len);
}

static char	*ft_lltoa(long long n)
{
	char			*str;
	long long		sign;
	int				len;

	sign = 1;
	len = ft_count_len1(n);
	if (n < 0)
	{
		sign = -1;
		len++;
	}
	str = (char *) malloc (sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	str[len] = '\0';
	while (len--)
	{
		str[len] = n % 10 * sign + '0';
		n /= 10;
	}
	if (sign == -1)
		str[0] = '-';
	return (str);
}

static int	exit_range(char *ptr)
{
	char	*tmp;

	tmp = ft_lltoa(ft_atoll(ptr));
	if (ft_strncmp(tmp, ptr, ft_strlen(ptr)))
		return (1);
	return (0);
}

void	ft_exit(char **ptr)
{
	ft_putstr_fd("exit\n", 2);
	if (ptr[1] == NULL)
		exit(0);
	else if (!only_digit(ptr[1]) || exit_range(ptr[1]))
	{
		ft_putstr_fd("minishell: exit: ", 2);
		ft_putstr_fd(ptr[1], 2);
		ft_putstr_fd(": numeric argument required\n", 2);
		exit(2);
	}
	else if (ptr[2] != NULL)
		ft_putstr_fd("minishell: exit: too many arguments\n", 2);
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

void	ft_echo(char **ptr)
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
	while (i > 0)
	{
		free(ptr[i]);
		i--;
	}
	free(ptr);
}

void	handle_cd_command(t_redir *command, char ***envp)
{
	char	*tmp_pwd;
	char	**cd;
	char	**cd_path;
	char	*old;
	int		i;

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
				tmp_pwd = ft_strdup(extract_home(*envp));
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
					tmp_pwd = ft_strjoin_opts(tmp_pwd, "/", 1);
					tmp_pwd = ft_strjoin_opts(tmp_pwd, cd_path[i], 1);
				}
			}
			i++;
		}
	}
	if (chdir(tmp_pwd) == -1)
	{
		ft_putstr_fd("minishell: cd: ", 2);
		ft_putstr_fd(cd[1], 2);
		ft_putstr_fd(": No such file or directory\n", 2);
		set_dollar(1, envp);
	}
	else
	{
		set_env(ft_strjoin("OLDPWD=", old), 1, envp); // free
		set_env(ft_strjoin("PWD=", tmp_pwd), 1, envp); // free
	}
}

void	handle_export_command(t_redir *command, char ***envp)
{
	char	**cd;

	cd = ft_strdups(command->cmd->cmd_val);
	if (cd[1] == NULL)
	{
		print_envp(*envp, 1);
	}
	else
		set_dollar(ft_export(cd, envp), envp);
}

void	handle_env_command(t_redir *command, char **envp)
{
	print_envp(envp, 0);
}

void	handle_exit_command(t_redir *command)
{
	char	**cd;

	cd = ft_strdups(command->cmd->cmd_val);
	ft_exit(cd);
}

void	handle_unset_command(t_redir *command, char **envp)
{
	char	**cd;

	cd = ft_strdups(command->cmd->cmd_val);
	ft_unset(cd, envp);
}

void	handle_pwd_command(void)
{
	char	*tmp_pwd;

	tmp_pwd = getcwd(NULL, BUFSIZ);
	printf("%s\n", tmp_pwd);
	free(tmp_pwd);
}

void	handle_echo_command(t_redir *command, char **envp)
{
	char	**cd;
	int i = 0;

	cd = ft_strdups(command->cmd->cmd_val);
	ft_echo(cd);
}
