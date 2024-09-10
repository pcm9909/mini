#include "main.h"

// char	*parse_env(char *str)
// {
// 	char	*env_val;
// 	int		i;
// 	while(str[i] != '=')
// 	return (env_val);
// }

char *handle_single_quotes(const char *str, int *i)
{
    int		start;
	char	*content;
	char	*env_var;
	int		j;

	start = ++(*i);
    while (str[*i] && str[*i] != '\'')
        (*i)++;
    if (str[*i] != '\'')
    {
        fprintf(stderr, "Error: Unmatched single quote\n");
		//g_verification = 1;
		return NULL;
    }
    content = ft_substr(str, start, *i - start);
    (*i)++;
    return content;
}

static int	is_envp_vars(int c)
{
	if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_'))
	{
		return (1);
	}
	else
		return (0);
}

char *handle_double_quotes(const char *str, int *i, char **envp)
{
    int start = ++(*i);
    char *temp;
    char *envp_var; // $ 뒤에 나오는 것들에 대해서
    char *envp_val; // envp가 있으면 저장될 변수
    char *content; // 최종 return 될 변수

    content = ft_strdup("");
    while (str[*i] && str[*i] != '"')
    {
        if (str[*i] == '$')
        {
            temp = ft_substr(str, start, (*i) - start);
            content = ft_strjoin_with_free(content, temp);
            free(temp);
            (*i)++;
            start = (*i);
            while (is_envp_vars(str[*i]))
                (*i)++;
            envp_var = ft_substr(str, start, (*i) - start);
            envp_val = search_env(envp, envp_var, 1);
            free(envp_var);
            content = ft_strjoin_with_free(content, envp_val);
            free(envp_val);
            start = (*i);
        }
        else
        {
            (*i)++;
        }
    }
    if (str[*i] != '"')
    {
        fprintf(stderr, "Error: Unmatched double quote\n");
        free(content);
        return NULL;
    }
    else
    {
        temp = ft_substr(str, start, (*i) - start);
        content = ft_strjoin_with_free(content, temp);
        free(temp);
    }
    (*i)++;
    return content;
}
