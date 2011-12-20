#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "component.h"
#include <unistd.h>
#include "logger.h"
#include <time.h>

#define ID "[multi-component-2] "
#define MAX_BUF 1024




int poll_and_dispatch(void) {
  struct timespec sleeptime = {0, 10000};
  
  struct timespec leftover_time = {0, 10000};
 
  int started = 0;

  void * buffer = NULL;
  buffer = malloc(sizeof(MAX_BUF));
  if (buffer == NULL) {
    perror("malloc failure");
     exit(EXIT_FAILURE);
  }
  cid caller = 0;
  while (caller >= 0) {

    memset(buffer, 0, MAX_BUF);

    caller =  comp_accept(0, buffer, MAX_BUF);
    printf(ID "caller was %d\n", caller);
    if (caller < 0) {
      fprintf(stderr, ID "accept has failed.\n");
      exit(EXIT_FAILURE);
      continue;
    }
    if (caller == 0) {
      if (started == 1) {
	return 0;
      }
      nanosleep(&sleeptime, &leftover_time);
      continue;
   }
    
    started = 1;
    
    struct message * mesg = buffer;
  
    int pri = mesg->priority;
    char * str = &(mesg->c);
    printf("PRIORITY=%d\n", mesg->priority);
    log_message(pri, str);
 
  }
  return caller;
}


int main(int argc, char *argv[])
{
  printf("Starting SO-Server:... done.");
  return poll_and_dispatch();
 
}
  

 
