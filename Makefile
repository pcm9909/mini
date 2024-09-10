all:
	make -C libft/
	clang *.c libft/libft.a -lreadline -g -fsanitize=address -o minishell
