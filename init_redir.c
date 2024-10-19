#include "main.h"

static t_cmd	*create_command(void)
{
	t_cmd	*cmd;

	cmd = malloc(sizeof(t_cmd));
	if (!cmd)
		error_exit("minishell: Error: allocation failed");
	cmd->cmd_val = NULL;
	cmd->order = false;
	cmd->exist = false;
	return (cmd);
}

void	initialize_redirection(t_redir **redirection)
{
	*redirection = malloc(sizeof(t_redir));
	(*redirection)->heredoc_redir = create_command();
	(*redirection)->append_redir = create_command();
	(*redirection)->cmd = create_command();
	(*redirection)->input_redir = create_command();
	(*redirection)->output_redir = create_command();
	(*redirection)->executable = true;
	(*redirection)->heredoc = ft_strdup("");
}
