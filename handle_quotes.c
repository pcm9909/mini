#include "main.h"

int	is_envp_vars(int c)
{
	if (!is_whitespace(c) && c && ft_isalnum(c))
	{
		return (1);
	}
	else
		return (0);
}

char	*print_qutoes_error(const char *str, \
								t_redir *command, char **content)
{
	write(2, "minishell: Error: Unmatched quote\n", \
			ft_strlen("minishell: Error: Unmatched quote\n"));
	command->executable = false;
	free(*content);
	return (NULL);
}

char	*handle_single_quotes(const char *str, int *i, t_redir *command)
{
	int		start;
	char	*content;

	start = ++(*i);
	content = ft_strdup("");
	while (str[*i] && str[*i] != '\'')
		(*i)++;
	if (str[*i] != '\'')
		return (print_qutoes_error(str, command, &content));
	content = ft_substr(str, start, *i - start);
	(*i)++;
	return (content);
}

void	handle_dollar(int *i, char **content, const char *str, char **envp)
{
	char	*temp;
	char	*envp_var;
	char	*envp_val;
	int		idx;
	int		start;

	(*i)++;
	start = (*i);
	if (ft_isalpha(str[*i]) || str[*i] == '_')
	{
		while (is_envp_vars(str[*i]))
			(*i)++;
	}
	else
		(*i)++;
	envp_var = ft_substr(str, start, (*i) - start);
	idx = ft_strlen(envp_var) + 1;
	envp_val = ft_strdup(envp[search_env(envp, envp_var, 1)]);
	if (envp_val)
		(*content) = ft_strjoin_opts((*content), &envp_val[idx], 1);
	free(envp_val);
	free(envp_var);
}

char	*handle_double_quotes(const char *str, int *i, \
							char **envp, t_redir *command)
{
	int		start;
	char	*temp;
	char	*content;

	start = ++(*i);
	content = ft_strdup("");
	while (str[*i] && str[*i] != '"')
	{
		(*i)++;
	}
	if (str[*i] != '"')
		return (print_qutoes_error(str, command, &content));
	else
		content = ft_strjoin_opts(content, \
					ft_substr(str, start, (*i) - start), 3);
	(*i)++;
	return (content);
}

char	*handle_command(const char *str, int *i, char **envp)
{
	int		start;
	char	*val;

	start = (*i);
	val = ft_strdup("");
	while (str[*i] && !is_whitespace(str[*i]) && str[*i] != '>' && \
			str[*i] != '<' && str[*i] != '"' && str[*i] != '\'')
	{
		if (str[*i] != ' ')
			(*i)++;
		else
			break ;
	}
	val = ft_strjoin_opts(val, ft_substr(str, start, (*i) - start), 3);
	return (val);
}
