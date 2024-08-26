#include "main.h"

void print(t_redirection *cmd)
{
	int i = 0;
	if(cmd->left_brace->command)
	{
		while(cmd->left_brace->command[i])
		{
			printf("[lb]\n");
			printf("%s\n", cmd->left_brace->command[i]);
			i++;
		}
	}
	i = 0;
	if(cmd->double_left_brace->command)
	{
		while(cmd->double_left_brace->command[i])
		{
			printf("[dlb]\n");
			printf("%s\n", cmd->double_left_brace->command[i]);
			i++;
		}
	}
	i = 0;
	if(cmd->command->command)
	{
		while(cmd->command->command[i])
		{
			printf("[cmd]\n");
			printf("%s\n", cmd->command->command[i]);
			i++;
		}
	}
	i = 0;
	if(cmd->right_brace->command)
	{
		while(cmd->right_brace->command[i])
		{
			printf("[rb]\n");
			printf("%s\n", cmd->right_brace->command[i]);
			i++;
		}
	}
	i = 0;
	if(cmd->double_right_brace->command)
	{
		while(cmd->double_right_brace->command[i])
		{
			printf("[drb]\n");
			printf("%s\n", cmd->double_right_brace->command[i]);
			i++;
		}
	}
}



char *ft_strrev(char *str)
{
	int i = 0;
	int len = ft_strlen(str);
	char *rev = malloc(len + 1);

	rev[len] = '\0';
	while(str[i])
	{
		rev[i] = str[len - i - 1];
		i++;
	}
	return rev;
}