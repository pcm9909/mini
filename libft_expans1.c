/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_expans1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chunpark <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 18:09:41 by chunpark          #+#    #+#             */
/*   Updated: 2024/10/21 18:09:41 by chunpark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static int	ft_count_wordss(const char *s, char c)
{
	int	i;
	int	count;
	int	in_quotes;

	i = 0;
	count = 0;
	in_quotes = 0;
	while (s[i])
	{
		if (s[i] == '"' || s[i] == '\'')
			in_quotes = !in_quotes;
		if (s[i] != c || in_quotes)
		{
			count++;
			while (s[i] && (s[i] != c || in_quotes))
			{
				if (s[i] == '"' || s[i] == '\'')
					in_quotes = !in_quotes;
				i++;
			}
		}
		else
			i++;
	}
	return (count);
}

static char	**ft_free_arrs(char **arr, size_t i)
{
	while (i > 0)
	{
		free(arr[i]);
		i--;
	}
	free(arr);
	return (NULL);
}

static void	set_splits_vars(int *i, int *j, int *in_quotes)
{
	(*i) = -1;
	(*j) = 0;
	(*in_quotes) = 0;
}

static void	set_in_quotes(const char *s, int *j, char c, int in_quotes)
{
	while (s[*j] && (s[*j] != c || in_quotes))
	{
		if (s[*j] == '"' || s[*j] == '\'')
			in_quotes = !in_quotes;
		(*j)++;
	}
}

char	**split_cmp_quotes(char const *s, char c)
{
	char	**arr;
	int		in_quotes;
	int		i;
	int		j;
	int		k;

	if (!s)
		return (NULL);
	arr = (char **)malloc(sizeof(char *) * (ft_count_wordss(s, c) + 1));
	if (!arr)
		return (NULL);
	set_splits_vars(&i, &j, &in_quotes);
	while (++i < ft_count_wordss(s, c))
	{
		while (s[j] == c && !in_quotes)
			j++;
		k = j;
		set_in_quotes(s, &j, c, in_quotes);
		arr[i] = ft_substr(s, k, j - k);
		if (!arr[i])
			return (ft_free_arrs(arr, i));
	}
	arr[i] = NULL;
	return (arr);
}
