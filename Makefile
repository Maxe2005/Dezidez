# A simple Makefile

# set the compiler
CC := gcc

# source files path
SRC_DIRS := ./

# set the compiler flags
CFLAGS := -lm

# name of executable
EXEC := main

clean:
	@ rm -f $(EXEC)

compile:
	$(CC) $(SRC_DIRS)/*.c -o $(EXEC) $(CFLAGS)

main_compile_only:
	$(CC) main.c -o $(EXEC) $(CFLAGS)

run:
	./$(EXEC)

# clear the terminal
clear:
	clear

all: clear compile run clean