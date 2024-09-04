COMPILER = gcc
NAME = mp3p
FLAGS = -Wall -Wextra -Werror
INCLUDES = -lreadline -Iinclude

SRC_FOLDER = src/linux
SRC_FILES = main.c parsing.c
SRC = $(addprefix $(SRC_FOLDER)/, $(SRC_FILES))

LIB_FOLDER = $(SRC_FOLDER)/lib
PRG_FOLDER = $(SRC_FOLDER)/programs


# LIB OBJECTS #

%.o: %.c
	$(COMPILER) -Iinclude $(FLAGS) -c -o $@ $<

frames: $(LIB_FOLDER)/frames.o
tags: $(LIB_FOLDER)/tags.o frames
id3tagged_file: $(LIB_FOLDER)/id3tagged_file.o tags

# BINARY FILES #

all: $(NAME)

$(NAME): $(PRG_FOLDER)/main.o id3tagged_file src/linux/lib/parsing.o
	$(COMPILER) $(FLAGS) -o $(NAME) $< $(INCLUDES)

clean:

fclean: clean
	@rm -rf $(NAME)

re: fclean all

leaks: $(NAME)
	@valgrind --leak-check=full --show-leak-kinds=all ./$(NAME) 2> shell_leaks.txt

.PHONY: all clean fclean re frames tags id3tagged_file