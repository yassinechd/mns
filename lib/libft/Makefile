NAME	=	libft.a
SOURCES	=	ft_isalnum.c ft_isprint.c ft_memcpy.c ft_putendl_fd.c ft_strchr.c ft_strlcat.c ft_strncmp.c ft_substr.c \
			ft_atoi.c ft_isalpha.c ft_itoa.c ft_memmove.c ft_putnbr_fd.c ft_strdup.c ft_strlcpy.c ft_strnstr.c ft_tolower.c \
			ft_bzero.c ft_isascii.c ft_memchr.c ft_memset.c ft_putstr_fd.c ft_striteri.c ft_strlen.c ft_strrchr.c ft_toupper.c \
			ft_calloc.c	ft_isdigit.c ft_memcmp.c ft_putchar_fd.c ft_split.c ft_strjoin.c ft_strmapi.c ft_strtrim.c ft_printf.c \
			prints.c prints2.c get_next_line.c get_next_line_utils.c \
			ft_lstadd_back.c ft_lstadd_front.c ft_lstclear.c ft_lstdelone.c ft_lstlast.c ft_lstnew.c ft_lstsize.c
OBJS	=	$(SOURCES:.c=.o)
CC	=	gcc
CFLAGS	= -Wall -Wextra -Werror
AR	= ar

all:
	@echo "...Compilation en cours..."
	@$(MAKE) $(NAME)

$(NAME): $(OBJS)
	@$(AR) -rcs $(NAME) $(OBJS)

%.o: %.c
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@echo "...Suppression des fichiers objets..."
	@rm -f $(OBJS)

fclean: clean
	@echo "...Suppression de la librairie..."
	@rm -f $(NAME)

re: fclean all
	@echo "...Recompilation terminée."
.PHONY: 		all clean fclean re