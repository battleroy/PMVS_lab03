#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

void *client_thread_func(int *sock)
{
	int thread_sock = *sock;
	while(1) {
		printf("thread_func began, sock: %d\n", thread_sock);
		char buf[2048];
		FILE *command_file; 
		int nread, bytes_read;
		bzero(buf, sizeof(buf));
		nread = 0;
		bytes_read = recv(thread_sock, buf, sizeof(buf), 0);
		if(bytes_read <= 0)
		    break;
		printf("Command: %s\n", buf);
		command_file = popen(buf, "r");

		while((nread = fread(buf, 1, sizeof(buf), command_file)) > 0) {
		    fwrite(buf, sizeof(char), nread, stdout);
		    send(thread_sock, buf, nread, 0);
		}

		bzero(buf, sizeof(buf));
		send(thread_sock, buf, sizeof(buf), 0);
		pclose(command_file);
	}

	close(sock);
	return NULL;
}
 
int main(int argc, char **argv)
{
    int listener;
    struct sockaddr_in addr;
    int bytes_read; 
    int portno; 
    int nread = 0;
    
    if (argc < 2) {
         fprintf(stderr,"ERROR, no port provided\n");
         exit(1);
     }
 
    listener = socket(AF_INET, SOCK_STREAM, 0);
    if(listener < 0) {
        perror("socket");
        exit(1);
    }
    portno = atoi(argv[1]);
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(portno);
    if(bind(listener, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("bind");
        exit(2);
    }
 
    listen(listener, 5);
    
    while(1) {
	    int sock = accept(listener, NULL, NULL);
	    pthread_t thread;

	    if(sock < 0) {
	        perror("ERROR accepting socket conn");
	        exit(3);
	    }

	    if(pthread_create(&thread, NULL, (void * (*)(void *))client_thread_func, &sock)) {
	    	perror("ERROR creating thread");
	    	exit(4);
	    }
	}

	return 0;
}