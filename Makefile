CC=clang
FLAGS42=-Wall -Wextra -Werror -Wconversion
FLAGS=

NAME=lemipc

DIR_LIBFT=libs/libft
LIBS_BIN=$(DIR_LIBFT)/libft.a
LIBS=-L libs/libft/ -lft -L -lpthread -lrt -lm

SRC_DIR=src
INCLUDES=-I ./ -I$(SRC_DIR) -I $(DIR_LIBFT)

BUILD_DIR=__build

SRC=\
	main.c		\
	shm.c		\
	sem.c		\
	mq.c		\
	player.c	\
	utils.c		\
	iabombard.c	\
	iaduban.c	\

OBJ=$(addprefix $(BUILD_DIR)/,$(SRC:.c=.o))

all:$(BUILD_DIR) lemipc

$(BUILD_DIR):
	@mkdir -p $@

$(BUILD_DIR)/%.o:$(SRC_DIR)/%.c
	@$(CC) $(FLAGS) -c $< $(INCLUDES) -o $@

$(LIBS_BIN):
	@make -C $(DIR_LIBFT)

$(NAME):$(LIBS_BIN) $(OBJ)
	@$(CC) $(FLAGS) $(OBJ) $(LIBS) $(INCLUDES) -o $@
	@echo "$@ was created"

clean:
	@rm -rf $(BUILD_DIR)

fclean:clean
	@rm -f $(NAME)
	@make $@ -C $(DIR_LIBFT)

re: fclean all
