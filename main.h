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

char	*ft_strjoin_with_free(char const *s1, char const *s2);

void handle_cd_command(t_redirection *command, char **envp);
void handle_export_command(t_redirection *command, char **envp);
void handle_env_command(t_redirection *command, char **envp);
void handle_exit_command(t_redirection *command);
void handle_unset_command(t_redirection *command, char **envp);
void handle_pwd_command();
void handle_echo_command(t_redirection *command, char **envp);

void handle_double_left_brace(t_redirection *command);
void handle_left_brace(t_redirection *command);
void handle_right_brace(t_redirection *command);
void handle_double_right_brace(t_redirection *command);
	// void free_redirection(t_redirection **redirection);

void execute_command(t_redirection *command, char **envp, int input_fd, int output_fd);
void execute_external_command(t_redirection *command, char **envp, int input_fd, int output_fd);

char *handle_single_quotes(const char *str, int *i);
char *handle_double_quotes(const char *str, int *i);

char **update_envp(char **envp, int type, char *new);
char **initialize_environment(char *env[]);
char	*extract_home(char *envp[]);

void	all_free(char **ptr);