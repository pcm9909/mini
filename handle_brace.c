#include "main.h"

char	*check_input(const char *str, char **envp)
{
	int		i;
	int		start;
	char	*val;

	i = 0;
	start = i;
	val = ft_strdup("");
	while (str[i])
	{
		if (str[i] == '$')
		{
			val = ft_strjoin_with_free(val, ft_substr(str, start, i - start));
			handle_dollar(&i, &val, str, envp);
			start = i;
		}
		else
			i++;
	}
	val = ft_strjoin_with_free2(val, ft_substr(str, start, i - start));
	return (val);
}

// char	*handle_double_quotes4(const char *str, t_redirection *command)
// {
// 	int 	i;
// 	int		start;
// 	char	*content;

// 	i = 0;
// 	start = ++i;
// 	content = ft_strdup("");
// 	while (str[i] && str[i] != '"')
// 		i++;
// 	if (str[i] != '"')
// 		return (print_qutoes_error(str, command, &content));
// 	else
// 		content = ft_substr(str, start, i - start);
// 	return (content);
// }

char	*handle_quotes4(const char *str, t_redirection *command, char param, int *idx)
{
	int		start;
	char	*content;
//	int	*ptr;

	//printf("q: %p\n", idx);
	(*idx) = (*idx) + 1;
	start = (*idx);
	content = ft_strdup("");
	while (str[(*idx)] && str[(*idx)] != param)
		(*idx)++;
	if (str[(*idx)] != param)
	{
		return (print_qutoes_error(str, command, &content));
	}
	else
	{
		content = ft_substr(str, start, (*idx) - start);
	}
	return (content);
}

char	*handle_quotes3(const char *str, t_redirection *command, int *j)
{
	//printf("h: %p\n",j);
	if (str[*j] == '"')
		return (handle_quotes4(str, command, '"' ,j));
	else
		return (handle_quotes4(str, command, '\'' ,j));
}

	// while (str[*i])
	// {

	// 	if (str[*i] == '"' || str[*i] == '\'')
	// 		content = ft_strjoin_with_free(content, \
	// 					handle_quotes(str, i, envp, command));
	// 	else
	// 		content = ft_strjoin_with_free(content, \
	// 					handle_command(str, i, envp));
	// 	if (str[*i] == 0 || str[*i] == ' ')
	// 		break ;
	// }

	// 			if(cmd->double_left_brace->command[i][j] == '"' || cmd->double_left_brace->command[i][j] == '\'')
	// 		{
	// 			temp = handle_quotes3(cmd->double_left_brace->command[i], cmd);
	// 			free(cmd->double_left_brace->command[i]);
	// 			cmd->double_left_brace->command[i] = ft_strdup(temp);
	// 			flag = 1;
	// 		}

void	handle_double_left_brace(t_redirection *cmd, int check, char **envp)
{
	char	*read;
	int		i;
	int		j;
	int		start;
	int 	flag;
	char	*temp;
	char	*content1;
	char	*content2;

	i = -1;
	flag = 0;
	while (cmd->double_left_brace->command && cmd->double_left_brace->command[++i])
	{
		j = 0;
		temp = ft_strdup("");
		content1 = ft_strdup("");
		start = 0;
		while(cmd->double_left_brace->command[i][j])
		{
			if (cmd->double_left_brace->command[i][j] == '"' || cmd->double_left_brace->command[i][j] == '\'')
			{
				content1 = ft_substr(cmd->double_left_brace->command[i], start, j  - start);
				temp = handle_quotes3(cmd->double_left_brace->command[i], cmd, &j);
				content2 = ft_strjoin(content1, temp);
				free(content1);
				free(temp);
 				start = ++j;
				flag = 1;
			}
			else
				j++;
		}
		temp = ft_strjoin(temp, ft_substr(cmd->double_left_brace->command[i], start, j - start));
		free(cmd->double_left_brace->command[i]);
		cmd->double_left_brace->command[i] = ft_strdup(temp);
		free(temp);

		while (1)
		{
			printf("%s", cmd->double_left_brace->command[i]);
			read = readline(">");
			if (!read || (ft_strncmp(read, cmd->double_left_brace->command[i], \
			ft_strlen(cmd->double_left_brace->command[i])) == 0 && \
			ft_strlen(read) == ft_strlen(cmd->double_left_brace->command[i])))
			{
				free(read);
				break ;
			}
			if (cmd->double_left_brace->command[i + 1] == NULL)
			{
				add_history(read);
				if(flag)
					read = ft_strjoin_with_free(read, "\n");
				else
					read = ft_strjoin_with_free(check_input(read, envp), "\n");
				cmd->here_doc = ft_strjoin_with_free2(cmd->here_doc, read);
			}
		}
	}
}

int	handle_left_brace(t_redirection *cmd)
{
	int	i;
	int		j;
	int		start;
	int 	flag;
	char	*temp;
	int	fd;

	i = 0;
	while (cmd->left_brace->command && cmd->left_brace->command[i])
	{
		j = 0;
		temp = ft_strdup("");
		start = 0;
		while(cmd->double_left_brace->command[i][j])
		{
			if (cmd->double_left_brace->command[i][j] == '"' || cmd->double_left_brace->command[i][j] == '\'')
			{
				temp = ft_strjoin(ft_substr(cmd->double_left_brace->command[i], start, j  - start) ,handle_quotes3(cmd->double_left_brace->command[i], cmd, &j));
 				start = ++j;
				flag = 1;
			}
			else
				j++;
		}
		temp = ft_strjoin(temp, ft_substr(cmd->double_left_brace->command[i], start, j - start));
		free(cmd->double_left_brace->command[i]);
		cmd->double_left_brace->command[i] = ft_strdup(temp);
		free(temp);
		fd = open(cmd->left_brace->command[i], O_RDONLY);
		if (fd == -1)
		{
			perror(cmd->left_brace->command[i]);
			return (EXIT_FAILURE);
		}
		if (cmd->left_brace->command[i + 1] == NULL && \
			cmd->left_brace->order)
		{
			dup2(fd, 0);
			break ;
		}
		close(fd);
		i++;
	}
	return (EXIT_SUCCESS);
}

int	handle_right_brace(t_redirection *cmd)
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
			return(EXIT_FAILURE);
		}
		if (cmd->right_brace->order == true)
			dup2(fd, 1);
		close(fd);
		perror(cmd->right_brace->command[i]);
		i++;
	}
	return (EXIT_SUCCESS);
}

int	handle_double_right_brace(t_redirection *cmd)
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
			return (EXIT_FAILURE);
		}
		if (cmd->double_right_brace->order == true)
			dup2(fd, 1);
		close(fd);
		perror(cmd->double_right_brace->command[i]);
		i++;
	}
	return (EXIT_SUCCESS);
}
