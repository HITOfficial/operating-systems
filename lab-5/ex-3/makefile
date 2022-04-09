cc = gcc


all: run_tests

compile_consumer:
		$(cc) consumer.c -o consumer

compile_producer:
		$(cc) producer.c -o producer

compile_tests:
		$(cc) tests.c -o tests

compile_all:
		make -s compile_producer
		make -s compile_consumer
		make -s compile_tests

clean:
		rm -f consumer producer tests pipe
		rm -f results.txt

run_tests: compile_all
		@echo TESTING
		@echo
		./tests
		@echo
		@echo ==========================================================
		@echo TEST ENDED SUCCESFULLY!
		@echo CHECK results.txt to see results
