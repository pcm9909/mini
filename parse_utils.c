#include "main.h"

void	set_extract_val(int *j, char **tmp, int *i)
{
	*j = *i;
	*tmp = ft_strdup("");
}

char	*extract_content(const char *str, int *i, char **envp, t_redir *cmd)
{
	char	*tmp;
	int		j;

	set_extract_val(&j, &tmp, i);
	while (str[*i] && !is_whitespace(str[*i]) && \
				str[*i] != '>' && str[*i] != '<')
	{
		if (str[*i] == '"' || str[*i] == '\'')
		{
			tmp = ft_strjoin_opts(tmp, \
ft_strjoin_opts(ft_substr(str, j, *i - j), handle_quotes(str, i, cmd), 3), 3);
			j = *i;
		}
		else if (str[*i] == '$' && str[*i + 1] && !is_whitespace(str[*i + 1]))
		{
			tmp = ft_strjoin_opts(tmp, ft_substr(str, j, *i - j), 3);
			if (handle_dollar1(i, &tmp, str, envp))
				cmd->executable = false;
			j = *i;
		}
		else
			(*i)++;
	}
	tmp = ft_strjoin_opts(tmp, ft_substr(str, j, *i - j), 3);
	return (tmp);
}
