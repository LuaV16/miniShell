NAME=miniShell
CC=cc
CFLAGS=-Wall -Wextra -Werror -Ilibft -g #-fsanitize=address
LDCFLAGS=-lreadline
SRCS=main.c signals.c tokens/tokens.c tokens/token_errors.c tokens/add_to_struct.c exec/init_exec_struct.c exec/builtin_export.c exec/builtin_export_tools.c exec/builtin_unset.c exec/builtin_env.c
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