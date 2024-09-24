#include "main.h"

// void	handle_double_left_brace(t_redirection *cmd)
// {
//     int		i;
//     char	*input;
//     char	*str;
//     int		pipe_fd[2];
//     pid_t   pid;

//     i = 0;
//     while (cmd->double_left_brace->command && \
//             cmd->double_left_brace->command[i])
//     {
//         if (pipe(pipe_fd) == -1)
//         {
//             perror("pipe");
//             exit(EXIT_FAILURE);
//         }

//         pid = fork();
//         if (pid == -1)
//         {
//             perror("fork");
//             exit(EXIT_FAILURE);
//         }

//         if (pid == 0) // 자식 프로세스
//         {
//             close(pipe_fd[0]); // 읽기 끝을 닫음
//             str = NULL;
//             while (1)
//             {
//                 input = readline(">");
//                 if (!input)
//                     break;
//                 if (ft_strncmp(input, cmd->double_left_brace->command[i], \
//                     ft_strlen(cmd->double_left_brace->command[i])) == 0 && \
//                 ft_strlen(input) == ft_strlen(cmd->double_left_brace->command[i]))
//                 {
//                     free(input);
//                     break;
//                 }
//                 str = ft_strjoin_with_free(str, input);
//                 str = ft_strjoin_with_free(str, "\n");
//                 add_history(input);
//                 free(input);
//             }
//             if (str)
//             {
//                 write(pipe_fd[1], str, ft_strlen(str));
//                 free(str);
//             }
//             close(pipe_fd[1]);
//             exit(EXIT_SUCCESS);
//         }
//         else // 부모 프로세스
//         {
//             close(pipe_fd[1]); // 쓰기 끝을 닫음
//             waitpid(pid, NULL, 0); // 자식 프로세스가 끝날 때까지 기다림
//             if (cmd->double_left_brace->command[i + 1] == NULL)
//             {
//                 dup2(pipe_fd[0], 0); // 파이프의 읽기 끝을 표준 입력으로 복제
//             }
//             close(pipe_fd[0]);
//         }
//         i++;
//     }
// }

// void	handle_double_left_brace(t_redirection *cmd)
// {
// 	int		i;
// 	char	*input;
// 	char	*str;
// 	int		pipe_fd[2];

// 	i = 0;
// 	while (cmd->double_left_brace->command && \
// 			cmd->double_left_brace->command[i])
// 	{
// 		if (pipe(pipe_fd) == -1)
// 		{
// 			perror("pipe");
// 			exit(EXIT_FAILURE);
// 		}
// 		str = ft_strdup("");
// 		input = readline(">");
// 		while (input)
// 		{
// 			if (ft_strncmp(input, cmd->double_left_brace->command[i], \
// 				ft_strlen(cmd->double_left_brace->command[i])) == 0 && \
// 			ft_strlen(input) == ft_strlen(cmd->double_left_brace->command[i]))
// 			{
// 				write(pipe_fd[1], str, ft_strlen(str));
// 				free(input);
// 				input = NULL;
// 				break ;
// 			}
// 			str = ft_strjoin_with_free(str, input);
// 			str = ft_strjoin_with_free(str, "\n");
// 			add_history(str);
// 			free(input);
// 			input = readline(">");
// 		}
// 		free(str);
// 		close(pipe_fd[1]);
// 		if (cmd->double_left_brace->command[i + 1] == NULL)
// 		{
// 			dup2(pipe_fd[0], 0);
// 		}
// 		close(pipe_fd[0]);
// 		i++;
// 	}
// }

void	handle_left_brace(t_redirection *command)
{
	int	i;
	int	fd;

	i = 0;
	while (command->left_brace->command && command->left_brace->command[i])
	{
		fd = open(command->left_brace->command[i], O_RDONLY);
		if (fd == -1)
		{
			perror(command->left_brace->command[i]);
			exit(EXIT_FAILURE);
		}
		if (command->left_brace->command[i + 1] == NULL && \
			command->left_brace->order)
		{
			dup2(fd, 0);
			break ;
		}
		close(fd);
		i++;
	}
}

void	handle_right_brace(t_redirection *cmd)
{
	int	i;
	int	fd;

	i = 0;
	while (cmd->right_brace->command && cmd->right_brace->command[i])
	{
		fd = open(cmd->right_brace->command[i], \
					O_CREAT | O_TRUNC | O_WRONLY, 0644);
		if (fd == -1)
		{
			perror(cmd->right_brace->command[i]);
			exit(EXIT_FAILURE);
		}
		if (cmd->right_brace->order == true)
			dup2(fd, 1);
		close(fd);
		perror(cmd->right_brace->command[i]);
		i++;
	}
}

void	handle_double_right_brace(t_redirection *cmd)
{
	int	i;
	int	fd;

	i = 0;
	while (cmd->double_right_brace->command && \
			cmd->double_right_brace->command[i])
	{
		fd = open(cmd->double_right_brace->command[i], \
					O_CREAT | O_APPEND | O_WRONLY, 0644);
		if (fd == -1)
		{
			perror(cmd->double_right_brace->command[i]);
			exit(EXIT_FAILURE);
		}
		if (cmd->double_right_brace->order == true)
			dup2(fd, 1);
		close(fd);
		perror(cmd->double_right_brace->command[i]);
		i++;
	}
}
