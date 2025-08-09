COMPILER = gcc
NAME = id3shell
FLAGS = -Wall -Wextra -Werror
INCLUDES = -lreadline -Iinclude -Iinclude/id3lib

SRC_FOLDER = src/linux
INC_FOLDER = include
LIB_FOLDER = lib
BIN_FOLDER = bin

OBJ = $(BIN_OBJ) $(ID3LIB_OBJ) $(APIC_OBJ) $(SRC_FOLDER)/parsing.o $(SRC_FOLDER)/utils/album_pics.o $(SRC_FOLDER)/utils/lyrics.o

# Main rules #
all: $(ID3SHELL) clear_padding delete_frame export_tag print_tag

%.o: %.c
	@$(COMPILER) -Iinclude -Iinclude/id3lib $(FLAGS) -c -o $@ $<

clean:
	@rm -rf $(OBJ)

fclean: clean
	@rm -rf $(LIB_FOLDER)
	@rm -rf $(BIN_FOLDER)

re: fclean all

# ID3 Library #
ID3LIB_INC = $(INC_FOLDER)/id3lib
ID3LIB_SRC_FOLDER = $(SRC_FOLDER)/id3lib
ID3LIB_SRC_FILES = id3tagged_file.c tags.c tag_headers.c framelists.c frames.c frame_headers.c frame_content.c
ID3LIB_SRC = $(addprefix $(ID3LIB_SRC_FOLDER)/, $(ID3LIB_SRC_FILES))
ID3LIB_OBJ = $(ID3LIB_SRC:.c=.o)

ID3LIB_FOLDER = $(LIB_FOLDER)/id3lib
ID3LIB = $(ID3LIB_FOLDER)/id3lib.a

$(ID3LIB_OBJ): %.o: %.c
	@$(COMPILER) -I$(ID3LIB_INC) $(FLAGS) -c -o $@ $<

$(ID3LIB): $(ID3LIB_OBJ)
	@mkdir -p $(ID3LIB_FOLDER)
	@ar rcs $@ $^
	@rm -rf $(ID3LIB_OBJ)

# APIC Library #
APIC_INC = $(INC_FOLDER)/apiclib
APIC_SRC_FOLDER = $(SRC_FOLDER)/apic_lib
APIC_SRC_FILES = album_pics.c
APIC_SRC = $(addprefix $(APIC_SRC_FOLDER)/, $(APIC_SRC_FILES))
APIC_OBJ = $(APIC_SRC:.c=.o)

APIC_FOLDER = $(LIB_FOLDER)/apiclib
APIC = $(APIC_FOLDER)/apic.a

$(APIC_OBJ): %.o: %.c $(ID3LIB_OBJ)
	@$(COMPILER) -I$(APIC_INC) -I$(ID3LIB_INC) $(FLAGS) -c -o $@ $<

$(APIC): $(APIC_OBJ)
	@mkdir -p $(APIC_FOLDER)
	@ar rcs $@ $^

# Binaries #
BIN_SRC_FOLDER = $(SRC_FOLDER)/programs
BIN_SRC_FILES = clear_padding.c delete_frame.c id3shell.c export_tag.c print_tag.c
BIN_SRC = $(addprefix $(BIN_SRC_FOLDER)/, $(BIN_SRC_FILES))
BIN_OBJ = $(BIN_SRC:.c=.o)

ID3SHELL = $(BIN_FOLDER)/$(NAME)

$(ID3SHELL): $(BIN_SRC_FOLDER)/id3shell.o $(ID3LIB) $(SRC_FOLDER)/parsing.o
	@mkdir -p $(BIN_FOLDER)
	@$(COMPILER) $(FLAGS) -Iinclude -o $(ID3SHELL) $^ -lreadline -D BIN_DIR_NAME=$(BIN_FOLDER)

clear_padding: $(BIN_SRC_FOLDER)/clear_padding.o $(ID3LIB)
	@mkdir -p $(BIN_FOLDER)
	@$(COMPILER) $(FLAGS) -I$(ID3LIB_INC) -o $(BIN_FOLDER)/$@ $^

export_tag: $(BIN_SRC_FOLDER)/export_tag.o $(ID3LIB)
	@mkdir -p $(BIN_FOLDER)
	@$(COMPILER) $(FLAGS) -I$(ID3LIB_INC) -o $(BIN_FOLDER)/$@ $^

print_tag: $(BIN_SRC_FOLDER)/print_tag.c $(ID3LIB)
	@mkdir -p $(BIN_FOLDER)
	@$(COMPILER) $(FLAGS) -I$(ID3LIB_INC) -o $(BIN_FOLDER)/$@ $^

delete_frame: $(BIN_SRC_FOLDER)/delete_frame.o $(ID3LIB)
	@mkdir -p $(BIN_FOLDER)
	@$(COMPILER) $(FLAGS) -I$(ID3LIB_INC) -o $(BIN_FOLDER)/$@ $^

export_picture: $(APIC) $(ID3LIB) src/linux/programs/export_picture.c
	@mkdir -p $(BIN_FOLDER)
	@$(COMPILER) $(FLAGS) -I$(ID3LIB_INC) -I$(APIC_INC) -o $(BIN_FOLDER)/$@ $^

export_lyrics: $(USLT) $(ID3LIB) src/linux/programs/export_picture.c
	@mkdir -p $(BIN_FOLDER)
	@$(COMPILER) $(FLAGS) -I$(ID3LIB_INC) -I$(USLT_INC) -o $(BIN_FOLDER)/$@ $^
#  #
leaks: $(ID3SHELL)
	@valgrind --leak-check=full --show-leak-kinds=all ./$(ID3SHELL) 2> shell_leaks.txt

.PHONY: all clean fclean re print_tag export_picture