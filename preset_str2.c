#include "main.h"

void	handle_dollar(int *i, char **content, const char *str, char **envp)
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
	free(envp_val);
	free(envp_var);
}

static void	set_heredoc_redir(int *i, char **tmp, char *str, int *start)
{
	while (is_whitespace(str[*i]))
		(*i)++;
	while (!is_whitespace(str[*i]) && str[*i])
		(*i)++;
	*tmp = ft_strjoin_opts(*tmp, ft_substr(str, *start, *i - *start), 3);
	*start = *i;
}

static void	set_left_redir(int *i, char **tmp, char *str, int *start)
{
	(*i)++;
	if (str[*i] == '<')
	{
		(*i)++;
		set_heredoc_redir(i, tmp, str, start);
	}
	while (str[*i] && is_whitespace(str[*i]))
		(*i)++;
	if (str[*i] == '$')
	{
		while (str[*i] && !is_whitespace(str[*i]))
			(*i)++;
	}
	else
		return ;
}

static void	set_right_redir(int *i, char *str)
{
	(*i)++;
	if (str[*i] == '>')
		(*i)++;
	while (str[*i] && is_whitespace(str[*i]))
		(*i)++;
	if (str[*i] == '$')
	{
		while (str[*i] && !is_whitespace(str[*i]))
			(*i)++;
	}
	else
		return ;
}

void	set_redir(int *i, char **tmp, char *str, int *start)
{
	if (str[*i] == '<')
	{
		set_left_redir(i, tmp, str, start);
	}
	else if (str[*i] == '>')
	{
		set_right_redir(i, str);
	}
}
