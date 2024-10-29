/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_cmd5.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jakim <jakim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 22:09:29 by chunpark          #+#    #+#             */
/*   Updated: 2024/10/24 16:49:44 by jakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void	set_order(t_redir *command, char *str)
{
	char	*rev;
	int		dual;
	int		single;

	dual = ft_strlen(ft_strnstr(str, "<<", sizeof(str)));
	single = ft_strlen(ft_find_single_redirect(str, '<'));
	if (dual > single)
		command->heredoc_redir->order = true;
	else if (dual < single)
		command->input_redir->order = true;
	rev = ft_strrev(str);
	dual = ft_strlen(ft_strnstr(rev, ">>", ft_strlen(str)));
	single = ft_strlen(ft_find_single_redirect(rev, '>'));
	if (dual > single)
		command->append_redir->order = true;
	else if (dual < single)
		command->output_redir->order = true;
	free(rev);
}

char	*ft_find_single_redirect(char *str, char c)
{
	int	i;

	i = 0;
	if (!str)
		return (NULL);
	while (str[i])
	{
		if (str[i] == c)
		{
			if (str[i + 1] == c)
			{
				i++;
			}
			else
			{
				return (&str[i]);
			}
		}
		i++;
	}
	return (NULL);
}

char	*handle_env(int *i, const char *str, char **envp)
{
	char	*envp_var;
	char	*envp_val;
	char	*temp;

	(*i)++;
	envp_var = extract_env_var(i, str);
	envp_val = ft_strdup(envp[search_env(envp, envp_var, 1)]);
	if (ft_strlen(envp_val))
		temp = ft_strdup(&envp_val[ft_strlen(envp_var) + 1]);
	else
		temp = ft_strdup("");
	free(envp_var);
	free(envp_val);
	return (temp);
}

void	handle_double_quoutes_dollar(const char *str, int *i, \
			char **temp, char **envp)
{
	if (str[*i + 1] && is_envp_vars(str[*i + 1]))
		handle_quotes_dollar_sign(i, temp, str, envp);
	else
	{
		while (str[*i] && str[*i] == '$')
		{
			*temp = ft_strjoin_opt(*temp, "$", 1);
			(*i)++;
		}
	}
}
