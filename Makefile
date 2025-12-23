# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: luevange <luevange@student.42roma.it>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/10/26 18:40:00 by luevange          #+#    #+#              #
#    Updated: 2025/12/19 15:50:06 by luevange         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= minishell

CC			= cc
CFLAGS		= -Wall -Wextra -Werror -g
READLINE	= -lreadline

SRC_DIR		= src
OBJ_DIR		= obj
UTILS_DIR	= utils
INCLUDES	= -I./includes -I./$(UTILS_DIR)/includes

LEXER_DIR	= $(SRC_DIR)/lexer
PARSER_DIR	= $(SRC_DIR)/parser
VALIDATE_DIR = $(SRC_DIR)/validate
EXECUTOR_DIR = $(SRC_DIR)/executor
ENV_DIR		= $(SRC_DIR)/handle_env
BUILTIN_DIR	= $(SRC_DIR)/built-in
UTILS_SRC_DIR = $(SRC_DIR)/utils

SRCS		= $(SRC_DIR)/minishell.c \
			  $(LEXER_DIR)/lexer_utils.c \
			  $(LEXER_DIR)/tokenize.c \
			  $(LEXER_DIR)/tokenize2.c \
			  $(LEXER_DIR)/tokenize3.c \
			  $(VALIDATE_DIR)/check_syntax.c \
			  $(VALIDATE_DIR)/check_operators.c \
			  $(VALIDATE_DIR)/check_operators_utils.c \
			  $(VALIDATE_DIR)/check_parens.c \
			  $(VALIDATE_DIR)/print_error.c \
			  $(PARSER_DIR)/parser.c \
			  $(PARSER_DIR)/parser_utils.c \
			  $(PARSER_DIR)/parser_redir.c \
			  $(PARSER_DIR)/parser_primary.c \
			  $(PARSER_DIR)/parser_command.c \
			  $(PARSER_DIR)/parser_free.c \
			  $(EXECUTOR_DIR)/executor.c \
			  $(EXECUTOR_DIR)/executor_simple.c \
			  $(EXECUTOR_DIR)/executor_logic.c \
			  $(EXECUTOR_DIR)/execute_command.c \
			  $(EXECUTOR_DIR)/path_utils.c \
			  $(EXECUTOR_DIR)/execute_pipe.c \
			  $(EXECUTOR_DIR)/redirections.c \
			  $(EXECUTOR_DIR)/heredoc.c \
			  $(ENV_DIR)/env_init.c \
			  $(ENV_DIR)/env_utils.c \
			  $(ENV_DIR)/env_array.c \
			  $(BUILTIN_DIR)/builtin_utils.c \
			  $(BUILTIN_DIR)/builtin_echo.c \
			  $(BUILTIN_DIR)/builtin_cd.c \
			  $(BUILTIN_DIR)/builtin_pwd.c \
			  $(BUILTIN_DIR)/builtin_export.c \
			  $(BUILTIN_DIR)/builtin_unset.c \
			  $(BUILTIN_DIR)/builtin_env.c \
			  $(BUILTIN_DIR)/builtin_exit.c \
			  $(UTILS_SRC_DIR)/expand.c \
			  $(UTILS_SRC_DIR)/expand_utils.c \
			  $(UTILS_SRC_DIR)/expand_process.c \
			  $(UTILS_SRC_DIR)/signals.c \
			  $(UTILS_SRC_DIR)/banner.c

OBJS		= $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

LIBFT		= $(UTILS_DIR)/libft.a
GREEN		= \033[0;32m
BLUE		= \033[0;34m
YELLOW		= \033[0;33m
RED			= \033[0;31m
RESET		= \033[0m

all: $(NAME)

$(NAME): $(LIBFT) $(OBJS)
	@echo "$(BLUE)Linking $(NAME)...$(RESET)"
	@$(CC) $(CFLAGS) $(OBJS) $(LIBFT) $(READLINE) -o $(NAME)
	@echo "$(GREEN)✓ $(NAME) compiled successfully!$(RESET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@echo "$(YELLOW)Compiling $<...$(RESET)"
	@$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(LIBFT):
	@echo "$(BLUE)Building libft...$(RESET)"
	@$(MAKE) -C $(UTILS_DIR)
	@echo "$(GREEN)✓ libft compiled!$(RESET)"

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

bonus: all

debug: CFLAGS += -g3 -fsanitize=address
debug: re
run:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=readline.supp  ./$(NAME)
valgrind: CFLAGS += -g3
valgrind: re

.PHONY: all clean fclean re bonus debug valgrind run
