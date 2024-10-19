#include "main.h"

int is_upright_vars(int c)
{
    return (c == '>' || c == '<' || c == '\0' ||
            c == '|' || c == '&' || c == ';');
}

int is_whitespace(int c)
{
    return ((c >= 9 && c <= 13) || c == 32);
}

int is_envp_vars(int c)
{
    if (c && (ft_isalnum(c) || c == '_' || c == '?'))
        return (1);
    else
        return (0);
}