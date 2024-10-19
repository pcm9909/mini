#include "main.h"

char *handle_single_quotes(const char *str, int *i, t_redir *command)
{
    int start;
    char *content;

    start = ++(*i);
    while (str[*i] && str[*i] != '\'')
        (*i)++;
    if (str[*i] != '\'')
        return (print_qutoes_error(command));
    content = ft_substr(str, start, *i - start);
    (*i)++;
    return (content);
}

char *handle_double_quotes(const char *str, int *i, t_redir *command)
{
    int start;
    char *content;

    start = ++(*i);
    while (str[*i] && str[*i] != '"')
    {
        (*i)++;
    }
    if (str[*i] != '"')
        return (print_qutoes_error(command));
    else
	{
		content = ft_strdup("");
        content = ft_strjoin_opts(content,
                                  ft_substr(str, start, (*i) - start), 3);
	}
    (*i)++;
    return (content);
}

void append_env(char *envp_var, char *envp_val, t_redir *cmd)
{
    char **envp_val_split;
    char *temp;
    int idx;
    int j;

    j = 0;
    idx = ft_strlen(envp_var) + 1;
    temp = ft_strdup(&envp_val[idx]);
    envp_val_split = split_cmp_quotes(temp, ' ');
    while (envp_val_split[j])
    {
        cmd->cmd->exist = true;
        cmd->cmd->cmd_val =
            append_command(&cmd->cmd->cmd_val, envp_val_split[j]);
        j++;
    }
    all_free(envp_val_split);
    free(temp);
}

void handle_env(int *i, const char *str, char **envp, t_redir *cmd)
{
    char *envp_var;
    char *envp_val;
    int start;

    (*i)++;
    start = (*i);
    while (is_envp_vars(str[*i]))
        (*i)++;
    envp_var = ft_substr(str, start, (*i) - start);
    envp_val = ft_strdup(envp[search_env(envp, envp_var, 1)]);
    if (ft_strlen(envp_val))
        append_env(envp_var, envp_val, cmd);
    free(envp_val);
    free(envp_var);
}

char *handle_command(const char *str, int *i)
{
    int start;
    char *val;

    start = (*i);
    val = ft_strdup("");
    while (str[*i] && !is_whitespace(str[*i]) && str[*i] != '>' &&
           str[*i] != '<' && str[*i] != '"' && str[*i] != '\'')
    {
        if (str[*i] != ' ')
            (*i)++;
        else
            break;
    }
    val = ft_strjoin_opts(val, ft_substr(str, start, (*i) - start), 3);
    return (val);
}