#include "main.h"

char	*check_input(const char *str, char **envp)
{
	int		i;
	int		start;
	int		idx;
	char	*content;
	char	*temp;
	char	*env_var;
	char	*env_val;

	i = 0;
	start = i;
	content = ft_strdup("");
	while(str[i])
	{
		if (str[i] == '$')
		{
			temp = ft_substr(str, start, i - start);
			content = ft_strjoin_with_free(content, temp);
			i++;
			start = i;
			while(is_envp_vars(str[i]))
				i++;
			env_var = ft_substr(str, start, i - start);
			idx = ft_strlen(env_var) + 1;
			env_val = ft_strdup(envp[search_env(envp, env_var, 1)]);
			free(env_var);
			if (env_val)
				content = ft_strjoin_with_free(temp, &env_val[idx]);
			free(env_val);
			start = i;
		}
		else
			i++;
	}
	temp = ft_substr(str, start, i - start);
	content = ft_strjoin_with_free(content, temp);
	free(temp);
	return (content);
}

void handle_double_left_brace(t_redirection *cmd, int check, char **envp)
{
    char    *input;
	int 	i;

	i = 0;
    while (cmd->double_left_brace->command && cmd->double_left_brace->command[i])
    {
        while (1)
        {
            input = readline(">");
            if(!input)
            {
				free(input);
                break;
            }
            if (ft_strncmp(input, cmd->double_left_brace->command[i], ft_strlen(cmd->double_left_brace->command[i])) == 0 && ft_strlen(input) == ft_strlen(cmd->double_left_brace->command[i]))
            {
                free(input);
                break;
            }
            if(cmd->double_left_brace->command[i + 1] == NULL)
            {
				input = check_input(input, envp);
                cmd->here_doc = ft_strjoin_with_free(cmd->here_doc, input);
                cmd->here_doc = ft_strjoin_with_free(cmd->here_doc, "\n");
				add_history(input);
                free(input);
            }
        }
		i++;
    }
}

void	handle_left_brace(t_redirection *command)
{
	int	i;
	int	fd;

	i = 0;
	while (command->left_brace->command && command->left_brace->command[i])
	{
		fd = open(command->left_brace->command[i], O_RDONLY);
		if (fd == -1)
		{
			perror(command->left_brace->command[i]);
			exit(EXIT_FAILURE);
		}
		if (command->left_brace->command[i + 1] == NULL && \
			command->left_brace->order)
		{
			dup2(fd, 0);
			break ;
		}
		close(fd);
		i++;
	}
}

void	handle_right_brace(t_redirection *cmd)
{
	int	i;
	int	fd;

	i = 0;
	while (cmd->right_brace->command && cmd->right_brace->command[i])
	{
		fd = open(cmd->right_brace->command[i], \
					O_CREAT | O_TRUNC | O_WRONLY, 0644);
		if (fd == -1)
		{
			perror(cmd->right_brace->command[i]);
			exit(EXIT_FAILURE);
		}
		if (cmd->right_brace->order == true)
			dup2(fd, 1);
		close(fd);
		perror(cmd->right_brace->command[i]);
		i++;
	}
}

void	handle_double_right_brace(t_redirection *cmd)
{
	int	i;
	int	fd;

	i = 0;
	while (cmd->double_right_brace->command && \
			cmd->double_right_brace->command[i])
	{
		fd = open(cmd->double_right_brace->command[i], \
					O_CREAT | O_APPEND | O_WRONLY, 0644);
		if (fd == -1)
		{
			perror(cmd->double_right_brace->command[i]);
			exit(EXIT_FAILURE);
		}
		if (cmd->double_right_brace->order == true)
			dup2(fd, 1);
		close(fd);
		perror(cmd->double_right_brace->command[i]);
		i++;
	}
}
