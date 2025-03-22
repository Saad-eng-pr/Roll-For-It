# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall -Wextra -std=c11

# Linker flags
LDFLAGS = -lSDL2 -lSDL2_image -lm

# Source files
SRCS = roll_for_it.c mcts.c SDL1.c

# Header files
HDRS = roll_for_it.h

# Object files
OBJS = $(SRCS:.c=.o)

# Executable
TARGET = jeu

# Default target
all: $(TARGET)

# Link object files to create the executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LDFLAGS)

# Compile source files to object files
%.o: %.c $(HDRS)
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean
