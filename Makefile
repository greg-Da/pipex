FLAGS_DEBUG= -fsanitize=address -g3 -O1 -fno-omit-frame-pointer -fno-optimize-sibling-calls
CFLAGS=-Wall -Wextra -Werror
FILES= 	main.c\
		here_doc.c\
		ft_custom_split.c\
		fct_utils.c\
		invalid.c\
		exec_unix.c\
		init_struct.c

FILES_BONUS= 	main_bonus.c\
				here_doc_bonus.c\
				ft_custom_split_bonus.c\
				fct_utils_bonus.c\
				invalid_bonus.c\
				exec_unix_bonus.c\
				init_struct_bonus.c

LIBFT=./libft/libft.a
OBJS=$(FILES:.c=.o)
OBJS_BONUS=$(FILES_BONUS:.c=.o)
NAME=pipex
BONUS=pipex


all: $(NAME)

bonus: $(OBJS_BONUS) $(LIBFT)
	gcc $(CFLAGS) -o $(NAME) $(OBJS_BONUS) $(LIBFT)

$(NAME): $(OBJS) $(LIBFT)
	gcc $(CFLAGS) -o $(NAME) $(OBJS) $(LIBFT)

%.o: %.c
	gcc $(CFLAGS) -I includes -c $< -o $@

debug: $(OBJS) $(LIBFT)
	gcc $(CFLAGS) $(FLAGS_DEBUG) -o $(NAME) $(OBJS) $(LIBFT)

clean:
	rm -f $(OBJS) $(OBJS_BONUS)
	make clean -C ./libft

fclean: clean
	rm -f $(NAME)
	make fclean -C ./libft

re: fclean all

$(LIBFT):
	make -C ./libft

# .PHONY: debug bonus