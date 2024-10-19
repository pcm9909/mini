#include "main.h"

void handle_cd_command(t_redir *command, char ***envp)
{
    char *tmp_pwd;
    char **cd;
    char **cd_path;
    char *old;
    int i;

    tmp_pwd = getcwd(NULL, BUFSIZ);
    old = getcwd(NULL, BUFSIZ);
    cd = ft_strdups(command->cmd->cmd_val);
    if (cd[1] == NULL)
    {
        free(tmp_pwd);
        tmp_pwd = ft_strdup(extract_home(*envp));
    }
    else if (cd[2] != NULL)
    {
        ft_putstr_fd("minishell: cd: too many arguments\n", 2);
        set_dollar(1, envp);
    }
    else
    {
        if (!ft_strncmp(cd[1], "/", 1))
        {
            free(tmp_pwd);
            tmp_pwd = ft_strdup("/");
        }
        i = 0;
        cd_path = ft_split(cd[1], '/');
        while (cd_path[i] != NULL)
        {
            if (i == 0 && !ft_strncmp(cd_path[i], "~", 4))
            {
                free(tmp_pwd);
                tmp_pwd = ft_strdup(extract_home(*envp));
            }
            else
            {
                if (!ft_strncmp(cd_path[i], "..", 5))
                {
                    if (ft_strrchr(tmp_pwd, '/') == tmp_pwd)
                        *(ft_strrchr(tmp_pwd, '/') + 1) = '\0';
                    else
                        *(ft_strrchr(tmp_pwd, '/')) = '\0';
                }
                else
                {
                    tmp_pwd = ft_strjoin_opts(tmp_pwd, "/", 1);
                    tmp_pwd = ft_strjoin_opts(tmp_pwd, cd_path[i], 1);
                }
            }
            i++;
        }
        all_free(cd_path);
    }
    if (chdir(tmp_pwd) == -1)
    {
        ft_putstr_fd("minishell: cd: ", 2);
        ft_putstr_fd(cd[1], 2);
        ft_putstr_fd(": No such file or directory\n", 2);
        set_dollar(1, envp);
    }
    else
    {
        set_env(ft_strjoin("OLDPWD=", old), 1, envp);
        set_env(ft_strjoin("PWD=", tmp_pwd), 1, envp);
    }
    free(old);
    free(tmp_pwd);
    all_free(cd);
}