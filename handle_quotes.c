#include "main.h"

static char *handle_env_variable(const char *str, int *i, char **envp, char *content)
{
    int start = *i;
    char *temp;
    char *envp_var;
    char *envp_val;

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
    return content;
}

char *handle_single_quotes(const char *str, int *i)
{
    int start;
    char *content;

    start = ++(*i);
    while (str[*i] && str[*i] != '\'')
        (*i)++;
    if (str[*i] != '\'')
    {
        fprintf(stderr, "Error: Unmatched single quote\n");
        return NULL;
    }
    content = ft_substr(str, start, *i - start);
    (*i)++;
    return content;
}

static int is_envp_vars(int c)
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
    char *content;

    content = ft_strdup("");
    while (str[*i] && str[*i] != '"')
    {
        if (str[*i] == '$')
        {
            content = handle_env_variable(str, i, envp, content);
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

char *handle_command(const char *str, int *i, char **envp)
{
    int start = *i;
    char *content;
    char *temp;

    content = ft_strdup(""); // content를 빈 문자열로 초기화
    while (str[*i] && !is_whitespace(str[*i]) && str[*i] != '>' && str[*i] != '<' && str[*i] != '"' && str[*i] != '\'')
    {
        if (str[*i] == '$')
        {
            content = handle_env_variable(str, i, envp, content);
            start = (*i);
        }
        else
        {
            (*i)++;
        }
    }
    temp = ft_substr(str, start, (*i) - start);
    content = ft_strjoin_with_free(content, temp);
    free(temp);
    return content;
}
