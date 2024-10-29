/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_expans3.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jakim <jakim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/24 16:10:24 by jakim             #+#    #+#             */
/*   Updated: 2024/10/24 16:17:27 by jakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static void	skip_word(const char *s, int *i, char c, t_split_vars *state)
{
	while (s[*i] && (s[*i] != c || state->in_quotes))
	{
		set_quotes_state(s[*i], state);
		(*i)++;
	}
}

void	set_quotes_state(char c, t_split_vars *state)
{
	if ((c == '"' || c == '\'') && \
		(state->in_quotes == 0 || c == state->quote_char))
	{
		state->in_quotes = !state->in_quotes;
		if (state->in_quotes)
			state->quote_char = c;
		else
			state->quote_char = '\0';
	}
}

int	ft_count_wordss(const char *s, char c)
{
	t_split_vars	state;
	int				i;
	int				count;

	i = 0;
	count = 0;
	state.in_quotes = 0;
	state.quote_char = '\0';
	while (s[i])
	{
		set_quotes_state(s[i], &state);
		if (s[i] != c || state.in_quotes)
		{
			count++;
			skip_word(s, &i, c, &state);
		}
		else
			i++;
	}
	return (count);
}
