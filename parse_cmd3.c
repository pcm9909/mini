/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chunpark <chunpark@student.42gyeongsan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 18:09:47 by chunpark          #+#    #+#             */
/*   Updated: 2024/10/21 22:13:50 by chunpark         ###   ########.fr       */
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

char	*handle_double_quotes(const char *str, int *i, t_redir *command, char **envp)
{
	char	*content;
	char	*temp;
	int		start;
	int		j;

	content = ft_strdup("");
	start = (*i);
	(*i)++;
	while (str[*i] && str[*i] != '"')
		(*i)++;
	if (str[*i] != '"')
		return (print_qutoes_error(command));
	else
	{
		content = ft_strdup("");
		content = ft_strjoin_opts(content, \
									ft_substr(str, start, (*i) - start + 1), 3);
	}
	(*i)++;
	temp = ft_strdup("");
	j = 0;
	start = j++;
	printf("content = %s\n", content);
	while(content[j])
	{
		if(str[j] == '$')
		{
			printf("in~~\n");
			temp = ft_strjoin_opts(content, ft_substr(temp, start, j - start), 3);
			if(str[j + 1] && !is_whitespace(str[j + 1]))
				temp = ft_strjoin(temp, handle_env(i, content, envp));
			else if (str[j + 1] && (is_whitespace(str[j + 1]) || str[j + 1] == '"'))
			{
				temp = ft_strjoin_opts(temp, "$", 1);
				j++;
			}
			start = j;
		}
		else
			j++;
	}
	return (content);
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
	int		idx;
	int		start;

	(*i)++;
	start = (*i);
	while (is_envp_vars(str[*i]))
	{
		if(str[*i] == '?')
		{
			(*i)++;
			break;
		}
		(*i)++;
	}
	envp_var = ft_substr(str, start, (*i) - start);
	envp_val = ft_strdup(envp[search_env(envp, envp_var, 1)]);
	if (ft_strlen(envp_val))
	{
		idx = ft_strlen(envp_var) + 1;
		temp = ft_strdup(&envp_val[idx]);
		free(envp_var);
		free(envp_val);
	}
	return temp;
}

char	*handle_command(const char *str, int *i)
{
	char	*val;
	int		start;

	start = (*i);
	val = ft_strdup("");
	while (str[*i] && !is_whitespace(str[*i]) && str[*i] != '>' && \
			str[*i] != '<' && str[*i] != '\'' && str[*i] != '"'  && str[*i] != '$' )
	{
		if (str[*i] != ' ')
			(*i)++;
		else
			break ;
	}
	val = ft_strjoin_opts(val, ft_substr(str, start, (*i) - start), 3);
	return (val);
}
