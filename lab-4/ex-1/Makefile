all:
	make run_all

run_all:
	make build
	@echo INGORE
	make run_ignore
	@echo MASK
	make run_mask
	@echo PENDING
	make run_pending
	@echo HANDLER
	make run_handler


build_exec:
	gcc exec.c -o exec

build_main:
	gcc main.c -o main


build:
	make build_exec
	make build_main


run_ignore:
	./main "ignore"

run_pending:
	./main "pending"

run_mask:
	./main "mask"

run_handler:
	./main "handler"

clear:
	rm -f main
	rm -f exec