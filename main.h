#include "libft/libft.h"
#include <stdio.h>
#include <stdbool.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <readline/readline.h>
#include <sys/wait.h>
#include <readline/history.h>
#include <errno.h>
#include <signal.h>
#include <termios.h>

typedef struct s_command
{
	char	**command;
	bool	order;
	bool	exist;
}				t_command;

typedef struct s_redirection
{
	struct s_command	*double_left_brace;
	struct s_command	*left_brace;
	struct s_command	*command;
	char				*full_cmd;
	struct s_command	*right_brace;
	struct s_command	*double_right_brace;
}				t_redirection;


void print(t_redirection *cmd);

char	*get_path(char **envp);
char	*get_cmd_path(char *cmd, char *path);

char *ft_strrev(char *str);

void free_command_list(char ***command);

	// void free_redirection(t_redirection **redirection);
