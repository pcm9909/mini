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
	t_cmd	*heredoc_redir;
	t_cmd	*input_redir;
	t_cmd	*cmd;
	t_cmd	*output_redir;
	t_cmd	*append_redir;
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

void handle_cd_command(t_redir *command, char ***envp);
void handle_echo_command(t_redir *command);
void handle_env_command(char **envp);
void handle_exit_command(t_redir *command, char ***envp);
void handle_export_command(t_redir *command, char ***envp);
void handle_pwd_command(void);
void handle_unset_command(t_redir *command, char **envp);
void set_dollar(int ptr, char ***envp);
char **update_envp(char **envp, int type, char *new);
int search_env(char **envp, char *name, int flag);
//int is_validname(char *ptr);
int env_validate(char *ptr);
int env_validate(char *ptr);
char *extract_home(char *envp[]);
char *set_env(char *name, int flag, char ***envp);
void print_envp(char **envp, int flag);
int check_builtin_num(t_redir *cmd);
void handle_builtin_command(t_redir *cmd, char ***envp, int builtin_num);
void handle_builtin(t_proc_data *data, char ***envp, int i);
void handle_heredoc(t_redir *cmd, int i, char **envp);
void handle_heredoc_redir(t_redir *cmd, char **envp);
void handle_non_builtin(t_proc_data *data, char ***envp, int i);
void execute_command(t_redir *cmd, char ***envp);
void check_executable(char *path);
void execute_external_command(t_redir *command, char **cmd, char **envp);
int open_redirection_files(t_redir *command);
char *process_command(char *command, t_redir *cmd);
void all_free(char **ptr);
void free_command(t_cmd *cmd);
void free_redirection(t_redir *redirection);
void free_command_list(char ***command);
void cleanup_resources(t_proc_data *data);
char *get_path(char **envp);
char *get_cmd_path(char *cmd, char *path);
void initialize_redirection(t_redir **redirection);
char **split_cmp_quotes(char const *s, char c);
char *ft_strrev(char *str);
char *ft_strjoin_opts(char const *s1, char const *s2, int i);
char **ft_strdups(char **source);
void parse_redir(char *str, t_redir *cmd, char **envp);
char *handle_quotes(const char *str, int *i, t_redir *command);
int handle_dollar1(int *i, char **content, const char *str, char **envp);
char **append_command(char ***cmd, char *str);
void handle_redirection(char *str, int *i, t_redir *command, char **envp);
char *handle_single_quotes(const char *str, int *i, t_redir *command);
char *handle_double_quotes(const char *str, int *i, t_redir *command);
void append_env(char *envp_var, char *envp_val, t_redir *cmd);
void handle_env(int *i, const char *str, char **envp, t_redir *cmd);
char *handle_command(const char *str, int *i);
void parse_left_redir(const char *str, int *i, t_redir *cmd, char **envp);
void parse_right_redir(char *str, int *i, char **envp, t_redir *cmd);
void set_extract_val(int *j, char **tmp, int *i);
char *extract_content(const char *str, int *i, char **envp, t_redir *cmd);
void wait_for_children(t_proc_data *data, char ***envp);
void create_pipes(int i, int cnt, int pipe_fd[2]);
void handle_dollar(int *i, char **content, const char *str, char **envp);
char *set_str(char *str, char **envp);
void error_exit(char *str);
void perror_exit(char *str);
void print_error(char *target);
void handle_parse_error(const char *str, int *i, t_redir *cmd);
char *print_qutoes_error(t_redir *command);
void set_process_data(t_proc_data *data, char *str, char ***envp);
char *build_prompt(char **envp);
void input_sig(struct termios *old);
void end_sig(struct termios *old);
void none_sig(void);
void heredoc_sig(struct termios *old);
int sigcheck(int type);
int is_upright_vars(int c);
int is_whitespace(int c);
int is_envp_vars(int c);

#endif	