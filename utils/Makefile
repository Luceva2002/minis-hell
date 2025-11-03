# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: luevange <luevange@student.42roma.it>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/11/02 00:00:00 by luevange          #+#    #+#              #
#    Updated: 2025/11/03 01:15:00 by luevange         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= libft.a
OBJ_DIR		= obj

# Directories
LIBFT_DIR	= libft
PRINTF_DIR	= printf
GNL_DIR		= get_next_line

# Source files
LIBFT_SRCS	= $(LIBFT_DIR)/ft_atoi.c \
			  $(LIBFT_DIR)/ft_bzero.c \
			  $(LIBFT_DIR)/ft_calloc.c \
			  $(LIBFT_DIR)/ft_isalnum.c \
			  $(LIBFT_DIR)/ft_isalpha.c \
			  $(LIBFT_DIR)/ft_isascii.c \
			  $(LIBFT_DIR)/ft_isdigit.c \
			  $(LIBFT_DIR)/ft_isprint.c \
			  $(LIBFT_DIR)/ft_itoa.c \
			  $(LIBFT_DIR)/ft_memchr.c \
			  $(LIBFT_DIR)/ft_memcmp.c \
			  $(LIBFT_DIR)/ft_memcpy.c \
			  $(LIBFT_DIR)/ft_memmove.c \
			  $(LIBFT_DIR)/ft_memset.c \
			  $(LIBFT_DIR)/ft_putchar_fd.c \
			  $(LIBFT_DIR)/ft_putendl_fd.c \
			  $(LIBFT_DIR)/ft_putnbr_fd.c \
			  $(LIBFT_DIR)/ft_putstr_fd.c \
			  $(LIBFT_DIR)/ft_split.c \
			  $(LIBFT_DIR)/ft_strchr.c \
			  $(LIBFT_DIR)/ft_strdup.c \
			  $(LIBFT_DIR)/ft_striteri.c \
			  $(LIBFT_DIR)/ft_strjoin.c \
			  $(LIBFT_DIR)/ft_strlcat.c \
			  $(LIBFT_DIR)/ft_strlcpy.c \
			  $(LIBFT_DIR)/ft_strlen.c \
			  $(LIBFT_DIR)/ft_strmapi.c \
			  $(LIBFT_DIR)/ft_strncmp.c \
			  $(LIBFT_DIR)/ft_strndup.c \
			  $(LIBFT_DIR)/ft_strnstr.c \
			  $(LIBFT_DIR)/ft_strrchr.c \
			  $(LIBFT_DIR)/ft_strtrim.c \
			  $(LIBFT_DIR)/ft_substr.c \
			  $(LIBFT_DIR)/ft_tolower.c \
			  $(LIBFT_DIR)/ft_toupper.c

PRINTF_SRCS	= $(PRINTF_DIR)/ft_printf.c \
			  $(PRINTF_DIR)/ft_put_char.c \
			  $(PRINTF_DIR)/ft_put_hex_nmb.c \
			  $(PRINTF_DIR)/ft_put_nbr.c \
			  $(PRINTF_DIR)/ft_put_pntr.c \
			  $(PRINTF_DIR)/ft_put_str.c \
			  $(PRINTF_DIR)/ft_put_uns_nmb.c

GNL_SRCS	= $(GNL_DIR)/get_next_line_bonus.c \
			  $(GNL_DIR)/get_next_line_utils_bonus.c

ALL_SRCS	= $(LIBFT_SRCS) $(PRINTF_SRCS) $(GNL_SRCS)

# Object files (tutti nella cartella obj/)
OBJS		= $(patsubst %.c,$(OBJ_DIR)/%.o,$(ALL_SRCS))

CC			= cc
CFLAGS		= -Wall -Wextra -Werror

all: $(NAME)

$(NAME): $(OBJS)
	@ar rcs $(NAME) $(OBJS)
	@echo "✓ libft.a created!"

# Crea directory obj/ mantenendo la struttura delle sottocartelle
$(OBJ_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJ_DIR)

fclean: clean
	@rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
