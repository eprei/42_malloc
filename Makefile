NAME = malloc

SRCS = src/malloc.c src/free.c src/realloc.c src/show_alloc_mem.c src/utils.c
OBJ = $(SRCS:.c=.o)
TEST_DIR = test
TEST_SRC = main.c
TEST_BINARY = test_malloc

CC = gcc
CFLAGS = -g #-Wall -Wextra -Werror

LIBFTPATH = ./libft

BOLD = \033[1m
RESET = \033[0m

ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

LIBNAME = libft_malloc_${HOSTTYPE}.so
SYM_LINK = libft_malloc.so

all: symlink $(NAME)

symlink:
	@echo "Creating symbolic link $(SYM_LINK) -> $(LIBNAME)"
	ln -sf $(LIBNAME) $(SYM_LINK)
	ls -l $(SYM_LINK)

$(NAME): $(OBJ)
	make -C $(LIBFTPATH)
	$(CC) $(CFLAGS) -fPIC $(SRCS) -L$(LIBFTPATH) -lft -shared -o $(LIBNAME)
	echo $(LIBNAME)

pre: $(OBJ)
	make -C $(LIBFTPATH)
	$(CC) $(CFLAGS) -fPIC $(SRCS) -L$(LIBFTPATH) -lft -shared -E

clean:
	make clean -C $(LIBFTPATH)
	rm -rf $(OBJ)

fclean: clean
	make fclean -C $(LIBFTPATH)
	rm -rf $(NAME) $(LIBNAME)* $(SYM_LINK) $(TEST_DIR)/$(TEST_BINARY)

test: all
	@echo "Running test with my malloc"
	$(CC) $(CFLAGS) $(TEST_DIR)/$(TEST_SRC) -o $(TEST_DIR)/$(TEST_BINARY) -L. -lft_malloc
	./run.sh $(TEST_DIR)/$(TEST_BINARY)

gdb: all
	@echo "Running test with my malloc"
	$(CC) $(CFLAGS) $(TEST_DIR)/$(TEST_SRC) -o $(TEST_DIR)/$(TEST_BINARY)
	./run.sh gdb $(TEST_DIR)/$(TEST_BINARY)

test_system: all
	@echo "Running test system malloc"
	$(CC) $(CFLAGS) -Wl,-flat_namespace -Wl,-undefined,dynamic_lookup  $(TEST_DIR)/$(TEST_SRC) -o $(TEST_DIR)/$(TEST_BINARY)
	$(TEST_DIR)/$(TEST_BINARY)

re: fclean all

.PHONY: all clean fclean re test
