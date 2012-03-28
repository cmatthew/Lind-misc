/*
 * Listing 2:
 * An example client for "Hello, World!" server
 * Ivan Griffin (ivan.griffin@ul.ie)
 */

#include <stdio.h>                /* perror() */
#include <stdlib.h>               /* atoi() */
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>               /* read() */
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <assert.h>
#include <errno.h>
#include "common.h"

int main()
{

    int clientSocket,
        remotePort,
        status = 0;
    struct hostent *hostPtr = NULL;
    struct sockaddr_in serverName;

    make_files();


    memset(&serverName, 0, sizeof(serverName));
    char buffer[256] = "";
    char *remoteHost = NULL;


    remoteHost = "localhost";
    remotePort = atoi("22001");


    debug("[client] make client socket");
    clientSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (-1 == clientSocket) {
        perror("socket()");
        exit(1);
    }

    /*
     * need to resolve the remote server name or
     * IP address */
    debug("[client] gethostbyname\n");
    hostPtr = gethostbyname(remoteHost);
    const char * ip = inet_ntop(AF_INET, hostPtr->h_addr, malloc(100), 100);
    printf("hostname %s is: %s\n", remoteHost, ip );
    assert(strcmp(ip,"127.0.0.1")==0);

    if (NULL == hostPtr)
    {
      debug("[client] gethostbyaddr\n");
        hostPtr = gethostbyaddr(remoteHost, strlen(remoteHost), AF_INET);
        if (NULL == hostPtr)
        {
        perror("Error resolving server address");
        exit(1);
        }
    }

    serverName.sin_family = AF_INET;
    serverName.sin_port = htons(PORT);
    
    if(!inet_pton(AF_INET, "127.0.0.1", &serverName.sin_addr)){
      debug("[client] inetpton failed");
    } 


    debug("[client]connect");
    status = connect(clientSocket, (struct sockaddr*) &serverName, sizeof(serverName));
    if (-1 == status)
    {
        perror("connect()");
        exit(1);
    }

    /*
     * Client application specific code goes here
     *
     * e.g. receive messages from server, respond,
     * etc. */
    debug("[client] recving");
    status = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    
    assert (status > 0);
    assert (strcmp(buffer, MESSAGE) == 0);
    
    debug("[client] done recving");

    if (-1 == status)
    {
        perror("read()");
    }

    /* close(clientSocket); */

    return 0;
}
