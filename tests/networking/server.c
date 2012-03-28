/*
 * Listing 1:
 * Simple "Hello, World!" server
 * Ivan Griffin (ivan.griffin@ul.ie)
 */

#include <stdio.h>   /* */
#include <stdlib.h>  /* exit() */
#include <string.h>  /* memset(), memcpy() */
#include <sys/utsname.h>   /* uname() */
#include <sys/types.h>
#include <sys/socket.h>   /* socket(), bind(),  listen(), accept() */
#include <assert.h>                           
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>  /* fork(), write(), close() */
#include "common.h"

/*
 * prototypes
 */
int _GetHostName(char *buffer, int length);

/*
 * constants
 */
const int BACK_LOG = 5;

int main() {
  int serverSocket = 0,
    on = 0,
    status = 0;
  char hostname[80] = "localhost";
  struct sockaddr_in serverName;

  memset(&serverName, 0, sizeof(serverName));

  serverSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

  if (-1 == serverSocket) {
      perror("socket()");
      exit(1);
  }

  /*
   * turn off bind address checking, and allow
   * port numbers to be reused - otherwise
   * the TIME_WAIT phenomenon will prevent
   * binding to these address.port combinations
   * for (2 * MSL) seconds.
   */

  on = 1;

  status = setsockopt(serverSocket, SOL_SOCKET,
		      SO_REUSEADDR,
		      (const char *) &on, sizeof(on));

  if (-1 == status) {
      perror("setsockopt(...,SO_REUSEADDR,...)");
  }

  /*

   * when connection is closed, there is a need
   * to linger to ensure all data is
   * transmitted, so turn this on also
   */
  {
    struct linger linger;
    memset(&linger, 0, sizeof(linger));

    linger.l_onoff = 1;
    linger.l_linger = 30;
    status = setsockopt(serverSocket,
			SOL_SOCKET, SO_LINGER,
			(const char *) &linger,
			sizeof(linger));

    if (-1 == status) {
      perror("setsockopt(...,SO_LINGER,...)");
    }
  }

  /*
   * find out who I am
   */
  debug("[server] gethostname");
  status = _GetHostName(hostname, sizeof(hostname));
  if (-1 == status) {
      perror("_GetHostName()");
      exit(1);
  }

  debug("[server] skipping gethostbyname");
  /* hostPtr = gethostbyname(hostname); */
  /* if (NULL == hostPtr) { */
  /*     perror("gethostbyname()"); */
  /*     exit(1); */
  /* } */

  (void) memset(&serverName, 0, sizeof(serverName));

  if(!inet_pton(AF_INET, "127.0.0.1", &serverName.sin_addr)){
    debug("inetpton failed");
  } 
  
 
  /*
   * to allow server be contactable on any of
   * its IP addresses, uncomment the following
   * line of code:
   */
  /* serverName.sin_addr.s_addr=htonl(INADDR_ANY); */

  serverName.sin_family = AF_INET;
  /* network-order */
  serverName.sin_port = htons(PORT);
  
  debug("[server] calling bind");
  status = bind(serverSocket, (struct sockaddr *) &serverName, sizeof(serverName));
  if (-1 == status) {
      perror("bind()");
      exit(1);
  }
  debug("[server] calling listen");
  status = listen(serverSocket, BACK_LOG);
  if (-1 == status) {
      perror("listen()");
      exit(1);
  }

  struct sockaddr_in clientName;
  memset(&clientName, 0, sizeof(clientName));

  socklen_t clientLength = sizeof(clientName);

  (void) memset(&clientName, 0, sizeof(clientName));

  debug("[server] calling accept");

  int sock_fd = accept(serverSocket, (struct sockaddr *) &clientName, &clientLength);

  if (-1 == sock_fd) {
    perror("accept()");
    exit(1);
  }

      
  debug("[server] calling getpeername");
  if (-1 == getpeername(sock_fd, (struct sockaddr *) &clientName, &clientLength))
  	{
  	  perror("getpeername()");
  	}
  else
  	{
  	  printf("Connection request from %s\n", inet_ntoa(clientName.sin_addr));
  	}

  /*
   * Server application specific code
   * goes here, e.g. perform some
   * action, respond to client etc.
   */
  printf("[server]server is writing size %d of: %s\n", strlen(MESSAGE), MESSAGE);
  debug("[server] calling send");

  int rc = send(sock_fd, MESSAGE, strlen(MESSAGE) + 1, 0);
  close(sock_fd);

  assert(rc >= 0);
  printf("[server] sent %d bytes\n", rc);
  assert((unsigned int)rc == strlen(MESSAGE) + 1);

  return 0;
}

/*
 * Local replacement of gethostname() to aid
 * portability */
int _GetHostName(char *buffer, int length)
{
      strncpy(buffer,"foo" , length);
      return 0;
}
