#include "main.h"

void print(t_redir *cmd)
{
    int i = 0;

    if (cmd->heredoc_redir && cmd->heredoc_redir->cmd_val)
    {
        while (cmd->heredoc_redir->cmd_val[i])
        {
            printf("[heredoc_redir]\n");
            printf("%s\n", cmd->heredoc_redir->cmd_val[i]);
            i++;
        }
    }

    i = 0;
    if (cmd->input_redir && cmd->input_redir->cmd_val)
    {
        while (cmd->input_redir->cmd_val[i])
        {
            printf("[input_redir]\n");
            printf("%s\n", cmd->input_redir->cmd_val[i]);
            i++;
        }
    }

    i = 0;
    if (cmd->cmd && cmd->cmd->cmd_val)
    {
        while (cmd->cmd->cmd_val[i])
        {
            printf("[cmd]\n");
            printf("%s\n", cmd->cmd->cmd_val[i]);
            i++;
        }
    }

    i = 0;
    if (cmd->output_redir && cmd->output_redir->cmd_val)
    {
        while (cmd->output_redir->cmd_val[i])
        {
            printf("[output_redir]\n");
            printf("%s\n", cmd->output_redir->cmd_val[i]);
            i++;
        }
    }

    i = 0;
    if (cmd->append_redir && cmd->append_redir->cmd_val)
    {
        while (cmd->append_redir->cmd_val[i])
        {
            printf("[append_redir]\n");
            printf("%s\n", cmd->append_redir->cmd_val[i]);
            i++;
        }
    }
}