#include "main.h"

void	free_command(t_command *cmd)
{
	if (cmd)
	{
		if (cmd->command)
		{
			free_command_list(&cmd->command);
		}
		free(cmd);
	}
}

void	free_redirection(t_redirection *redirection)
{
	if (redirection)
	{
		free_command(redirection->double_left_brace);
		free_command(redirection->double_right_brace);
		free_command(redirection->command);
		free_command(redirection->left_brace);
		free_command(redirection->right_brace);
		free(redirection->full_cmd);
		free(redirection->here_doc);
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

void	all_free(char **ptr)
{
	while (*ptr != NULL)
	{
		free(*ptr);
		ptr++;
	}
	free(ptr);
}
