/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chunpark <chunpark@student.42gyeongsan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 21:46:11 by chunpark          #+#    #+#             */
/*   Updated: 2024/09/11 22:37:28 by chunpark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

int	is_envp_vars(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_') || (c >= '0' && c <= '9'))
	{
		return (1);
	}
	else
		return (0);
}

char	*handle_single_quotes(const char *str, int *i)
{
	int		start;
	char	*content;

	start = ++(*i);
	while (str[*i] && str[*i] != '\'')
		(*i)++;
	if (str[*i] != '\'')
	{
		fprintf(stderr, "Error: Unmatched single quote\n");
		return (NULL);
	}
	content = ft_substr(str, start, *i - start);
	(*i)++;
	return (content);
}

char	*handle_double_quotes(const char *str, int *i, char **envp)
{
	int		start;
	int		idx;
	char	*temp;
	char	*content;
	char	*envp_var;
	char	*envp_val;

	start = ++(*i);
	content = ft_strdup("");
	while (str[*i] && str[*i] != '"')
	{
		if (str[*i] == '$')
		{
			temp = ft_substr(str, start, (*i) - start);
			content = ft_strjoin_with_free(content, temp);
			free(temp);
			(*i)++;
			start = (*i);
			while (is_envp_vars(str[*i]))
				(*i)++;
			envp_var = ft_substr(str, start, (*i) - start);
			idx = ft_strlen(envp_var) + 1;
			envp_val = ft_strdup(envp[search_env(envp, envp_var, 1)]);
			free(envp_var);
			if (envp_val)
				content = ft_strjoin_with_free(content, &envp_val[idx]);
			free(envp_val);
			start = (*i);
		}
		else
		{
			(*i)++;
		}
	}
	if (str[*i] != '"')
	{
		fprintf(stderr, "Error: Unmatched double quote\n");
		free(content);
		return (NULL);
	}
	else
	{
		temp = ft_substr(str, start, (*i) - start);
		content = ft_strjoin_with_free(content, temp);
		free(temp);
	}
	(*i)++;
	return (content);
}

char	*handle_command(const char *str, int *i, char **envp)
{
	int		start;
	int		idx;
	char	*temp;
	char	*content;
	char	*envp_var;
	char	*envp_val;

	start = (*i);
	content = ft_strdup("");
	while (str[*i] && !is_whitespace(str[*i]) && str[*i] != '>' && str[*i] != '<' && str[*i] != '"' && str[*i] != '\'')
	{
		if (str[*i] == '$')
		{
			temp = ft_substr(str, start, (*i) - start);
			content = ft_strjoin_with_free(content, temp);
			free(temp);
			(*i)++;
			start = (*i);
			while (is_envp_vars(str[*i]))
				(*i)++;
			envp_var = ft_substr(str, start, (*i) - start);
			idx = ft_strlen(envp_var) + 1;
			envp_val = ft_strdup(envp[search_env(envp, envp_var, 1)]);
			free(envp_var);
			if (envp_val)
				content = ft_strjoin_with_free(content, &envp_val[idx]);
			free(envp_val);
			start = (*i);
		}
		else
		{
			(*i)++;
		}
	}
	temp = ft_substr(str, start, (*i) - start);
	content = ft_strjoin_with_free(content, temp);
	free(temp);
	while (is_whitespace(str[*i]))
		(*i)++;
	return (content);
}
