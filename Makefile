# Source and header files
SOURCES = vector.c main.c
HEADERS = vector.h

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11

# Directory
TARGET_DIR = target
BIN_DIR = $(TARGET_DIR)/bin
OBJ_DIR = $(TARGET_DIR)/obj

# Object files
OBJECTS = $(patsubst %.c, $(OBJ_DIR)/%.o, $(SOURCES))

# Target executable
TARGET = $(BIN_DIR)/main

# Default rule to run the program
default: run
all: $(TARGET)

# Rule to link the object files to create the executable
$(TARGET): $(OBJECTS) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

# Rule to compile the .c files into .o files
$(OBJ_DIR)/%.o: %.c $(HEADERS) | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Rule to create the target directories if they don't exist
$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

# Rule to clean up the compiled files
clean:
	rm -rf $(TARGET_DIR)

# Rule to run the program
run: $(TARGET)
	./$(TARGET)
