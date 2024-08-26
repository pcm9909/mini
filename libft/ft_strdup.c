/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chunpark <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 11:23:33 by chunpark          #+#    #+#             */
/*   Updated: 2024/02/29 11:24:01 by chunpark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

char	*ft_strdup(const char *str)
{
	char	*dup;
	size_t	len;

	len = ft_strlen (str) + 1;
	dup = (char *)malloc(len);
	if (dup)
		ft_memcpy(dup, str, len);
	return (dup);
}
