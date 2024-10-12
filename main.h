#ifndef MAIN_H
# define MAIN_H
# include "libft/libft.h"
# include <stdio.h>
# include <stdbool.h>
# include <fcntl.h>
# include <stdlib.h>
# include <unistd.h>
# include <readline/readline.h>
# include <sys/wait.h>
# include <readline/history.h>
# include <errno.h>
# include <signal.h>
# include <termios.h>
# include <sys/types.h>
# include <sys/stat.h>

typedef struct s_cmd
{
	char	**cmd_val;
	bool	order;
	bool	exist;
}				t_cmd;

typedef struct s_redir
{
	struct s_cmd	*heredoc_redir;
	struct s_cmd	*input_redir;
	struct s_cmd	*cmd;
	struct s_cmd	*output_redir;
	struct s_cmd	*append_redir;
	char				*full_cmd;
	char				*heredoc;
	bool				executable;
}				t_redir;

typedef struct s_proc_data
{
	t_redir	**command;
	pid_t			*pids;
	struct termios	old;
	char			**split;
	int				pipe_fd[2];
	int				cnt;
	int				input_fd;
	int				builtin_num;
	int				in;
	int				out;
} t_proc_data;


int		sigcheck(int type);
int		check_builtin_num(t_redir *cmd);
int		open_redirection_files(t_redir *command);
void	handle_builtin_command(t_redir *cmd, \
								char ***envp, int builtin_num);

void	print(t_redir *cmd);

void	handle_dollar(int *i, char **content, const char *str, char **envp);
char	*get_path(char **envp);
char	*get_cmd_path(char *cmd, char *path);

char	*ft_strrev(char *str);

void	free_command_list(char ***command);
void	handle_cd_command(t_redir *command, char ***envp);
void	handle_export_command(t_redir *command, char ***envp);
void	handle_env_command(t_redir *command, char **envp);
void	handle_exit_command(t_redir *command);
void	handle_unset_command(t_redir *command, char **envp);
void	handle_pwd_command(void);
void	handle_echo_command(t_redir *command, char **envp);
void	set_dollar(int ptr, char ***envp);

int		handle_left_brace(t_redir *command);
int		handle_right_brace(t_redir *command);
int		handle_double_right_brace(t_redir *command);

void	execute_command(t_redir *command, char ***envp, \
							int input_fd, int output_fd);
void	execute_external_command(t_redir *command, char **envp, \
							int input_fd, int output_fd);

char	*handle_single_quotes(const char *str, int *i, t_redir *command);
char	*handle_double_quotes(const char *str, int *i, char **envp, \
								t_redir *command);

char	**update_envp(char **envp, int type, char *new);
char	**initialize_environment(char *env[]);
char	*extract_home(char *envp[]);

void	all_free(char **ptr);

char	**update_envp(char **envp, int type, char *new);
char	**initialize_environment(char *env[]);
int		double_ptr_size(char **ptr);
int		search_env(char **envp, char *name, int flag); // free
int		is_validname(char *ptr);
int		env_validate(char *ptr);
char	*set_env(char *name, int flag, char ***envp);
void	print_envp(char **envp, int flag);
int		ft_export(char **ptr, char ***envp);
int		only_digit(char *ptr);
void	ft_exit(char **ptr);
void	ft_unset(char **ptr, char **envp);
void	ft_echo(char **ptr, char **envp);
int		is_whitespace(int c);
char	*handle_command(const char *str, int *i, char **envp);

void	sg(int signal);
void	sg2(int signal);
void	input_sig(struct termios *old);
void	end_sig(struct termios *old);
void	none_sig(struct termios *old);

void	parse_left_redirection(const char *str, int *i, t_redir *command, char **evnp);
void	parse_right_redirection(char *str, int *i, char **envp, t_redir *command);
void	parse_redirection(char *str, t_redir *command, char **envp);
char	*set_command(t_cmd *command);
void	set_order(t_redir *command, char *str);
char	*ft_find_single_redirect(char *str, char c);
void	parse_command(char *str, int *i, t_redir *command, char **envp);

char	**append_command(char ***cmd, char *str);

void	free_redirection(t_redir *redirection);
void	free_command(t_cmd *cmd);
void	all_free(char **ptr);

char	*build_prompt(char **envp);
char	*extract_home(char *envp[]);
char	*extract_location(char *envp[]);
char	*extract_name(char *envp[]);
char	**extract_path(char *envp[]);

int		is_whitespace(int c);
int		cnt_cmd(char **split);

void	handle_double_left_brace(t_redir *cmd, char **envp);

int		is_envp_vars(int c);

void	perror_exit(char *str);

char	*handle_quotes(const char *str, int *i, char **envp, t_redir *command);
char	*handle_double_quotes2(const char *str, int *i, \
							char **envp, t_redir *command);

char	*handle_quotes2(const char *str, int *i, char **envp, t_redir *command);
char	*handle_single_quotes2(const char *str, int *i, t_redir *command);

char	*print_qutoes_error(const char *str, \
								t_redir *command, char **content);

char	*ft_strjoin_opts(char const *s1, char const *s2, int i);
char	**ft_splits(char const *s, char c);
char	**ft_strdups(char	**source);
void	error_exit(char	*str);
void	heredoc_sig(struct termios *old);

#endif