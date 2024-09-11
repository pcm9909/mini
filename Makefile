all:
	make -C libft/
	clang *.c libft/libft.a -lreadline -g -o minishell