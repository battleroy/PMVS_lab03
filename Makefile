all:
	gcc server.c -o server
	gcc client.c -o client
	gcc tryex.c -o tryex
clean:
	rm client
	rm server