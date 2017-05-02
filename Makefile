CC=clang
FLAGS=
FLAGS42=-Wall -Wextra -Werror -Wconversion

NAME=lemipc

DIR_LIBFT=libs/libft
DIR_LIST=libs/list
LIBS_BIN=$(DIR_LIBFT)/libft.a $(DIR_LIST)/liblist.a
LIBS=-L libs/libft/ -lft -L libs/list/ -llist -lpthread -lrt

SRC_DIR=src
INCLUDES=-I ./ -I$(SRC_DIR) -I $(DIR_LIBFT) -I $(DIR_LIST)

BUILD_DIR=__build

SRC=\
	main.c		\
	shm.c		\
	sem.c		\
	game.c		\

OBJ=$(addprefix $(BUILD_DIR)/,$(SRC:.c=.o))

all:$(BUILD_DIR) $(NAME)

$(BUILD_DIR):
	@mkdir -p $@

$(BUILD_DIR)/%.o:$(SRC_DIR)/%.c
	@$(CC) $(FLAGS) -c $< $(INCLUDES) -o $@

$(LIBS_BIN):
	@make -c $(DIR_LIBFT)
	@make -c $(DIR_LIST)

$(NAME):$(LIBS_BIN) $(OBJ)
	@$(CC) $(FLAGS) $(OBJ) $(LIBS) -o $@
	@echo "$@ was created"

clean:
	@rm -rf $(BUILD_DIR)

fclean:clean
	@rm -f $(NAME)

re: fclean all
