all:
	gcc $(CFLAGS) -pthread server.c -o server
	gcc client.c -o client
clean:
	rm -f client
	rm -f server