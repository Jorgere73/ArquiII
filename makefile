# Compiler
CC = gcc

# Compiler flags
CFLAGS = -Wall 

# Source files
SRCS = proceso.c principal.c stats_process.c

# Output files
PROGS = proceso principal stats_process

# Default target
all: $(PROGS)

# Compile each source file into its corresponding executable
%: %.c
	$(CC) $(CFLAGS) -o $@ $<

# Clean target to remove compiled executables
clean:
	rm -f $(PROGS)

