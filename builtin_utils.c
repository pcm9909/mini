#include "main.h"

void set_dollar(int ptr, char ***envp)
{
    char *tmp;
    char *jo;

    tmp = ft_itoa(ptr);
    jo = ft_strjoin_opts("?=", tmp, 2);
    set_env(jo, 1, envp);
}

char **update_envp(char **envp, int type, char *new)
{
    int i;
    char **ptr;

    i = 0;
    ptr = envp;
    while (*ptr != NULL)
    {
        i++;
        ptr++;
    }
    ptr = (char **)malloc((i + 5) * sizeof(char *));
    i = 0;
    while (envp[i] != NULL)
    {
        ptr[i] = ft_strdup(envp[i]);
        i++;
    }
    ptr[i++] = new;
    ptr[i] = NULL;
    if (type == 1)
        all_free(envp);
    return (ptr);
}

int search_env(char **envp, char *name, int flag)
{
    int i;
    char *re;
    char *tmp;

    i = 0;
    if (ft_strchr(name, '=') && flag == 1)
        re = ft_substr(name, 0, ft_strchr(name, '=') - name);
    else
        re = ft_strdup(name);
    while (*envp)
    {
        if (ft_strchr(*envp, '='))
            tmp = ft_substr(*envp, 0, ft_strchr(*envp, '=') - *envp);
        else
            tmp = ft_strdup(*envp);
        if (!ft_strncmp(tmp, re, ft_strlen(re) + 1))
        {
            free(tmp);
            break;
        }
        envp++;
        i++;
        free(tmp);
    }
    free(re);
    return (i);
}

static int is_validname(char *ptr)
{
    while (*ptr && *ptr != '=')
    {
        if (!ft_isalnum(*ptr) && *ptr != '_')
            return (0);
        ptr++;
    }
    return (1);
}

int env_validate(char *ptr)
{
    char *dup_ptr;

    dup_ptr = ft_strdup(ptr);
    if ((ft_isalpha(*dup_ptr) || *dup_ptr == '_') && is_validname(dup_ptr))
    {
        if (ft_strchr(dup_ptr, '='))
        {
            if ((ft_strchr(dup_ptr, '=') + 1)[0] == '\0')
            {
                free(dup_ptr);
                return (1);
            }
            if (ft_strchr(dup_ptr, '=') != ft_strrchr(dup_ptr, '='))
            {
                free(dup_ptr);
                return (1);
            }
            free(dup_ptr);
            return (1);
        }
        else
        {
            free(dup_ptr);
            return (0);
        }
    }
    else
    {
        free(dup_ptr);
        return (-1);
    }
}

char *extract_home(char *envp[])
{
    char *tmp;

    tmp = NULL;
    while (*envp)
    {
        if (!ft_strncmp(*envp, "HOME=", 5))
            break;
        envp++;
    }
    tmp = (*envp);
    if (tmp)
        return (tmp + 5);
    else
        return (tmp);
}

char *set_env(char *name, int flag, char ***envp)
{
    int i;

    if (flag < 0)
        return (name);
    else
    {
        i = search_env(*envp, name, 1);
        if ((*envp)[i] != NULL)
        {
            if (!ft_strchr(name, '='))
                return (NULL);
            free((*envp)[i]);
            (*envp)[i] = name;
        }
        else
            *envp = update_envp(*envp, 1, name);
    }
    return (NULL);
}

void print_envp(char **envp, int flag)
{
    while (*envp != NULL)
    {
        if (flag == 1)
        {
            if (env_validate(*envp) == 1)
            {
                ft_putstr_fd("declare -x ", 1);
                write(1, *envp, ft_strchr(*envp, '=') - *envp);
                write(1, "=\"", 2);
                ft_putstr_fd(ft_strchr(*envp, '=') + 1, 1);
                write(1, "\"\n", 2);
            }
            else if (env_validate(*envp) == 0)
                printf("declare -x %s\n", *envp);
        }
        else
        {
            if (env_validate(*envp) == 1)
                printf("%s\n", *envp);
        }
        envp++;
    }
}