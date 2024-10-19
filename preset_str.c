#include "main.h"

void handle_dollar(int *i, char **content, const char *str, char **envp)
{
    char *envp_var;
    char *envp_val;
    int idx;
    int start;

    (*i)++;
    start = (*i);
    while (is_envp_vars(str[*i]))
        (*i)++;
    envp_var = ft_substr(str, start, (*i) - start);
    idx = ft_strlen(envp_var) + 1;
    envp_val = ft_strdup(envp[search_env(envp, envp_var, 1)]);
    if (envp_val)
        (*content) = ft_strjoin_opts((*content), &envp_val[idx], 1);
    free(envp_val);
    free(envp_var);
}

static void set_heredoc_redir(int *i, char **tmp, char *str, int *start)
{
    while (is_whitespace(str[*i]))
        (*i)++;
    while (!is_whitespace(str[*i]) && str[*i])
        (*i)++;
    *tmp = ft_strjoin_opts(*tmp, ft_substr(str, *start, *i - *start), 3);
    *start = *i;
}

static void set_single_quotes(const char *str, char **temp, int *i)
{
    char *content;
    int start;

    start = (*i)++;
    while (str[*i] && str[*i] != '\'')
        (*i)++;
    if (str[*i] == '\'')
        (*i)++;
    content = ft_substr(str, start, *i - start);
    *temp = ft_strjoin_opts(*temp, content, 3);
}

static void set_double_quotes(const char *str, char **temp, int *i, char **envp)
{
    int start;

    start = *i;
    (*i)++;
    while (str[*i] && str[*i] != '"')
    {
        if (str[*i] == '$' && str[*i + 1] &&
            (ft_isalpha(str[*i + 1]) || str[*i + 1] == '?'))
        {
            *temp = ft_strjoin_opts(*temp,
                                    ft_substr(str, start, *i - start), 3);
            handle_dollar(i, temp, str, envp);
            start = *i;
        }
        else
            (*i)++;
    }
    if (str[*i] == '"')
        (*i)++;
    *temp = ft_strjoin_opts(*temp, ft_substr(str, start, *i - start), 3);
}



static void set_left_redir(int *i, char **tmp, char *str, int *start)
{
    (*i)++;
    if (str[*i] == '<')
    {
        (*i)++;
        set_heredoc_redir(i, tmp, str, start);
    }
    while (str[*i] && is_whitespace(str[*i]))
        (*i)++;
    if (str[*i] == '$')
    {
        while (str[*i] && !is_whitespace(str[*i]))
            (*i)++;
    }
    else
        return;
}

static void set_right_redir(int *i, char *str)
{
    (*i)++;
    if (str[*i] == '>')
        (*i)++;
    while (str[*i] && is_whitespace(str[*i]))
        (*i)++;
    if (str[*i] == '$')
    {
        while (str[*i] && !is_whitespace(str[*i]))
            (*i)++;
    }
    else
        return;
}

static void set_redir(int *i, char **tmp, char *str, int *start)
{
    if (str[*i] == '<')
    {
        set_left_redir(i, tmp, str, start);
    }
    else if (str[*i] == '>')
    {
        set_right_redir(i, str);
    }
}

static void set_quotes_and_dollar(const char *str, int *i, char **temp, char **envp)
{
    if (str[*i] == '\'')
    {
        set_single_quotes(str, temp, i);
    }
    else if (str[*i] == '"')
    {
        set_double_quotes(str, temp, i, envp);
    }
}

static void set_str_vars(int *i, int *si, char **tmp)
{
    *i = 0;
    *si = 0;
    *tmp = ft_strdup("");
}

char *set_str(char *str, char **envp)
{
    char *tmp;
    int i;
    int si;

    set_str_vars(&i, &si, &tmp);
    while (str[i])
    {
        if (str[i] == '\'' || str[i] == '"')
        {
            tmp = ft_strjoin_opts(tmp, ft_substr(str, si, i - si), 3);
            set_quotes_and_dollar(str, &i, &tmp, envp);
            si = i;
        }
        else if (str[i] == '<' || str[i] == '>')
            set_redir(&i, &tmp, str, &si);
        else
            i++;
    }
    tmp = ft_strjoin_opts(tmp, ft_substr(str, si, i - si), 3);
    return (tmp);
}