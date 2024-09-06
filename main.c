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

void	check_err(int n, int tar, int status, int type)
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
		rl_redisplay();
		return ;
	}
}

void	sg2(int signal)
{
	if (signal == SIGINT)
	{
		return ;
	}
	else if (signal == SIGTERM)
	{
		printf("exit\n");
		exit(0);
	}
	else if (signal == SIGQUIT)
	{
		rl_on_new_line();
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
	signal(SIGQUIT, sg);
}

void	end_sig(struct termios *old)
{
	tcgetattr(0, old);
	old->c_lflag |= (512);
	tcsetattr(0, TCSANOW, old);
	signal(SIGINT, sg2);
	signal(SIGQUIT, sg2);
}

void	none_sig(struct termios *old)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
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

void parse_command(char *str, int *i, t_redirection *command, char **envp)
{
    int j;
    char *content;
	char *temp;

    while (str[*i])
    {
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
            while (str[*i] && !is_whitespace(str[*i]) && str[*i] != '>' && str[*i] != '<' && str[*i] != '"' && str[*i] != '\'')
                (*i)++;
            if (j == *i)
                return;
            content = ft_substr(str, j, *i - j);
        }
        command->command->command = append_command(&command->command->command, content);
        free(content);
    }
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

void parse_command_fix(char *str, int *i, t_redirection *command, char **envp)
{
    int j;
    char *temp;
    char *content;
    int env_idx;

    content = ft_strdup("");
	while(str[*i] && str[*i] != ' ')
	{
		if (str[*i] == '"')
		{
			(*i)++;
			j = (*i);
			while (str[*i] && str[*i] != '"' && str[*i] != '$')
				(*i)++;
			if (str[*i] == '$')
			{
				temp = ft_substr(str, j, *i -j);
				(*i)++;
				j = (*i);
				while (str[*i] && str[*i] != '\'' && str[*i] != '"')
					(*i)++;
				char *env_var = ft_substr(str, j, *i - j);
				temp = ft_strdup(envp[search_env(envp, env_var, 1)]);
				free(env_var);
			}
			temp = ft_substr(str, j, *i - j);
			(*i)++;
		}
		else if (str[*i] == '\'')
		{
			(*i)++;
			j = (*i);
			while (str[*i] && str[*i] != '\'')
				(*i)++;
			temp = ft_substr(str, j, *i - j);
			(*i)++;
		}
		else if (str[*i] == '$')
		{
			(*i)++;
			j = (*i);
			while (str[*i] && str[*i] != '\'' && str[*i] != '"')
				(*i)++;
			char *env_var = ft_substr(str, j, *i - j);
			temp = ft_strdup(envp[search_env(envp, env_var, 1)]);
			free(env_var);
		}
		else
		{
			j = (*i);
			while (str[*i] && str[*i] != '\'' && str[*i] != '"' && !is_whitespace(str[*i]) && str[*i] != '$')
				(*i)++;
			if((*i) == j)
				return ;
			temp = ft_substr(str, j, *i - j);
			if(!temp)
				return ;
		}
		char *new_content = ft_strjoin_with_free(content, temp);
		free(temp);
		content = new_content;
	}
    command->command->command = append_command(&command->command->command, content);
    free(content);
}


void parse_redirection(char *str, t_redirection *command, char **envp)
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
            parse_command_fix(str, &i, command, envp);
        }
    }
	//print(command);
	command->full_cmd = set_command(command->command);
    set_order(command, str);
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

void	all_free(char **ptr)
{
	while (*ptr != NULL)
	{
		free(*ptr);
		ptr++;
	}
	free(ptr);
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
void process_input(char *str, char ***envp)
{
	static int	exit_code;
    str = umm(str);
    char **split = ft_split(str, '|');
    int cnt = cnt_cmd(split);
    t_redirection **command = (malloc(sizeof(t_redirection *) * cnt));
    int input_fd = 0;
    int pipe_fd[2];
    pid_t *pids = malloc(sizeof(pid_t) * cnt);
	struct termios old;

    for (int i = 0; i < cnt; i++)
    {
        initialize_redirection(&command[i]);
        parse_redirection(split[i], command[i], *envp);
    }

    for (int i = 0; i < cnt; i++)
    {
        if (i < cnt - 1)
		{
            if (pipe(pipe_fd) == -1)
            {
                perror("pipe\n");
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            pipe_fd[0] = 0;
            pipe_fd[1] = 1;
        }
        pids[i] = fork();
        if (pids[i] == -1)
        {
            perror("fork\n");
            exit(EXIT_FAILURE);
        }

        if (pids[i] == 0)
        {
			end_sig(&old);
            // Child process
            if (i > 0)
            {
                dup2(input_fd, 0);
                close(input_fd);
            }
            if (i < cnt - 1)
            {
                dup2(pipe_fd[1], 1);
                close(pipe_fd[1]);
            }
            execute_command(command[i], envp, input_fd, pipe_fd[1]);
            exit(EXIT_SUCCESS);
        }
        else
        {
            // Parent process
            if (i > 0)
            {
                close(input_fd);
            }
            if (i < cnt - 1)
            {
                close(pipe_fd[1]);
            }
            input_fd = pipe_fd[0];
        }
    }

    for (int i = 0; i < cnt; i++)
    {
		int statloc;
        waitpid(pids[i], &statloc, 0);
		//printf("statloc : %d\n",WIFEXITED(statloc));
		if (WIFEXITED(statloc))
			exit_code = WEXITSTATUS(statloc);
		if (WIFSIGNALED(statloc))
		{
			exit_code = 128 + WTERMSIG(statloc);
			if (WTERMSIG(statloc) == 2)
				printf("\n");
			else if(WTERMSIG(statloc) == 3)
				printf("Quit (core dumped)\n");
		}
		if (!ft_strncmp(command[i]->full_cmd, "export ", 7) || !ft_strncmp(command[i]->full_cmd, "export", 8))
			{
				char **cd = ft_split(command[i]->full_cmd, ' ');
				if (cd[1] != NULL)
					exit_code = ft_export(cd, envp);
			}
    }

    for (int i = 0; i < cnt; i++)
    {
        free_redirection(command[i]);
    }

    for (int i = 0; split[i]; i++)
    {
        free(split[i]);
    }
    free(command);
    free(split);
    free(str);
    free(pids);
	printf("exit_code : %d\n",exit_code);
}

void cleanup(char *str)
{
    free(str);
    printf("exit\n");
    exit(EXIT_SUCCESS);
}

int main(int argc, char **argv, char *env[])
{
    char *str;
	char **envp;
	pid_t	pid;
	struct termios old;

	envp = initialize_environment(env);
	while (1)
	{
		char *cwd = build_prompt(envp);
		input_sig(&old);
		str = readline(cwd);
		none_sig(&old);
		if (ft_strlen(str))
			add_history(str);
		if (str)
		{
			process_input(str, &envp);
		}
		else
		{
			cleanup(str);
		}
	}
    return 0;
}