/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chunpark <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 11:17:41 by chunpark          #+#    #+#             */
/*   Updated: 2024/02/29 15:52:14 by chunpark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

int	ft_count_words(const char *s, char c)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	while (s[i])
	{
		if (s[i] != c)
		{
			count++;
			while (s[i] && s[i] != c)
				i++;
		}
		else
			i++;
	}
	return (count);
}

char	**ft_free_arr(char **arr, size_t i)
{
	while (i > 0)
	{
		free(arr[i]);
		i--;
	}
	free (arr);
	return (NULL);
}

char	**ft_split(char const *s, char c)
{
	char	**arr;
	int		i;
	int		j;
	int		k;

	if (!s)
		return (NULL);
	arr = (char **)malloc(sizeof(char *) * (ft_count_words(s, c) + 1));
	if (!arr)
		return (NULL);
	i = -1;
	j = 0;
	while (++i < ft_count_words(s, c))
	{
		while (s[j] == c)
			j++;
		k = j;
		while (s[j] && s[j] != c)
			j++;
		arr[i] = ft_substr (s, k, j - k);
		if (!arr[i])
			return (ft_free_arr(arr, i));
	}
	arr[i] = NULL;
	return (arr);
}
