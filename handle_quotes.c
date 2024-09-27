#include "main.h"

int	is_envp_vars(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || \
		(c == '_') || (c >= '0' && c <= '9'))
	{
		return (1);
	}
	else
		return (0);
}

char	*print_qutoes_error(const char *str, \
								t_redirection *command, char **content)
{
	write(2, "Error: Unmatched quote\n", \
			ft_strlen("Error: Unmatched quote\n"));
	command->executable = false;
	free(*content);
	return (NULL);
}

char	*handle_single_quotes(const char *str, int *i, t_redirection *command)
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
	while (is_envp_vars(str[*i]))
		(*i)++;
	envp_var = ft_substr(str, start, (*i) - start);
	idx = ft_strlen(envp_var) + 1;
	envp_val = ft_strdup(envp[search_env(envp, envp_var, 1)]);
	free(envp_var);
	if (envp_val)
		(*content) = ft_strjoin_with_free((*content), &envp_val[idx]);
	free(envp_val);
}

char	*handle_double_quotes(const char *str, int *i, \
							char **envp, t_redirection *command)
{
	int		start;
	char	*temp;
	char	*content;

	start = ++(*i);
	content = ft_strdup("");
	while (str[*i] && str[*i] != '"' && str[*i] != ' ')
	{
		if (str[*i] == '$')
		{
			content = ft_strjoin_with_free2(content, \
							ft_substr(str, start, (*i) - start));
			handle_dollar(i, &content, str, envp);
			start = (*i);
		}
		else
			(*i)++;
	}
	if (str[*i] != '"')
		return (print_qutoes_error(str, command, &content));
	else
		content = ft_strjoin_with_free2(content, \
					ft_substr(str, start, (*i) - start));
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
		if (str[*i] == '$')
		{
			val = ft_strjoin_with_free2(val, \
						ft_substr(str, start, (*i) - start));
			handle_dollar(i, &val, str, envp);
			start = (*i);
		}
		else
		{
			if (str[*i] != ' ')
				(*i)++;
			else
				break ;
		}
	}
	val = ft_strjoin_with_free2(val, ft_substr(str, start, (*i) - start));
	return (val);
}
