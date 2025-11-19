# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: luevange <luevange@student.42roma.it>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/26 18:40:00 by luevange          #+#    #+#              #
#    Updated: 2025/11/10 15:28:14 by luevange         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# ============================================================================ #
#                                  VARIABLES                                   #
# ============================================================================ #

NAME		= minishell

# Compiler settings
CC			= cc
CFLAGS		= -Wall -Wextra -Werror -g
READLINE	= -lreadline

# Directories
SRC_DIR		= src
OBJ_DIR		= obj
UTILS_DIR	= utils
INCLUDES	= -I./includes -I./$(UTILS_DIR)/includes

# Source directories
LEXER_DIR	= $(SRC_DIR)/lexer
PARSER_DIR	= $(SRC_DIR)/parser
VALIDATE_DIR = $(SRC_DIR)/validate
EXECUTOR_DIR = $(SRC_DIR)/executor
ENV_DIR		= $(SRC_DIR)/handle_env
BUILTIN_DIR	= $(SRC_DIR)/built-in
UTILS_SRC_DIR = $(SRC_DIR)/utils

# Source files
SRCS		= $(SRC_DIR)/minishell.c \
			  $(LEXER_DIR)/lexer_utils.c \
			  $(LEXER_DIR)/tokenize.c \
			  $(LEXER_DIR)/tokenize2.c \
			  $(VALIDATE_DIR)/check_syntax.c \
			  $(VALIDATE_DIR)/check_operators.c \
			  $(VALIDATE_DIR)/check_parens.c \
			  $(VALIDATE_DIR)/print_error.c \
			  $(PARSER_DIR)/parser.c \
			  $(PARSER_DIR)/parser_utils.c \
			  $(PARSER_DIR)/parser_redir.c \
			  $(PARSER_DIR)/parser_primary.c \
			  $(PARSER_DIR)/parser_command.c \
			  $(PARSER_DIR)/parser_free.c \
			  $(EXECUTOR_DIR)/executor.c \
			  $(EXECUTOR_DIR)/execute_command.c \
			  $(EXECUTOR_DIR)/execute_pipe.c \
			  $(EXECUTOR_DIR)/redirections.c \
			  $(ENV_DIR)/env_init.c \
			  $(ENV_DIR)/env_utils.c \
			  $(BUILTIN_DIR)/builtin_utils.c \
			  $(BUILTIN_DIR)/builtin_echo.c \
			  $(BUILTIN_DIR)/builtin_cd.c \
			  $(BUILTIN_DIR)/builtin_pwd.c \
			  $(BUILTIN_DIR)/builtin_export.c \
			  $(BUILTIN_DIR)/builtin_unset.c \
			  $(BUILTIN_DIR)/builtin_env.c \
			  $(BUILTIN_DIR)/builtin_exit.c \
			  $(UTILS_SRC_DIR)/expand.c \
			  $(UTILS_SRC_DIR)/signals.c

# Object files (tutti nella cartella obj/)
OBJS		= $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

# Libraries
LIBFT		= $(UTILS_DIR)/libft.a

# Colors for pretty output
GREEN		= \033[0;32m
BLUE		= \033[0;34m
YELLOW		= \033[0;33m
RED			= \033[0;31m
RESET		= \033[0m

# ============================================================================ #
#                                   RULES                                      #
# ============================================================================ #

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	@echo "$(BLUE)Linking $(NAME)...$(RESET)"
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(READLINE) -o $(NAME)
	@echo "$(GREEN)✓ $(NAME) compiled successfully!$(RESET)"

# Crea directory obj/ mantenendo la struttura delle sottocartelle
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "$(YELLOW)Compiling $<...$(RESET)"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

# Compila la libreria utils
$(LIBFT):
	@echo "$(BLUE)Building libft...$(RESET)"
	@$(MAKE) -C $(UTILS_DIR)
	@echo "$(GREEN)✓ libft compiled!$(RESET)"

# ============================================================================ #
#                                 CLEANUP                                      #
# ============================================================================ #

clean:
	@echo "$(RED)Cleaning object files...$(RESET)"
	@rm -rf $(OBJ_DIR)
	@$(MAKE) -C $(UTILS_DIR) clean
	@echo "$(GREEN)✓ Clean complete!$(RESET)"

fclean: clean
	@echo "$(RED)Removing $(NAME)...$(RESET)"
	@rm -f $(NAME)
	@$(MAKE) -C $(UTILS_DIR) fclean
	@echo "$(GREEN)✓ Full clean complete!$(RESET)"

re: fclean all

# ============================================================================ #
#                                  BONUS                                       #
# ============================================================================ #

bonus: all

# ============================================================================ #
#                                  DEBUG                                       #
# ============================================================================ #

debug: CFLAGS += -g3 -fsanitize=address
debug: re

valgrind: CFLAGS += -g3
valgrind: re

# ============================================================================ #
#                                  PHONY                                       #
# ============================================================================ #

.PHONY: all clean fclean re bonus debug valgrind
