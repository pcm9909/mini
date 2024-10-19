#include "main.h"

char	*handle_quotes(const char *str, int *i, t_redir *command)
{
	char	*temp;

	if (str[*i] == '"')
	{
		temp = handle_double_quotes(str, i, command);
	}
	else
	{
		temp = handle_single_quotes(str, i, command);
	}
	return (temp);
}

int	handle_dollar1(int *i, char **content, const char *str, char **envp)
{
	char	*envp_var;
	char	*envp_val;
	int		idx;
	int		start;

	(*i)++;
	start = (*i);
	while (is_envp_vars(str[*i]))
		(*i)++;
	envp_var = ft_substr(str, start, (*i) - start);
	idx = ft_strlen(envp_var) + 1;
	envp_val = ft_strdup(envp[search_env(envp, envp_var, 1)]);
	if (envp_val)
		(*content) = ft_strjoin_opts((*content), &envp_val[idx], 1);
	if (ft_strlen(envp_val) == 0)
	{
		printf("minishell: $%s: ambiguous redirect\n", envp_var);
		free(envp_var);
		free(envp_val);
		return (1);
	}
	free(envp_var);
	free(envp_val);
	return (0);
}

static char	**allocate_and_copy(char **cmd, int size)
{
	char	**new_cmd;
	int		j;

	j = 0;
	new_cmd = malloc(sizeof(char *) * size);
	if (!new_cmd)
		error_exit("minishell: Error: allocation failed");
	while (j < size - 1)
	{
		if (cmd[j] == NULL)
			new_cmd[j] = NULL;
		else
			new_cmd[j] = ft_strdup(cmd[j]);
		j++;
	}
	return (new_cmd);
}

char	**append_command(char ***cmd, char *str)
{
	char	**new_cmd;
	int		i;
	int		j;

	i = 0;
	if (*cmd == NULL)
	{
		new_cmd = malloc(sizeof(char *) * 2);
		new_cmd[0] = ft_strdup(str);
		new_cmd[1] = NULL;
		return (new_cmd);
	}
	while ((*cmd)[i])
		i++;
	new_cmd = allocate_and_copy(*cmd, i + 2);
	if (!new_cmd)
		return (NULL);
	new_cmd[i] = ft_strdup(str);
	new_cmd[i + 1] = NULL;
	j = -1;
	while (++j < i)
		free((*cmd)[j]);
	free(*cmd);
	return (new_cmd);
}

void	handle_redirection(char *str, int *i, t_redir *command, char **envp)
{
	if (str[*i] == '<')
	{
		parse_left_redir(str, i, command, envp);
	}
	if (str[*i] == '>')
	{
		parse_right_redir(str, i, envp, command);
	}
	while (str[*i] && is_whitespace(str[*i]))
		(*i)++;
}
