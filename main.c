#include "main.h"

char *local;

void	error_end(int er)
{
	errno = er;
	if (errno == EOPNOTSUPP)
	{
		perror("command not found");
		exit(127);
	}
	perror("Error");
	exit(1);
}

static void	check_err(int n, int tar, int status, int type)
{
	if (type)
	{
		if (n == tar)
			error_end(status);
	}
	else if (type == 0)
	{
		if (n != tar)
			error_end(status);
	}
	else
	{
		if (n < tar)
			error_end(status);
	}
}

char	*ft_strdup2(const char *s, int len)
{
	char	*mem;
	char	*ptr;
	char	*f;

	mem = NULL;
	mem = (char *)malloc(sizeof(char) * (len + 1));
	if (mem == NULL)
		return (mem);
	if (!s)
	{
		mem[0] = 0;
		return (mem);
	}
	f = (char *)s;
	ptr = mem;
	while (*s)
		*(ptr++) = *(char *)(s++);
	*ptr = 0;
	free(f);
	return (mem);
}

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

char	**make_arg(char *argv)
{
	char	**re;
	char	*tmp;

	re = ft_split(argv, ' ');
	if (!re)
		return (NULL);
	tmp = ft_strdup2(re[0], 100);
	re[0] = tmp;
	return (re);
}

void	f_all(char **target)
{
	char	**tmp;

	tmp = target;
	while (*target)
	{
		free(*target);
		target++;
	}
	free(tmp);
}

void	access_test(char *path, char **ar, char *envp[], int *flag)
{
	char	*tmp;

	tmp = ft_strjoin(path, ar[0]);
	if (!access(tmp, X_OK))
	{
		free(ar[0]);
		ar[0] = ft_strdup(tmp);
		*flag = execve(tmp, ar, envp);
	}
	free(tmp);
}

