/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jakim <jakim@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/21 18:10:01 by chunpark          #+#    #+#             */
/*   Updated: 2024/10/24 23:30:43 by jakim            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "main.h"

static void	sg3(int signal)
{
	if (signal == SIGINT)
	{
		rl_on_new_line();
		rl_redisplay();
		printf("^C\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		exit(130);
	}
	else if (signal == SIGTERM)
	{
		printf("exit\n");
		exit(0);
	}
	else if (signal == SIGQUIT)
	{
		rl_on_new_line();
		rl_redisplay();
		return ;
	}
}

void	none_sig(void)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}

void	heredoc_sig(struct termios *old)
{
	tcgetattr(0, old);
	old->c_lflag &= ~(512);
	tcsetattr(0, TCSANOW, old);
	signal(SIGINT, sg3);
	signal(SIGQUIT, sg3);
}

int	sigcheck(int type)
{
	static int	flag;

	if (type == 1)
		flag = 1;
	if (type == 0)
		flag = 0;
	return (flag);
}
