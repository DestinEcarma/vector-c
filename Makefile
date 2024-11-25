# Change the commands that needs to be used for Windows and Unix systems
ifeq ($(OS),Windows_NT)
    EXE_EXT = .exe
    MKDIR = mkdir
    RM = rmdir /S /Q
	SEP = \\
else
    EXE_EXT = 
    MKDIR = mkdir -p
    RM = rm -rf
	SEP = /
endif

# Source and header files
SOURCES = vector.c main.c
HEADERS = vector.h

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11

# Directory
TARGET_DIR = target
BIN_DIR = $(TARGET_DIR)$(SEP)bin
OBJ_DIR = $(TARGET_DIR)$(SEP)obj

# Object files
OBJECTS = $(patsubst %.c,$(OBJ_DIR)$(SEP)%.o,$(SOURCES))

# Target executable
TARGET = $(BIN_DIR)$(SEP)main$(EXE_EXT)

# Default rule to run the program
default: run
all: $(TARGET)

# Rule to link the object files to create the executable
$(TARGET): $(OBJECTS) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

# Rule to compile the .c files into .o files
$(OBJ_DIR)$(SEP)%.o: %.c $(HEADERS) | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to create the target directories if they don't exist
$(BIN_DIR) $(OBJ_DIR):
	$(MKDIR) $@

# Rule to clean up the compiled files
clean:
	$(RM) $(TARGET_DIR)

# Rule to run the program
run: $(TARGET)
	.$(SEP)$(TARGET)
