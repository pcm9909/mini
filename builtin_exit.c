#include "main.h"

static int ft_isspace1(int c)
{
    return (c == ' ' || (c >= '\t' && c <= '\r'));
}

static long long ft_atoll(const char *str)
{
    int i;
    long long sign;
    long long result;

    i = 0;
    sign = 1;
    result = 0;
    while (ft_isspace1((unsigned char)str[i]))
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

static int ft_count_len1(long long n)
{
    long long temp;
    int len;

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

static char *ft_lltoa(long long n)
{
    char *str;
    long long sign;
    int len;

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

static int exit_range(char *ptr)
{
    char *tmp;

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

int only_digit(char *ptr)
{
    if (*ptr == '-' || *ptr == '+')
        ptr++;
    while (*ptr)
    {
        if (!ft_isdigit(*ptr))
            return (0);
        ptr++;
    }
    return (1);
}

static void ft_exit(char **ptr, char ***envp)
{
    ft_putstr_fd("exit\n", 2);
    if (ptr[1] == NULL)
        exit(0);
    else if (!only_digit(ptr[1]) || exit_range(ptr[1]))
    {
        ft_putstr_fd("minishell: exit: ", 2);
        ft_putstr_fd(ptr[1], 2);
        ft_putstr_fd(": numeric argument required\n", 2);
        exit(2);
    }
    else if (ptr[2] != NULL)
    {
        ft_putstr_fd("minishell: exit: too many arguments\n", 2);
        set_dollar(1, envp);
    }
    else
        exit((unsigned char)ft_atoi(ptr[1]));
}

void handle_exit_command(t_redir *command, char ***envp)
{
    char **cd;

    cd = ft_strdups(command->cmd->cmd_val);
    ft_exit(cd, envp);
    all_free(cd);
}