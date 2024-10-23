/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execute_heredoc2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: chunpark <chunpark@student.42gyeongsan.    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 18:09:33 by chunpark          #+#    #+#             */
/*   Updated: 2024/10/23 21:45:35 by chunpark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

void	append_until_dollar(char **proc_read, const char *read, int *j)
{
	char	*temp;
	int		start;

	start = *j;
	while (read[*j] && read[*j] != '$')
	{
		(*j)++;
	}
	temp = ft_substr(read, start, *j - start);
	*proc_read = ft_strjoin_opt(*proc_read, temp, 3);
}

void	proc_read_input(char *read, int pipe_fd[2], char **envp)
{
	char	*proc_read;
	int		j;

	add_history(read);
	proc_read = ft_strdup("");
	j = 0;
	while (read[j])
	{
		if (read[j] == '$')
			handle_dollar(&j, &proc_read, read, envp);
		else
			append_until_dollar(&proc_read, read, &j);
	}
	proc_read = ft_strjoin_opt(proc_read, "\n", 1);
	write(pipe_fd[1], proc_read, ft_strlen(proc_read));
	free(proc_read);
	free(read);
}
