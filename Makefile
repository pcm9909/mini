all:
	make -C libft/
	clang *.c libft/libft.a -lreadline -g -o minishell -g

asan:
	make -C libft/
	clang *.c libft/libft.a -lreadline -g -o minishell -g -fsanitize=address

mac:
	make -C libft/
	clang *.c libft/libft.a -lreadline -g -o minishell -g -I/opt/homebrew/opt/readline/include -L/opt/homebrew/opt/readline/lib -lreadline