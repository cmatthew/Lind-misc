#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <strings.h>
#include <unistd.h>


int BUFF_LEN = 256;
void error(char *msg) {
	perror(msg);
	exit(1);
}


int start_server(int argc, char *argv[]) {
	int sockfd, newsockfd, portno, clilen,n;
	char buffer[BUFF_LEN];
	
	struct sockaddr_in serv_addr, cli_addr;
	
	struct sockaddr_in {
		short sin_family; /* must be AF_INET */
		u_short sin_port;
		struct in_addr sin_addr;
		char sin_zero[8]; /* apparently not used, must be zero */
	};

	if (argc < 2) {
		fprintf(stderr, "ERROR, no port provided\n");
		exit(1);
	}
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0) {
		error("ERROR opening socket");
	}
	bzero((char *) &serv_addr, sizeof(serv_addr));
	portno = atoi(argv[1]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(portno);
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		error("ERROR on binding");
	}
	while(1) {
		listen(sockfd,1); /* number of how many connections can be waiting */
		clilen = sizeof(cli_addr);
		newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
		if (newsockfd < 0) {
			error("ERROR on accept");
		}

		bzero(buffer,BUFF_LEN);
		n = read(newsockfd,buffer,BUFF_LEN-1);
		if (n < 0) {
			error("ERROR reading from socket");
		}
		printf("Here is the message: %s\n",buffer);
		n = write(newsockfd,"I got your message",18);
		if (n < 0) {
			error("ERROR writing to socket");
		}
	} 
	return 0;
}


int main (int argc, char *argv[]) {
	int works;
	works = start_server(argc, argv);
	return works;
}
