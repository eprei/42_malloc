NAME = malloc

SRCS = src/main.c
OBJ = $(SRCS:.c=.o)

CC = gcc
CFLAGS = -Wall -Wextra -Werror -g

LIBFTPATH = ./libft

BOLD = \033[1m
RESET = \033[0m
GREEN = \033[0;32m
RED = \033[0;31m

all: $(NAME)

$(NAME): $(OBJ)
	make -C $(LIBFTPATH)
	$(CC) $(CFLAGS) $(OBJ) -L$(LIBFTPATH) -lft -o $(NAME)

clean:
	rm -rf $(OBJ)
	make clean -C $(LIBFTPATH)

fclean: clean
	rm -rf $(NAME)
	make fclean -C $(LIBFTPATH)

re: fclean all

.PHONY: all clean fclean re test
