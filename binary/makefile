#
# Written by Hampus Fridholm
#
# Last updated: 2024-09-22
#

PROGRAM := minesweeper

CLEAN_TARGET := clean
HELP_TARGET  := help

DELETE_CMD := rm

COMPILER := gcc
COMPILE_FLAGS := -Wall -Werror -g -Og -std=gnu99 -oFast -pthread

LINKER_FLAGS := -lm `sdl2-config --libs --cflags` -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer

SOURCE_DIR := ../source
OBJECT_DIR := ../object
BINARY_DIR := ../binary

SOURCE_FILES := $(wildcard $(SOURCE_DIR)/*/*.c $(SOURCE_DIR)/*.c)
HEADER_FILES := $(wildcard $(SOURCE_DIR)/*/*.h $(SOURCE_DIR)/*.h)

OBJECT_FILES := $(addprefix $(OBJECT_DIR)/, $(notdir $(SOURCE_FILES:.c=.o)))

# This is used because these targets are not files
.PHONY: $(CLEAN_TARGET) $(HELP_TARGET)

all: $(PROGRAM)

$(PROGRAM): $(OBJECT_FILES) $(SOURCE_FILES) $(HEADER_FILES)
	$(COMPILER) $(OBJECT_FILES) $(COMPILE_FLAGS) $(LINKER_FLAGS) -o $(BINARY_DIR)/$@

# These are rules for compiling object files out of source files
$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/*/%.c 
	$(COMPILER) $< -c $(COMPILE_FLAGS) -o $@

$(OBJECT_DIR)/%.o: $(SOURCE_DIR)/%.c 
	$(COMPILER) $< -c $(COMPILE_FLAGS) -o $@

# These are the files that should be kept (not automatically deleted)
.PRECIOUS: $(OBJECT_DIR)/%.o $(PROGRAM)

$(CLEAN_TARGET):
	$(DELETE_CMD) $(OBJECT_DIR)/*.o $(PROGRAM)

$(HELP_TARGET):
	@echo $(PROGRAM) $(CLEAN_TARGET)
