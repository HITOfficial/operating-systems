# -c compile
CFLAGS = -c 
CC = gcc
RM = rm -f
RES= fileresults.txt

# small, medium, large, cycles number, cycle1, ...

test1=5 1 1 1 1 
test2=1 5 1 2 1 2
test3=1 1 5 3 1 1 1
test4=1 1 1 5 1 2 3 4 1
test5=3 3 3 2 4 3
test6=7 3 2 5 1 2 3 1 2
test7=4 2 7 5 1 2 5 2 1
test8=9 8 6 1 3
test9=1 1 5 3 2 2 2

all:
	make runtests

static_lib:
	$(CC) $(CFLAGS) filelib.c
	ar rcs filelib.a filelib.o
	$(CC) $(CFLAGS) main.c
	$(CC) main.o filelib.a -o main

dynamic_lib:
	$(CC) $(CFLAGS) -fPIC filelib.c
	$(CC) -shared -fPIC -o filelib.so filelib.o
	$(CC) -o main main_dynamic.c -ldl

shared_lib:
	$(CC) $(CFLAGS) -fPIC filelib.c
	$(CC) -shared -fPIC -o filelib.so filelib.o
	$(CC) -o main main.c -L. filelib.so -Wl,-rpath=`pwd`
	
clean:
	$(RM) *.so
	$(RM) *.o
	$(RM) *.a
	$(RM) main
	$(RM) filelib

runtests:
	for lib in static_lib shared_lib; do \
		for o in O0 O1 O2 O3 Ofast Os ; do \
			echo "TESTING WITH $$lib and OPTIMIZATION -$$o" >> $(RES) ;\
			echo -----------------------------------;\
			make $$lib OPTIMIZATION=-$o; \
			echo -----------------------------------;\
			echo TEST 1 ;\
			echo -----------------------------------;\
			./main $(test1); \
			echo -----------------------------------;\
			echo TEST 2 ;\
			echo -----------------------------------;\
			./main $(test2); \
			echo -----------------------------------;\
			echo TEST 3 ;\
			echo -----------------------------------;\
			./main $(test3); \
			echo -----------------------------------;\
			echo TEST 4 ;\
			echo -----------------------------------;\
			./main $(test4); \
			echo -----------------------------------;\
			echo TEST 5 ;\
			echo -----------------------------------;\
			./main $(test5); \
			echo -----------------------------------;\
			echo TEST 6 ;\
			echo -----------------------------------;\
			./main $(test6); \
			echo -----------------------------------;\
			echo TEST 7 ;\
			echo -----------------------------------;\
			./main $(test7); \
			echo -----------------------------------;\
			echo TEST 8 ;\
			echo -----------------------------------;\
			./main $(test8); \
			echo -----------------------------------;\
			echo TEST 9 ;\
			echo -----------------------------------;\
			./main $(test9); \
		done ; \
    done

