NAME        := ft_irc
CC          := c++
CFLAGS      := -Wall -Wextra -Werror -g -I includes -std=c++98
RM          := rm -rf
MAKE        := make --no-print-directory

OBJ_DIR     := build
SRC_DIR     := .

SRC_MAIN_DIR    := $(SRC_DIR)
SRC_MAIN        := main.cpp

SRCS        := $(addprefix $(SRC_MAIN_DIR)/, $(SRC_MAIN))

OBJS        := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) -o $(NAME)
	@echo "\033[1;32m$(NAME) created.\033[0m"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@$(RM) $(OBJ_DIR)
	@echo "\033[1;32mObjects cleaned.\033[0m"

fclean: clean
	@$(RM) $(NAME)
	@echo "\033[1;32mEverything cleaned.\033[0m"

re: fclean all

.PHONY: all clean fclean re
