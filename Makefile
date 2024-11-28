FLAGS_DEBUG= -fsanitize=address -g3 -O1 -fno-omit-frame-pointer -fno-optimize-sibling-calls
CFLAGS=-Wall -Wextra -Werror
FILES= main.c

LIBFT=./libft/libft.a
OBJS=$(FILES:.c=.o)
OBJS_BONUS=$(FILES_BONUS:.c=.o)
NAME=pipex


all: $(NAME)

# bonus: $(OBJS_BONUS)
# 	ar rcs $(NAME) $(OBJS_BONUS)

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