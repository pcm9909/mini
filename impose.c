#include "main.h"

void	print(t_redirection *cmd)
{
	int	i;

	i = 0;
	if (cmd->left_brace->command)
	{
		while (cmd->left_brace->command[i])
		{
			printf("[lb]\n");
			printf("%s\n", cmd->left_brace->command[i]);
			i++;
		}
	}
	i = 0;
	if (cmd->double_left_brace->command)
	{
		while (cmd->double_left_brace->command[i])
		{
			printf("[dlb]\n");
			printf("%s\n", cmd->double_left_brace->command[i]);
			i++;
		}
	}
	i = 0;
	if (cmd->command->command)
	{
		while (cmd->command->command[i])
		{
			printf("[cmd]\n");
			printf("%s\n", cmd->command->command[i]);
			i++;
		}
	}
	i = 0;
	if (cmd->right_brace->command)
	{
		while (cmd->right_brace->command[i])
		{
			printf("[rb]\n");
			printf("%s\n", cmd->right_brace->command[i]);
			i++;
		}
	}
	i = 0;
	if (cmd->double_right_brace->command)
	{
		while (cmd->double_right_brace->command[i])
		{
			printf("[drb]\n");
			printf("%s\n", cmd->double_right_brace->command[i]);
			i++;
		}
	}
}

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

// char	*ft_strdup2(const char *s, int len)
// {
// 	char	*mem;
// 	char	*ptr;
// 	char	*f;

// 	mem = NULL;
// 	mem = (char *)malloc(sizeof(char) * (len + 1));
// 	if (mem == NULL)
// 		return (mem);
// 	if (!s)
// 	{
// 		mem[0] = 0;
// 		return (mem);
// 	}
// 	f = (char *)s;
// 	ptr = mem;
// 	while (*s)
// 		*(ptr++) = *(char *)(s++);
// 	*ptr = 0;
// 	free(f);
// 	return (mem);
// }

int is_whitespace(int c)
{
    return ((c >= 9 && c <= 13) || c == 32);
}

int cnt_cmd(char **split)
{
    int i = 0;

    if (!split)
    {
        return 0;
    }
    while (split[i])
    {
        i++;
    }
    return i;
}