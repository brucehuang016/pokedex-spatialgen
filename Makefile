# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -O2 -std=c99

# Target executable
TARGET = pokedex

# Source files
SRCS = main.c pokedex.c

# Object files
OBJS = $(SRCS:.c=.o)

# Default target
all: $(TARGET)

# Build executable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)

# Compile source files to object files
%.o: %.c pokedex.h
	$(CC) $(CFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -f $(OBJS) $(TARGET) $(TARGET).exe

# Clean including state file (fresh start)
cleanall: clean
	rm -f pokedex_state.dat

# Run the program
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean cleanall run
