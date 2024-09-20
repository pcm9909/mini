#include "main.h"

void	sg(int signal)
{
	if (signal == SIGINT)
	{
		rl_on_new_line();
		rl_redisplay();
		printf("^C\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
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

void	sg2(int signal)
{
	if (signal == SIGINT)
	{
		return ;
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

void	input_sig(struct termios *old)
{
	tcgetattr(0, old);
	old->c_lflag &= ~(512);
	tcsetattr(0, TCSANOW, old);
	signal(SIGINT, sg);
	signal(SIGQUIT, sg);
}

void	end_sig(struct termios *old)
{
	tcgetattr(0, old);
	old->c_lflag |= (512);
	tcsetattr(0, TCSANOW, old);
	signal(SIGINT, sg2);
	signal(SIGQUIT, sg2);
}

void	none_sig(struct termios *old)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
}
