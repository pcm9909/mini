/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit2.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chunpark <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 18:09:19 by chunpark          #+#    #+#             */
/*   Updated: 2024/10/21 18:09:20 by chunpark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static long long	ft_atoll(const char *str)
{
	int			i;
	long long	sign;
	long long	result;

	i = 0;
	sign = 1;
	result = 0;
	while (is_space((unsigned char)str[i]))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			sign = -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = result * 10 + (str[i] - '0');
		i++;
	}
	return (result * sign);
}

static int	ft_count_len1(long long n)
{
	long long	temp;
	int			len;

	if (n == 0)
		return (1);
	temp = n;
	len = 0;
	while (temp)
	{
		len++;
		temp /= 10;
	}
	return (len);
}

static char	*ft_lltoa(long long n)
{
	char		*str;
	long long	sign;
	int			len;

	sign = 1;
	len = ft_count_len1(n);
	if (n < 0)
	{
		sign = -1;
		len++;
	}
	str = (char *)malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	str[len] = '\0';
	while (len--)
	{
		str[len] = n % 10 * sign + '0';
		n /= 10;
	}
	if (sign == -1)
		str[0] = '-';
	return (str);
}

int	exit_range(char *ptr)
{
	char	*tmp;

	tmp = ft_lltoa(ft_atoll(ptr));
	if (*ptr == '+')
		ptr++;
	if (ft_strncmp(tmp, ptr, ft_strlen(ptr)))
	{
		free(tmp);
		return (1);
	}
	free(tmp);
	return (0);
}
