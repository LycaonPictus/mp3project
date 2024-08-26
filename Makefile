COMPILER = gcc
NAME = mp3p
FLAGS = -Wall -Wextra -Werror
INCLUDES = -lreadline -Iinclude

SRC_FOLDER = src/linux
SRC_FILES = main.c parsing.c
SRC = $(addprefix $(SRC_FOLDER)/, $(SRC_FILES))

$(NAME): $(SRC)
	@$(COMPILER) $(FLAGS) -o $(NAME) $(SRC) $(INCLUDES)

all: $(NAME)

clean:

fclean: clean
	@rm -rf $(NAME)

re: fclean all

leaks: $(NAME)
	@valgrind --leak-check=full --show-leak-kinds=all ./$(NAME) 2> shell_leaks.txt

.PHONY: all clean fclean re