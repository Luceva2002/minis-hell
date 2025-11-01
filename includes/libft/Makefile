NAME = libft.a

CC = gcc

CFLAGS += -Wall -Wextra -Werror

SRC = ft_isascii.c  ft_memmove.c  ft_strjoin.c	ft_strtrim.c\
ft_atoi.c  ft_isdigit.c  ft_memset.c  ft_strlcpy.c  ft_substr.c\
ft_bzero.c    ft_isprint.c    ft_strlen.c	ft_tolower.c\
ft_calloc.c   ft_memchr.c   ft_strlcat.c   ft_strncmp.c	ft_toupper.c\
ft_isalnum.c  ft_memcmp.c   ft_strchr.c   ft_isalpha.c  ft_memcpy.c \
ft_strdup.c   ft_strrchr.c  ft_strnstr.c ft_strmapi.c ft_striteri.c  ft_strndup.c\
ft_putchar_fd.c  ft_putstr_fd.c  ft_putendl_fd.c  ft_putnbr_fd.c  ft_split.c ft_itoa.c

BNS = ft_lstnew.c  ft_lstadd_front.c  ft_lstsize.c  ft_lstlast.c\
ft_lstadd_back.c ft_lstdelone.c  ft_lstclear.c  ft_lstiter.c ft_lstmap.c

all: $(NAME)

OBJ = $(SRC:.c=.o)

BNS_OBJ = $(BNS:.c=.o)

bonus: $(BNS_OBJ)
	ar rcs $(NAME) $(BNS_OBJ)

$(NAME): $(OBJ)
	ar rcs $(NAME) $(OBJ)

%.o: %.c
	$(CC) $(CFLAGS)	-c	$<	-o	$@

clean:
	rm -f $(OBJ) $(BNS_OBJ)

fclean: clean
	rm -f $(NAME)

re: fclean all
