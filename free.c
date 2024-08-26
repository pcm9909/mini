// #include "main.h"

// void free_redirection(t_redirection **redirection)
// {
//     free_command_list(&(*redirection)->double_left_brace->command);
//     free_command_list(&(*redirection)->left_brace->command);
//     free_command_list(&(*redirection)->double_right_brace->command);
//     free_command_list(&(*redirection)->right_brace->command);
//     free_command_list(&(*redirection)->command->command);

//     free((*redirection)->double_left_brace);
//     free((*redirection)->left_brace);
//     free((*redirection)->command);
//     free((*redirection)->double_right_brace);
//     free((*redirection)->right_brace);
//     free(*redirection);
//     *redirection = NULL;
// }