int	execute(char *argv, char *envp[])
{
	char	**path;
	char	**ar;
	int		flag;
	int		i;

	i = 0;
	flag = -1;
	path = extract_path(envp);
	ar = make_arg(argv);
	while (path[i])
	{
		access_test(path[i], ar, envp, &flag);
		i++;
	}
	access_test("", ar, envp, &flag);
	f_all(ar);
	f_all(path);
	return (flag);
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

void	sg(int signal)
{
	if (signal == SIGINT)
	{
		rl_on_new_line();
		rl_redisplay();
		printf("^C\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	else if (signal == SIGTERM)
	{
		printf("exit\n");
		exit(0);
	}
	else if (signal == SIGQUIT)
	{
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
		return ;
	}
}

void	input_sig(struct termios *old)
{
	tcgetattr(0, old);
	old->c_lflag &= ~(512);
	tcsetattr(0, TCSANOW, old);
	signal(SIGINT, sg);
	signal(SIGTERM, sg);
	signal(SIGQUIT, sg);
}

void	end_sig(struct termios *old)
{
	tcgetattr(0, old);
	old->c_lflag |= 512;
	tcsetattr(0, TCSANOW, old);
	signal(SIGINT, SIG_DFL);
	signal(SIGTERM, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}

void	all_free(char **ptr)
{
	while (*ptr != NULL)
	{
		free(*ptr);
		ptr++;
	}
	free(ptr);
}

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

int is_whitespace(int c)
{
    return ((c >= 9 && c <= 13) || c == 32);
}

t_command *create_command(void)
{
    t_command *cmd = malloc(sizeof(t_command));
    if (!cmd)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    cmd->command = NULL;
    cmd->order = false;
    cmd->exist = false;
    return cmd;
}

void free_command(t_command *cmd)
{
    if (cmd)
    {
        if (cmd->command)
        {
            free_command_list(&cmd->command);
        }
        free(cmd);
    }
}

void initialize_redirection(t_redirection **redirection)
{
    *redirection = malloc(sizeof(t_redirection));
    if (!*redirection)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    (*redirection)->double_left_brace = create_command();
    (*redirection)->double_right_brace = create_command();
    (*redirection)->command = create_command();
    (*redirection)->left_brace = create_command();
    (*redirection)->right_brace = create_command();
}

void free_redirection(t_redirection *redirection)
{
    if (redirection)
    {
        free_command(redirection->double_left_brace);
        free_command(redirection->double_right_brace);
        free_command(redirection->command);
        free_command(redirection->left_brace);
        free_command(redirection->right_brace);
		free(redirection->full_cmd);
        free(redirection);
    }
}

char **allocate_and_copy(char **cmd, int size)
{
    char **new_cmd = malloc(sizeof(char *) * size);
    if (!new_cmd)
    {
        perror("malloc");
        return NULL;
    }
    for (int j = 0; j < size - 1; j++)
    {
        if (cmd[j] == NULL)
        {
            new_cmd[j] = NULL;
        }
        else
        {
            new_cmd[j] = ft_strdup(cmd[j]);
            if (!new_cmd[j])
            {
                perror("ft_strdup");
                for (int k = 0; k < j; k++)
                {
                    free(new_cmd[k]);
                }
                free(new_cmd);
                return NULL;
            }
        }
    }
    return new_cmd;
}

char **append_command(char ***cmd, const char *str)
{
    int i = 0;
    char **new_cmd;

    if (*cmd == NULL)
    {
        new_cmd = malloc(sizeof(char *) * 2);
        if (!new_cmd)
        {
            perror("malloc");
            return NULL;
        }
        new_cmd[0] = ft_strdup(str);
        new_cmd[1] = NULL;
        return new_cmd;
    }
    while ((*cmd)[i])
        i++;
    new_cmd = allocate_and_copy(*cmd, i + 2);
    if (!new_cmd)
        return NULL;
    new_cmd[i] = ft_strdup(str);
    new_cmd[i + 1] = NULL;
    for (int j = 0; j < i; j++)
    {
        free((*cmd)[j]);
    }
    free(*cmd);
    return new_cmd;
}

void free_command_list(char ***command)
{
    if (*command)
    {
        int i = 0;
        while ((*command)[i])
        {
            free((*command)[i]);
            i++;
        }
        free(*command);
        *command = NULL;
    }
}

void parse_left_redirection(const char *str, int *i, t_redirection *command)
{
    int j, flag = 0;
    char *content;

    (*i)++;
    if (str[*i] == '<')
    {
        (*i)++;
        flag = 1;
    }
    while (is_whitespace(str[*i]))
        (*i)++;
    j = *i;
    if (str[*i] == '>' || str[*i] == '<' || str[*i] == '\0' || str[*i] == '|' || str[*i] == '&' || str[*i] == ';')
    {
        printf("zsh: parse error near `%c'\n", str[*i]);
        exit(258);
    }
    while (str[*i] && !is_whitespace(str[*i]) && str[*i] != '>' && str[*i] != '<')
        (*i)++;
    content = ft_substr(str, j, *i - j);
    if (flag == 1)
    {
        command->double_left_brace->exist = true;
        command->double_left_brace->command = append_command(&command->double_left_brace->command, content);
    }
    else
    {
        command->left_brace->exist = true;
        command->left_brace->command = append_command(&command->left_brace->command, content);
    }
    free(content);
}

void parse_right_redirection(char *str, int *i, t_redirection *command)
{
    int j, flag = 0;
    char *content;

    (*i)++;
    if (str[*i] == '>')
    {
        (*i)++;
        flag = 1;
    }
    while (is_whitespace(str[*i]))
        (*i)++;
    if (str[*i] == '>' || str[*i] == '<' || str[*i] == '\0' || str[*i] == '|' || str[*i] == '&' || str[*i] == ';')
    {
        printf("zsh: parse error near `%c'\n", str[*i]);
        exit(258);
    }
    j = *i;
    while (str[*i] && !is_whitespace(str[*i]) && str[*i] != '>' && str[*i] != '<')
        (*i)++;
    content = ft_substr(str, j, *i - j);
    if (flag == 1)
    {
        command->double_right_brace->exist = true;
        command->double_right_brace->command = append_command(&command->double_right_brace->command, content);
    }
    else
    {
        command->right_brace->exist = true;
        command->right_brace->command = append_command(&command->right_brace->command, content);
    }
    free(content);
}

char *handle_single_quotes(const char *str, int *i)
{
    int start = ++(*i);
    while (str[*i] && str[*i] != '\'')
        (*i)++;
    if (str[*i] != '\'')
    {
        fprintf(stderr, "Error: Unmatched single quote\n");
        exit(EXIT_FAILURE);
    }
    char *content = ft_substr(str, start, *i - start);
    (*i)++;
    return content;
}

char *handle_double_quotes(const char *str, int *i)
{
    int start = ++(*i);
    while (str[*i] && str[*i] != '"')
    {
        if (str[*i] == '\\' && (str[*i + 1] == '"' || str[*i + 1] == '\\'))
            (*i)++;
        (*i)++;
    }
    if (str[*i] != '"')
    {
        fprintf(stderr, "Error: Unmatched double quote\n");
        exit(EXIT_FAILURE);
    }
    char *content = ft_substr(str, start, *i - start);
    (*i)++;
    return content;
}

void parse_command(char *str, int *i, t_redirection *command)
{
    int j;
    char *content;

    while (is_whitespace(str[*i]))
        (*i)++;
    if (str[*i] == '"')
    {
        content = handle_double_quotes(str, i);
    }
    else if (str[*i] == '\'')
    {
        content = handle_single_quotes(str, i);
    }
    else
    {
        j = *i;
        while (str[*i] && !is_whitespace(str[*i]) && str[*i] != '>' && str[*i] != '<')
            (*i)++;
        if (j == *i)
            return;
        content = ft_substr(str, j, *i - j);
    }
    command->command->command = append_command(&command->command->command, content);
    free(content);
}

char *ft_find_single_redirect(char *str, char c)
{
    int i = 0;

    if (!str)
        return NULL;
    while (str[i])
    {
        if (str[i] == c)
        {
            if (str[i + 1] == c)
            {
                i++;
            }
            else
            {
                return &str[i];
            }
        }
        i++;
    }
    return NULL;
}

void set_order(t_redirection *command, char *str)
{
    int dual;
    int single;
	char *rev;

    dual = ft_strlen(ft_strnstr(str, "<<", sizeof(str)));
    single = ft_strlen(ft_find_single_redirect(str, '<'));
    if (dual > single)
    {
        command->double_left_brace->order = true;
    }
    else if (dual < single)
    {
        command->left_brace->order = true;
    }
	rev = ft_strrev(str);
    dual = ft_strlen(ft_strnstr(rev, ">>", ft_strlen(str)));
    single = ft_strlen(ft_find_single_redirect(rev, '>'));
    if (dual > single)
    {
        command->double_right_brace->order = true;
    }
    else if (dual < single)
    {
        command->right_brace->order = true;
    }
	free(rev);
}
char *set_command(t_command *command)
{
    if (!command || !command->command)
        return NULL;

    int total_length = 0;
    int i = 0;

    while (command->command[i])
    {
        total_length += ft_strlen(command->command[i]) + 1;
        i++;
    }

    char *result = malloc(total_length + 1);
    if (!result)
    {
        perror("malloc");
        return NULL;
    }
    result[0] = '\0';
    i = 0;
    while (command->command[i])
    {
        ft_strlcat(result, command->command[i], total_length + 1);
        if (command->command[i + 1])
            ft_strlcat(result, " ", total_length + 1);
        i++;
    }
    return result;
}


void parse_redirection(char *str, t_redirection *command)
{
    int i = 0;

    while (str[i])
    {
        while (is_whitespace(str[i]))
            i++;
        if (str[i] == '<')
        {
            parse_left_redirection(str, &i, command);
        }
        else if (str[i] == '>')
        {
            parse_right_redirection(str, &i, command);
        }
        else
        {
            parse_command(str, &i, command);
        }
    }
	command->full_cmd = set_command(command->command);
    set_order(command, str);
}

void open_redirection_files(t_redirection *command)
{
    int i = 0;
    int fd;

    while (command->double_left_brace->command && command->double_left_brace->command[i])
    {
        int pipe_fd[2];
        if (pipe(pipe_fd) == -1)
        {
            perror("pipe");
            exit(EXIT_FAILURE);
        }

        pid_t pid = fork();
        if (pid == -1)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pid == 0)
        {
            // Child process
            close(pipe_fd[0]);
            char *str = ft_strdup("");
            char *input;
            input = readline(">");
            while (input)
            {
                if (ft_strncmp(input, command->double_left_brace->command[i], ft_strlen(input)) == 0 && ft_strlen(input) != 0 && command->double_left_brace->order == true)
                {
                    write(pipe_fd[1], str, ft_strlen(str));
                    free(input);
                    input = NULL;
                    break;
                }
                str = ft_strjoin_with_free(str, input);
                str = ft_strjoin_with_free(str, "\n");
                add_history(str);
                free(input);
                input = readline(">");
            }
            free(str);
            close(pipe_fd[1]);
            exit(EXIT_SUCCESS);
        }
        else
        {
            // Parent process
            close(pipe_fd[1]);
            wait(NULL);
            if (command->double_left_brace->command[i + 1] == NULL) // 마지막 <<의 경우
            {
                dup2(pipe_fd[0], 0);
            }
            close(pipe_fd[0]);
        }
        i++;
    }
    i = 0;
    while (command->left_brace->command && command->left_brace->command[i])
    {
        int pipe_fd[2];
        if (pipe(pipe_fd) == -1)
        {
            perror("pipe");
            exit(EXIT_FAILURE);
        }

        pid_t pid = fork();
        if (pid == -1)
        {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        if (pid == 0)
        {
            // Child process
            close(pipe_fd[0]);
            fd = open(command->left_brace->command[i], O_RDONLY);
            if (fd == -1)
            {
                perror(command->left_brace->command[i]);
                exit(EXIT_FAILURE);
            }
            char buffer[1024];
            ssize_t bytes_read;
            while ((bytes_read = read(fd, buffer, sizeof(buffer))) > 0)
            {
                if (write(pipe_fd[1], buffer, bytes_read) == -1)
                {
                    perror("write");
                    exit(EXIT_FAILURE);
                }
            }
            close(fd);
            close(pipe_fd[1]);
            exit(EXIT_SUCCESS);
        }
        else
        {
            // Parent process
            close(pipe_fd[1]);
            wait(NULL);
            if (command->left_brace->command[i + 1] == NULL && command->left_brace->order) // 마지막 <의 경우
            {
                dup2(pipe_fd[0], 0);
            }
            close(pipe_fd[0]);
        }
        i++;
    }

    i = 0;
    while (command->right_brace->command && command->right_brace->command[i])
    {
        fd = open(command->right_brace->command[i], O_CREAT | O_TRUNC | O_WRONLY, 0644);
        if (fd == -1)
        {
            perror(command->right_brace->command[i]);
            exit(EXIT_FAILURE);
        }
        if (command->right_brace->order == true)
        {
            dup2(fd, 1);
        }
        i++;
    }

    i = 0;
    while (command->double_right_brace->command && command->double_right_brace->command[i])
    {
        fd = open(command->double_right_brace->command[i], O_CREAT | O_APPEND | O_WRONLY, 0644);
        if (fd == -1)
        {
            perror(command->double_right_brace->command[i]);
            exit(EXIT_FAILURE);
        }
        if (command->double_right_brace->order == true)
        {
            dup2(fd, 1);
        }
        i++;
    }
}

int cnt_cmd(char **split)
{
    int i = 0;

    if (!split)
    {
        return 0;
    }
    while (split[i])
    {
        i++;
    }
    return i;
}

void exe(t_redirection *command, char **cmd, char **envp)
{
    char *cmd_path = NULL;
    char *path = get_path(envp);
    if (cmd)
    {
        cmd_path = get_cmd_path(cmd[0], path);
    }
    if (execve(cmd_path, cmd, envp))
    {
        if (cmd)
            printf("%s: command not found\n", cmd[0]);
    }
    free(path);
}

void execute_command(t_redirection *command, char **envp, int input_fd, int output_fd)
{
    pid_t pid;
	char	*tmp_pwd;
	char	**cd;
	int	i;
	char	**cd_path;


	printf("%s\n", command->full_cmd);
	if (command->full_cmd && command->full_cmd[0] == 'c' && command->full_cmd[1] == 'd' && (command->full_cmd[2] == ' ' || command->full_cmd[2] == '\0'))//!ft_strncmp(cin, "cd", 4))
	{
		tmp_pwd = getcwd(NULL, BUFSIZ);
		cd = ft_split(command->full_cmd, ' '); //free
		if (cd[1] == NULL)
		{
			free(tmp_pwd);
			tmp_pwd = ft_strdup(extract_home(envp));
		}
		else if(cd[2] != NULL)
		{
			printf("minishell: cd: too many arguments\n"); //표준에러로 바꾸는게 날거같긴함
			//exit(1);
		}
		else
		{
			if(!ft_strncmp(cd[1], "/", 1)) //boom
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
					tmp_pwd = ft_strdup(extract_home(envp)); // free
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
						tmp_pwd = ft_strjoin(tmp_pwd, "/"); //free 해야함
						tmp_pwd = ft_strjoin(tmp_pwd, cd_path[i]); //free 해야함
					}
				}
				i++;
			}
		}
		if (chdir(tmp_pwd) == -1)
		{
			printf("minishell: cd: %s: No such file or directory\n", cd[1]);
			//exit(1);
		}
	}
	else if(!ft_strncmp(command->full_cmd, "export ",7) || !ft_strncmp(command->full_cmd, "export", 8))
	{
		cd = ft_split(command->full_cmd, ' ');
		if (cd[1] == NULL)
			print_envp(envp, 1);
		else
			ft_export(cd, &envp);
	}
	else if (!ft_strncmp(command->full_cmd, "env",5) || !ft_strncmp(command->full_cmd, "env ", 4))
	{
		print_envp(envp, 0);
		// exit(0);
	}
	else if (!ft_strncmp(command->full_cmd, "exit",5) || !ft_strncmp(command->full_cmd, "exit ", 5))
	{
		cd = ft_split(command->full_cmd, ' ');
		ft_exit(cd);
	}
	else if (!ft_strncmp(command->full_cmd, "unset", 6) || !ft_strncmp(command->full_cmd, "unset ", 6))
	{
		cd = ft_split(command->full_cmd, ' ');
		ft_unset(cd, envp);
		// exit(0);
	}
	else if (!ft_strncmp(command->full_cmd, "pwd", 6) || !ft_strncmp(command->full_cmd, "pwd " , 4))
	{
		tmp_pwd = getcwd(NULL, BUFSIZ);
		printf("%s\n", tmp_pwd);
		free(tmp_pwd);
		// exit(0);
	}
	else if (!ft_strncmp(command->full_cmd, "echo", 6) || !ft_strncmp(command->full_cmd, "echo " , 5))
	{
		ft_echo(command->full_cmd, envp);
		// exit(0);
	}
	else
	{
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}

		if (pid == 0)
		{
			// Child process
			if (input_fd != 0)
			{
				dup2(input_fd, 0);
				close(input_fd);
			}
			if (output_fd != 1)
			{
				dup2(output_fd, 1);
				close(output_fd);
			}
			open_redirection_files(command);
			exe(command, command->command->command, envp);
		}
		else
			waitpid(pid,NULL,0);
	// exit(EXIT_FAILURE); // execve가 실패한 경우
	}
}


