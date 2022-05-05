CC=gcc
C_FLAGS=-Wall -std=gnu11 -g
FILE_NAMES=client server
TARGETS=$(FILE_NAMES)

all: compile

compile:
	@COMP_NAME=''; \
	for OUT_NAME in $(FILE_NAMES); do \
		COMP_NAME=`expr $$OUT_NAME`.c; \
		$(CC) $(C_FLAGS) $$COMP_NAME -o $$OUT_NAME; \
	done; \

clean:
	@rm -f $(FILE_NAMES)