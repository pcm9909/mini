#include "main.h"

char *handle_single_quotes(const char *str, int *i)
{
    int start = ++(*i);
    while (str[*i] && str[*i] != '\'')
        (*i)++;
    if (str[*i] != '\'')
    {
        fprintf(stderr, "Error: Unmatched single quote\n");
    }
    char *content = ft_substr(str, start, *i - start);
    (*i)++;
    return content;
}

char *handle_double_quotes(const char *str, int *i)
{
    int start = ++(*i);
    while (str[*i] && str[*i] != '"')
    {
        if (str[*i] == '\\' && (str[*i + 1] == '"' || str[*i + 1] == '\\'))
            (*i)++;
        (*i)++;
    }
    if (str[*i] != '"')
    {
        fprintf(stderr, "Error: Unmatched double quote\n");
    }
    char *content = ft_substr(str, start, *i - start);
    (*i)++;
    return content;
}