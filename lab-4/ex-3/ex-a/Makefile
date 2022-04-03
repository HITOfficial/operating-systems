
# to works fine run new terminal and add (signals to send)<-int (signal_type) <- string (kill, sigqueue, sigrt)
# example: ./catcher 7777 1000 kill
# 		   ./catcher 7777 1000 sigqueue
# 		   ./catcher 7777 1000 sigrt


all: run_catcher

build_catcher:
	gcc catcher.c -w -o catcher

build_sender:
	gcc sender.c -w -o sender

build_catcher_sender: build_catcher build_sender


run_catcher: build_catcher_sender
	./catcher


