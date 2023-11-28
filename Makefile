# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra

# Source directory
SRC_DIR = src

# Source files
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)

# Object files
OBJ_FILES = $(SRC_FILES:.c=.o)

# Executable name
EXECUTABLE = myproject

# Default target
all: $(EXECUTABLE)

# Compile source files into object files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Link object files into the executable
$(EXECUTABLE): $(OBJ_FILES)
	$(CC) $(CFLAGS) $^ -o $@

# Clean the project
clean:
	rm -f $(OBJ_FILES) $(EXECUTABLE)
