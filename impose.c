#include "main.h"

char	*ft_strrev(char *str)
{
	int		i;
	int		len;
	char	*rev;

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

void perror_exit(char *str)
{
	perror(str);
	exit(EXIT_FAILURE);
}

char	*ft_strjoin_with_free2(char const *s1, char const *s2)
{
	char	*str;
	size_t	len;

	if (!s1 || !s2)
		return (NULL);
	len = ft_strlen(s1) + ft_strlen(s2);
	str = (char *)malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	ft_strlcpy (str, s1, ft_strlen(s1) + 1);
	ft_strlcat (str, s2, len + 1);
	free((char *)s1);
	free((char *)s2);
	return (str);
}

char	*ft_strjoin_with_free(char const *s1, char const *s2)
{
	char	*str;
	size_t	len;

	if (!s1 || !s2)
		return (NULL);
	len = ft_strlen(s1) + ft_strlen(s2);
	str = (char *)malloc(sizeof(char) * (len + 1));
	if (!str)
		return (NULL);
	ft_strlcpy (str, s1, ft_strlen(s1) + 1);
	ft_strlcat (str, s2, len + 1);
	free((char *)s1);
	return (str);
}

int	is_whitespace(int c)
{
	return ((c >= 9 && c <= 13) || c == 32);
}

int	cnt_cmd(char **split)
{
	int	i;

	i = 0;
	if (!split)
	{
		return (0);
	}
	while (split[i])
	{
		i++;
	}
	return (i);
}
