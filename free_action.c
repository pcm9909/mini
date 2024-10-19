#include "main.h"

void	all_free(char **ptr)
{
	char	**tmp;

	tmp = ptr;
	while (*ptr != NULL)
	{
		free(*ptr);
		ptr++;
	}
	free(tmp);
}

void	free_command(t_cmd *cmd)
{
	if (cmd)
	{
		if (cmd->cmd_val)
		{
			free_command_list(&cmd->cmd_val);
		}
		free(cmd);
	}
}

void	free_redirection(t_redir *redirection)
{
	if (redirection)
	{
		free_command(redirection->heredoc_redir);
		free_command(redirection->append_redir);
		free_command(redirection->cmd);
		free_command(redirection->input_redir);
		free_command(redirection->output_redir);
		free(redirection->heredoc);
		free(redirection);
	}
}

void	free_command_list(char ***command)
{
	int	i;

	if (*command)
	{
		i = 0;
		while ((*command)[i])
		{
			free((*command)[i]);
			i++;
		}
		free(*command);
		*command = NULL;
	}
}

void	cleanup_resources(t_proc_data *data)
{
	int	i;

	i = 0;
	while (i < data->cnt)
	{
		free_redirection(data->command[i]);
		i++;
	}
	i = 0;
	while (data->split[i])
	{
		free(data->split[i]);
		i++;
	}
	free(data->command);
	free(data->split);
	free(data->pids);
}
