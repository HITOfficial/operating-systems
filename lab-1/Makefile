# -c compile
# - warnings
CFLAGS = -c -Wall
CC = gcc
RM = rm -f

#1. compile to .o
#2. .o to program

# dependency

all:
	static_lib

static_lib: filelib.c main.c
	$(CC) $(CFLAGS) filelib.c
	ar rcs filelib.a filelib.o
	$(CC) $(CFLAGS) main.c 
	$(CC) main.o filelib.a -o main
	
shared_lib:
	$(CC)

clean:
	$(RM) main
	$(CC) main.o -l filelib -L ./ -o main
