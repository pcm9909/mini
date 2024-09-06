all:
	make -C libft/
	cc *.c libft/libft.a -lreadline -g -o minishell