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
	$(COMPILER) -Iinclude $(FLAGS) -c -o $@ $< -lreadline

frames = $(LIB_FOLDER)/frames.o
tags = $(LIB_FOLDER)/tags.o
id3tagged_file = $(LIB_FOLDER)/id3tagged_file.o
parsing = $(LIB_FOLDER)/parsing.o

$(id3tagged_file): $(tags)

$(tags): $(frames)

# BINARY FILES #

all: $(NAME)

$(NAME): $(PRG_FOLDER)/main.o $(id3tagged_file) $(tags) $(frames) $(parsing)
	$(COMPILER) $(FLAGS) -Iinclude -o $(NAME) $^ -lreadline

bin/clear_padding: $(PRG_FOLDER)/clear_padding.o $(id3tagged_file) $(tags) $(frames)
	mkdir -p bin
	$(COMPILER) $(FLAGS) -Iinclude -o $@ $^

clean:
	@rm -rf $(LIB_FOLDER)/*.o $(PRG_FOLDER)/*.o
	@rm -rf .temp

fclean: clean
	@rm -rf $(NAME)
	@rm -rf bin

re: fclean all

leaks: $(NAME)
	@valgrind --leak-check=full --show-leak-kinds=all ./$(NAME) 2> shell_leaks.txt

.PHONY: all clean fclean re