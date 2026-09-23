NAME        := ircserv
CC          := c++
CFLAGS      := -Wall -Wextra -Werror -g -I includes -std=c++98
RM          := rm -rf
MAKE        := make --no-print-directory

OBJ_DIR     := build
SRC_DIR     := srcs

SRCS_DIR    := $(SRC_DIR)
SRCS      := main.cpp Server.cpp signal.cpp Client.cpp Channel.cpp

SRCS        := $(addprefix $(SRCS_DIR)/, $(SRCS))

OBJS        := $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRCS))

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) $(LDFLAGS) $(OBJS) -o $(NAME)
	@echo "\033[1;32m$(NAME) created.\033[0m"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
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
