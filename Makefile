all:
	make -C libft/
	clang *.c libft/libft.a -lreadline -g -o minishell -g -fsanitize=address

hi:
	make -C libft/
	clang *.c libft/libft.a -lreadline -g -o minishell -g

mac:
	make -C libft/
	clang *.c libft/libft.a -lreadline -g -o minishell -g -I/opt/homebrew/opt/readline/include -L/opt/homebrew/opt/readline/lib -lreadline -fsanitize=address
