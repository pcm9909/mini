/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chunpark <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 11:10:12 by chunpark          #+#    #+#             */
/*   Updated: 2024/02/29 11:10:44 by chunpark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
#include "libft.h"

void	*ft_memset(void *ptr, int value, size_t len)
{
	unsigned char	*dest;
	size_t			i;

	dest = (unsigned char *)ptr;
	i = 0;
	while (i < len)
	{
		dest[i] = (unsigned char)value;
		i++;
	}
	return (ptr);
}
