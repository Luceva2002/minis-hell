# **************************************************************************** #
#                                  Minishell                                   #
# **************************************************************************** #

NAME = minishell

# Compiler setup
CC = cc
CFLAGS = -Wall -Wextra -Werror -g
RM = rm -f

# Directories
SRC_DIR = src
OBJ_DIR = obj
INC_DIR = includes
LIBFT_DIR = libft

# Sources (only parsing stage for now)
SRC = $(SRC_DIR)/main.c \
      $(SRC_DIR)/parser/lexer.c \
      $(SRC_DIR)/parser/parser.c

# Build object file names in obj/ from sources
OBJ = $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Libft archive
LIBFT = $(LIBFT_DIR)/libft.a

# Default target
all: $(NAME)

# Link final executable
$(NAME): $(OBJ) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJ) -L$(LIBFT_DIR) -lft -lreadline -o $(NAME)

# Compile .c → .o into obj/
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I$(INC_DIR) -I$(LIBFT_DIR) -c $< -o $@

# Build libft first
$(LIBFT):
	@$(MAKE) -C $(LIBFT_DIR)

# Cleaning rules
clean:
	$(RM) -r $(OBJ_DIR)
	@$(MAKE) -C $(LIBFT_DIR) clean

fclean: clean
	$(RM) $(NAME)
	@$(MAKE) -C $(LIBFT_DIR) fclean

re: fclean all

.PHONY: all clean fclean re
