#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "component.h"

int main() {

  printf("Starting component 1\n");

  printf("Interface attach:\n");

  cid logger = comp_interface_attach(100, 0);
  printf("done.\n");

  if (logger < 0) {
    fprintf(stderr, "[multi-component-1] Bind has failed.\n");
    exit(EXIT_FAILURE);
  }

  const char * message = "Hello logging World!";
  int call_status = comp_call(logger, strlen(message)+1, (void*)message);

  if (call_status < 0) {
    fprintf(stderr, "[multi-component-1] Send has failed.\n");
    exit(EXIT_FAILURE);
  }
  
  return 0;
}
