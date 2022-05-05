all: compile

compile:
	$(CC) server.c -o server -lrt
	$(CC) -pthread client.c -o client -lrt

clean:
	@rm -f $(FILE_NAMES)