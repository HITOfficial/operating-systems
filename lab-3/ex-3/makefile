CC = gcc
RES= results.txt

test1 = "." "data" 5
test2 = "." "data" 3
test3 = "." "data" 1
test4 = "." "lorem" 5
test5 ="./" "0 RESULTS :)" 1000


all:
	make runtests


build:
	$(CC) main.c -o main


runtests : build
	@echo TEST 1 PATH: "." PIVOT: "DATA" DEPTH: 5
	echo TEST 1 PATH: "." PIVOT: "DATA" DEPTH: 5 >> $(RES)
	./main $(test1);
	@echo -----------------------------------------
	@echo TEST 2 PATH: "." PIVOT: "DATA" DEPTH: 3
	echo TEST 2 PATH: "." PIVOT: "DATA" DEPTH: 3 >> $(RES)
	./main $(test2);
	@echo -----------------------------------------
	@echo TEST 3 PATH: "." PIVOT: "DATA" DEPTH: 1
	echo TEST 3 PATH: "." PIVOT: "DATA" DEPTH: 1 >> $(RES)
	./main $(test3);
	@echo TEST 3 PATH: "." PIVOT: "DATA" DEPTH: 1
	@echo -----------------------------------------
	@echo TEST 4 PATH: "." PIVOT: "DATA" DEPTH: 5
	echo TEST 4 PATH: "." PIVOT: "DATA" DEPTH: 5 >> $(RES)
	./main $(test4);
	@echo -----------------------------------------
	@echo TEST 5 PATH: "." PIVOT: "DATA" DEPTH: 1000
	echo TEST 5 PATH: "." PIVOT: "DATA" DEPTH: 1000 >> $(RES)
	./main $(test5);
	@echo -----------------------------------------

clean:
	rm -f main.o
	rm -f main
	rm -f results.txt