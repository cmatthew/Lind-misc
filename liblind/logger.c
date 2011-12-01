#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "component.h"

#define ID "[multi-component-2] "
#define MAX_BUF 8192
int main() {

  cid caller = comp_accept(0);
  
  if (caller < 0) {
    fprintf(stderr, ID "accept has failed.\n");
    exit(EXIT_FAILURE);
  }

  void * message = malloc(sizeof(MAX_BUF));

  int call_status = comp_recv(message, MAX_BUF);

  if (call_status < 0) {
    fprintf(stderr, ID "comp_recv has failed.\n");
    exit(EXIT_FAILURE);
  }
  
  return 0;
}
