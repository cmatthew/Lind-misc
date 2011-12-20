#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "component.h"
#include "logger.h"


int main() {

  printf("Starting component 1\n");

  char * message = "Hello logging World!";

  log_message(1, message);
  log_message(1, "Some message");
  log_message(1, "xaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaax");
  log_message(1, "xaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaax");
  
  return 0;
}
