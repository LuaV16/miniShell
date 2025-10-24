NAME=miniShell
CC=cc
CFLAGS=-Wall -Wextra -Werror -Ilibft -g -fsanitize=address
LDCFLAGS=-lreadline
SRCS=main.c signals.c tokens/tokens.c tokens/token_errors.c tokens/add_to_struct.c builtins/builtin_export.c builtins/builtin_export_tools.c builtins/builtin_unset.c builtins/builtin_env.c builtins/builtin_pwd.c builtins/builtin_cd.c builtins/builtin_echo.c builtins/dispatch.c parser.c executer.c cleanup.c exec/child_exec.c exec/cleanup.c exec/errors.c exec/exec_init.c exec/fork_exec.c  exec/io.c exec/paths.c exec/validation.c utils/memory.c tokens/lexer_build.c tokens/lexer_char.c tokens/lexer_count.c tokens/lexer_quote.c tokens/skip.c tokens/cmd_utils.c tokens/expansion_utils.c tokens/expansion.c tokens/heredoc.c tokens/pipe.c tokens/redirections.c tokens/double_out.c
OBJS=$(SRCS:.c=.o)
LIBFT_DIR=libft
LIBFT=$(LIBFT_DIR)/libft.a

all : $(NAME)

$(NAME):$(OBJS)
	@make bonus -C $(LIBFT_DIR)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT) $(LDCFLAGS)

%.o:%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@make clean -C $(LIBFT_DIR)
	@rm -rf $(OBJS)

fclean : clean
	@make fclean -C $(LIBFT_DIR)
	@rm -rf $(NAME)

re:fclean all

.PHONY:all clean fclean re