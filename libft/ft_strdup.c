/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chunpark <chunpark@student.42gyeongsan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/08 14:44:58 by chunpark          #+#    #+#             */
/*   Updated: 2024/10/08 14:44:59 by chunpark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strdup(const char *str)
{
	char	*dup;
	size_t	len;

	if (!str)
		return (NULL);
	len = ft_strlen (str) + 1;
	dup = (char *)malloc(len);
	if (dup)
		ft_memcpy(dup, str, len);
	return (dup);
}
