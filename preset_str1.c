/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   preset_str1.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chunpark <chunpark@student.42gyeongsan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 12:05:36 by chunpark          #+#    #+#             */
/*   Updated: 2024/10/25 23:22:27 by chunpark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static void	set_single_quotes(const char *str, char **temp, int *i)
{
	char	*content;
	int		start;

	start = (*i)++;
	while (str[*i] && str[*i] != '\'')
		(*i)++;
	if (str[*i] == '\'')
		(*i)++;
	content = ft_substr(str, start, *i - start);
	*temp = ft_strjoin_opt(*temp, content, 3);
}

static void	set_double_quotes(const char *str, char **temp, int *i)
{
	int	start;

	start = *i;
	(*i)++;
	while (str[*i] && str[*i] != '"')
		(*i)++;
	if (str[*i] == '"')
		(*i)++;
	*temp = ft_strjoin_opt(*temp, ft_substr(str, start, *i - start), 3);
}

static void	set_quotes(const char *str, \
							int *i, char **temp)
{
	if (str[*i] == '\'')
	{
		set_single_quotes(str, temp, i);
	}
	else if (str[*i] == '"')
	{
		set_double_quotes(str, temp, i);
	}
}

static void	set_str_vars(int *i, int *si, char **tmp)
{
	*i = 0;
	*si = 0;
	*tmp = ft_strdup("");
}

char	*set_str(char *str)
{
	char	*tmp;
	int		i;
	int		si;
	char	*re;

	set_str_vars(&i, &si, &tmp);
	while (str && str[i])
	{
		if (str[i] == '\'' || str[i] == '"')
		{
			tmp = ft_strjoin_opt(tmp, ft_substr(str, si, i - si), 3);
			set_quotes(str, &i, &tmp);
			si = i;
		}
		else if (str[i] == '<' || str[i] == '>')
			set_redir(&i, &tmp, str, &si);
		else
			i++;
	}
	re = ft_strjoin_opt(tmp, ft_substr(str, si, i - si), 2);
	free(tmp);
	return (re);
}
