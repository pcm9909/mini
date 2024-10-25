CC = cc

CFLAGS = -Wextra -Wall -Werror -g -lreadline
NAME = minishell

LIBFT = libft/libft.a

SRCS = builtin_cd.c \
		builtin_echo.c \
		builtin_env.c \
		builtin_exit1.c \
		builtin_exit2.c \
		builtin_export.c \
		builtin_pwd.c \
		builtin_unset.c \
		builtin_utils1.c \
		builtin_utils2.c \
		execute_builtin.c \
		execute_heredoc1.c \
		execute_heredoc2.c \
		execute_non_builtin.c \
		execute_redir1.c \
		execute_redir2.c \
		free_action.c \
		get_command_path.c \
		init_redir.c \
		libft_expans1.c \
		libft_expans2.c \
		libft_expans3.c	\
		parse_cmd1.c \
		parse_cmd2.c \
		parse_cmd3.c \
		parse_cmd4.c \
		parse_cmd5.c \
		parse_left_redir.c \
		parse_right_redir.c \
		parse_utils.c \
		preset_execute.c \
		preset_str1.c \
		preset_str2.c \
		put_msg.c \
		set_proc_date.c \
		set_prompt.c \
		signal1.c \
		signal2.c \
		validate_utils.c \
		main.c


OBJS = $(SRCS:.c=.o)

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

all: $(NAME)

$(NAME): $(OBJS)
		@make -C libft
		@$(CC) $(OBJS) $(LIBFT) $(CFLAGS) -o $(NAME)

clean:
	@rm -rf $(OBJS)

fclean: clean
	@rm -rf $(NAME)

re: fclean all


.PHONY: all clean fclean re
