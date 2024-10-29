/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   validate_utils.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chunpark <chunpark@student.42gyeongsan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 18:10:02 by chunpark          #+#    #+#             */
/*   Updated: 2024/10/23 21:43:59 by chunpark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

int	is_upright_vars(int c)
{
	return (c == '>' || c == '<' || c == '\0' || \
			c == '|' || c == '&' || c == ';');
}

int	is_whitespace(int c)
{
	return ((c >= 9 && c <= 13) || c == 32);
}

int	is_envp_vars(int c)
{
	if (c && (ft_isalnum(c) || c == '_' || c == '?'))
		return (1);
	else
		return (0);
}

int	is_space(int c)
{
	return (c == ' ' || (c >= '\t' && c <= '\r'));
}
