# -c compile
# - warnings
SRC_FILES = funct.c main.c
CFLAGS = -c -Wall

#1. compile to .o
#2. .o to program

# dependency
link: compile
	gcc compile -o main

compile:
	gcc $(CFLAGS) $(SRC_FILES)

clean:
	rm -f *.o main