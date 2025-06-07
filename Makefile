NAME			=	minishell

# Directories
LIBFT			=	./lib/libft/libft.a
INC				=	include/
SRC_DIR			=	src/
OBJ_DIR			=	obj/

# Compiler and CFlags
CC 				=	cc
CFLAGS			=	-Wall -Werror -Wextra 
LDFLAGS			=	-lreadline 

# Sources
MINISHELL_FILE	=	main.c utils.c lexer.c parser.c tok-list.c tok-split.c exec.c built-in.c

SRCS			= $(addprefix $(SRC_DIR), $(MINISHELL_FILE))
OBJ				= $(SRCS:$(SRC_DIR)%.c=$(OBJ_DIR)%.o)

# Build rules 
all:			$(LIBFT) $(NAME)

$(LIBFT):
				@echo "Building libft..."
				@make -C ./lib/libft
				@echo "libft built successfully!"

$(NAME):		$(OBJ) $(LIBFT)
				@echo "Building minishell..."
				$(CC) $(CFLAGS) -I $(INC) $(OBJ) $(LIBFT) -o $(NAME) $(LDFLAGS)
				@echo "minishell built successfully!"

$(OBJ_DIR)%.o:	$(SRC_DIR)%.c
				@mkdir -p $(OBJ_DIR)
				@$(CC) $(CFLAGS) -I $(INC) -c $< -o $@
				@echo "Compiled $< to $@"

clean:
				@echo "Cleaning object files..."
				@rm -f -r $(OBJ_DIR)
				@make clean -C ./lib/libft
				@echo "Cleaned successfully!"

fclean: 		clean
				@echo "Removing executable..."
				@rm -f $(NAME)
				@rm -f $(LIBFT)
				@echo "Executable removed successfully!"


re:				fclean all

.PHONY: 		all clean fclean re