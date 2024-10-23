/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_expans2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chunpark <chunpark@student.42gyeongsan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 18:09:42 by chunpark          #+#    #+#             */
/*   Updated: 2024/10/23 21:45:35 by chunpark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

char	*ft_strrev(char *str)
{
	char	*rev;
	int		i;
	int		len;

	i = 0;
	len = ft_strlen(str);
	rev = malloc(len + 1);
	rev[len] = '\0';
	while (str[i])
	{
		rev[i] = str[len - i - 1];
		i++;
	}
	return (rev);
}

char	*ft_strjoin_opt(char const *s1, char const *s2, int i)
{
	char	*str;
	size_t	len;

	if (!s1 || !s2)
		return (NULL);
	len = ft_strlen(s1) + ft_strlen(s2);
	str = (char *)malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	ft_strlcpy(str, s1, ft_strlen(s1) + 1);
	ft_strlcat(str, s2, len + 1);
	if (i == 1 || i == 3)
		free((char *)s1);
	if (i == 2 || i == 3)
		free((char *)s2);
	return (str);
}

char	**ft_strdups(char **source)
{
	char	**dup;
	int		cnt;
	int		i;

	i = 0;
	cnt = 0;
	while (source[cnt])
		cnt++;
	dup = malloc(sizeof(char *) * (cnt + 1));
	if (!dup)
		return (NULL);
	dup[cnt] = NULL;
	while (source[i])
	{
		dup[i] = ft_strdup(source[i]);
		i++;
	}
	return (dup);
}
