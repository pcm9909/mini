/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strdup.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chunpark <chunpark@student.42gyeongsan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/29 11:23:33 by chunpark          #+#    #+#             */
/*   Updated: 2024/09/10 17:41:56 by chunpark         ###   ########.fr       */
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
