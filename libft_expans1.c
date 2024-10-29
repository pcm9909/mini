/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_expans1.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jakim <jakim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 18:09:41 by chunpark          #+#    #+#             */
/*   Updated: 2024/10/24 16:19:12 by jakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

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

static void	set_splits_vars(t_split_vars *vars)
{
	vars->i = -1;
	vars->j = 0;
	vars->in_quotes = 0;
	vars->quote_char = '\0';
}

void	set_in_quotes(const char *s, int *j, char c, t_split_vars *vars)
{
	while (s[*j] && (s[*j] != c || vars->in_quotes))
	{
		set_quotes_state(s[*j], vars);
		(*j)++;
	}
}

static int	process_split(char const *s, char c, char **arr, t_split_vars *vars)
{
	int	k;

	while (s[vars->j] == c && !vars->in_quotes)
		vars->j++;
	k = vars->j;
	set_in_quotes(s, &vars->j, c, vars);
	arr[vars->i] = ft_substr(s, k, vars->j - k);
	if (!arr[vars->i])
		return (0);
	return (1);
}

char	**split_cmp_quotes(char const *s, char c)
{
	t_split_vars	vars;
	char			**arr;

	if (!s)
		return (NULL);
	arr = (char **)malloc(sizeof(char *) * (ft_count_wordss(s, c) + 1));
	if (!arr)
		return (NULL);
	set_splits_vars(&vars);
	while (++vars.i < ft_count_wordss(s, c))
	{
		if (!process_split(s, c, arr, &vars))
			return (ft_free_arrs(arr, vars.i));
	}
	arr[vars.i] = NULL;
	return (arr);
}
