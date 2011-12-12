#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "component.h"
#include <unistd.h>


#define ID "[multi-component-2] "
#define MAX_BUF 1024
int main() {

  printf("Starting compoent 2\n");
  fflush(stdout);
  void * message = NULL;
  message = malloc(sizeof(MAX_BUF));
  if (message == NULL) {
    perror("malloc failure");
     exit(EXIT_FAILURE);

  }
  memset(message, 0, MAX_BUF);

  cid caller =  comp_accept(0, message, MAX_BUF);
  
   if (caller < 0) {
     fprintf(stderr, ID "accept has failed.\n");
     exit(EXIT_FAILURE);
   }

  printf("Logger got message.\n");
  printf("Message is: %s\n", (char*) message); 
  fflush(stdout);

  return 0;
}
