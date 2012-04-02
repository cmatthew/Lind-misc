#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#define PORT        8000		/* port number as an integer */
#define BUFSIZE 8196

int pexit(char * msg) {
	perror(msg);
	return EXIT_FAILURE;
}

int main(int argc, char **argv) {
	//char * ip = "74.125.127.94";	/* IP address as a string */
	char * ip = "127.0.0.1";	/* IP address as a string */
	int i,sockfd;
	char buffer[BUFSIZE];
	static struct sockaddr_in serv_addr;
	char * ip_address;

	if(argc == 2) {
		ip_address = malloc(strlen(argv[1])+1);
		memset(ip_address, '\0',strlen(argv[1])) ;
		pexit("memset");
		memcpy(ip_address, argv[1], strlen(argv[1]));
		pexit("memcopy");	
	
	 } else {
		ip_address = malloc(strlen(ip) + 1);
		strcpy(ip_address, ip);
	}
	
	printf("client trying to connect to %s and port %d\n",ip_address,PORT);
	if((sockfd = socket(AF_INET, SOCK_STREAM,0)) <0) {
		pexit("socket() failed");
	}
	
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(ip_address);
	serv_addr.sin_port = htons(PORT);

	if(connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) <0) {
		pexit("connect() failed");
	}

	/* now the sockfd can be used to communicate to the server */
	write(sockfd, "GET /index.html \r\n", 18);
	/* note second space is a delimiter and important */

	/* this displays the raw HTML file as received by the browser */
	while( (i=read(sockfd,buffer,BUFSIZE)) > 0) {
		write(1,buffer,i);
	}
	return EXIT_SUCCESS;
}
