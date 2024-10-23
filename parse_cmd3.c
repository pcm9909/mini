/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chunpark <chunpark@student.42gyeongsan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 18:09:47 by chunpark          #+#    #+#             */
/*   Updated: 2024/10/23 12:07:32 by chunpark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

char	*handle_single_quotes(const char *str, int *i, t_redir *command)
{
	char	*content;
	int		start;

	start = (*i);
	(*i)++;
	while (str[*i] && str[*i] != '\'')
		(*i)++;
	if (str[*i] != '\'')
		return (print_qutoes_error(command));
	content = ft_substr(str, start, *i - start + 1);
	(*i)++;
	return (content);
}

static void	handle_quotes_dollar_sign(int *i, char **temp, \
									const char *str, char **envp)
{
	if (str[*i + 1] && str[*i + 1] != '$' && !is_whitespace(str[*i + 1]))
	{
		*temp = ft_strjoin_opts(*temp, handle_env(i, str, envp), 3);
	}
	else
	{
		while (str[*i] && str[*i] == '$')
		{
			*temp = ft_strjoin_opts(*temp, "$", 1);
			(*i)++;
		}
	}
}

char	*handle_double_quotes(const char *str, int *i, \
							t_redir *command, char **envp)
{
	char	*temp;
	int		start;
	int		j;

	start = (*i);
	(*i)++;
	temp = ft_strdup("");
	while (str[*i] && str[*i] != '"')
	{
		if (str[*i] == '$')
		{
			temp = ft_strjoin_opts(temp, \
						ft_substr(str, start, (*i) - start), 3);
			handle_quotes_dollar_sign(i, &temp, str, envp);
			start = *i;
		}
		else
			(*i)++;
	}
	if (str[*i] != '"')
		return (print_qutoes_error(command));
	temp = ft_strjoin_opts(temp, ft_substr(str, start, (*i) - start + 1), 3);
	(*i)++;
	return (temp);
}

void	append_env(char *envp_var, char *envp_val, t_redir *cmd)
{
	char	**envp_val_split;
	char	*temp;
	int		idx;
	int		j;

	j = 0;
	idx = ft_strlen(envp_var) + 1;
	temp = ft_strdup(&envp_val[idx]);
	envp_val_split = split_cmp_quotes(temp, ' ');
	while (envp_val_split[j])
	{
		cmd->cmd->exist = true;
		cmd->cmd->cmd_val = \
			append_command(&cmd->cmd->cmd_val, envp_val_split[j]);
		j++;
	}
	all_free(envp_val_split);
	free(temp);
}

char	*handle_env(int *i, const char *str, char **envp)
{
	char	*envp_var;
	char	*envp_val;
	char	*temp;
	int		start;

	(*i)++;
	start = (*i);
	envp_var = extract_env_var(i, str);
	envp_val = ft_strdup(envp[search_env(envp, envp_var, 1)]);
	if (ft_strlen(envp_val))
		temp = ft_strdup(&envp_val[ft_strlen(envp_var) + 1]);
	else
		temp = ft_strdup("");
	free(envp_var);
	free(envp_val);
	return (temp);
}

char	*handle_command(const char *str, int *i)
{
	char	*val;
	int		start;

	start = (*i);
	val = ft_strdup("");
	while (str[*i] && !is_whitespace(str[*i]) && str[*i] != '>' && \
		str[*i] != '<' && str[*i] != '\'' && str[*i] != '"' && str[*i] != '$')
	{
		if (str[*i] != ' ')
			(*i)++;
		else
			break ;
	}
	val = ft_strjoin_opts(val, ft_substr(str, start, (*i) - start), 3);
	return (val);
}
