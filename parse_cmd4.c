/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd4.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chunpark <chunpark@student.42gyeongsan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 22:09:22 by chunpark          #+#    #+#             */
/*   Updated: 2024/10/23 22:09:24 by chunpark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static char	*handle_single_quotes2(const char *str, int *i)
{
	char	*content;
	int		start;

	start = ++(*i);
	while (str[*i] && str[*i] != '\'')
		(*i)++;
	content = ft_substr(str, start, *i - start);
	(*i)++;
	return (content);
}

static char	*handle_double_quotes2(const char *str, int *i)
{
	char	*content;
	int		start;

	start = ++(*i);
	while (str[*i] && str[*i] != '"')
	{
		(*i)++;
	}
	content = ft_strdup("");
	content = ft_strjoin_opt(content, \
								ft_substr(str, start, (*i) - start), 3);
	(*i)++;
	return (content);
}

static char	*handle_quotes2(const char *str, int *i)
{
	char	*temp;

	if (str[*i] == '"')
	{
		temp = handle_double_quotes2(str, i);
	}
	else
	{
		temp = handle_single_quotes2(str, i);
	}
	return (temp);
}

static char	*handle_command2(const char *str, int *i)
{
	char	*val;
	int		start;

	start = (*i);
	val = ft_strdup("");
	while (str[*i] && str[*i] != '\'' && str[*i] != '"')
	{
		if (str[*i] != ' ')
			(*i)++;
		else
			break ;
	}
	val = ft_strjoin_opt(val, ft_substr(str, start, (*i) - start), 3);
	return (val);
}

char	*get_cmd_value(char *str, int *i)
{
	char	*temp;

	temp = ft_strdup("");
	while (str[*i])
	{
		if (str[*i] == '"' || str[*i] == '\'')
			temp = ft_strjoin_opt(temp, handle_quotes2(str, i), 3);
		else
			temp = ft_strjoin_opt(temp, handle_command2(str, i), 3);
	}
	return (temp);
}
