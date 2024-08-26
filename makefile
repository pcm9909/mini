all:
	cc *.c libft/libft.a -lreadline -g -fsanitize=address -o minishell
