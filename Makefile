all:
	make -C libft/
	clang *.c libft/libft.a -lreadline -g -o minishell -g

san:
	make -C libft/
	clang *.c libft/libft.a -lreadline -g -o minishell -g -fsanitize=address