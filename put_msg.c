#include "main.h"

void error_exit(char *str)
{
    ft_putstr_fd(str, 2);
    exit(EXIT_FAILURE);
}

void perror_exit(char *str)
{
    perror(str);
    exit(EXIT_FAILURE);
}

void print_error(char *target)
{
    ft_putstr_fd("minishell: ", 2);
    perror(target);
}

void handle_parse_error(const char *str, int *i, t_redir *cmd)
{
    ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
    ft_putchar_fd(str[*i], 2);
    ft_putstr_fd("'\n", 2);
    cmd->executable = false;
    while (str[*i] && (str[*i] == '>' || str[*i] == '<' || str[*i] == '\0' ||
                       str[*i] == '|' || str[*i] == '&' || str[*i] == ';'))
        (*i)++;
}

char    *print_qutoes_error(t_redir *command)
{
    write(2, "minishell: Error: Unmatched quote\n",
          ft_strlen("minishell: Error: Unmatched quote\n"));
    command->executable = false;
    return (ft_strdup(""));
}