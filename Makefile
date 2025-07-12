COMPILER = gcc
NAME = id3shell
FLAGS = -Wall -Wextra -Werror
INCLUDES = -lreadline -Iinclude

SRC_FOLDER = src/linux

ID3LIB_SRC_FOLDER = $(SRC_FOLDER)/id3
ID3LIB_SRC_FILES = id3tagged_file.c tags.c tag_headers.c framelists.c frames.c frame_headers.c
ID3LIB_SRC = $(addprefix $(ID3LIB_SRC_FOLDER)/, $(ID3LIB_SRC_FILES))

ID3LIB_OBJ = $(ID3LIB_SRC:.c=.o)

ID3LIB_FOLDER = $(SRC_FOLDER)/lib
ID3LIB = $(ID3LIB_FOLDER)/id3lib.a

BIN_SRC_FOLDER = $(SRC_FOLDER)/programs
BIN_SRC_FILES = clear_padding.c delete_frame.c id3shell.c export_tag.c print_tag.c
BIN_SRC = $(addprefix $(BIN_SRC_FOLDER)/, $(BIN_SRC_FILES))
BIN_OBJ = $(BIN_SRC:.c=.o)

OBJ = $(BIN_OBJ) $(SRC_FOLDER)/parsing.o

BIN_FOLDER = bin

ID3SHELL = $(BIN_FOLDER)/$(NAME)

all: $(ID3SHELL) clear_padding delete_frame export_tag print_tag

# LIB OBJECTS #

%.o: %.c
	@$(COMPILER) -Iinclude $(FLAGS) -c -o $@ $< -lreadline

$(ID3LIB): $(ID3LIB_OBJ)
	@mkdir -p $(ID3LIB_FOLDER)
	@ar rcs $@ $^
	@rm $(ID3LIB_OBJ)

# BINARY FILES #

$(ID3SHELL): $(BIN_SRC_FOLDER)/id3shell.o $(ID3LIB) $(SRC_FOLDER)/parsing.o
	@mkdir -p $(BIN_FOLDER)
	@$(COMPILER) $(FLAGS) -Iinclude -o $(ID3SHELL) $^ -lreadline -D BIN_DIR_NAME=$(BIN_FOLDER)

clear_padding: $(BIN_SRC_FOLDER)/clear_padding.o $(ID3LIB)
	@mkdir -p $(BIN_FOLDER)
	@$(COMPILER) $(FLAGS) -Iinclude -o $(BIN_FOLDER)/$@ $^

export_tag: $(BIN_SRC_FOLDER)/export_tag.o $(ID3LIB)
	@mkdir -p $(BIN_FOLDER)
	@$(COMPILER) $(FLAGS) -Iinclude -o $(BIN_FOLDER)/$@ $^

print_tag: $(BIN_SRC_FOLDER)/print_tag.o $(ID3LIB)
	@$(COMPILER) $(FLAGS) -Iinclude -o $(BIN_FOLDER)/$@ $^

delete_frame: $(BIN_SRC_FOLDER)/delete_frame.o $(ID3LIB)
	@$(COMPILER) $(FLAGS) -Iinclude -o $(BIN_FOLDER)/$@ $^

clean:
	@rm -rf $(OBJ)
	@rm -rf $(ID3LIB_FOLDER)

fclean: clean
	@rm -rf $(BIN_FOLDER)

re: fclean all

leaks: $(ID3SHELL)
	@valgrind --leak-check=full --show-leak-kinds=all ./$(ID3SHELL) 2> shell_leaks.txt

.PHONY: all clean fclean re