COMPILER = gcc
NAME = mp3p
FLAGS = -Wall -Wextra -Werror
INCLUDES = -lreadline -I include

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

.PHONY: all clean fclean re