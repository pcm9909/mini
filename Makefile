all:
	cc *.c libft/libft.a -lreadline -g -o minishell -fsanitize=address
