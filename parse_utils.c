/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chunpark <chunpark@student.42gyeongsan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 18:09:51 by chunpark          #+#    #+#             */
/*   Updated: 2024/10/25 23:43:36 by chunpark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
			tmp = ft_strjoin_opt(tmp, ft_substr(str, j, *i - j), 3);
			tmp = ft_strjoin_opt(tmp, handle_quotes(str, i, cmd, envp), 3);
			j = *i;
		}
		else if (str[*i] == '$' && str[*i + 1] && !is_whitespace(str[*i + 1]))
		{
			tmp = ft_strjoin_opt(tmp, ft_substr(str, j, *i - j), 3);
			if (handle_redir_dollar(i, &tmp, str, envp))
				cmd->executable = false;
			j = *i;
		}
		else
			(*i)++;
	}
	tmp = ft_strjoin_opt(tmp, ft_substr(str, j, *i - j), 3);
	return (tmp);
}

void	free_readline(char *cwd, char *tmp, char *str)
{
	free(cwd);
	if (tmp != str && tmp != NULL)
		free(tmp);
	free(str);
}

char	*extract_env_var(int *i, const char *str)
{
	int	start;

	start = *i;
	while (is_envp_vars(str[*i]))
	{
		if (str[*i] == '?')
		{
			(*i)++;
			break ;
		}
		(*i)++;
	}
	return (ft_substr(str, start, (*i) - start));
}

int	append_str(char **tmp, char *str)
{
	*tmp = readline(">");
	if (*tmp == NULL)
	{
		free(str);
		return (0);
	}
	*tmp = ft_strjoin_opt(str, *tmp, 3);
	if (*tmp != NULL)
		add_history(*tmp);
	return (1);
}
