all:
	gcc server.c -o server
	gcc client.c -o client
fork: all
thread:
	gcc -pthread server_thread.c -o server
	gcc client.c -o client
clean:
	rm -f client
	rm -f server