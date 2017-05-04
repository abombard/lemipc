CC=clang
FLAGS42=-Wall -Wextra -Werror -Wconversion
FLAGS=

NAME=lemipc

DIR_LIBFT=libs/libft
DIR_LIST=libs/list
LIBS_BIN=$(DIR_LIBFT)/libft.a $(DIR_LIST)/liblist.a
LIBS=-L libs/libft/ -lft -L libs/list/ -llist -lpthread -lrt -lm

SRC_DIR=src
INCLUDES=-I ./ -I$(SRC_DIR) -I $(DIR_LIBFT) -I $(DIR_LIST)

BUILD_DIR=__build

SRC=\
	main.c		\
	shm.c		\
	sem.c		\
	mq.c		\
	player.c	\
	utils.c		\
	iabombard.c	\
	#iaduban.c	\

OBJ=$(addprefix $(BUILD_DIR)/,$(SRC:.c=.o))

all:$(BUILD_DIR) lemipc_iabombard #lemipc_iaduban 

$(BUILD_DIR):
	@mkdir -p $@

$(BUILD_DIR)/%.o:$(SRC_DIR)/%.c
	@$(CC) $(FLAGS) -c $< $(INCLUDES) -o $@

$(LIBS_BIN):
	@make -C $(DIR_LIBFT)
	@make -C $(DIR_LIST)

lemipc_iaduban:$(LIBS_BIN) $(OBJ)
	@$(CC) $(FLAGS) $(addprefix $(SRC_DIR)/,$(SRC)) $(LIBS) $(INCLUDES) -o $@ -D IADUBAN
	@echo "$@ was created"

lemipc_iabombard:$(LIBS_BIN) $(OBJ)
	@$(CC) $(FLAGS) $(addprefix $(SRC_DIR)/,$(SRC)) $(LIBS) $(INCLUDES) -o $@ -D IABOMBARD
	@echo "$@ was created"

clean:
	@rm -rf $(BUILD_DIR)

fclean:clean
	@rm -f lemipc_iabombard lemipc_iaduban
	@make $@ -C $(DIR_LIBFT)
	@make $@ -C $(DIR_LIST)

re: fclean all
