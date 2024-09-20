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
