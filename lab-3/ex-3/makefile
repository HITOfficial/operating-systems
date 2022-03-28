CC = gcc


test1 = "." "data" 5
test2 = "." "data" 3
test3 = "." "data" 1
test4 = "." "lorem" 5
test5 ="./" "0 RESULTS :)" 1000


all:
	make build
	make runtests

build:
	$(CC) main.c -o main


runtests:
	./main $(test1)

clean:
	rm -f main.o
	rm -f main