char *umm(char *str)
{
    if (str)
    {
        int len = ft_strlen(str);
        char *tmp;

        if (len == 0) // 문자열이 비어 있는 경우
            return str;

        len--;
        while (is_whitespace(str[len]) > 0 && len > 0)
        {
            len--;
        }
        if (is_whitespace(str[len]) && len == 0) // 문자열이 공백만 포함하는 경우
            return str;

        if (str[len] == '|')
        {
            tmp = ft_strjoin_with_free(str, readline(">"));
            add_history(tmp);
        }
        else
            return str;
        return umm(tmp);
    }
    return NULL;
}

int main(int argc, char **argv, char *env[])
{
    char *str;
	char	*cin;
	pid_t	pid;
	int	status;
	char	*cwd;
	char	*pwd;
	char	*tmp_pwd;
	char	**cd;
	char	**cd_path;
	char	**envp;
	int i;
	struct termios	old;

	envp = update_envp(env, 0, NULL);
    while (1)
    {
        // pid_t pid;
        // pid = fork();
        // if (pid == 0)
        // {
			pwd = getcwd(NULL, BUFSIZ);
			if (!ft_strncmp(pwd, extract_home(envp), ft_strlen(extract_home(envp))))
			{
				cwd = pwd + ft_strlen(extract_home(envp));
				cwd = ft_strjoin("~", cwd);
			}
			else
				cwd = pwd;
			cwd = ft_strjoin(cwd, "$ ");
			cwd = ft_strjoin(":", cwd);
			cwd = ft_strjoin(extract_location(envp), cwd);
			cwd = ft_strjoin("@", cwd);
			cwd = ft_strjoin(extract_name(envp), cwd);
			//free 해야함
			input_sig(&old);
            // 이게 원본 str = readline(cwd);
			str = readline("command : ");
			//end_sig(&old);
            if (ft_strlen(str))
                add_history(str);
            if (str)
            {
                str = umm(str);
                char **split = ft_split(str, '|');
                int cnt = cnt_cmd(split);
                int i = 0;
                t_redirection **command = (malloc(sizeof(t_redirection *) * cnt));
                int input_fd = 0;
                int pipe_fd[2];

                if (split)
                {
                    while (split[i])
                    {
                        initialize_redirection(&command[i]);
                        if (str == NULL)
                            exit(EXIT_FAILURE);
                        parse_redirection(split[i], command[i]);
                        i++;
                    }
                }

                for (i = 0; i < cnt; i++)
                {
                    if (i < cnt - 1)
                    {
                        if (pipe(pipe_fd) == -1)
                        {
                            perror("pipe");
                            exit(EXIT_FAILURE);
                        }
                    }
                    else
                    {
                        pipe_fd[0] = 0;
                        pipe_fd[1] = 1;
                    }
                    execute_command(command[i], envp, input_fd, pipe_fd[1]);

                    if (input_fd != 0)
                        close(input_fd);
                    if (pipe_fd[1] != 1)
                        close(pipe_fd[1]);

                    input_fd = pipe_fd[0];
                }

                for (i = 0; i < cnt; i++)
                {
                    wait(NULL);
                }

                for (i = 0; i < cnt; i++)
                {
                    free_redirection(command[i]);
                }

                if (split)
                {
					i = 0;
                    while (split[i])
                    {
                        free(split[i]);
                        i++;
                    }
                }
                free(command);
                free(split);
                free(str);
                //exit(EXIT_SUCCESS);
            }
            else
            {
                free(str);
				printf("exit\n");
                exit(EXIT_SUCCESS);
            }
        }
    return 0;
}